#ifndef GTHREADET_C 
#define GTHREADET_C GTHREADET_C
#include"row_dispenser.c"
#include<time.h>
/*
 * using the GNU multiprecision library because there are no specified limits to the matrix elements (even if there were the det is going to be huge regardless)
 * and because it has handy rational numbers and input functions
 */ 
#include"gmp.h"
/*
 * posix threads for maximum portability :>
 */
#include<pthread.h>


mpq_t* threadet(mpq_t **matrix,size_t size,size_t number_of_threads,char verbose);
void* dethread(void* thr);
void calc_row(mpq_t **matrix,size_t size,size_t target,size_t source);

/*
 * here the process creates and dispatches number_of_threads threads
 */
mpq_t* threadet(mpq_t **matrix,size_t size,size_t number_of_threads,char verbose)
{
	size_t i;
	size_t j;
	/* shared thread parameters */
	struct Pair_Dispenser thr;
	/* swap */
	mpq_t *hold;
	/* array of the threads we are executing. We are going to join these every column of the matrix cleaned because it is a very convinient sync.*/
	pthread_t* threds;
	/* result is accumulated here */
	mpq_t *res;
	/* placeholder for thread return values. They all return NULL but join() requires a pointer to store return */
	void **temp;
	/* we can't clean a column with a zero so we swap a impotent column with a column with a non zero value ( if there is such) */
	char sign;
	/* timers  REALTIME CLOCK could be fooled by an administrator changing the system time, but is the only guaranteed clock in the posix standart*/
	struct timespec pass_start,pass_finish;
	struct timespec start,finish;

	if(verbose==1)
	{
		clock_gettime(CLOCK_REALTIME,&start);
	}



	threds=malloc(sizeof(pthread_t*) * number_of_threads);
	res=malloc(sizeof(mpq_t));
	temp = malloc(sizeof(void*));

	Pair_Dispenser_Init(&thr,size,matrix,verbose);
	mpq_init(*res);
	thr.current.target_row=1;
	sign=1;

	for(i=0;i<size;++i)
	{
		/* if the row can't be used to clean the column search for one that can */
		if(mpq_sgn(matrix[i][i])==0)
		{
			for(j=i+1;j<=size && mpq_sgn(matrix[j][i])==0;++j);
			if(j>size)
			{
				mpq_set_d(*res,0);

				free(threds);
				free(temp);
				if(verbose == 1)
				{
						clock_gettime(CLOCK_REALTIME,&finish);
						finish.tv_sec-=start.tv_sec;
						finish.tv_nsec-=start.tv_nsec;
						printf("TIME: %f\n",(double)(finish.tv_sec + 0.000000001*finish.tv_nsec));
				}
				return res;
			}else
			{
				hold = matrix[j];
				matrix[j] = matrix[i];
				matrix[i] = hold;
				/* change the sign approprietly */
				sign = (sign + (j-i)%2)%2;
			}
		}
		if(verbose==1)
		{
			clock_gettime(CLOCK_REALTIME,&pass_start);
		}
		/* clean the i-th row this is one pass*/
   
		for(j=0;j<number_of_threads;++j)
		{
			pthread_create(threds+j,NULL,dethread,&thr);
		}
		/* wait for all the threads to finish calculating this is the second 1/2 that could slow down parallel execution*/

		/* critical part */
		for(j=0;j<number_of_threads;++j)
		{
			pthread_join(threds[j],temp);
		}
		/* critical part */

		if(verbose == 1)
		{
			clock_gettime(CLOCK_REALTIME,&pass_finish);
			pass_finish.tv_sec-=pass_start.tv_sec;
			pass_finish.tv_nsec-=pass_start.tv_nsec;
			printf("A pass has finished, taking: %f seconds\n\n",(double)(pass_finish.tv_sec + 0.000000001*pass_finish.tv_nsec));
		}

		thr.current.target_row = (++thr.current.source_row) + 1;

	}

	mpq_set_d(*res,((sign == 1)?1:-1));
	for(i=0;i<=size;++i)
	{
		mpq_mul(*res,*res,matrix[i][i]);
	}
	free(threds);
	free(temp);
	if(verbose == 1)
	{
			clock_gettime(CLOCK_REALTIME,&finish);
			finish.tv_sec-=start.tv_sec;
			finish.tv_nsec-=start.tv_nsec;
			printf("TIME: %f\n",(double)(finish.tv_sec + 0.000000001*finish.tv_nsec));
	}
	return res;
}
void* dethread(void* thr)
{
	#define THR ((struct Pair_Dispenser*)thr)
	clock_t start,finish;
	if(THR->verbose == 1)
	{
		start=clock();
	}
	struct row_pair temp;
	/* while you can get a valid pair - calculate */
	while((temp=Pair_Dispenser_Get_Pair(THR)).target_row <= THR->max)
	{
	       	calc_row(THR->matrix,THR->max,temp.target_row,temp.source_row);
	}	
	if(THR->verbose == 1)
	{
		finish=clock();
		printf("A thread has finished, taking: %f seconds\n",((double)(finish-start))/CLOCKS_PER_SEC);
	}
	return NULL;
	#undef THR
}
void calc_row(mpq_t **matrix,size_t size,size_t target,size_t source)
{
	mpq_t mod;
	mpq_t temp1;

	mpq_init(mod);
	mpq_init(temp1);
	

	mpq_div(mod,matrix[target][source],matrix[source][source]);
	size_t k;
	for(k=0;k<=size;++k)
	{
		mpq_mul(temp1,matrix[source][k],mod);
		mpq_sub(matrix[target][k],matrix[target][k],temp1);
		
	}
	mpq_clear(mod);
	mpq_clear(temp1);
}

#endif //#ifndef GTHREADET_C 
