//Configuration parameters
int COUNT_OF_GENERATION = 10000;

int FREQUENCY_OF_MEMORY_UPDATE = 10;


int PARAM_SEVERTY_OF_CHANGE = 1;
double BASE_SEVERTY_OF_CHANGE= 0.01;
double SEVERTY_OF_CHANGE= 0.01; //SEVERTY_OF_CHANGE must be in [0,1]

int SEED = 0;


int PARAM_FREQUENCY_OF_CHANGE =1;
int BASE_FREQUENCY_OF_CHANGE = 100;
int FREQUENCY_OF_CHANGE = 100;

int FL = 0;
int FU = 5;

int SIZE_OF_MEMORY_POPULATION = 0;
int SIZE_OF_MERGED_POPULATION = 0;

FILE* fpt_best;
FILE* fpt_offlineperformance;

//Local variables
int generation_counter = 0;
Individual* memory_best;
Individual* search_best;
Individual* best;
int current_best_fitness;
int best_so_far_fitness = INT_MAX;
int sum_of_best_so_far_fitness = 0;
int average_best_so_far_fitness = 0;
char report_best_file_name[50];
char report_offline_perf_file_name[50];
