
/****************************************************************
			Hyper-Heuristics
*****************************************************************/
void simpleRandom(Individual* candidate_solution);
void randomDescent(Individual * candidate_solution);
void randomPermutation(Individual * candidate_solution);
void randomPermutationDescent(Individual * candidate_solution);
void choiceFunction(Individual * candidate_solution);
/*
	Heuristic Selection Mechanism according to strategy parameter.
*/
void (*selectHeuristicStrategy(h_selection_strategy_t strategy))(Individual *candidate_solution)
{
	switch(strategy)
	{
		case SR: return &simpleRandom;
		case RD: return &randomDescent;
		case RP: return &randomPermutation;
		case RPD:return &randomPermutationDescent;
		case CF:return &choiceFunction;
		default: return NULL;
	}
}


/*  A low-level heuristic is selected randomly based on a uniform
distribution and it is applied once.
*/
void simpleRandom(Individual* candidate_solution)
{
	heuristic_t h;
	void (*selected_heuristic)(Individual *, int isReal); 

	h = (heuristic_t)( (int) rand() % COUNT_OF_HEURISTICS );

	selected_heuristic = selectHeuristic(h);

	(*selected_heuristic)(candidate_solution, 1);
}

/* Same as the SR, but each low level heuristic is applied continuously
until there is no improvement.
*/
void randomDescent(Individual * candidate_solution)
{
	heuristic_t h;
	void (*selected_heuristic)(Individual *, int isReal); 
	int prev_fitness_value = INT_MAX;

	h = (heuristic_t)( (int) rand() % COUNT_OF_HEURISTICS );

	selected_heuristic = selectHeuristic(h);

	while(1) {
		
		prev_fitness_value = candidate_solution->fitness_value;

		(*selected_heuristic)(candidate_solution, 1);

		candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);

		//that mean there is no improvement
		if(candidate_solution->fitness_value > prev_fitness_value )
			break;
	}
}

void randomPermutation(Individual * candidate_solution)
{
	int i;
	int* p_array;
	heuristic_t h;
	void (*selected_heuristic)(Individual *,int isReal); 

	p_array = generatePermutation(COUNT_OF_HEURISTICS);

	for(i=0;i<COUNT_OF_HEURISTICS;i++)
	{
		h = (heuristic_t)( p_array[i] );

		selected_heuristic = selectHeuristic(h);

		(*selected_heuristic)(candidate_solution, 1);	

		candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);
	} 
}

void randomPermutationDescent(Individual * candidate_solution)
{
	int i;
	int* p_array;
	heuristic_t h;
	void (*selected_heuristic)(Individual *, int isReal); 
	int prev_fitness_value = INT_MAX;

	p_array = generatePermutation(COUNT_OF_HEURISTICS);

	for(i=0;i<COUNT_OF_HEURISTICS;i++)
	{
		h = (heuristic_t)( p_array[i] );

		selected_heuristic = selectHeuristic(h);

		while(1) {

			prev_fitness_value = candidate_solution->fitness_value;

			(*selected_heuristic)(candidate_solution, 1);	

			candidate_solution->fitness_value = calcIndividualFitness(candidate_solution);

			//that mean there is no improvement
			if(candidate_solution->fitness_value > prev_fitness_value)
				break;
		}
	} 

}

/****************************************************************
			Choice Function Begins Here
*****************************************************************/
//the amount of CPU time taken is omitted in this implementation
//Because it is a negligible time in miliseconds.
//the varible is for the importance attached to recent performance should be modified.
//I think that there is some confused parts here in Eric's PhD thesis.
int n_f1(heuristic_t h)
{
	switch(h)
	{
		case SM: return f1_SM_sum_value;
		case IM: return f1_IM_sum_value;
		case RM: return f1_RM_sum_value;
		default: return 0;
	}
}

int n_f2(heuristic_t h)
{
	switch(h)
	{
		case SM: return f2_h_matrix[heuristic_prev][0];
		case IM: return f2_h_matrix[heuristic_prev][1];
		case RM: return f2_h_matrix[heuristic_prev][2];
		default: return 0;
	}
}

int n_f3(heuristic_t h)
{
	switch(h)
	{
		case SM: return generation_counter - f_SM_last_invoking;
		case IM: return generation_counter - f_IM_last_invoking;
		case RM: return generation_counter - f_RM_last_invoking;
		default: return 0;
	}
}

void updateP_Q(heuristic_t n1,heuristic_t n2, int H_F[][3])
{
	double v, f3_diff;
	int i, f_n1,f_n2,f3_n1,f3_n2;

	f_n1=0;f_n2=0;
	v	= 0.00000001;

	for(i=0;i<3;i++){
		f_n1 += H_F[(int)n1][i];
		f_n2 += H_F[(int)n2][i];
	}

	f3_n1 = H_F[(int)n1][2];
	f3_n2 = H_F[(int)n2][2];
	
	f3_diff = f3_n2 - f3_n1;

	if(f3_diff == 0)//chekcs to division by zero
		f3_diff = v;

	f_q = ((double)((f_n2 - f_n1)/f3_diff)) + v;

	f_p = ((double)((f_n1 - f_n2)/f3_diff)) + v;

}

