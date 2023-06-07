#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<math.h>
#include "mpi.h"

int sumArray(int *arr, int arrSz){
    int sum = 0;
    for (int i = 0; i < arrSz; ++i) {
        sum += arr[i];
    }
    return sum;
}

double calcSqDiff(int *arr, int arrSz, double mean){
    double sq_diff = 0;
    for (int i = 0; i < arrSz; ++i) {
        sq_diff += (arr[i]-mean)*(arr[i]-mean);
    }
    return sq_diff;
}

int main(int argc, char* argv[])
{
    double start_time, end_time, execution_time=0.0; /* for calculation execution time */
    int my_rank; /* rank of process */
    int p; /* number of process */
    int source; /* rank of sender */
    int dest; /* rank of reciever */
    int tag = 0; /* tag for messages */
    MPI_Status status; /* return status for */


    /* Start up MPI */
    MPI_Init(&argc, &argv);
    start_time = MPI_Wtime();

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int arr_size, par_size, remaining, local_sum=0, total_sum=0;
    double mean=0.0, local_sqDiff=0.0, sqDiff = 0.0, variance=0.0;
    int* arr, *local_arr;

    if (my_rank == 0) {
        printf("Array size:");
        scanf("%d", &arr_size);
		par_size = arr_size / p;
        remaining = arr_size % p;
        arr = (int *) malloc(sizeof(int) * arr_size);
        printf("Array elements:");
        for (int i = 0; i < arr_size; ++i) {
            scanf("%d", &arr[i]);
        }
        //sum = sumArray(arr, arr_size);
        //mean = (double)sum / arr_size;					// 2 4 4 4 5 5 7 9
    }
	
    MPI_Bcast(&par_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_arr = (int*)malloc(sizeof(int) * par_size);
    MPI_Scatter(arr, par_size,MPI_INT ,local_arr, par_size, MPI_INT, 0, MPI_COMM_WORLD);
	
    start_time=MPI_Wtime();
    //Get local_sum
	local_sum = sumArray(local_arr,par_size);
    end_time=MPI_Wtime();
    execution_time += end_time - start_time;
    MPI_Reduce(&local_sum, &total_sum, 1,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);

    if (my_rank == 0)
    {
		total_sum += sumArray(arr+(p*par_size),remaining);
        mean = (double)total_sum/(double)arr_size;
    }
    // send mean to all slaves
    MPI_Bcast(&mean, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
    start_time=MPI_Wtime();
    local_sqDiff= calcSqDiff(local_arr,par_size, mean);
    end_time=MPI_Wtime();
    execution_time += end_time - start_time;
    
    MPI_Reduce(&local_sqDiff, &sqDiff, 1,MPI_DOUBLE, MPI_SUM,0,MPI_COMM_WORLD);
    // without dividing mean
    if (my_rank == 0)
    {
		sqDiff += calcSqDiff(arr+(p*par_size),remaining, mean);
        variance = sqDiff / arr_size;
        double stdDev = sqrt(variance);
        printf("\nOutput: Mean = %.4f, Variance = %.4f, Standard deviation = %.4f\n", mean, variance, stdDev);
        printf("Time Taken for Parallel Part = %f \n" , execution_time);
    }


    /* shutdown MPI */
    MPI_Finalize();
    return 0;
//    7
//    4 5 6 6 7 9 12
}
