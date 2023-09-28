/*************************************************************************************/
/*      Copyright 2015 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <chrono>

#include "plugin.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "instrumentationcontext_decl.hpp"
#include "os.hpp"

#include "nextsim/core/trace/ompss/Trace.h"
#include "nextsim/core/trace/BinaryEventStream.h"

// This should be defined at compile time...:
#define CAA

#ifdef CAA
struct myId{
    int  ppid;
    unsigned rank;
    char hostname[256];
};
#endif

namespace nanos {
// CAA : variable to keep the current mader ID.
#ifdef CAA
unsigned int MASTER_OFFSET = 150000;
unsigned int master_alias = 1;
#endif

#if defined(NDEBUG)
#define verify(expr) expr
#else
#define verify(expr) assert(expr)
#endif

class TaskSimEvents {
private:
    std::vector<unsigned> count_vector_;

public:
    static unsigned long long       proc_timebase_mhz_;
    uint64_t lost_time_;

#if defined(__x86_64__) || defined(__amd64__)
    static __inline__ unsigned long long getns(void)
    {
        unsigned long long low, high;
        asm volatile("rdtsc": "=a" (low), "=d" (high));
        return (low | (((uint64_t)high) << 32)) * 1000 / proc_timebase_mhz_;
    }
#endif
#if defined(__i386__)
    static __inline__ unsigned long long getns(void)
    {
        unsigned long long ret;
        __asm__ __volatile__("rdtsc": "=A" (ret));
        // no input, nothing else clobbered
        return ret*1000 / proc_timebase_mhz_;
    }
#endif

#if defined(__arm__)
    static __inline__ unsigned long long getns(void)
    {
        uint32_t val;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(val) );
        return val;
    }
#endif

#if defined(__aarch64__)
    static __inline__ unsigned long long getns(void)
    {
        uint64_t cycles = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        return cycles /** 1000 / proc_timebase_mhz_*/;
    }
#endif

private:
    sim::trace::ompss::FileTrace<sim::trace::BinaryEventStream> *trace_writer_;
    size_t locked_address_;
    uint64_t duplication_time_;

