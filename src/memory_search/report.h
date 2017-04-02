

/* Output of individuals in the objective space */
void report_best_fitness(int seed, int best_fitness, FILE *fpt)
{
	fprintf(fpt,"%d %d\n", seed, best_fitness);
	return;
}