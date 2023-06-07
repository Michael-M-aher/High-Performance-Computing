#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

int main(int argc, char *argv[]) {
    int i;
    int local_sum = 0,arr_size;
    double variance, std_dev, mean, local_sq_diff = 0.0, start_time, end_time, execution_time=0.0;

    printf("Array size:");
    scanf("%d", &arr_size);

    int* data = (int *) malloc(arr_size * sizeof(int));
    printf("Array elements:");
    for (i = 0; i < arr_size ; i++)
    {
        scanf("%d",&data[i]);
    }
    
    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:local_sum)
    local_sum = sumArray(data,arr_size);
    end_time = omp_get_wtime();
    execution_time += end_time - start_time;
    mean = (float)local_sum / arr_size;

    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:local_sq_diff)
    local_sq_diff = calcSqDiff(data,arr_size,mean);
    execution_time += end_time - start_time;

    variance = local_sq_diff / arr_size;
    std_dev = sqrt(variance);

    printf("\nOutput: Mean = %.4f, Variance = %.4f, Standard deviation = %.4f\n", mean, variance, std_dev);
    printf("Time Taken for Parallel Part = %f \n" , execution_time);
    return 0;
}
