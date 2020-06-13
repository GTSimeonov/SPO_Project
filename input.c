#ifndef DET_INPUT_C
#define DET_INPUT_C DET_INPUT_C
//how many digits is a number from -n is allowed to have
#define MAX_DIGITS 100
#include<stdlib.h>
#include<stdio.h>
#include"gmp.h"
#include<time.h>



void input_det(FILE* input,mpq_t ***det,size_t *size)
{


	fscanf(input,"%zu",size);
	(*det)=malloc(sizeof(mpq_t*)*(*size) + sizeof(mpq_t)*(*size)*(*size));
	size_t i,j;

	for(i=0;i<(*size);++i)
	{
		(*det)[i]=((mpq_t*)(((u_int8_t*)(*det)) + sizeof(mpq_t*)*(*size) + sizeof(mpq_t)*(*size)*i));
		for(j=0;j<(*size);++j)
		{
			
			mpq_init((*det)[i][j]);
			gmp_fscanf(input,"%Qi",(*det)[i][j]);
		}
	}
}
/*
 * places a randomised matrix in det
 * whose digits are 1 to MAX_DIGITS long
 *
 */
void random_table(mpq_t ***det,size_t size)
{
	(*det)=malloc(sizeof(mpq_t*)*size + sizeof(mpq_t)*size*size);
	size_t i,j;
	size_t sz;
	char *number;//we don't want a stack overflow so it goes on the heap
	number = malloc(sizeof(char)*MAX_DIGITS+1);
	srand(time(NULL));

	for(i=0;i<size;++i)
	{
		(*det)[i]=((mpq_t*)(((u_int8_t*)(*det)) + sizeof(mpq_t*)*size + sizeof(mpq_t)*size*i));
		for(j=0;j<size;++j)
		{
			
			
			for(sz=rand()%MAX_DIGITS,number[sz+1]='\0';sz>0;number[sz]=rand()%10+'0',--sz);
			number[0]=rand()%9 + '1';

			mpq_init((*det)[i][j]);
			gmp_sscanf(number,"%Qi",(*det)[i][j]);
		}
	}
}
#endif//#ifndef DET_INPUT_C
