#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"row_dispenser.c"
#include"threadet.c"
#include"input.c"

int main(int argc,char **argv)
{
	mpq_t **det=NULL;
	size_t n=1;
	size_t i,j,y;
	size_t number_of_threads=1;
	char verbose=1;
	FILE *out=stdout;
	FILE *dm=NULL;
/*
 * menuing
 *
 */
	for(i=0;i<argc;++i)
	{
		if(strcmp("-i",argv[i]) == 0)
		{
			if(det!=NULL)free(det);
			++i;
			if(i == argc) 
			{
				printf("No name\n");
				return -1;
			}
			FILE *in = fopen(argv[i],"r");
			if(in == NULL)
			{
				printf("No such file\n");
				return -1;
			}
			input_det(in,&det,&n);
			fclose(in);
		}else if(strcmp("-t",argv[i]) == 0)
		{
			++i;
			if(i == argc) 
			{
				printf("No number of threads\n");
				return -1;
			}
			for(j=0;argv[i][j]!='\0';++j)
				if(argv[i][j] < '0' || argv[i][j] > '9')
				{
					printf("Number of threads format not supported\n");
					return -1;
				}
			sscanf(argv[i],"%zu",&number_of_threads);

		}else if(strcmp("-q",argv[i]) == 0)
		{
			verbose = 0;
		}else if(strcmp("-n",argv[i]) == 0)
		{
			if(det!=NULL)free(det);
			++i;
			if(i == argc) 
			{
				printf("No matrix size\n");
				return -1;
			}
			for(j=0;argv[i][j]!='\0';++j)
				if(argv[i][j] < '0' || argv[i][j] > '9')
				{
					printf("Matrix size format not supported\n");
					return -1;
				}
			sscanf(argv[i],"%zu",&n);
			random_table(&det,n);
		}else if(strcmp("-o",argv[i]) == 0)
		{
			++i;
			if(i == argc) 
			{
				printf("No name\n");
				return -1;
			}
			//fclose(out);
			out = fopen(argv[i],"w");
			if(out == NULL)
			{
				printf("No such file\n");
				return -1;
			}
		}else if(strcmp("-dm",argv[i]) == 0)
		{
			++i;
			if(i == argc) 
			{
				printf("No name\n");
				return -1;
			}
			dm = fopen(argv[i],"w");
		}

	}
	if(det==NULL)
	{
		printf("Not matrix specified\n");
		return -1;
	}
	if(dm!=NULL)
	{
		fprintf(dm,"%zu\n",n);
		for(i=0;i<n;++i)
		{
			for(j=0;j<n;++j)
			{
				gmp_fprintf(dm,"%Qi ",det[i][j]);
			}
			fprintf(dm,"\n");
		}
	}
	mpq_t *res = threadet(det,n-1,number_of_threads,verbose);
	gmp_fprintf(out,"%Qi",*res);
	for(i=0;i<n;++i)
		for(j=0;j<n;++j)
			mpq_clear(det[i][j]);	
	mpq_clear(*res);
	free(det);
	free(res);
	return 0;
}
