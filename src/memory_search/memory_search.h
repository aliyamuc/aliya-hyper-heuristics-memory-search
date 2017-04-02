
int isMemoryFull(){
	if(SIZE_OF_MEMORY_POPULATION == MEMORY_SIZE)
		return 1;
	else
		return 0; 
}


int findWorstIndividiualInMindistIndividuals(Memory *m)
{
	int i,j, diff, min_dist, min_dist_worse ,min_dist_worst, min_dist_worst_index, min_dist_better_index, min_dist_worse_index;

	min_dist = INT_MAX;
	min_dist_worst = 0;
	min_dist_worst_index = 0;

	//Find mindist individuals in the memory firstly then find worst individual among them.
		for(i=0; i<SIZE_OF_MEMORY_POPULATION	- 1; i++)
		{
			for(j=i+1; j<SIZE_OF_MEMORY_POPULATION; j++)
			{
				diff = abs(m->individuals[i].fitness_value	- m->individuals[j].fitness_value); 
					
				if(diff <= min_dist )
				{
					min_dist = diff;

					if(m->individuals[i].fitness_value < m->individuals[j].fitness_value){

						min_dist_worse			= m->individuals[j].fitness_value;
						min_dist_better_index	= i;
						min_dist_worse_index	= j;

					}else{
						min_dist_worse =  m->individuals[i].fitness_value;
						min_dist_better_index	= j;
						min_dist_worse_index	= i;
					}

					if(min_dist_worse > min_dist_worst)
					{
						min_dist_worst = min_dist_worse;
						min_dist_worst_index = min_dist_worse_index;
					}
					
				}
			}
		}

		return min_dist_worst_index;
}

void updateMemory(Memory *m, Individual* best){
	int min_dist_worst_index;

	
	if(!isMemoryFull())
	{
		m->individuals[SIZE_OF_MEMORY_POPULATION] = *createNewIndividual();

		setIndividualValues( best , &m->individuals[SIZE_OF_MEMORY_POPULATION]);

		SIZE_OF_MEMORY_POPULATION++;

	}else{

		//THE WORST ONE in mindist individuals in the memory is found.
		min_dist_worst_index = findWorstIndividiualInMindistIndividuals(m);

		//replaced only when min_dist_worst is worse than the new individual, otherwise the new individual is discarded.
		if(m->individuals[min_dist_worst_index].fitness_value > best->fitness_value )
		{
			//The new individual is replaced.
			setIndividualValues(best,&m->individuals[min_dist_worst_index]);
		}

	}
}


Population* merge_memory_and_memory_population(Population *mp, Memory *m){
	int i;	
	Population* merged_population;

	SIZE_OF_MERGED_POPULATION = (POPULATION_SIZE + SIZE_OF_MEMORY_POPULATION);

	merged_population = (Population*) malloc(sizeof(Population));
	merged_population->individuals = (Individual*) malloc (sizeof (Individual) * (SIZE_OF_MERGED_POPULATION) );

	for(i=0; i< POPULATION_SIZE; i++)
	{
		merged_population->individuals[i] = *createNewIndividual();
		setIndividualValues(&mp->individuals[i], &(merged_population->individuals[i]));
	}

	for(i=0; i< SIZE_OF_MEMORY_POPULATION; i++)
	{
		merged_population->individuals[POPULATION_SIZE + i] = *createNewIndividual();
		setIndividualValues(&mp->individuals[i], &(merged_population->individuals[POPULATION_SIZE + i]));
	}
	

	return merged_population; 
}


void reEvaluateMergedPopulation(Population *p)
{
	int i;

	for(i=0; i< SIZE_OF_MERGED_POPULATION; i++)
	{
		p->individuals[i].fitness_value = calcIndividualFitness(&(p->individuals[i]));
	}
}
void reEvaluateExplicitMemory(Memory *m)
{
	int i;

	for(i=0; i< SIZE_OF_MEMORY_POPULATION; i++)
	{
		m->individuals[i].fitness_value = calcIndividualFitness(&(m->individuals[i]));
	}
}


void constituteMemoryPopulation(Population *merged_p,Population *memory_p)
{
	int i;

	for(i=0; i< POPULATION_SIZE; i++)
	{
		setIndividualValues( &(merged_p->individuals[i]), &(memory_p->individuals[i] ));
	}
}

void deAllocateMemoryforPopulation(Population* merged_population){
	free(merged_population->individuals);
	free(merged_population);
}

void reInitializeSearchPopulation(Population* search_population){

	int i;
	Individual* new_individual;
	for(i=0; i< POPULATION_SIZE; i++)
	{
		new_individual = createNewIndividual();
		setIndividualValues( new_individual, &(search_population->individuals[i]) );
		free(new_individual);
	}
}