
int compareFitness(const void * elem1, const void * elem2) {
  Individual * i1, *i2;
  i1 = (Individual*)elem1;
  i2 = (Individual*)elem2;
  return i1->fitness_value - i2->fitness_value;
}

void sortPopulation(Individual* arr, size_t num) {
	qsort(arr, num, sizeof(Individual), compareFitness);	
}


//A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int arr[], int n )
{
	int i,j;
	 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

void displayNode(Node n)
{
	printf("node id: %d x: %.10f y: %.10f \n",n.id, n.x, n.y);
}

void displayAllNodes()
{
	int i;
	for(i=0; i< COUNT_OF_NODES; i++)
	{
		displayNode(nodes[i]);
	}
}




void displayIndividual(Individual *s)
{
	int i;
	printf("\n-------\n");
	for(i=0; i<COUNT_OF_NODES; i++)
	{
		printf("%d - ",s->values[i]);
	}
	printf("\n-----\n");
}


void displayPopulation(Population *p)
{
	int i;
	for(i=0; i< POPULATION_SIZE; i++)
	{	
		displayIndividual(&p->individuals[i]);		
	}
}

void displayPopulationFitness(Population *p, int SIZE)
{
	int i;
	printf("\n Population Fitness:\n");
	for(i=0;i<SIZE;i++)
	{
		printf("Fitness  of %d. individual: %d\n",i, p->individuals[i].fitness_value);
	}
}

void displayMemory(Memory *k)
{
	int i;
	for(i=0; i< SIZE_OF_MEMORY_POPULATION; i++)
	{	
		displayIndividual(&k->individuals[i]);	
		/*printf("\n Memory k%d: %d\n",i, k->individuals[i].fitness_value);
		system("pause");*/
	}
}


int getEUC_2D(Node *n1, Node *n2)
{
	double xd, yd,dn12;
	xd = n1->x - n2->x;
	yd = n1->y - n2->y;
	
	dn12 = nint(sqrt(xd*xd+yd*yd));
	return (int)dn12;
}

//In this context, fitness function is minimization of round-trip city distances.
//The distance metric here is euclidean distance - EUC_2D
int calcIndividualFitness(Individual *individual){	
	int i,distance,cost;
	Node *city_from, *city_to;
	Node *first_city, *last_city;
	int total_fitness_value = 0; //Total trip distances
	
	for(i=0;i<COUNT_OF_NODES-1;i++)
	{
		city_from = &nodes[individual->values[i]-1];
		city_to = &nodes[individual->values[i+1]-1];
		distance = getEUC_2D(city_from,city_to);

		cost = distance * (1+traffic[individual->values[i]-1][individual->values[i+1]-1]);

		total_fitness_value += cost;
	}

	first_city = &nodes[individual->values[0]-1];
	last_city  = &nodes[individual->values[COUNT_OF_NODES-1]-1];
	distance = getEUC_2D(city_from,city_to);

	cost = distance * (1	+	traffic[individual->values[COUNT_OF_NODES-1]-1][individual->values[0]-1]);

	total_fitness_value += cost;

	return total_fitness_value;
}


void setIndividualValues(Individual *parent, Individual *offspring){
	int i;
	for(i=0;i<COUNT_OF_NODES; i++)
	{
		offspring->values[i] = parent->values[i];
	}

	offspring->fitness_value = parent->fitness_value;
}

int* generatePermutation(int upto){
	int i;
	int* p_array;
	p_array = (int*) malloc(sizeof(int)*upto);

	for(i=0;i<upto;i++)
	{
		p_array[i] = i;
	} 

	shuffle(p_array, upto);

    return p_array;
}


double generateRandomTrafficFactor(int fl, int fu)
{
	int diff = fu - fl;
	double r = ((double) rand() / (RAND_MAX)) * diff;
	return r + fl;
}

void changeTrafficFactors(double severty_of_change, int fl, int fu)
{
	int i,j;
	double r;
	for(i=0;i<COUNT_OF_NODES;i++)
	{
		for(j=0;j<COUNT_OF_NODES;j++)
		{
			if(i == j)
				traffic[i][j] = 0;
			else{
				r = ((double) rand() / (RAND_MAX));
				
				if(severty_of_change > r)
				{					
					traffic[i][j]=generateRandomTrafficFactor(fl,fu);
				}
			}
		}
	}
}

void displayTrafficFactors()
{
	int i,j;
	for(i=0;i<COUNT_OF_NODES;i++)
	{
		for(j=0;j<COUNT_OF_NODES;j++)
		{
			printf("Traffic(%d,%d): %f\n",i,j,traffic[i][j]);
		}
	}
}


Individual* getBestIndividual(Population *p)
{
	Individual *best;
	int i, current_fitness, best_fitness;
	best_fitness = p->individuals[0].fitness_value;
	best = &p->individuals[0];

	for(i=0;i<POPULATION_SIZE;i++)
	{
		current_fitness = p->individuals[i].fitness_value;
		 
		if(current_fitness < best_fitness )
		{
			best_fitness = current_fitness;
			best = &p->individuals[i];
		}
	}
	return best;
}


