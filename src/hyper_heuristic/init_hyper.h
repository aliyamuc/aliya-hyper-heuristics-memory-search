#define COUNT_OF_HEURISTICS 3 //Count of cities

//SM: Swap Mutation
//IM: Insert Mutation
//RM: Inverse Mutation
typedef enum {SM, IM, RM} heuristic_t;

//SR: Simple Random 
//RD: Random Descent
//RP: Random Permutation
//RPD:Random Permutation Descent
//CF: Choice Function
typedef enum {SR, RD, RP,RPD, CF} h_selection_strategy_t;

//AM: All Move
//OI: Only Improvement
//IE: Improvement and Equal
//MC: Monte Carlo
typedef enum {AM,OI,IE,MC} move_acceptance_t;

h_selection_strategy_t HS = CF;
move_acceptance_t MA = AM;

int generation_counter;
Individual *candidate_solution;//current solution in each generation

int non_improved_generation = 0;
int best_so_far = INT_MAX;

int eval_of_init_s = 0;

int FL = 0;
int FU = 5;

double SEVERTY_OF_CHANGE= 0.01; //SEVERTY_OF_CHANGE must be in [0,1]
double PARAM_SEVERTY_OF_CHANGE = 1;
double BASE_SEVERTY_OF_CHANGE = 0.01;

int SEED = 0;

int FREQUENCY_OF_CHANGE = 100;
int PARAM_FREQUENCY_OF_CHANGE = 1;
int BASE_FREQUENCY_OF_CHANGE = 100;


int non_improved_generation_threshold = COUNT_OF_HEURISTICS;

int T = 100; //the amount of CPU time used is taken as constant here.
int f1_SM_sum_value = 0;
int f1_IM_sum_value = 0;
int f1_RM_sum_value = 0;

int f2_h_matrix[COUNT_OF_HEURISTICS][COUNT_OF_HEURISTICS] = {0,0};
int f2_h_matrix_num_of_invoking[COUNT_OF_HEURISTICS][COUNT_OF_HEURISTICS] = {0,0};
int heuristic_prev = -1;

int f_SM_last_invoking = 0;
int f_IM_last_invoking = 0;
int f_RM_last_invoking = 0;

int f_SM_num_of_invoking = 0;
int f_IM_num_of_invoking = 0;
int f_RM_num_of_invoking = 0;

double f1_alpha = 0.5;
double f2_beta = 0.5;
double f3_gama = 0.5;

double f_epsilon = 0.1; // infinitesimal number for adjusting f parameters
double f_p = 0.01;
double f_q = 0.02;

FILE* fpt_best;
FILE* fpt_offlineperformance;

char report_best_file_name[600];
char report_offline_perf_file_name[600];

int sum_of_best_so_far_fitness = 0;
int average_best_so_far_fitness = 0;



