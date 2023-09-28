/*
 * Copyright (c) 2015, Barcelona Supercomputing Center
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer; redistributions in binary form
 * must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with
 * the distribution; neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CORE_UTILS_INSTRUMENTATION_MITOS_H_
#define CORE_UTILS_INSTRUMENTATION_MITOS_H_

#include <string>
#include <vector>

#include "Instrumentation.h"
#include "MitosDefs.h"

namespace sim {
namespace utils {
namespace instrumentation {

class Mitos : public Instrumentation<Mitos>
{
public:
    void generate_final_mitos_file(const std::string &output_filename, const std::string &input_filename);
private:
    static std::string new_trace_base_name(const std::string trace_base);
    static std::string generate_tmp_file_name();
    static sim::engine::ModuleType filter_instrumented_modules(std::vector<std::string> &accepted_modules_type_names);

    /** Number of instrumentalized modules */
    unsigned num_modules_;
    /** Total number of instrumentalized components */
    unsigned num_components_;
    /** Base name for the trace file (.csv). */
    std::string trace_base_name_;
    /** Name of the temporary binary file used to store a partial trace */
    std::string temporary_file_name_;
    /** Stream for the temporary binary file used to store a partial trace */
    std::fstream temporary_file_;
    /** Finished Instrumentation Items buffer */
    InstrumentationBuffer item_buffer_;
    /** Set containing the names of the modules to instrumentalize */
    sim::engine::ModuleType instrumented_modules_;
    /** Number of already instrumentalized modules per module name */
    std::unordered_map<std::string, unsigned> module_count_;
    /** List of component names that will be used as thread names in Paraver */
    std::vector<std::string> component_name_;
    /** List of module names that will be used as cpu names in Paraver */
    std::vector<std::string> module_name_;
    /** Marks if the intermediate trace should be kept or removed. */
    bool keep_intermediate_;
    /** Enable/Disable tracing. */
    bool disable_trace_generation_;

		std::map<std::string, uint64_t> cacheLatencies;

public:
    void flush(bool force);
    void init(const sim::engine::Config &config);
    void finalize();

    template<typename ModuleType>
    int add_module(const ModuleType &module, unsigned num_components = 1);
    void start_state(const InstrumentationItem&);
    void end_state(const InstrumentationItem&);
    void start_comm(const InstrumentationItem&);
    void end_comm(const InstrumentationItem&);
    /** Add event call for modules which may potentially be shared by several CPUs. */
    template<typename ModuleType>
    void add_event(const InstrumentationItem &event, const ModuleType &module);
    /** Normal call that adds one event: */
    void add_event(const InstrumentationItem &event);
    void add_function_name(const char* name);
		void add_precise_memory_event(const InstrumentationItem &event);
		sim::engine::cycles_t get_hardware_sampling_interval();
    // Function to close fast the intermediate buffer in case of unexpected simulator finish.
    void dump_buffer();
};

}  // namespace instrumentation
}  // namespace utils
}  // namespace sim

#include "Mitos-impl.h"

#endif  // CORE_UTILS_INSTRUMENTATION_MITOS_H_
