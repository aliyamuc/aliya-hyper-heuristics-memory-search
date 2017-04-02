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

Individual* createNewIndividual()
{
	Individual *s = (Individual*) malloc (sizeof (Individual));

	int i;
	for(i=0; i< COUNT_OF_NODES; i++)
	{
		s->values[i] = i+1;
	}

	shuffle(s->values, COUNT_OF_NODES);
	
	return s;
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
	distance = 0;
	for(i=0;i<COUNT_OF_NODES-1;i++)
	{
		city_from = &nodes[individual->values[i]-1];
		city_to = &nodes[individual->values[i+1]-1];
		distance = getEUC_2D(city_from,city_to);

		cost = (int) (distance * (1+traffic[individual->values[i]-1][individual->values[i+1]-1]));

		total_fitness_value += cost;
	}

	first_city = &nodes[individual->values[0]-1];
	last_city  = &nodes[individual->values[COUNT_OF_NODES-1]-1];
	distance = getEUC_2D(city_from,city_to);

	cost = (int) (distance * (1	+	traffic[individual->values[COUNT_OF_NODES-1]-1][individual->values[0]-1]) );

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

void displayChoiceF2Matrix()
{
	int i,j;

	for(i=0;i<COUNT_OF_HEURISTICS;i++)
	{
		for(j=0;j<COUNT_OF_HEURISTICS;j++)
		{
			printf("%d ",f2_h_matrix[i][j]);
		}
		printf("\n");
	}
}

int maxGfactor(int f1, int f2, int f3)
{
	if(f1>f2 && f1>f3) return 1;
	
	if(f2>f1 && f2>f3) return 2;

	if(f3>f1 && f3>f2) return 3;

	//tie situation, any f factor is not maximum. there is an equality.
	return 0;
}


