
#include "paraver.h"
#include "tasksimclient.h"


void tsim_load_task_config(struct t_task *);
void tasksim_init_task(struct t_task *);
void tsim_load_environment_delimiters(struct t_task *);
void tsim_load_relation(struct t_task *);
void tsim_load_phase_sim_params(struct t_task *, tsim_phase *);


/// this is the one to replace
double tsim_get_presimulation(int , tsim_phase *);


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
int tasksimManageBurst(t_nano *burst_time, struct t_thread *thread, dimemas_timer current_time) {

  int replacement = 0 ;

  if (is_tasksim_presim_enabled()) {
    replacement = tasksimManageBurst_presim (burst_time, thread, current_time);
  } else if (is_tasksim_online_enabled()) {
    // Currently unsupported
    replacement = tasksimManageBurst_online (burst_time, thread, current_time);
  }

  return replacement;
}

int tasksimManageBurst_online (t_nano *burst_time, struct t_thread *thread, dimemas_timer current_time) {

    // Currently unsopported
    return 0;
}

int tasksimManageBurst_presim (t_nano *burst_time, struct t_thread *thread, dimemas_timer current_time) {
/*
    t_task *task        = thread->task;
    tsim_metaphase *mph = task->metaphase_current;

    //// CREATE NEW EVENT
    tsim_event *ev = (tsim_event *) malloc (sizeof(tsim_event));
    if (ev == NULL) {
      printf("Error allocating event \n");
      exit(-1);
    }

    // update event information
    ev->time = (double)*burst_time;
    ev->next = NULL;
    ev->replacement_phase = NULL;
    ev->replace = 0;
    ev->ev_id = task->event_counter++;

    // INSERT EVENT ON LIST
    // insert event on current meta pahse
    if (mph->ev_init == NULL) {
        mph->ev_init = ev;
        mph->ev_current = ev;
        ev->prev = NULL;
    } else {
        ev->prev = mph->ev_end;
        mph->ev_end->next = ev;
    }
    mph->ev_end = ev;
    mph->time_events +=ev->time;
    mph->num_events++;

    // CHECK REPLACEMENT
    if (mph->phase_init != NULL && mph->phase_end->next != mph->phase_current) {
      if (mph->phase_current == NULL) {
        mph->phase_current = mph->phase_init;
      } else {
        if (mph->phase_current->sim.to_simulate || mph->phase_current->sim.presimulated) {
          ev->replace = 1;
          // push tasksim event and insert subtrace if paraver enabled.
          tasksim_insert_subtrace(thread,current_time);
        }

        ev->replacement_phase = mph->phase_current;
        mph->phase_current = mph->phase_current->next;
      }
    }

    // update time when replaced by ompss....
    if (ev->replace) {
      mph->time += ev->replacement_phase->sim.time;
    } else {
      mph->time += ev->time;
    }

    return (ev->replace);
*/
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_init_task(struct t_task *task)
{
    printf("[TASKSIM] Initializing task %d\n", task->taskid+1);

    // load task parameters
    printf("TASKSIM -- load task config\n");
    tsim_load_task_config(task);


    // check environmental variables
    printf("TASKSIM -- environmental variables\n");
    tsim_load_environment_delimiters(task);

    // read relation file and create phases
    printf("TASKSIM -- load mpiphases\n");
    fflush(stdout);
    tsim_load_relation(task);


    printf("TASKSIM -- load sim params\n");
    fflush(stdout);
    tsim_phase *ph;
    for (ph=task->phase_init; ph != NULL ; ph= ph->next) {    
      tsim_load_phase_sim_params(task, ph);
    }
 
    if(is_tasksim_presim_enabled()){
      printf("TASKSIM -- get presimulation\n");
      fflush(stdout);
      for (ph=task->phase_init; ph != NULL ; ph= ph->next) {    
        if (ph->sim.presimulated) {
          ph->sim.time = tsim_get_presimulation(task->taskid, ph);
        }
      }
     printf("[TASKSIM] Rank %d presimulations loaded \n",task->taskid+1);
     fflush(stdout);
    }

   
    // remove INIT phaase, go to next phase
    task->phase_current = task->phase_current->next;


    //DEBUG
    /*
    printf("TASKSIM -- show phase info\n");
    for (ph=task->phase_init; ph != NULL ; ph= ph->next) {    
        tsim_show_phase_info(task->taskid,ph);
    }
    */
 

    return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void TASKSIM_PreInit(void){
  tasksim_get_environment_vars();

  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// INPUT PARAMETERS ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable_config(const char* params)
{
  if (params != NULL) {
    tasksim_config_file = strdup(params);
    tasksim_config = TRUE;
  }
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable_paraver(const char* params) {
  if (params != NULL) {
    param_temp_folder = strdup(params);
    tasksim_paraver = TRUE;
  }
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable_presim_file(const char* params) {
  if (params != NULL) {
    param_presim_file = strdup(params);
    tasksim_presim_file = TRUE;
  }
  return;

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable_trace_folder(const char* params) {
  if (params != NULL) {
    param_trace_folder = strdup(params);
    param_trace_phases = strdup(params);
    tasksim_trace_folder = TRUE;
  }
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable_trace_name(const char* params) {
  if (params != NULL) {
    param_prefix_trace = strdup(params);
    tasksim_trace_name = TRUE;
  }
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tasksim_enable(const char* params)
{
  if (params != NULL) {
    if (!strcmp(params, "online")) {
      tasksim_online = TRUE;
    }
    if (!strcmp(params, "presim")) {
      tasksim_presim = TRUE;
    }
  }

  if (!tasksim_presim && !tasksim_online) {
    printf("Error, using unknown tasksim mode; -tasksim [online/presim]\n");
    tasksim_usage();
    exit (-1);
  }

  tasksim_enabled = TRUE;
  return;
}



//////////////////////////
void tasksim_usage (void){
  printf("[DIMEMAS] ------------------------------------------\n");
  printf("[DIMEMAS] Usage: Dimemas -tasksim plugin            \n");
  printf("[DIMEMAS] ------------------------------------------\n");
//  printf("[DIMEMAS] ONLINE;                                   \n");
//  printf("[DIMEMAS] Required arguments:                       \n");
//  printf("[DIMEMAS]   -tasksim          online                \n");
//  printf("[DIMEMAS]   -ts_trace_folder  [trace_folder]        \n");
//  printf("[DIMEMAS]   -ts_trace_name    [binary_name]         \n");
//  printf("[DIMEMAS]   -ts_config        [tasksim_config_file] \n");
//  printf("[DIMEMAS]   {-ts_prv          [output:prv_folder]}  \n");
//  printf("[DIMEMAS]\n");
  printf("[DIMEMAS] PRESIM (after presimulation computation)  \n");
  printf("[DIMEMAS] Required arguments:                       \n");
  printf("[DIMEMAS]   -tasksim          presim                \n");
  printf("[DIMEMAS]   -ts_trace_folder  [trace_folder]        \n");
  printf("[DIMEMAS]   -ts_trace_name    [binary_name]         \n");
  printf("[DIMEMAS]   -ts_presim_file   [tasksim_presim_file] \n");
  printf("[DIMEMAS]   {-ts_prv          [input:prv_folder]}   \n");
  printf("[DIMEMAS] ------------------------------------------\n");
  return;
}


/////////////////////////////
// TODO: Check parameters, not implemented yet, just the structure.
void tasksim_check_options (void) {

  // here show report of env vars and tasksim options if enabled.
  if (!is_tasksim_enabled()) return;

  if (!is_tasksim_paraver_enabled()){
    param_temp_folder = strdup(".");   
  }


  printf("=========================================================\n");
  printf("TASKSIM plugin is enabled\n");
  printf("=========================================================\n");
  if (is_tasksim_online_enabled()) {
      printf("Sorry but online is enabled but currently unsupported\n");
      exit(-1);
      printf("TaskSim cconfig file %s\n",ts_get_config());
      printf("Paraver traces are stored in %s\n",param_temp_folder);
  } else {
      // There is no need to check that is presim mode. Previously checked
    if (is_tasksim_paraver_enabled()) {
      printf("Presimulation mode with paraver integration\n");
      printf("Paraver traces are stored in %s\n",param_temp_folder);
    }else{
      printf("Presimulation mode\n");
      // this should be the most common way.
    }
  }
 
  if (ts_sim_mem == 1) {
    printf("Dimemas memory mode is enabled.\n");
    if (ts_mem_ini != 0 || ts_mem_fin != 0 ){
      if (ts_mem_ini > ts_mem_fin) {
        int tmp= ts_mem_ini;
        ts_mem_ini = ts_mem_fin;
        ts_mem_fin= tmp;
      }
      printf("Memory filtering is set from %d to %d\n", ts_mem_ini, ts_mem_fin);
    }
    if (ts_keep_log == 1 && is_tasksim_online_enabled()) {
      printf("Logs for memory simulation are going to be keept\n");
    } 
  }

  if (ts_rng_ini != 0 || ts_rng_fin != 0) {
    if (ts_rng_ini > ts_rng_fin) {
      int tmp = ts_rng_ini;
      ts_rng_ini = ts_rng_fin;
      ts_rng_fin = ts_rng_ini;
    }
    printf("Burst mode filtering from phase #%d up to %d\n",ts_rng_ini, ts_rng_fin);

  }

  printf("Current tasksim simulation considers %d cores per rank\n",ts_online_cpus);
  printf("=========================================================\n");

  return;
}





/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// getters ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
t_boolean is_tasksim_enabled() {
  return tasksim_enabled;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// PARAVER INPUT/OUTPUT (optional)
t_boolean  is_tasksim_paraver_enabled() {
    if (!tasksim_enabled) return 0;
    return tasksim_paraver;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// PRESIMULATION FILE (conditional)
t_boolean is_tasksim_presim_enabled() {
  if (!tasksim_enabled) return 0;
  return tasksim_presim;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// ONLINE SIMULATION (conditional)
t_boolean is_tasksim_online_enabled() {
  if (!tasksim_enabled || ts_online_cpus == 0) return 0;
  return tasksim_online;
}
/////////////////////////////////////////////////////////////////////////////////////////////
char* ts_get_config(void) {
  return (tasksim_config_file);
}
/////////////////////////////////////////////////////////////////////////////////////////////
char* tasksim_get_trace_name(){
  return last_tasksim_paraver_trace;
}
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned int tasksim_get_sim_th(void){
  return ts_num_sim_th;
  return 0;
}
/////////////////////////////////////////////////////////////////////
void tasksim_get_environment_vars (void) { 
  char *cad; 
 
  cad     = getenv("TS_RANGE_INI"); 
  if (cad != NULL) ts_rng_ini = atol(cad); 
  cad     = getenv("TS_RANGE_FIN"); 
  if (cad != NULL) ts_rng_fin = atol(cad); 
  cad =     getenv("TS_MEM_INI"); 
  if (cad != NULL) ts_mem_ini = atol(cad); 
  cad =     getenv("TS_MEM_FIN"); 
  if (cad != NULL) ts_mem_ini = atol(cad); 
  cad     = getenv("TS_SIM_MEM"); 
  if (cad != NULL) ts_sim_mem = atoi(cad); 
  cad     = getenv("TS_SIM_PRV"); 
  if (cad != NULL) ts_sim_prv = atoi(cad); 
  cad     = getenv("TS_SIM_CORES"); 
  if (cad != NULL) ts_online_cpus = atoi(cad); 
  cad     = getenv("TS_SIM_LOG"); 
  if (cad != NULL) ts_keep_log = atoi(cad); 
 
  return; 
}




///////////////////////////////////////////////////////////////////// 
void tsim_load_task_config(struct t_task *task) {
  int task_id = task->taskid;
  tsim_config *config = &task->sim_config;


/*
  // CAA2
  task->phase_counter               = 0;
  task->event_counter               = 0;
  task->phase_init                  = NULL;
  task->phase_end                   = NULL;
  task->phase_current               = NULL;
*/


  if (tasksim_config) { 
    config->filename_config = strdup(tasksim_config_file); 
  } else { 
    config->filename_config = NULL;
  }
  config->filename_effective_config = NULL;
  config->filename_relation         = NULL;
  config->filename_default_trace    = NULL;
  config->filename_streaminfo       = NULL;
  config->full_trace_prefix         = NULL;
  config->prefix_name               = NULL;


  char padrank[7];
  sprintf(padrank,"%06d",task_id+1);


  char prefix[256];
  char short_prefix[256];
  char trace_phases[256];
  char trace_default[256];
  char trace_stream[256];
  memset(prefix,'\0',sizeof(prefix));
  memset(short_prefix,'\0',sizeof(short_prefix));
  memset(trace_phases,'\0',sizeof(trace_phases));
  memset(trace_default,'\0',sizeof(trace_default));
  memset(trace_stream,'\0',sizeof(trace_stream));

  strcpy(prefix,param_trace_folder);
  strcat(prefix,"/");
  strcat(prefix,param_prefix_trace);
  strcat(prefix,"_proc_");
  strcat(prefix,padrank);
  strcat(prefix,".ts");
  //printf("prefix = %s\n",prefix);

  strcat(short_prefix,param_prefix_trace);
  strcat(short_prefix,"_proc_");
  strcat(short_prefix,padrank);
  strcat(short_prefix,".ts");
  
  strcpy(trace_phases,prefix);
  strcat(trace_phases,".mpiphases");
  strcpy(trace_default,prefix);
  strcat(trace_default,".default.trace");
  strcpy(trace_stream,prefix);
  strcat(trace_stream,".streaminfo"); 


  // CAA2:
  config->filename_relation            = strdup(trace_phases);
  config->filename_default_trace       = strdup(trace_default);
  config->filename_streaminfo          = strdup(trace_stream);
  config->full_trace_prefix            = strdup(short_prefix);
  config->prefix_name                  = strdup(prefix);


  fflush(stdout); fflush(stderr);
  printf("[Ptask] %6.d -------------------------------------\n", task_id+1);
  printf("[Ptask] %6.d TASKSIM:\n"                             , task_id+1);
  printf("[Ptask] %6.d \t\t\t config file     : %s\n"          , task_id+1, config->filename_config);
  printf("[Ptask] %6.d \t\t\t relation file   : %s\n"          , task_id+1, config->filename_relation);
  printf("[Ptask] %6.d \t\t\t tracefile file  : %s\n"          , task_id+1, config->filename_default_trace);
  printf("[Ptask] %6.d \t\t\t streaminfo file : %s\n"          , task_id+1, config->filename_streaminfo);
  printf("[Ptask] %6.d \t\t\t prefix          : %s\n"          , task_id+1, config->prefix_name);
  printf("[Ptask] %6.d \t\t\t trace prefix    : %s\n"          , task_id+1, config->full_trace_prefix);
  printf("[Ptask] %6.d -------------------------------------\n", task_id+1);
  fflush(stdout); fflush(stderr);

  task->in_ompss=0;
  task->pending_replacement=0;
  task->phase_to_replace=NULL;
  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tsim_load_environment_delimiters(struct t_task *task) {
  // default values.
  task->sim_config.range_ini   = 0;
  task->sim_config.range_end   = 2e9;
  task->sim_config.mem_ini     = 0;
  task->sim_config.mem_end     = 2e9;
  task->sim_config.mem_enable  = 0;

  // check simulation ranges
  if (ts_rng_ini <= ts_rng_fin && ts_rng_fin != 0) {
    task->sim_config.range_ini   = ts_rng_ini;
    task->sim_config.range_end  = ts_rng_fin;

    if (ts_sim_mem) {
      task->sim_config.mem_enable = 1;
      // default range
      task->sim_config.mem_ini    = ts_rng_ini;
      task->sim_config.mem_end    = ts_rng_fin;
      // check memory range is between default ranges.
      if (ts_mem_ini <= ts_mem_fin &&
            ts_mem_fin != 0 &&
            ts_mem_ini >= ts_rng_ini &&
            ts_mem_ini <= ts_rng_fin &&
            ts_mem_fin >= ts_rng_ini &&
            ts_mem_fin <= ts_rng_fin) {
          task->sim_config.mem_ini = ts_mem_ini;
          task->sim_config.mem_end = ts_mem_fin;
      }
    }
  }
  return;
}





/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tsim_load_relation(struct t_task *task) {
  FILE *fp;
  unsigned MAX_LINE_SIZE = 256;
  char *line = (char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
  char *saveptr;
  int counter = 0;
  tsim_phase *ph;

  if ((fp = fopen(task->sim_config.filename_relation, "r")) == NULL) {
      printf("Error Opening Relation File [%s]\n", task->sim_config.filename_relation);
      exit(0);
  }

  // printf("Opening Relation File [%s]\n", task->sim_config.filename_relation);

  // read new line and split elements (X:Y:Z)
  while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
    ph = (tsim_phase*) malloc(sizeof(tsim_phase));

    if (ph == NULL) {
        printf("Error allocating memory for phases \n");
        exit(0);
    }

    ph->position = counter;
    counter++;

    // replace newline to end of string
    char *cad = strchr(line, '\n');
    if (cad) *cad = 0;

    // split line using : delimiter.
    cad = strtok_r(line, ":", &saveptr);
    ph->phase_num = atol(cad);
    cad = strtok_r(NULL, ":", &saveptr);
    if (cad == NULL) {
        printf("Error Accessing Relation Data\n");
        exit(0);
    }

    ph->master_id = atol(cad);
    cad = strtok_r(NULL, ":", &saveptr);
    if (cad == NULL) {
        printf("Error Accessing Relation Data\n");
        exit(0);
    }

    ph->ompss_flag = atoi(cad);
    cad = strtok_r(NULL, ":", &saveptr);
    if (cad == NULL) {
        printf("Error Accessing Relation Data\n");
        exit(0);
    }

    ph->mem_flag = atoi(cad);
    cad = strtok_r(NULL, ":", &saveptr);
    if (cad == NULL) {
        printf("Error Accessing Relation Data\n");
        exit(0);
    }

    ph->mpi_code = atoi(cad);
    cad = strtok_r(NULL, ":", &saveptr);
    if (cad == NULL) {
        printf("Error Accessing Relation Data\n");
        exit(0);
    }

    ph->mpi_name = strdup(cad);

    //printf("LOAD %d %d %d %d %d %s\n", ph->phase_num, ph->master_id, ph->ompss_flag,ph->mem_flag, ph->mpi_code, ph->mpi_name);


    // connect the node
    ph->next = NULL;
    if (task->phase_init == NULL) {
      ph->prev = NULL;
      task->phase_init = ph;
      task->phase_current = ph;
    } else {
        task->phase_end->next = ph;
    }
    ph->prev = task->phase_end;
    task->phase_end = ph;
    task->phase_counter++;
  }
  fclose(fp);
  free(line);
  return;
}




/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tsim_load_phase_sim_params(struct t_task *task, tsim_phase *ph) {
  // for the new phase create the simulation parameters
  int in_range      = 0;
  int in_mem_range  = 0;
  int task_id       = task->taskid;

  ph->sim.mode = 0;
  ph->sim.pending = 0;
  ph->sim.to_simulate = 0;
  ph->sim.presimulated = 0;
  ph->sim.paraver_file[0] = '\0';

  // check if phase between ranges
  if (ph->ompss_flag && \
    ph->phase_num >= task->sim_config.range_ini && \
    ph->phase_num <= task->sim_config.range_end) {
          in_range = 1;
  }

  // check if phase is memory and should be memory simulated
  if ( in_range && \
      task->sim_config.mem_enable && \
      ph->mem_flag && \
      ph->phase_num >= task->sim_config.mem_ini && \
      ph->phase_num <= task->sim_config.mem_end) {
          in_mem_range = 1;
  }

  // set up the triggers for simulation
  if (in_range) {
    ph->sim.pending = 1;
    if (in_mem_range) {
      ph->sim.mode = 1;
    }
  }

  // compose simulation parameters
  if (ph->sim.pending) {
    sprintf(ph->sim.out_file,
                "%s/%s_%06d_%s.simout",
                param_temp_folder,
                task->sim_config.prefix_name,
                ph->master_id,
                (ph->sim.mode) ? "MEMO":"BRST");

    sprintf(ph->sim.in_config_file,
                 "%s/%s_%06d_%s.conf",
                 param_temp_folder,
                 task->sim_config.prefix_name,
                 ph->master_id,
                 (ph->sim.mode) ? "MEMO":"BRST");

    sprintf(ph->sim.paraver_file,
                 "%s/%s_%06d_%s.conf-0001.prv",
                 param_temp_folder,
                 task->sim_config.prefix_name,
                 ph->master_id,
                 (ph->sim.mode) ? "MEMO":"BRST");


    ph->sim.out_file_exists   = 0;
    ph->sim.in_config_exists  = 0;
    ph->sim.paraver_exists    = 0;
    ph->sim.presimulated      = 0;

    if (access(ph->sim.out_file        , F_OK) != -1) {ph->sim.out_file_exists  = 1;}
    if (access(ph->sim.in_config_file  , F_OK) != -1) {ph->sim.in_config_exists = 1;}
    if (access(ph->sim.paraver_file    , F_OK) != -1) {ph->sim.paraver_exists   = 1;}


    if (is_tasksim_presim_enabled()) {
      if (is_tasksim_paraver_enabled()) {
        if (ph->sim.paraver_exists) {
          ph->sim.presimulated = 1;
        } else {
          // At this point we used to re-simulate the miss presimulation phase.
          // Now we crash. It is not allowed a mix. Presim or Online (one or the other)
          printf("[DIMEMAS] Missing presimulated paraver files [%s]\n",ph->sim.paraver_file);
          exit(-1);
        }
      } else {
        // if is ompss.
        if (ph->ompss_flag) {
          ph->sim.presimulated = 1;
        }
      }
    }else if (is_tasksim_online_enabled()) {
      if (ph->ompss_flag) {
        ph->sim.to_simulate = 1;
      }
    }
  }


  return;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void tsim_show_phase_info (int task_id, tsim_phase *ph){
  
  fflush(stdout); fflush(stderr);
  // PRESIMULATED
  if (ph->sim.presimulated) {
      printf("\n[PRE_SIM]");
      printf("[Ptask] %6.d\t\t PRESIMULATION WITH TASKSIM on phase %d\n"  , task_id+1, ph->phase_num);
      printf("[Ptask] %6.d\t\t\t TS Config.      : \t%s\n"                , task_id+1, ph->sim.in_config_file);
      printf("[Ptask] %6.d\t\t\t Output file     : \t%s\n"                , task_id+1, ph->sim.out_file);
      printf("[Ptask] %6.d\t\t\t Paraver trace   : \t%s\n"                , task_id+1, ph->sim.paraver_file);
      printf("[Ptask] %6.d\t\t\t Simulated flag  : \t%d\n"                , task_id+1, ph->sim.presimulated);
      printf("[Ptask] %6.d\t\t\t Simulated time  : \t[%15.0f]\n"                , task_id+1, ph->sim.time);
  } else {
    // ONLINE SIMULATION
    if (ph->sim.to_simulate) {
      printf("\n[ONLINE_SIM]");
      printf("[Ptask] %6.d\t\t SIMULATION WITH TASKSIM on phase %d\n"  , task_id+1, ph->phase_num);
      printf("[Ptask] %6.d\t\t\t TS Config.      : \t%s\n"   , task_id+1, ph->sim.in_config_file);
      printf("[Ptask] %6.d\t\t\t Output file     : \t%s\n"   , task_id+1, ph->sim.out_file);
      printf("[Ptask] %6.d\t\t\t Paraver trace   : \t%s\n"   , task_id+1, ph->sim.paraver_file);
      printf("[Ptask] %6.d\t\t\t Simulated flag  : \t%d\n"   , task_id+1, ph->sim.presimulated);
      printf("[Ptask] %6.d\t\t\t Simulation Mode : \t%s\n"   , task_id+1, ph->sim.mode ? "MEMORY":"BURST_ONLY");
    }
  }
  fflush(stdout); fflush(stderr);


  return;
}




/////////////////////////////////////////////////////////////////////
double tsim_get_presimulation(int taskid, tsim_phase *ph) {
  // In contrast to the previous version, this one directly reads
  // the presimulatin results file to obtain the time.
  // this only can be done if a presim results file has been provided

  double result  = get_presimulation_cpp(param_presim_file, taskid, ph);

  return result; 
  fflush(stdout);



  int found = 0;
  double time =  0;
  char line[256];
  char *saveptr;

  FILE *fp = fopen (param_presim_file,"r");
  if (fp== NULL){ 
    printf("Error accessing presimulation file\n");
    exit(-1);
  }

  // search for rank and phase
  //printf("Search for rank %d and master_id %d\n", taskid, ph->master_id);

  while (fgets(line, sizeof(line), fp) != NULL && !found) {
    // replace newline to end of string
    char *cad = strchr(line, '\n');
    if (cad) *cad = 0;

    // split line using : delimiter.
    cad = strtok_r(line, ":", &saveptr);
    int rank = atoi(cad);
    if (rank == taskid+1){
      cad = strtok_r(NULL, ":", &saveptr);
      long phase = atol(cad);
      if (phase == ph->master_id) {
        found = 1;
        cad = strtok_r(NULL, ":", &saveptr);
        cad = strtok_r(NULL, ":", &saveptr);
        cad = strtok_r(NULL, ":", &saveptr);
        time = atof(cad);
      }
    }
  }
  fclose (fp);

  if (!found) {
    printf("[DIMEMAS] Error, time not found on presimulation file for "
                "Rank %d and master_id %d\n", taskid+1, ph->master_id);
    exit(-1);
  }


  printf("FOUND %8d %8d %12.0f\n", taskid+1, ph->master_id, time);
exit(-1);
  return time;
}



