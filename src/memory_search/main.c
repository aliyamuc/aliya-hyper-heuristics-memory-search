#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define _USE_MATH_DEFINES
#include<math.h> 
#include<string.h>
#include "init.h"
#include "init_ms.h"
#include "util.h"
#include "genetic.h"
#include "evolution.h"
#include "memory_search.h"
#include "report.h"

int report = 0;
int initParameters(int argc, char *argv[]);


int main(int argc, char *argv[]){

	Population memory_population;
	Population search_population;
	Population* merged_population;
	Memory m;

	if(!initParameters(argc, argv)) 
		return -1;

	memory_population.individuals = (Individual*) malloc (sizeof (Individual) * POPULATION_SIZE);
	search_population.individuals = (Individual*) malloc (sizeof (Individual) * POPULATION_SIZE);
	m.individuals				  = (Individual*) malloc (sizeof (Individual) * MEMORY_SIZE);

	srand ((unsigned int) time(NULL) );

	if(report)
	{
		fpt_best = fopen(report_best_file_name,"a");
		fpt_offlineperformance = fopen(report_offline_perf_file_name,"w");
	}
	//fprintf(fpt_best,"Generation BestFitness\n");

	generateInitialPopulation(&memory_population);
	generateInitialPopulation(&search_population);
	
	//initilize traffic matrix
	changeTrafficFactors(SEVERTY_OF_CHANGE,FL,FU);

	for(generation_counter = 1; generation_counter	<=	COUNT_OF_GENERATION; generation_counter++)
	{
		memory_best = evolvePopulation(&memory_population);
		search_best = evolvePopulation(&search_population);

		if(memory_best->fitness_value < search_best->fitness_value)
			current_best_fitness = memory_best->fitness_value ;
		else
			current_best_fitness = search_best->fitness_value;

		/*printf("Current Memory Population Best Fitness Value: %d \n",memory_best->fitness_value);
		printf("Current Search Population Best Fitness Value: %d \n",search_best->fitness_value);*/

		
		//Find best so far fitness value
		if(current_best_fitness < best_so_far_fitness )
			best_so_far_fitness = current_best_fitness;

		sum_of_best_so_far_fitness += best_so_far_fitness;
		average_best_so_far_fitness =	(int) sum_of_best_so_far_fitness / generation_counter;

		if(report)
		{
			report_best_fitness(generation_counter, best_so_far_fitness,fpt_best);
			fprintf(fpt_offlineperformance,"%d %d\n", generation_counter, average_best_so_far_fitness);
		}
		//Memory Update Event
		if( (generation_counter % FREQUENCY_OF_MEMORY_UPDATE) == 0)
		{
			if(memory_best->fitness_value < search_best->fitness_value)
				best = memory_best;
			else
				best = search_best;
			
			updateMemory(&m, best);
		}

		//Change Event
		if( (generation_counter % FREQUENCY_OF_CHANGE) == 0)
		{
			//change traffic matrix
			changeTrafficFactors(SEVERTY_OF_CHANGE,FL,FU);	

			//merge memory_population and memory and select best n from n+k
			merged_population = merge_memory_and_memory_population(&memory_population, &m);

			reEvaluateMergedPopulation(merged_population);	
			reEvaluateExplicitMemory(&m);
			sortPopulation(merged_population->individuals, SIZE_OF_MERGED_POPULATION);

			//The best n individuals among merged_population constitute the memory population;
			constituteMemoryPopulation(merged_population, &memory_population);
			
			deAllocateMemoryforPopulation(merged_population);

			//the search population is re-initialized in order to increase the diversity of individuals.
			reInitializeSearchPopulation(&search_population);

		}

	}

	if(report)
	{
		fclose(fpt_offlineperformance);
		fclose(fpt_best);
	}
	printf("average best: %d\n",average_best_so_far_fitness);
	//printf("This is memory/search program\n");

	return 0;
}



int initParameters(int argc, char *argv[]){
	
	char buffer[20];

	if( argc != 5 ) /* argc must be 3 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s  SEVERTY_OF_CHANGE FREQUENCY_OF_CHANGE SEED \n", argv[0] );
		system("pause");
		return 0;
    }else{

		PARAM_SEVERTY_OF_CHANGE = atoi(argv[1]);
		SEVERTY_OF_CHANGE = BASE_SEVERTY_OF_CHANGE * PARAM_SEVERTY_OF_CHANGE;
		
		PARAM_FREQUENCY_OF_CHANGE = atoi(argv[2]);
		FREQUENCY_OF_CHANGE = BASE_FREQUENCY_OF_CHANGE * PARAM_FREQUENCY_OF_CHANGE;

		SEED = atoi(argv[4]);

		//construct parametric report filename
		strcpy_s(report_best_file_name, sizeof( report_best_file_name ),"report/ms/ms_report_best_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[1]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[2]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),"_");
		strcat_s(report_best_file_name,sizeof(report_best_file_name),argv[3]);
		strcat_s(report_best_file_name,sizeof(report_best_file_name),".txt");

		//construct parametric report filename
		strcpy_s(report_offline_perf_file_name, sizeof( report_offline_perf_file_name ),"report/ms/ms_report_offl_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[1]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[2]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),"_");
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),argv[3]);
		strcat_s(report_offline_perf_file_name,sizeof(report_offline_perf_file_name),".txt");

		return 1;
	}
}