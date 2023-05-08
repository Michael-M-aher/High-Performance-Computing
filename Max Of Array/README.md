##Finding Maximum Number in an Array using Master-Slave Paradigm

### Master Process

The master process performs the following steps:

1. Read the size of the array.
2. Read the elements inside the array.
3. Distribute the work among slave processes by sending them the size of the array and the assigned partition of the array.
4. After each slave finishes its work, receive the maximum number and its index from each process.
5. Compute the final maximum number and its index by comparing the maximum numbers returned from each slave.
6. Output the final maximum number and its index in the original array to the user.

### Slave Processes

Each slave process performs the following steps:

1. Receive the size of the array from the master process.
2. Receive the portion of the array assigned by the master process.
3. Calculate the maximum number and its index in the assigned partition of the array.
4. Send the maximum number and its index back to the master process.

Note: The size of the array may not be divisible by the number of processes, so the program must handle this case.

## Usage

To use the matrix multiplication program, follow these steps:

1. Make sure you installed mpi on your computer
1. Clone the repository to your local machine.
1. Compile the program using the following command:

```
mpicc max.c -o out
```

1. Run the program using the following command:

```
mpiexec -n <num of threads> ./out
```

1. Enter the necessary input values.
1. The program will then compute the max and print it to the console.

## Example

Here's an example of how to use finding maximum number in an array program:

```
Hello from master process.
Number of slave processes is 5
Please enter size of array...
10
Please enter array elements ...
-3 4 2 10 5 8 9 7 -2 -5
Hello from slave#1 Max number in my partition is 4 and index is 1.
Hello from slave#2 Max number in my partition is 10 and index is 1.
Hello from slave#3 Max number in my partition is 8 and index is 1.
Hello from slave#4 Max number in my partition is 9 and index is 0.
Hello from slave#5 Max number in my partition is -2 and index is 0.
Master process announce the final max which is 10 and its index is 3.
Thanks for using our program
```

In this example, the user entered the necessary input values. The program then computed the max and printed it to the console.

