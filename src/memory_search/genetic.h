/****************************************************************
			Selection for Mating
*****************************************************************/
Individual* TournamentSelection(Population *p)
{
	int i, selected_individual, current_fitness = 0,best_fitness = 0, ind_best_fitness;
	
	for(i=0;i<TOURNAMENT_SIZE; i++)
	{
		selected_individual = (int) (rand() % POPULATION_SIZE);
		
		current_fitness = p->individuals[selected_individual].fitness_value;
		
		if(best_fitness == 0)
		{
			best_fitness = current_fitness;
			ind_best_fitness = selected_individual;
		}else if(current_fitness < best_fitness){
			best_fitness = current_fitness;
			ind_best_fitness = selected_individual;
		}
	
		//printf("Selected Indiviudal Fitness Value: %d \n",p->individuals[selected_individual].fitness_value);
	}

	//printf("Best Fitted Individual Ind: %d, Value: %d \n",ind_best_fitness, best_fitness);

	return &p->individuals[ind_best_fitness];
}
/****************************************************************
				Enf of Selection for Mating
*****************************************************************/


/****************************************************************
			Crossover
*****************************************************************/

//This function cuts offsprings with same two points. Firstly, cut points are determined randomly, and the points are ordered.
//After that, the subtour values between cut points are exchanged in order.
void cutAndExchangeSubTour(Individual *offspring1, Individual *offspring2, int* from_subtour, int* to_subtour)
{
	int cut1, cut2, tmp_value, i;

	cut1 = (int) (rand() % COUNT_OF_NODES);
	cut2 = (int) (rand() % COUNT_OF_NODES);
	
	//order cuts
	if( cut1 <= cut2 ){
		*from_subtour = cut1;
		*to_subtour = cut2;
	}else if(cut1 > cut2){
		*from_subtour = cut2;
		*to_subtour = cut1;
	}
	
	//Exchange
	for(i = *from_subtour; i < *to_subtour; i++)
	{
		tmp_value = offspring2->values[i];
		offspring2->values[i] = offspring1->values[i];
		offspring1->values[i] = tmp_value;
	}
}

//Permutation violation can be exist only on subtour because the remaining part is already permuted. 
//New values coming from other individual may duplicate and break permatution. 
//Hence, we only navigate on subtour which is the part of between cut points.
//This function is a recursive function. After a violation and assignment of the mapped value, 
//it invokes itself by providing new pivot value just assigned and other parameters which is same. 
void resolvePermutationConflicts(Individual* offspring1, Individual* offspring2, int pivot_index, int from_subtour, int to_subtour)
{
	int i;	
	for(i=from_subtour; i<to_subtour;i++)
	{
		if(offspring1->values[pivot_index] == offspring1->values[i])
		{	
			offspring1->values[pivot_index] = offspring2->values[i];
			resolvePermutationConflicts(offspring1, offspring2, pivot_index, from_subtour,to_subtour);
		}
		
		if(offspring2->values[pivot_index] == offspring2->values[i])
		{	
			offspring2->values[pivot_index] = offspring1->values[i];
			resolvePermutationConflicts(offspring1, offspring2, pivot_index, from_subtour,to_subtour);
		}
	}
	return;
}

//This function navigates on beginning to first cutpoint and second cut point to last element.
//Each node of individual is assessed whether it has a permuation break. 
//If it is so, then in order to fixing, partial maps are used.
void validationAndPartialMapping(Individual *offspring1, Individual * offspring2, int* from_subtour, int* to_subtour)
{
	int i;

	for(i=0;i<*from_subtour;i++)
	{
		 resolvePermutationConflicts(offspring1, offspring2, i, *from_subtour, *to_subtour);	
	}

	for(i=*to_subtour;i<COUNT_OF_NODES;i++)
	{
		 resolvePermutationConflicts(offspring1,offspring2, i, *from_subtour, *to_subtour);
	}
}


//Partially mapped crossover method has been applied here.
//Firstly we create fresh offsprings, because parents remain in population without no-change.
void crossover(Individual *offspring1, Individual *offspring2 )
{
	int from_subtour, to_tour;	
	cutAndExchangeSubTour(offspring1, offspring2, &from_subtour, &to_tour);
	validationAndPartialMapping(offspring1, offspring2,&from_subtour, &to_tour);	
	
	offspring1->fitness_value = calcIndividualFitness(offspring1);
	offspring2->fitness_value = calcIndividualFitness(offspring2);
}
/****************************************************************
			End of Crossover
*****************************************************************/



/****************************************************************
			Mutations
*****************************************************************/
void  swapMutation(Individual * candidate_solution)
{
	int i, j, tmp;

	i=rand()%COUNT_OF_NODES;
	j=rand()%COUNT_OF_NODES;

	tmp = candidate_solution->values[i];
	candidate_solution->values[i] = candidate_solution->values[j];
	candidate_solution->values[j] = tmp;

	candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);
}

void insertMutation(Individual* candidate_solution){
	int i, j, low, up, tmp, counter;

	i=rand()%COUNT_OF_NODES;
	j=rand()%COUNT_OF_NODES;

	//order i, j
	if(i < j){
		low = i;
		up = j;
	}else{
		low = j;
		up = i;
	}

	for(counter = up; counter>low+1;counter--)
	{
		tmp = candidate_solution->values[counter];
		candidate_solution->values[counter] = candidate_solution->values[counter-1];
		candidate_solution->values[counter-1] = tmp;
	}

	candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);
}

void  inverseMutation(Individual * candidate_solution)
{
	int i, j, num_of_swap, low, up, tmp, counter;

	i=rand()%COUNT_OF_NODES;
	j=rand()%COUNT_OF_NODES;

	//order i, j
	if(i < j){
		low = i;
		up = j;
	}else{
		low = j;
		up = i;
	}

	num_of_swap = ((up - low + 1)/2);

	for(counter = 0; counter < num_of_swap; counter++)
	{
		tmp = candidate_solution->values[up-counter];
		candidate_solution->values[up-counter] = candidate_solution->values[low+counter];
		candidate_solution->values[low+counter] = tmp;	
	}

	candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);
}

/****************************************************************
			End of Mutations
*****************************************************************/


/****************************************************************
			Survival of Fittest 
*****************************************************************/

void survivalSelectionAndInsertion(Population *p, Individual *offspring1, Individual * offspring2)
{
	int i, current_fitness, worst_fitness, index_worst_fitness, second_worst_fitness, index_second_worst_fitness;	
	worst_fitness = 0;second_worst_fitness = 0;
	index_second_worst_fitness = 0; index_worst_fitness = 0;

	for(i=0;i<POPULATION_SIZE;i++)
	{
		current_fitness = p->individuals[i].fitness_value;
		if(current_fitness > worst_fitness )
		{
			index_second_worst_fitness = index_worst_fitness;
			index_worst_fitness = i;
			
			second_worst_fitness = worst_fitness;
			worst_fitness = current_fitness;

		}else if(current_fitness > second_worst_fitness)
		{
			second_worst_fitness = current_fitness;
			index_second_worst_fitness = i;
		}
	}

	/*printf("\nWorst: %d \n",p->individuals[index_worst_fitness].fitness_value);
	printf("\nSeconf Worst: %d \n",p->individuals[index_second_worst_fitness].fitness_value);*/

	//New offsprings are copied to two worst individuals.
	setIndividualValues(offspring1, &p->individuals[index_worst_fitness]);
	setIndividualValues(offspring2, &p->individuals[index_second_worst_fitness]);

	//After copy, obsolute offsprings are deallocated
	free(offspring1);
	free(offspring2);
}