public:
    std::set<unsigned int> current_wds_;
    // constructor
    TaskSimEvents() :
        lost_time_(0), trace_writer_(NULL), duplication_time_(0)
    {
#if defined(__aarch64__)
        proc_timebase_mhz_ = 1000;
#else 
        FILE *fp;
        char buffer[ 32768 ];
        size_t bytes_read;
        double temp;
        int res;
        fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
        if (fp != NULL) {
            bytes_read = fread(buffer, 1, sizeof(buffer), fp);
            fclose(fp);
            if (bytes_read == 0) {
                return;
            }
            res = sscanf(buffer, "%lf", &temp);
            proc_timebase_mhz_ = (res == 1) ? temp / 1000.0 : 0;
        } else {
            std::cerr << "Opening backup processor frequency file. /etc/proc/cpuinfo" << std::endl;
            fp = fopen("/proc/cpuinfo", "r");
            bytes_read = fread(buffer, 1, sizeof(buffer), fp);
            fclose(fp);

            if (bytes_read == 0) {
                return;
            }
            buffer[ bytes_read ] = '\0';
            char *match = strstr(buffer, "cpu MHz");
            if (match == NULL) {
                return;
            }
            res = sscanf (match, "cpu MHz    : %lf", &temp);

            proc_timebase_mhz_ = (res == 1) ? temp : 0;
        }
#endif //__aarch64__
    }

    // destructor
    ~TaskSimEvents() { }

    inline
    void init()
    {
        std::string filename;
        if (getenv("OMPSS_TRACE_FILE") == NULL) {
            int rank = atoi(getenv("OMPI_COMM_WORLD_RANK"))+1;
            std::ostringstream os;
            os << std::setw(6) << std::setfill('0') << rank;
            filename = "./file" + os.str();
        } else {
            filename = std::string(getenv("OMPSS_TRACE_FILE"));
        }
        trace_writer_ = new sim::trace::ompss::Trace<sim::trace::BinaryEventStream>(filename.c_str());

        // Add the work descriptor for the main task
        trace_writer_->add_wd_info(1);
        current_wds_.insert(1);

        #ifdef CAA
        // If defined a value for MASTER_ALIAS, update the default value.
        const char *master_offset = getenv("TSMPI_MASTER_ALIAS");
        if (master_offset != NULL) {
            unsigned int master_env = atoi(master_offset);
            if (master_env > 3) {
                MASTER_OFFSET = master_env;
            } else {
                std::cerr << "[PLUGIN] Please, TSMPI_MASTER_ALIAS must be at least greater than 3" << std::endl;
                exit(-1);
            }
        }

        #endif
    }

    inline
    void fini()
    {
        #ifndef CAA
        // make sure main() WD is closed
        // This makes MUSA crash due to wd_info being flushed.
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(1);
        if (not wd.phase_stack_.empty()) {
            stop_wd_id(TaskSimEvents::getns(), 1, wd.phase_stack_.top());
        }
        #endif
        delete trace_writer_;
    }

    inline
    void add_task_name(unsigned wd_id, std::string &name)
    {
        unsigned name_id;
        if (trace_writer_->add_task_name(name, name_id) == true) {
            assert(count_vector_.size() == name_id);
            count_vector_.push_back(0);
        }
        verify(trace_writer_->add_event(sim::trace::ompss::event_t(wd_id,
                sim::trace::ompss::TASK_NAME, name_id, count_vector_[name_id]++)) == true);
    }

    inline
    void add_event(sim::trace::ompss::wd_info_t& wd, uint64_t instr_entry_time) {
        if (instr_entry_time >= wd.phase_st_time_ + lost_time_) {
            uint64_t time = instr_entry_time - wd.phase_st_time_ - lost_time_ + duplication_time_;
            duplication_time_ = 0;
            verify(trace_writer_->add_event(sim::trace::ompss::event_t(wd.wd_id_,
                    sim::trace::ompss::PHASE_EVENT, wd.phase_stack_.top(), time)) == true);
        }
        lost_time_ = 0;
    }

    inline
    void pause_wd(unsigned long long timestamp, unsigned int wd_id)
    {
       #ifdef CAA
       if (wd_id == 1) {
           wd_id = master_alias;
       }
       #endif
       sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);

       assert(wd.active_ == true and wd.phase_stack_.empty() == false);
       // Closing phase (if existed)
       add_event(wd, timestamp);

       wd.active_ = false;
       lost_time_ = 0;
    }

    uint64_t pause_parallel_master_wd(unsigned long long timestamp, unsigned int wd_id) {
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);

        assert(wd.active_ == true and wd.phase_stack_.empty() == false);

        // Closing phase (if existed)
        uint64_t shaved_time = timestamp - wd.phase_st_time_ - lost_time_ - 1;
        add_event(wd, wd.phase_st_time_ + lost_time_ + 1);

        wd.active_ = false;
        lost_time_ = 0;
        duplication_time_ = shaved_time;
        return duplication_time_;
    }

    inline
    void resume_wd(unsigned long long timestamp, unsigned int wd_id)
    {
        assert(wd_id > 0);
        #ifdef CAA
        if (wd_id == 1) {
            wd_id = master_alias;
        }
        #endif

        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);
        assert(wd.active_ == false);
        wd.active_ = true;

        if (wd.phase_stack_.empty() == false) {
            // Not start of wd
            wd.phase_st_time_ = timestamp;
        }
        lost_time_ = 0;
    }

    inline
    void start_phase(unsigned long long timestamp, unsigned int wd_id, unsigned int phase)
    {
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);

        assert(wd.active_ == true and wd.phase_stack_.empty() == false);

        // Close previous phase (if existed)
        add_event(wd, timestamp);

        // Start counting this starting one
        wd.phase_stack_.push(phase);
        wd.phase_st_time_ = timestamp;
    }


    inline
    void start_wd_id(unsigned long long timestamp, unsigned int wd_id, unsigned int phase)
    {
        assert(wd_id > 0);
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);
        #ifndef CAA
        assert(wd.active_ == true);
        #endif
        wd.active_ = true;
        assert(wd.phase_stack_.empty() == true);

        #ifdef CAA
        // Small exception for the main WD, it starts in main() (which is user code) and not in nanox code
        if (wd_id != 1 and wd_id == master_alias) {
            wd.phase_stack_.push(phase);
            wd.phase_st_time_ = timestamp - 1;
            add_event(wd, timestamp);
            phase = sim::trace::ompss::USER_CODE_PHASE;
        }
        #endif
        wd.phase_stack_.push(phase);
        wd.phase_st_time_ = timestamp;
        lost_time_ = 0;
    }


    inline
    void stop_phase(unsigned long long timestamp, unsigned int wd_id, unsigned int phase)
    {
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);
        assert(wd.active_ == true and wd.phase_stack_.empty() == false);
        assert(wd.phase_stack_.top() == phase);

        // Closing phase (if existed)
        add_event(wd, timestamp);

        wd.phase_stack_.pop();
        assert(wd.phase_stack_.empty() == false);
        wd.phase_st_time_ = timestamp;
    }


    inline
    void stop_wd_id(unsigned long long timestamp, unsigned int wd_id, unsigned int phase)
    {
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);

        assert(wd.active_ == true and wd.phase_stack_.empty() == false);
        #ifdef CAA
        // Exception for the main WD, it starts in main() (which is user code) and not in nanox code
        if (wd_id == 1 or wd_id >= MASTER_OFFSET) {
            phase = sim::trace::ompss::USER_CODE_PHASE;
        }
        #endif
        assert(wd.phase_stack_.top() == phase);

        // Closing phase (if existed)
        add_event(wd, timestamp);

        wd.active_ = false;
        wd.phase_stack_.pop();

        if (wd_id == 1 && !wd.phase_stack_.empty()) {
            // TODO: master adds an unexpected event. If required it must be removed from stack
            wd.phase_stack_.pop();
            // REPORTED BY DARIO; patch for OpenMP; Should happen only at the end of the execution.
            if (!wd.phase_stack_.empty()) {
                wd.phase_stack_.pop();
            }
        }

        #ifdef CAA
        // Exception for the main WD, it starts in main() (which is user code) and not in nanox code
        if (wd_id == 1 or wd_id >= MASTER_OFFSET) {
            if (not wd.phase_stack_.empty()) {
                assert(wd.phase_stack_.top() == sim::trace::ompss::WD_ID_PHASE);
                // add_event(wd, timestamp+1);
                wd.phase_stack_.pop();
            }
        }
        #endif
        assert(wd.phase_stack_.empty() == true);
        trace_writer_->flush_wd_contents();
    }


    inline
    void wd_creation(unsigned long long timestamp, unsigned int wd_id,
            int num_deps, nanos_data_access_t* dep)
    {
        // New wd in the map
        assert(current_wds_.find(wd_id) == current_wds_.end());
        sim::trace::ompss::wd_info_t &new_wd = trace_writer_->add_wd_info(wd_id);
        current_wds_.insert(wd_id);
        new_wd.active_ = false;

        for (int i = 0; i < num_deps; i++) {
            new_wd.deps_.push_back(dep[i]);
        }
        new_wd.phase_st_time_ = timestamp;
        lost_time_ = 0;
    }


    inline
    void add_event(unsigned long long timestamp, unsigned int wd_id,
            unsigned int type, unsigned int val1, unsigned int val2)
    {
        sim::trace::ompss::wd_info_t& wd = trace_writer_->get_wd_info(wd_id);
        assert(wd.active_ == true and wd.phase_stack_.empty() == false);

        // Close current phase (if existed) and re-open it after new event
        add_event(wd, timestamp);

        verify(trace_writer_->add_event(sim::trace::ompss::event_t(wd_id,
                static_cast<sim::trace::ompss::type_t>(type), val1, val2)) == true);

        wd.phase_st_time_ = timestamp;
    }


    inline
    void add_dep(unsigned wd_id, unsigned int num_deps, nanos_data_access_t* dep)
    {
        for (unsigned int i = 0; i < num_deps; i++) {
            trace_writer_->add_dep(sim::trace::ompss::dep_t(wd_id, dep[i]));
        }
    }

    inline
    void add_lock_address(unsigned long long timestamp, unsigned wd_id, size_t lock_address) {
        if (lock_address != 0) {
            add_event(timestamp, wd_id, sim::trace::ompss::SET_LOCK, lock_address, 0);
            locked_address_ = lock_address;
        } else {
            add_event(timestamp, wd_id, sim::trace::ompss::UNSET_LOCK, locked_address_, 0);
        }
    }

    inline
    unsigned get_tasks()
    {
        return trace_writer_->get_number_of_wd_info();
    }

    inline
    void add_event_instant(unsigned int wd_id, unsigned int type, unsigned int val1, unsigned int val2)
    {
        verify(trace_writer_->add_event(sim::trace::ompss::event_t(wd_id,
                static_cast<sim::trace::ompss::type_t>(type), val1, val2)) == true);
    }

    inline
    bool exists_wd(unsigned int wd_id)
    {
        return current_wds_.find(wd_id) != current_wds_.end();
    }
};

