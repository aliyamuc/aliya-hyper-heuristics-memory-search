#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "util.h"

int bootstrap_memory_search();
int bootstrap_hyper_heuristic();


int main(){
	int algorithm;
	int selection_strategy;
	int move_acceptance;

	printf("Which algorithm you will operate on: \n1 for Hyper-heuristics, 2 for Memory/Search Algorithms:\nEnter: "); 
	scanf("%d",&algorithm);
	if(algorithm == 1){
		printf("Which selection strategy?\n0 SR, 1 RD, 2 RP, 3 RPD, 4 CF\nEnter: "); 
		scanf("%d",&selection_strategy);
		printf("\Which move acceptance criteria?\n0 AM, 1 OI, 2 IE, 3 MC\nEnter: "); 
		scanf("%d",&move_acceptance);
		bootstrap_hyper_heuristic(selection_strategy, move_acceptance);
	}else{
		bootstrap_memory_search();
	}

	printf("\nThis is bootstrap program\n");
	system("pause");
	return 0;
}

int bootstrap_hyper_heuristic(	int selection_strategy, int move_acceptance){

	int i,j,k;
	char executed_program_name[20];
	char path_of_memory_search[150], parameters[120];
	char buffer[20];
	
	srand ((unsigned int) time(NULL) );

	strcpy_s(executed_program_name, sizeof( executed_program_name ),"hyper_heuristic.exe");

	//Severty of Change in 5 level
	for(i=1;i<=5;i++)
	{
		//Frequency of Change in 5 level
		for(j=1;j<=5;j++)
		{
		
			for(k=1;k<=1;k++) //iterate with 100 different seeds 
			{
				strcpy_s(path_of_memory_search, sizeof( path_of_memory_search ),executed_program_name);
				strcpy_s(parameters,sizeof(parameters)," "); 

				 _itoa_s(i,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				  _itoa_s(j,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				   _itoa_s(selection_strategy, buffer, sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				  _itoa_s(move_acceptance,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				   _itoa_s(k,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				 _itoa_s(rand(),buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(path_of_memory_search,sizeof(path_of_memory_search), parameters);

				printf("\nExecution string with parameters:  %s  \n\n",path_of_memory_search);
		
				//If file exist
				if(file_exists(executed_program_name) ) {
		
					system(path_of_memory_search);
					printf("Level of Severity of Change: %d, Level of Frequency of Change: %d\n",i,j);
					system("pause");
				}else{
					printf("file not exists");
					return -1;
				}
			}		
		}
	}
	return 1;
}

int bootstrap_memory_search(){
	int i,j,k;
	char executed_program_name[20];
	char path_of_memory_search[100], parameters[80];
	char buffer[20];
	
	srand ((unsigned int) time(NULL) );

	strcpy_s(executed_program_name, sizeof( executed_program_name ),"memory_search.exe");

	//Severty of Change in 5 level
	for(i=1;i<=5;i++)
	{
		//Frequency of Change in 5 level
		for(j=1;j<=5;j++)
		{
			for(k=1;k<=5;k++) //iterate with 100 different seeds 
			{
				strcpy_s(path_of_memory_search, sizeof( path_of_memory_search ),executed_program_name);
				strcpy_s(parameters,sizeof(parameters)," "); 

				 _itoa_s(i,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				 _itoa_s(j,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				 _itoa_s(k,buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(parameters, sizeof(parameters), " ");

				 _itoa_s(rand(),buffer,sizeof(buffer),10); 
				 strcat_s(parameters, sizeof(parameters), buffer);
				 strcat_s(path_of_memory_search,sizeof(path_of_memory_search), parameters);

				printf("\nExecution string with parameters:  %s  \n\n",path_of_memory_search);
		
				//If file exist
				if(file_exists(executed_program_name) ) {
		
					system(path_of_memory_search);

				}else{
					printf("file not exists");
					return -1;
				}
			}	
		}
	}

	return 1;
}