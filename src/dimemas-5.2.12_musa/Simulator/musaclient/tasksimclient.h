
int           tasksimManageBurst(t_nano*, struct t_thread * , dimemas_timer);
int           tasksimManageBurst_presim(t_nano*, struct t_thread * , dimemas_timer);
int           tasksimManageBurst_online(t_nano*, struct t_thread * , dimemas_timer);
void          tasksim_init_task(struct t_task *);
// the following are accessed from main.c
void          tasksim_enable_config       (const char*);
void          tasksim_enable_paraver      (const char*);
void          tasksim_enable_presim_file  (const char*);
void          tasksim_enable_trace_folder (const char*);
void          tasksim_enable_trace_name   (const char*);
void          tasksim_enable              (const char*);
void          tasksim_usage               (void);
void          tasksim_check_options       (void);

void          TASKSIM_PreInit             (void);



t_boolean     is_tasksim_enabled          (void);
t_boolean     is_tasksim_paraver_enabled  (void);
t_boolean     is_tasksim_presim_enabled   (void);
t_boolean     is_tasksim_online_enabled   (void);
char*         ts_get_config               (void);
char*         tasksim_get_trace_name      (void);
unsigned int  tasksim_get_sim_th          (void);

void          tasksim_get_environment_vars(void);

void          tsim_show_phase_info        (int, tsim_phase*);
double        tsim_get_presimulation      (int, tsim_phase*);



#define MAXSIZE 1024

static char* tasksim_config_file      = NULL;
static char* param_trace_folder       = NULL;
static char* param_prefix_trace       = NULL;
static char* param_temp_folder        = NULL;
static char* param_presim_file        = NULL;
static char* param_trace_phases       = NULL;

static t_boolean tasksim_enabled      = FALSE;    // enabled
static t_boolean tasksim_presim       = FALSE;    // presim mode
static t_boolean tasksim_online       = FALSE;    // online mode
static t_boolean tasksim_presim_file  = FALSE;    // presim presim_file provided
static t_boolean tasksim_config       = FALSE;    // online config provided
static t_boolean tasksim_paraver      = FALSE;    // paraver on
static t_boolean tasksim_trace_name   = FALSE;    // binary name
static t_boolean tasksim_trace_folder = FALSE;    // trace folder



// these names should be changed
static char*        last_tasksim_paraver_trace = NULL; 
static unsigned int ts_num_sim_th     = 0;



// form environmental vars.
static unsigned int ts_rng_ini        = 0;
static unsigned int ts_rng_fin        = 0;
static unsigned int ts_mem_ini        = 0;
static unsigned int ts_mem_fin        = 0;
static unsigned int ts_sim_mem        = 0;
static unsigned int ts_sim_prv        = 0;
static unsigned int ts_online_cpus    = 0;
static unsigned int ts_keep_log       = 0;





//void get_presimulation_cpp (void);
double  get_presimulation_cpp (char *, int , tsim_phase *);




