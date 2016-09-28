//The parallel Hello World Program
  #include<stdio.h>
  #include "mpi.h"
  #include <stdlib.h>
  #include <assert.h>

 /* void slave1 (int j){
     MPI_Status status;
     MPI_Recv(&j,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
   }
   void slave2 (int j){
     MPI_Status status;
     MPI_Recv(&j,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
   }
*/
  //create array of random numbers from 0 to 1
    float *create_rand_nums(int num_elements) {
      float *rand_nums = (float *) malloc(sizeof(float)*num_elements);
      assert(rand_nums != NULL);
      int i;
      for (i = 0; i < num_elements; i++){
        rand_nums[i] = (rand()/(float)RAND_MAX);
      }
      return rand_nums;
   } 

  
  int main(int argc, char *argv[])
  {
  int rank, size,t,data;
  int tag = 10101;
  float *rand_nums = NULL;
  float local_sum = 0;
  float global_sum = 0;
  int num_elements = 100;
  char *cpu_name;
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

   //Create random array of elements on all processes
  srand(10); //seed the random number generator
  rand_nums = create_rand_nums(num_elements);
  
  //Sum the numbers locally in each Processor
  int i,j;
  for (i = 0; i < num_elements; i++) {
    for (j = 0; j < num_elements; j++)
   local_sum +=rand_nums[i]*rand_nums[j];
  }

  //Print the Random numbers in each processes
    printf("Local sum for process %d - %f, avg = %f\n", rank,local_sum,local_sum/num_elements);
  
  //Summation Operations; local sum (local_sum) calculated in each process into global sum
  MPI_Reduce(&local_sum,&global_sum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

  
//On every process allocate space for the machine name
  cpu_name = (char*)calloc(80,sizeof(char));

//get the machine name of the particulasr host at least the 80 characters of it
  gethostname(cpu_name,80);

//Elaspsed Time
time_current=MPI_Wtime();
time = time_current - time_initial;

  if (rank == 0) {
   data = 10;
   int i; 
  printf("send data=%d with tag %d to all other processes\n",data, tag);
   for (i = 0; i < size; i++){
    if (i != 0){
    MPI_Send(&data, 1, MPI_INT,i,tag,MPI_COMM_WORLD);
      }
   } 
   printf("Total sum = %f, avg=%f\n",global_sum, global_sum/(size*num_elements));
   printf("ElapsedTime=%.3f: Hello World from master process= %d on machine=%s, of %d\n", time,rank, cpu_name,size);
   }
   else if (rank > 0) {
  /*     for (t=2;t<10;t++) {
          slave1(rank);
          MPI_Barrier(MPI_COMM_WORLD);
          slave2(rank);
          MPI_Barrier(MPI_COMM_WORLD);
    }    }
  */    MPI_Recv(&data, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Process %d received data %d with tag %d from master process\n",rank,data,tag);     
        printf("ElapsedTime=%.3f: Hello World from slave process= %d on machine=%s, of %d\n", time,rank, cpu_name,size);
  }
  //Clean up
    free(rand_nums);
   // Blocks until all the processes have reached this routing
      MPI_Barrier(MPI_COMM_WORLD);
  
  MPI_Finalize();
  return 0;
  }