unsigned long long TaskSimEvents::proc_timebase_mhz_;

class InstrumentationTasksimTrace: public Instrumentation
{
#ifndef NANOS_INSTRUMENTATION_ENABLED
public:
    // constructor
    InstrumentationTasksimTrace(): Instrumentation() { }
    // destructor
    virtual ~InstrumentationTasksimTrace() {}

    // low-level instrumentation interface (mandatory functions)
    virtual void initialize(void) {}
    virtual void finalize(void) {}
    virtual void threadStart(BaseThread &thread) {}
    virtual void threadFinish(BaseThread &thread) {}
    virtual void enable() {}
    virtual void disable() {}
    virtual void addResumeTask(nanos::WorkDescriptor&) {}
    virtual void addSuspendTask(nanos::WorkDescriptor&, bool) {}
    virtual void addEventList(unsigned int count, Event *events) {}

#else

private:
    TaskSimEvents      wd_events_;
    unsigned parallel_master_;

    #ifdef CAA
    std::string exp_path_prefix;
    unsigned int rank_min_;
    unsigned int rank_max_;
    unsigned int mem_min_;
    unsigned int mem_max_;
    unsigned int nanos_counter_;
    unsigned int master_phase_counter_;
    bool master_phase_with_nanos_;
    bool               instr_flag_;
    std::ofstream      mpiphase_rel;
    unsigned  mpi_rank;
    int mpi_size;
    char hostname[256];
    int pid;
    int ppid;
    #endif

