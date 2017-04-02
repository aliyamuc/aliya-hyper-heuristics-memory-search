#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define _USE_MATH_DEFINES
#include<math.h> 
#include<string.h>
#include "init.h"
#include "init_hyper.h"
#include "util_hyper.h"
#include "low_level_heuristics.h"
#include "hyperheuristics.h"

int report = 0;
int initParameters(int argc, char *argv[]);
void  updateF3gama();


int main(int argc, char *argv[])
{
	Individual* s_new;

	int counter_from_last_change = 1;
	double monte_carlo;
	int diff,r;
	void (*selected_heuristic_strategy)(Individual *) = NULL;
	srand ((unsigned int) time(NULL) );
	
	if(!initParameters(argc, argv)) 
		return -1;



	if(report){
		fpt_best = fopen(report_best_file_name,"a");
		fpt_offlineperformance = fopen(report_offline_perf_file_name,"w");
	}

	//randomly generate initial candidate solution s0
	candidate_solution = createNewIndividual();
	s_new = createNewIndividual();

	//Initilize traffic factor matrix
	changeTrafficFactors(SEVERTY_OF_CHANGE,FL,FU);
	

	for(generation_counter = 1; generation_counter<=COUNT_OF_GENERATION; generation_counter++)
	{
		
		//choose a heuristic h from {H1,H2,..,Hn}
		selected_heuristic_strategy=selectHeuristicStrategy(HS); 
		
		candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);

		setIndividualValues(candidate_solution, s_new);

		//apply Hyper-Heuristic
		//generate a new solution sn=h(s0)
		(*selected_heuristic_strategy)(s_new);
		
		s_new->fitness_value = calcIndividualFitness(s_new);

		
		//Move Acceptance Criteria
		switch (MA)
		{
			case OI: 
					if(s_new->fitness_value < candidate_solution->fitness_value )
						setIndividualValues(s_new, candidate_solution);
				break;
			case AM: 	
					setIndividualValues(s_new, candidate_solution);
				break;
			case IE: 
					if(s_new->fitness_value <= candidate_solution->fitness_value )
						setIndividualValues(s_new, candidate_solution);
				break;
			case MC: //Monte Carlo Case
					if(s_new->fitness_value < candidate_solution->fitness_value )
							setIndividualValues(s_new, candidate_solution);
					else{ //give a second chance

						diff =  candidate_solution->fitness_value - s_new->fitness_value;
						monte_carlo = exp((double) diff * counter_from_last_change / FREQUENCY_OF_CHANGE);
						r = rand();
						if(r  < monte_carlo){
							setIndividualValues(s_new, candidate_solution);
						}
					}
				break;
		}

		//!!!UPDATE PARAMETERS FOR CHOICE FUNCTION!!!
		updateF3gama();

		if(candidate_solution->fitness_value < best_so_far )
			best_so_far = candidate_solution->fitness_value;

		sum_of_best_so_far_fitness += best_so_far;
		average_best_so_far_fitness =	(int) sum_of_best_so_far_fitness / generation_counter;

		if(report){
			//Report
			fprintf(fpt_best,"%d %d\n", generation_counter, best_so_far);
			fprintf(fpt_offlineperformance,"%d %d\n", generation_counter, average_best_so_far_fitness);
		}

		counter_from_last_change++;
		//printf("Current Fitness Value: %d\n",candidate_solution->fitness_value);
		printf("Best so Far: %d\n",best_so_far);

		//Change Event
		if( (generation_counter % FREQUENCY_OF_CHANGE) == 0)
		{
			counter_from_last_change = 0;
			changeTrafficFactors(SEVERTY_OF_CHANGE,FL,FU);	
		}

	}

	printf("Avg. Best so Far: %d\n",average_best_so_far_fitness);

	if(report){
		fclose(fpt_offlineperformance);
		fclose(fpt_best);
	}

	system("pause");
	
	return 0;
}


int initParameters(int argc, char *argv[]){
	char buffer[20];

	if( argc != 7 ) /* argc must be 7 for correct execution */
    {
        /* We print argv[0] assuming it is the program name*/
        printf("usage: %s  SEVERTY_OF_CHANGE FREQUENCY_OF_CHANGE SEED \n", argv[0] );
		system("pause");
		
		return 0;

    }else{

		PARAM_SEVERTY_OF_CHANGE = atoi(argv[1]);
		SEVERTY_OF_CHANGE = BASE_SEVERTY_OF_CHANGE * PARAM_SEVERTY_OF_CHANGE;
		
		PARAM_FREQUENCY_OF_CHANGE = atoi(argv[2]);
		FREQUENCY_OF_CHANGE	= BASE_FREQUENCY_OF_CHANGE * PARAM_FREQUENCY_OF_CHANGE;

		HS = (h_selection_strategy_t)atoi(argv[3]);

		MA = (move_acceptance_t)atoi(argv[4]);

		SEED = atoi(argv[6]);

		srand ((unsigned int) SEED );

		//construct parametric report filename
		strcpy_s(report_best_file_name, sizeof( report_best_file_name ),"report/hh/hh_report_best_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[1]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[2]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[3]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[4]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[5]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),".txt");

		//construct parametric report filename
		strcpy_s(report_offline_perf_file_name, sizeof( report_offline_perf_file_name ),"report/hh/hh_report_offl_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[1]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[2]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[3]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[4]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[5]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),".txt");

	}

	f1_alpha = (rand()/(double)(RAND_MAX + 1));
	f2_beta =  (rand()/(double)(RAND_MAX + 1));
	f3_gama =  (rand()/(double)(RAND_MAX + 1));
	
	return 1;
}

void updateF3gama(){

	//Track num_of_nonimproved_generation
	
	if(candidate_solution->fitness_value > best_so_far )
	{
		non_improved_generation++;
		if(non_improved_generation == non_improved_generation_threshold)
		{
			//Rewarding F3 takes places only if there has been more than n consecutive absolute non-improvements
			//If a local optima trapped, explore more by increasing f3_gama
			f3_gama = f3_gama + f_p;
			non_improved_generation = 0;
		}
	}

}