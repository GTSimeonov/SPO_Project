#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_DIGITS 3000
#define MIN_DIGITS 100
#define VARIATION 100


int main()
{

	size_t size;
	size_t i,j;
	size_t sz;
	size_t heavy_line;
	char *number;//we don't want a stack overflow so it goes on the heap


	scanf("%zu",&size);
	scanf("%zu",&heavy_line);


	number = malloc(sizeof(char)*(MAX_DIGITS+10+VARIATION));
	if(number==NULL)return -1;
	srand(time(NULL));
	printf("%zu\n",size);

	for(i=0;i<heavy_line && i<size;++i)
	{
		for(j=0;j<size;++j)
		{
			for(sz=MIN_DIGITS,number[sz+1]='\0'; sz>0 ;number[sz]=rand()%10+'0',--sz);
			number[0]=rand()%9 + '1';
			printf("%s ",number);
		}
		printf("\n");
	}

	if(heavy_line<size)
	{
		for(j=0;j<size;++j)
		{
			
			for(sz=(MAX_DIGITS-(rand()%(VARIATION+1))),number[sz+1]='\0'; sz>0 ;number[sz]=rand()%10+'0',--sz);
			number[0]=rand()%9 + '1';
			printf("%s ",number);
		}
		printf("\n");
	}

	for(++i;i<size;++i)
	{
		for(j=0;j<size;++j)
		{
			for(sz=MIN_DIGITS,number[sz+1]='\0'; sz>0 ;number[sz]=rand()%10+'0',--sz);
			number[0]=rand()%9 + '1';
			printf("%s ",number);
		}
		printf("\n");
	}
	return 0;
}

