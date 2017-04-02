Individual* createNewIndividual();


Individual* evolvePopulation(Population *p)
{
	Individual *m_parent1, *m_parent2;
	Individual *m_offspring1, *m_offspring2;

	m_parent1 = TournamentSelection(p);
	m_parent2 = TournamentSelection(p);

	//New members of the population are created.
	m_offspring1 = createNewIndividual();
	m_offspring2 = createNewIndividual();
				
	//Selected parents' values assigned to offsprings
	setIndividualValues(m_parent1,m_offspring1);
	setIndividualValues(m_parent2,m_offspring2);

	//Always PMX is used
	crossover(m_offspring1, m_offspring2);

	//Mutations
	swapMutation(m_offspring1);
	swapMutation(m_offspring2);

	//Survival of Fittest
	survivalSelectionAndInsertion(p, m_offspring1, m_offspring2);
		
	return getBestIndividual(p);
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
	
	s->fitness_value = calcIndividualFitness(s);
	
	return s;
}



void generateInitialPopulation(Population *p)
{
	int i;	
	for(i=0; i< POPULATION_SIZE; i++)
	{
		p->individuals[i] = *createNewIndividual();
	}
}