    #ifdef CAA
    // It is a redefinition of a function but it is done is this way because
    // nanos is not linked against tasksim.
    void read_relation(unsigned *my_rank, int *my_size, int myPID, char *myHostname) {
        *my_rank = 0;
        *my_size = 0;
        int found = 0;
        int counter = 0;
        myId n;

        std::string myfile(exp_path_prefix + "/MUSA_relation_filename");
        FILE *fd = fopen(myfile.c_str(), "rb");
        if (fd == NULL) {
            printf("[PLUGIN] Relation file does not exists. MPI not detected. Aborting.\n");
            exit(-1);
        }

        while (!feof(fd)) {
            if (fread(&n, sizeof(myId), 1, fd) > 0) {
                counter++;
                if (n.ppid == myPID && strcmp(n.hostname, myHostname) == 0) {
                    found = 1;
                    *my_rank = n.rank;
                }
            }
        }
        fclose(fd);

        if (found) {
            *my_size = counter;
        } else {
            std::cerr << "[PLUGIN] Relation not found for PID+hostname "
                  <<  myPID << "_" << myHostname << std::endl;
            exit(-1);
        }
        return;
    }
    #endif

    inline unsigned int getMyWDId()
    {
        BaseThread *current_thread = getMyThreadSafe();
        if ((current_thread == NULL) or (current_thread->getCurrentWD() == NULL)) {
            return 0;
        }
        #ifdef CAA
        if (current_thread->getCurrentWD()->getId() == 1) {
            return master_alias;
        } else {
            assert(current_thread->getCurrentWD()->getId() < (long long)MASTER_OFFSET);
        }
        #endif
        return current_thread->getCurrentWD()->getId();
    }

public:
    // constructor
    InstrumentationTasksimTrace() :
        Instrumentation(*new InstrumentationContextDisabled()),
        nanos_counter_(0),
        master_phase_counter_(0),
        master_phase_with_nanos_(false)
    {}

    // destructor
    ~InstrumentationTasksimTrace() { }

    // low-level instrumentation interface (mandatory functions)

