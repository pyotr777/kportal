/*
 ============================================================================
 Name        : C_MPI.c
 Author      : Bryzgalov Peter
 Version     :
 Copyright   : 2014
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]){
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	int source;   /* rank of sender */
	int dest;     /* rank of receiver */
	int tag=0;    /* tag for messages */
	char message[100];        /* storage for message */
	MPI_Status status ;   /* return status for receive */
	
	/* start up MPI */
	
	MPI_Init(&argc, &argv);
	
	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
	
	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	
	
	if (my_rank !=0){
		/* create message */
		dest = 0;
		unsigned long res=1;
		int i;
		for (i=0; i < my_rank*10; i++) {
			res++;
			/*sleep(1);*/
		}
		sprintf(message, "Process #%d: sum=%d", my_rank,res);
		/* use strlen+1 so that '\0' get transmitted */
		MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	else{
		printf("C_MPI Num processes: %d\n",p);
		for (source = 1; source < p; source++) {
			MPI_Recv(message, 100, MPI_CHAR, source, tag,
			      MPI_COMM_WORLD, &status);
			printf("get: %s\n",message);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	/* shut down MPI */
	MPI_Finalize(); 
	
	
	return 0;
}
