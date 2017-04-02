/****************************************************************
			Low-level Heuristics
*****************************************************************/

void  swapMutation(Individual * candidate_solution, int isReal);
void  insertMutation(Individual* candidate_solution, int isReal);
void  inverseMutation(Individual * candidate_solution, int isReal);

/*
	Selection of Low-level Heuristic
*/
void  (*selectHeuristic(heuristic_t h))(Individual * candidate_solution, int isReal)
{
	switch(h)
	{
		case SM: return &swapMutation;
		case IM: return &insertMutation;
		case RM: return &inverseMutation;
		default: return &swapMutation;
	}
}

void keepHistory(int isReal, Individual * s_new, heuristic_t h);

void  swapMutation(Individual * s_new, int isReal)
{
	int i, j, tmp;

	i=rand()%COUNT_OF_NODES;
	j=rand()%COUNT_OF_NODES;

	tmp = s_new->values[i];
	s_new->values[i] = s_new->values[j];
	s_new->values[j] = tmp;

	//re-evaluate
	s_new->fitness_value = calcIndividualFitness(s_new);

	keepHistory(isReal,s_new,SM);
}

void insertMutation(Individual* s_new, int isReal){
	int i, j, low, up, tmp, counter, I;

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
		tmp = s_new->values[counter];
		s_new->values[counter] = s_new->values[counter-1];
		s_new->values[counter-1] = tmp;
	}


	//re-evaluate
	s_new->fitness_value = calcIndividualFitness(s_new);

	keepHistory(isReal,s_new,IM);	

}

void  inverseMutation(Individual * s_new, int isReal)
{
	int i, j, num_of_swap, low, up, tmp, counter, I;

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
		tmp = s_new->values[up-counter];
		s_new->values[up-counter] = s_new->values[low+counter];
		s_new->values[low+counter] = tmp;	
	}

	//re-evaluate
	s_new->fitness_value = calcIndividualFitness(s_new);

	keepHistory(isReal,s_new, RM);
}


void keepHistory(int isReal, Individual * s_new, heuristic_t h){
	int I;

	//We are negating here, because of needing computational simplicity.
		//a positive I means that the new solution is better than the previous(minimisation problem)
	
	I = candidate_solution->fitness_value - s_new->fitness_value;

	switch(h)
	{
		case SM: 
			f_SM_last_invoking = generation_counter;
			f_SM_num_of_invoking++;

			f1_SM_sum_value += (int) (pow(f1_alpha, f_SM_num_of_invoking - 1)) * ( I / T);

			if(heuristic_prev != -1)
			{
				f2_h_matrix_num_of_invoking[heuristic_prev][SM]++;
				f2_h_matrix[heuristic_prev][SM] += (int) pow(f2_beta, f2_h_matrix_num_of_invoking[heuristic_prev][SM] - 1) * (I/T);
			}

			heuristic_prev = SM;
			break;
		case IM:
			f_IM_last_invoking = generation_counter;
			f_IM_num_of_invoking++;

			f1_IM_sum_value += (int) (pow(f1_alpha, f_IM_num_of_invoking - 1)) * ( I / T);

			if(heuristic_prev != -1)
			{
				f2_h_matrix_num_of_invoking[heuristic_prev][IM]++;
				f2_h_matrix[heuristic_prev][IM] += (int) pow(f2_beta, f2_h_matrix_num_of_invoking[heuristic_prev][IM] - 1) * (I/T);
			}

			heuristic_prev = IM;
			break;

		case RM: 
			f_RM_last_invoking = generation_counter;
			f_RM_num_of_invoking++;

			f1_RM_sum_value += (int) (pow(f1_alpha, f_RM_num_of_invoking - 1)) * ( I / T);

			if(heuristic_prev != -1)
			{
				f2_h_matrix_num_of_invoking[heuristic_prev][RM]++;
				f2_h_matrix[heuristic_prev][RM] += (int) pow(f2_beta, f2_h_matrix_num_of_invoking[heuristic_prev][RM] - 1) * (I/T);
			}

			heuristic_prev = RM;
			break;
	}
}