    virtual void initialize()
    {
        #ifdef CAA
        // Filtering Option 2;
        if (getenv("TSMPI_RANK_INIT") == nullptr) {
            std::cout << "[ERROR] Environment variable $TSMPI_RANK_INIT undefined." << std::endl;
            exit(-1);
        }
        rank_min_ = std::atoi(getenv("TSMPI_RANK_INIT"));

        if (getenv("TSMPI_RANK_NUM") == NULL) {
            std::cout << "[ERROR] Environment variable $TSMPI_RANK_NUM undefined." << std::endl;
            exit(-1);
        }
        int num = std::atoi( getenv("TSMPI_RANK_NUM"));
        if (num == 0) {
             std::cout << "[ERROR] Environment variable $TSMPI_RANK_NUM set to 0." << std::endl;
             exit(-1);
        }
        rank_max_ = rank_min_ + num - 1;
        
        if (getenv("TSMPI_MEM_PHASES_INIT") == NULL) {
             std::cout << "[ERROR] Environment variable $TSMPI_MEM_PHASES_NUM set to 0." << std::endl;
             exit(-1);
        }
        mem_min_ = std::atoi(getenv("TSMPI_MEM_PHASES_INIT"));
        char *get_mem_phases_max = getenv("TSMPI_MEM_PHASES_NUM");
        if (getenv("TSMPI_MEM_PHASES_NUM") == NULL) {
             std::cout << "[ERROR] Environment variable $TSMPI_MEM_PHASES_NUM set to 0." << std::endl;
             exit(-1);
        }
        num = std::atoi(get_mem_phases_max);
        if (num == 0) {
             std::cout << "[ERROR] Environment variable $TSMPI_MEM_PHASES_NUM set to 0." << std::endl;
             exit(-1);
        }
        mem_max_ = mem_min_ + num - 1;

        /** Fet MUSA_EXP_NAME...if it does not exist therefore path would be
          * current directory. It is important to maintain coherence between
          * the nanos plugin and the tasksim wrapper
          */
        if (getenv("MUSA_EXP_NAME") != nullptr) {
            exp_path_prefix = std::string(getenv("MUSA_EXP_NAME"));
        } else {
            std::cout << "[WARNING] MUSA_EXP_PATH is not defined, using \".\"" << std::endl;
            exp_path_prefix = std::string("./");
        }

        mpi_rank = 0;
        instr_flag_ = true;

        // Hostname is intialized once.
        memset(hostname, 0, 256);
        gethostname(hostname, 256);
        pid = getpid();
        ppid = getppid();
        #endif

        wd_events_.init();

        #ifdef CAA
        // Set up the temporary relation file. This file is going to be read
        // for every mpi event on the interposition library.
        // Serves as communication between processes therefore it is a unique filename
        // per process.

        std::ostringstream stm;
        stm << ppid;
        std::string phase_rel_name = exp_path_prefix + "/" +
                "mpiphases_temp_rel_" + stm.str() + "_" + hostname;
        
        mpiphase_rel.open(phase_rel_name.c_str());
        if (mpiphase_rel.fail()) {
            std::cerr << "[PLUGIN] Error opening relation" << phase_rel_name
                      << " -- Aborting." << std::endl;
            std::exit(-1);
        }
        #endif
    }

    virtual void finalize()
    {
        wd_events_.fini();
        #ifdef CAA
        mpiphase_rel.close();
        #endif
    }

    virtual void enable()
    {
        #ifdef CAA
        unsigned int which_WD = getMyWDId();
        unsigned long long timestamp = wd_events_.getns();

        assert(which_WD == 1 || which_WD >= MASTER_OFFSET);
        // Add new Master Task Name
        std::string phase_name("main");
        wd_events_.add_task_name(which_WD, phase_name);

        // Crete Master Task
        wd_events_.wd_creation(timestamp, which_WD, 0, 0);

        // Enable Master Task
        wd_events_.start_wd_id(timestamp, which_WD, sim::trace::ompss::WD_ID_PHASE);

        instr_flag_ = true;
        #endif
    }

