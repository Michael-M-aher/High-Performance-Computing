## Calculating PI using Integration

This program calculates the value of PI the number of steps is divided among multiple processes, and each process calculates its own partial sum. The master process divides the number of steps into ranges and broadcasts the step size to all processes. Each slave process receives the step size and the range it will work on, then calculates its partial sum. The master process performs a reduce operation to compute the final sum, and uses it to calculate the value of PI.

## Usage

To use the program, follow these steps:

1. Make sure you installed mpi on your computer
2. Clone the repository to your local machine.
3. Compile the program using the following command:

```
mpicc pi.c -o out
```

4. Run the program using the following command:

```
mpiexec -n <num of threads> ./out
```

5. The program will then compute the value of PI and print it to the console.

## Example

Here's an example of how to use the program:

```
Master process announce PI = 3.14159265
```

In this example, The program computed the value of PI and printed it to the console.