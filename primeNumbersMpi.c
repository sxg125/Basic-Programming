//The parallel Hello World Program
  #include<stdio.h>
  #include "mpi.h"
  #include <stdlib.h>
  #include <assert.h>

int main(int argc, char *argv[])
 {
  int lower,upper,total,i,j,isprime;
  int rank, size;
  int local_total = 0;
  int global_total = 0;
  double time_initial,time_current,time;
  //Add in MPI startup Routines
  // Launch the MPI processes in each node
  MPI_Init(&argc, &argv);

  //Initialize the time
  time_initial = MPI_Wtime();

 // Request a thread id or rank from the MPI master process which had rank or tid = 0
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //Get the number of threads or processor launched by MPI
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Partial count of Prime Numbers
   local_total = 0;
   lower = 2;
   upper = 131072;

//Broadcast the upper limit i.e. that copy is shared among the processors
MPI_Bcast ( &upper, 1, MPI_INT, 0, MPI_COMM_WORLD );

for (i = lower + rank; i<= upper; i=i+size)
{
  isprime = 1;
//  MPI_Bcast ( &upper, 1, MPI_INT, 0, MPI_COMM_WORLD );

     for ( j=2; j<i; j++)
       {
        if (i % j == 0)
          {
           isprime = 0;
           break;
          }
       }
  local_total+=isprime;
}

  //Summation Operations; local partial total (local_total) calculated in each process into global_total
  MPI_Reduce(&local_total,&global_total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

time_current=MPI_Wtime();
time = time_current - time_initial;

//Print the output from  the Master processor
  if (rank == 0) {
   printf("Total Prime Numbers = %d\n",global_total);
   printf("ElapsedTime=%.3f\n",time);
   }
   
  // Blocks until all the processes have reached this routing
      MPI_Barrier(MPI_COMM_WORLD);
  
  MPI_Finalize();
  return 0;
  }