    virtual void disable()
    {
        #ifdef CAA
        unsigned int which_WD;
        unsigned long long timestamp = TaskSimEvents::getns();
        which_WD = getMyWDId();

        if (!mpi_rank) {
            read_relation(&mpi_rank, &mpi_size, ppid, hostname);
        }

        // DEBUG
        if (which_WD >= MASTER_OFFSET || which_WD == 1) {
            if (master_phase_with_nanos_) {
                nanos_counter_++;
            }
            int get_mem = 0;
            int valid_rank = 0;
            int valid_mem_phases = 0;

            if (mpi_rank >= rank_min_ && mpi_rank <= rank_max_) {
                valid_rank = 1;
            }
            if (nanos_counter_ >= mem_min_ && nanos_counter_ <= mem_max_) {
                valid_mem_phases = 1;
            }

            if (valid_rank == 1 && valid_mem_phases == 1 && master_phase_with_nanos_) {
                get_mem = 1;
            }

            // Add the phase information on the relation file
            if (master_alias != MASTER_OFFSET) {
                mpiphase_rel << master_phase_counter_ << ":"
                           << master_alias << ":"
                           << (int)master_phase_with_nanos_ << ":"
                           << get_mem << std::endl;
            } else {
                mpiphase_rel << master_phase_counter_ << ":"
                           << which_WD  << ":"
                           << (int)master_phase_with_nanos_ << ":"
                           << get_mem << std::endl;
            }
            // Update master alias!!
            master_alias = MASTER_OFFSET + master_phase_counter_;
            wd_events_.stop_wd_id(timestamp, which_WD, sim::trace::ompss::WD_ID_PHASE);

            // reset flags
            master_phase_with_nanos_ = false;
            instr_flag_ = false;
            master_phase_counter_++;
        }
        #endif
    }

    virtual void addResumeTask(nanos::WorkDescriptor& wd)
    {
        unsigned int which_WD = getMyWDId();
        uint64_t  timestamp = TaskSimEvents::getns();

        if (wd_events_.exists_wd(wd.getId())) {
            wd_events_.resume_wd(timestamp, wd.getId());
        } else {
            if (which_WD != static_cast<unsigned>(wd.getId())) {
                parallel_master_ = wd.getId();
            }

            wd_events_.add_event_instant(master_alias, sim::trace::ompss::CREATE_TASK_EVENT, wd.getId(), 0);
            wd_events_.wd_creation(timestamp, wd.getId(), 0, (nanos_data_access_t*) 0);
            wd_events_.resume_wd(timestamp, wd.getId());
        }
        uint64_t exit_timestamp = TaskSimEvents::getns();
        wd_events_.lost_time_ += exit_timestamp - timestamp;
    }

    virtual void addSuspendTask(nanos::WorkDescriptor& wd, bool)
    {
        if (static_cast<unsigned>(wd.getId()) == parallel_master_) {
            wd_events_.pause_parallel_master_wd(TaskSimEvents::getns(), wd.getId());
        } else {
            wd_events_.pause_wd(TaskSimEvents::getns(), wd.getId());
        }
    }

