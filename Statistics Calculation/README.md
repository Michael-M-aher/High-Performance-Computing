## Statistics Calculation

This program calculates the mean, variance, and standard deviation of a given set of numbers using parallel processing. The program distributes the data among multiple processes, each process calculating its local sum, squared differences, and partial results. The master process then combines the partial results to compute the final mean, variance, and standard deviation.

### Algorithm

1. Each process calculates its local sum on its portion of the data. If the division has a remainder, the master process handles it.
2. The master process calculates the mean by dividing the total sum by the number of elements and sends it to all processes.
3. Each process calculates the squared difference on its portion of the data.
4. The master process calculates the variance by dividing the total squared difference by the number of elements.
5. The master process calculates the standard deviation by taking the square root of the variance.
6. The master process prints the results (mean, variance, and standard deviation) to the console.

### MPI Usage

To use the program for mean, variance, and standard deviation calculation, follow these steps:

1. Make sure you have an MPI implementation installed on your computer.
2. Clone the repository to your local machine.
3. Compile the program using the following command:

```
mpicc stat_mpi.c -o out -lm
```

4. Run the program using the following command:

```
mpiexec -n <num of threads> ./out
```

5. Enter the necessary input values.

6. The program will calculate the mean, variance, and standard deviation and print the results to the console.

### Open MP Usage

To use the program for mean, variance, and standard deviation calculation, follow these steps:

1. Clone the repository to your local machine.
1. Compile the program using the following command:

```
g++ stat_omp.c -lgomp -o out
```

1. Run the program using the following command:

```
./out
```

1. Enter the necessary input values.

1. The program will calculate the mean, variance, and standard deviation and print the results to the console.

### Example

Here's an example of how to use the program to calculate the mean, variance, and standard deviation:

```
Array size: 8
Array elements: 2 4 4 4 5 5 7 9
```

Running the program with 4 processes:

```
mpiexec -n 4 ./out 8 2 4 4 4 5 5 7 9
```

Output:

```
Mean = 5.0, Variance = 4.0, Standard deviation = 2.0
```

Another example with 7 elements:

```
Array size: 7
Array elements: 4 5 6 6 7 9 12
```

Running the program with 3 processes:

```
mpiexec -n 3 ./out 7 4 5 6 6 7 9 12
```

Output:

```
Mean = 7.0, Variance = 6.2857, Standard deviation = 2.5071
```