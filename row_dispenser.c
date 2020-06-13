#ifndef GTHREAD_DISPENSER
#define GTHREAD_DISPENSER GTHREAD_DISPENSER

#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>//malloc et al.
#include"gmp.h"

/*
 * we clean the 'source_row' column using the source row
 */
struct row_pair
{
	size_t source_row;
	size_t target_row;
};

/*
 * stuff conserning the synchronisation of a single pass ( sync. between passes is in threadet)
 * could consider these the parameters passed to the threads ( but they are shared )
 */
struct Pair_Dispenser
{
	/* mutex */
	sem_t semp;
	
	/*
	 * current is the next pair to be calculated
	 * pairs given are unique in the scope of the pass
	 *
	 */
	struct row_pair current;
	/*
	 * max is the maximum index. = matrix size - 1
	 * this value is not modified by the threads
	 *
	 */
	size_t max;
	/*
	 * the matrix that we calculate the matrix for
	 * this value is not modified by the threads
	 *
	 */
	mpq_t** matrix;
	/*
	 * should the thread print stuff
	 * this value is not modified by the threads
	 *
	 */
	char verbose;

};

void Pair_Dispenser_Init(struct Pair_Dispenser *thrd,size_t max,mpq_t** matrix,char verbose)
{
	thrd->current.source_row = 0;
	thrd->current.target_row = 0;
	thrd->max=max;
	thrd->matrix=matrix;
	sem_init(&(thrd->semp),0,1);
	thrd->verbose = verbose;
}
/*
 * Get a row pair without breaking the row dispenser invariant
 */
struct row_pair Pair_Dispenser_Get_Pair(struct Pair_Dispenser *thrd)
{
	struct row_pair temp;
	/* this is 1/2 of what will slow down parallel execution */
	/* critical part */
	sem_wait(&(thrd->semp));	
	temp=thrd->current;
	if(thrd->current.target_row<= thrd->max)
	{
		++thrd->current.target_row; 
	}
	sem_post(&(thrd->semp));	
	/* critical part */
	return temp;

}




#endif