    virtual void addEventList(unsigned int count, Event *events)
    {
        #ifdef CAA
        // Check if tracing is enabled
        if (instr_flag_ == false) {
            return;
        }
        #endif
        static const nanos_event_key_t api          = getInstrumentationDictionary()->getEventKey("api");
        static const nanos_event_value_t wait_group = getInstrumentationDictionary()->getEventValue("api", "wg_wait_completion");
        static const nanos_event_key_t wd_id        = getInstrumentationDictionary()->getEventKey("wd-id");
        static const nanos_event_key_t user_func    = getInstrumentationDictionary()->getEventKey("user-funct-location");
        static const nanos_event_key_t user_code    = getInstrumentationDictionary()->getEventKey("user-code");
        static const nanos_event_key_t create_wd_id = getInstrumentationDictionary()->getEventKey("create-wd-id");
        static const nanos_event_key_t wd_ptr       = getInstrumentationDictionary()->getEventKey("create-wd-ptr");
        static const nanos_event_key_t wd_num_deps  = getInstrumentationDictionary()->getEventKey("wd-num-deps");
        static const nanos_event_key_t set_lock     = getInstrumentationDictionary()->getEventValue("api", "set_lock");
        static const nanos_event_key_t unset_lock   = getInstrumentationDictionary()->getEventValue("api", "unset_lock");
        static const nanos_event_key_t lock_addr    = getInstrumentationDictionary()->getEventKey("lock-addr");
#ifndef NDEBUG
        static const nanos_event_key_t wd_deps_ptr  = getInstrumentationDictionary()->getEventKey("wd-deps-ptr");
#endif

        unsigned long long timestamp = TaskSimEvents::getns();
        unsigned int which_WD = getMyWDId();

        /*
        std::cout << "============ Sta " << which_WD << " ========" << std::endl;
        std::vector<std::string> v_types({"NANOS_STATE_START", "NANOS_STATE_END",
                "NANOS_SUBSTATE_START", "NANOS_SUBSTATE_END", "NANOS_BURST_START",
                "NANOS_BURST_END", "NANOS_PTP_START", "NANOS_PTP_END",
                "NANOS_POINT", "EVENT_TYPES"});
        for (unsigned int i = 0; i < count; i++) {
            std::cout << v_types[events[i].getType()] << " " << events[i].getKey() << " " << events[i].getValue() << " "
                      << getInstrumentationDictionary()->getKeyDescription(events[i].getKey()) << " "
                      << getInstrumentationDictionary()->getValueDescription(events[i].getKey(), events[i].getValue()) << std::endl;
        }
        std::cout << "============ End " << which_WD << " ========" << std::endl;
        */
        for (unsigned int i = 0; i < count; i++) {
            Event &e = events[i];
            unsigned int type = e.getType();
            switch (type) {
                case NANOS_STATE_START:
                case NANOS_STATE_END:
                case NANOS_SUBSTATE_START:
                case NANOS_SUBSTATE_END:
                case NANOS_PTP_START:
                case NANOS_PTP_END:
                    break;
                case NANOS_POINT: {
                    if (e.getKey() == create_wd_id) {
                        unsigned int wd_id_num = e.getValue();
                        wd_events_.add_event(timestamp, which_WD, sim::trace::ompss::CREATE_TASK_EVENT, wd_id_num, 0);
                        e = events[++i];
                        assert(e.getKey() == wd_ptr);
                        #ifdef CAA
                        if (wd_id_num == 1) {
                            std::cout << "[ERROR] Creating master task from another task." << std::endl;
                            exit(-1);
                            wd_id_num = master_alias;
                        }
                        #endif
                        e = events[++i];
                        assert(e.getKey() == wd_num_deps);
                        int num_deps =  e.getValue();
                        e = events[++i];
                        assert(e.getKey() == wd_deps_ptr);
                        wd_events_.wd_creation(timestamp, wd_id_num, num_deps, (nanos_data_access_t*) e.getValue());
                    } else if (e.getKey() == wd_ptr) {
                        WD *wd = (WD*) e.getValue();
                        unsigned int wd_id_num = wd->getId();
                        wd_events_.add_event(timestamp, which_WD, sim::trace::ompss::CREATE_TASK_EVENT, wd_id_num, 0);
                        e = events[++i];
                        assert(e.getKey() == wd_num_deps);
                        int num_deps =  e.getValue();
                        e = events[++i];
                        assert(e.getKey() == wd_deps_ptr);
                        wd_events_.wd_creation(timestamp, wd_id_num, num_deps, (nanos_data_access_t*) e.getValue());
                    } else if (e.getKey() == wd_num_deps) {
                        // It's a wait on event
                        int num_deps = e.getValue();
                        e = events[++i];
                        assert(e.getKey() == wd_deps_ptr);
                        wd_events_.add_event(timestamp, which_WD, sim::trace::ompss::WAIT_ON_EVENT, num_deps, 0);
                        wd_events_.add_dep(which_WD, num_deps, (nanos_data_access_t*) e.getValue());
                    } else if (e.getKey() == lock_addr) {
                        nanos_event_key_t val = (e.getValue() != 0) ? set_lock : unset_lock;
                        wd_events_.start_phase(timestamp - 2, which_WD, val);
                        wd_events_.add_lock_address(timestamp, which_WD, e.getValue());
                        wd_events_.stop_phase(timestamp , which_WD, val);
                    } else {
                        // Ignore any other point event
                    }
                    break;
                }
                case NANOS_BURST_START: {
                    nanos_event_key_t e_key = e.getKey();
                    nanos_event_key_t e_val = e.getValue();

                    if (e_key == api) {
                        wd_events_.start_phase(timestamp, which_WD, e_val);
                    } else if (e_key == user_code) {
                        // if master task event, then rename master_id with master alias
                        #ifdef CAA
                        if (e_val == 1) {
                            e_val = master_alias;
                        }
                        #endif
                        which_WD = e_val;
                        wd_events_.start_phase(timestamp, which_WD, sim::trace::ompss::USER_CODE_PHASE);
                    } else if (e_key == wd_id) {
                        // if master task event, then rename master_id with master alias
                        #ifdef CAA
                        if (e_val == 1) {
                            e_val = master_alias;
                        }
                        #endif
                        which_WD = e_val;
                        wd_events_.start_wd_id(timestamp, which_WD, sim::trace::ompss::WD_ID_PHASE);
                        #ifdef CAA
                        if (e_val != 1) {
                            master_phase_with_nanos_ = true;
                        }
                        #endif
                    } else if (e_key == user_func) {
                        // virtual phase that gives us the name of the function
                        wd_events_.start_phase(timestamp, which_WD, sim::trace::ompss::USER_CODE_PHASE);
                        std::string name;
                        // FIXME: whenever the new getValueKey method is available, replace this by a call to it
                        InstrumentationDictionary::ConstKeyMapIterator k_it  = getInstrumentationDictionary()->beginKeyMap();
                        InstrumentationDictionary::ConstKeyMapIterator k_end = getInstrumentationDictionary()->endKeyMap();
                        bool k_found = false, v_found = false;
                        while (k_it != k_end and not k_found) {
                            if (k_it->second->getId() == e_key) {
                                InstrumentationKeyDescriptor::ConstValueMapIterator v_it  = k_it->second->beginValueMap();
                                InstrumentationKeyDescriptor::ConstValueMapIterator v_end = k_it->second->endValueMap();
                                while (v_it != v_end and not v_found) {
                                    if (v_it->second->getId() == e_val) {
                                        name = v_it->first;
                                        v_found = true;
                                    }
                                    v_it++;
                                }
                                k_found = true;
                            }
                            k_it++;
                        }
                        assert(v_found == true);
                        name = name.substr(0, name.find_first_of(":"));
                        name = name.substr(0, name.find("@"));
                        wd_events_.add_task_name(which_WD, name);
                    }
                    break;
                }
                case NANOS_BURST_END: {
                    nanos_event_key_t e_key = e.getKey();
                    nanos_event_key_t e_val = e.getValue();
                    if (e_key == api) {
                        if (e_val == wait_group) {
                            // After the end of wait group phase, add a wait group event         
                            wd_events_.add_event(timestamp, which_WD, sim::trace::ompss::WAIT_GROUP_EVENT, 0, 0);         
                        }  
                        wd_events_.stop_phase(timestamp, which_WD, e_val);
                    } else if (e_key == user_code) {
                        #ifdef CAA
                        if (e_val == 1)  {
                            e_val = master_alias;
                        }
                        #endif
                        which_WD = e_val;
                        wd_events_.stop_phase(timestamp, which_WD, sim::trace::ompss::USER_CODE_PHASE);
                    } else if (e_key == user_func) {
                        wd_events_.stop_phase(timestamp, which_WD, sim::trace::ompss::USER_CODE_PHASE);
                    } else if (e_key == wd_id) {
                        #ifdef CAA
                        if (e_val == 1) {
                            e_val = master_alias;
                        }
                        #endif
                        which_WD = e_val;
                        wd_events_.stop_wd_id(timestamp, which_WD, sim::trace::ompss::WD_ID_PHASE);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        uint64_t exit_timestamp = TaskSimEvents::getns();
        wd_events_.lost_time_ += exit_timestamp - timestamp;
    }

    virtual void threadStart(BaseThread &thread) {}
    virtual void threadFinish(BaseThread &thread) {}
#endif
};


namespace ext {

class InstrumentationTasksimTracePlugin : public Plugin {
public:
    InstrumentationTasksimTracePlugin() : Plugin("Instrumentor which generates tasksim traces.", 1) {}
    ~InstrumentationTasksimTracePlugin() {}

    void config(Config &cfg) {}

    void init()
    {
        sys.setInstrumentation(new InstrumentationTasksimTrace());
    }
};

}  // namespace ext
}  // namespace nanos

DECLARE_PLUGIN("placeholder-name", nanos::ext::InstrumentationTasksimTracePlugin);