void updateEpsilon(int I, heuristic_t h)
{
	int freq;
	switch(h){
		case SM: freq = f_SM_num_of_invoking;break;
		case IM: freq = f_IM_num_of_invoking;break;
		case RM: freq = f_RM_num_of_invoking;break;
		default: freq=0;break;
	}	
	
	if(I == 0)
	{
		f_epsilon = T / (pow((double)COUNT_OF_HEURISTICS,2) * freq);
	}else{
		f_epsilon = (double)(I / ((double) COUNT_OF_HEURISTICS * eval_of_init_s));
	}

}

void updateParameter(int f1_new, int f2_new,int f3_new)
{
	if(maxGfactor(f1_new,f2_new,f3_new)==1)
	{
		//f1_alpha = pow(f1_alpha,1	+	f_epsilon); 
		//An alternatif rewarding function but doesn't give any reasonable improvement. 
		f1_alpha = f1_alpha * (1	+	f_epsilon);
	}else if(maxGfactor(f1_new,f2_new,f3_new)==2)
	{
		//f2_beta = pow(f2_beta,1	+	f_epsilon);
		f2_beta = f2_beta * (1	+	f_epsilon);
	}
	if(f1_alpha < 0) f1_alpha = 0.001;
	if(f2_beta < 0) f2_beta = 0.001;
}

void choiceFunction(Individual * candidate_solution)
{	
	Individual* f_s_new;
	int i, f1_new, f2_new, f3_new, I, f, f_best, f12_best, f13_best;
	heuristic_t h, h_best,h12_best, h13_best;
	void (*selected_heuristic)(Individual *, int isReal); 
	int H_F[COUNT_OF_HEURISTICS][3];

	f_best		= INT_MIN;
	f12_best	= INT_MIN;
	f13_best	= INT_MIN;

	f_s_new = createNewIndividual();
	setIndividualValues(candidate_solution,	f_s_new);

	for(i=0;i<COUNT_OF_HEURISTICS;i++)
	{
		h = (heuristic_t)((int) i);
		
		selected_heuristic = selectHeuristic(h);

		//!!!Apply Heuristic for testing!!!
		(*selected_heuristic)(f_s_new, 0);	

		f_s_new->fitness_value = calcIndividualFitness(f_s_new);
		
		I = candidate_solution->fitness_value - f_s_new->fitness_value;

		f1_new = (int)( ( I / T) + (int) (f1_alpha * n_f1(h)));
		
		f2_new = (int)(( I / T) + (int) (f2_beta * n_f2(h)));

		f3_new = n_f3(h);

		H_F[i][0]=f1_new;	
		H_F[i][1]=f2_new;	
		H_F[i][2]=f3_new;


		f = (int)( (f1_alpha * f1_new) + (f2_beta * f2_new)	+ (f3_gama * f3_new)) ;

		if(f > f_best)
		{
			f_best = f;
			h_best = h;
		}

		//select heuristic that maximize f1 and f2
		if(f1_new + f2_new > f12_best)
		{
			f12_best = f1_new + f2_new;
			h12_best = h;
		}

		if(f1_new + f2_new > f13_best)
		{
			f13_best = f1_new + f3_new;
			h13_best = h;
		}
	}

	//!!!UPDATE PARAMETERS!!!
	updateP_Q(h12_best, h13_best,H_F);


	//If biggest contributor G in F is f1 or f2 then apply low-level heuristic Nj,
	if(maxGfactor(f1_new,f2_new,f3_new)==1 || maxGfactor(f1_new,f2_new,f3_new)==2)
	{
		selected_heuristic = selectHeuristic(h_best);

		(*selected_heuristic)(f_s_new, 0);

		f_s_new->fitness_value = calcIndividualFitness(f_s_new);

		I = candidate_solution->fitness_value - f_s_new->fitness_value;
		
		//!!!UPDATE PARAMETERS!!!
		//f_epsilon is a small number having the same sign as I
		updateEpsilon(I, h_best);

		//!!!UPDATE PARAMETERS!!!
		updateParameter(f1_new, f2_new,f3_new);

	}else if(maxGfactor(f1_new,f2_new,f3_new)==3)
	{
		//select heuristic that maximize f1+f2
		selected_heuristic = selectHeuristic(h12_best);
		
		(*selected_heuristic)(f_s_new, 0);

		f_s_new->fitness_value = calcIndividualFitness(f_s_new);
		I = candidate_solution->fitness_value - f_s_new->fitness_value;

		//punish f3 because it is biggest contributer in the generation.
		if(I>=0)
			f3_gama = f3_gama - f_q;
		else{
			if(heuristic_prev != -1) //If it is -1, means this is first generation.
				selected_heuristic = selectHeuristic((heuristic_t)heuristic_prev);
		
		}
		
	}else if(maxGfactor(f1_new,f2_new,f3_new)==0)
	{
		//In case of tie we apply heuristic Nj
		selected_heuristic = selectHeuristic(h_best);
	}

	//Apply once selected heuristic.
	(*selected_heuristic)(candidate_solution,1);

	//This is initial solution
	if(eval_of_init_s == 0)
		eval_of_init_s = candidate_solution->fitness_value;

}










