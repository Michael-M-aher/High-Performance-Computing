## Counting Prime Numbers within an Input Range

This program counts the number of prime numbers within a given input range using two different MPI methods. The program is parallelized by splitting the range into subranges and assigning each subrange to a different process. Then, the partial results are collected and combined to compute the final result.

### Method 1: MPI_Bcast and MPI_Reduce ONLY

#### Master Process:

1. Read the lower bound `x` and upper bound `y` of the range.
2. Calculate the subrange size `r` as `r = (y - x) / p` (where `p` is the number of processes).
3. Broadcast `x` and `r` to each slave process using MPI_Bcast.
4. Receive sub-count from each slave process using MPI_Reduce.
5. Compute the total count of primes between `x` and `y`.
6. Print the total count of primes to the user.

#### Slave Process:

1. Receive `x` and `r` through the MPI_Bcast call.
2. Calculate the lower bound `a` and upper bound `b` according to its rank using `a = x + (rank * r)` and `b = a + r - 1`.
3. Count primes in its subrange (between `a` and `b`).
4. Send this partial count to the master process using the MPI_Reduce call.

### Method 2: MPI_Send and MPI_Recv ONLY

#### Master Process:

1. Read the lower bound `x` and upper bound `y` of the range.
2. Calculate the subrange size `r` as `r = (y - x) / p` (where `p` is the number of processes).
3. Loop over each slave process and send `x` and `r` to each using MPI_Send.
4. Loop over each slave process and receive sub-count from each using MPI_Recv.
5. Compute the total count of primes between `x` and `y`.
6. Print the total count of primes to the user.

#### Slave Process:

1. Receive `x` and `r` through the MPI_Send call.
2. Calculate the lower bound `a` and upper bound `b` according to its rank using `a = x + (rank * r)` and `b = a + r - 1`.
3. Count primes in its subrange (between `a` and `b`).
4. Send this partial count to the master process using the MPI_Send call.

Note: The length of the range may not be divisible by the number of processes, so the program must handle this case.

## Usage

To use the prime number counting program, follow these steps:

1. Make sure you installed mpi on your computer
2. Clone the repository to your local machine.
3. Compile the program using the following command:

Method 1
```
mpicc prime1.c -o out
```
or Method 2
```
mpicc prime2.c -o out
```

4. Run the program using the following command:

```
mpiexec -n <num of threads> ./out
```
5. The program will then compute the count and print it to the console.

## Example

Here's an example of how to use the prime number counting program:

```
n = 4, x = 1, y = 16 → r = (16 - 1) / (4 - 1) = 5
p1: calculate partial count of prime numbers from 1 to 5 → Count = 3 (2, 3, 5)
p2: calculate partial count of prime numbers from 6 to 10 → Count = 1 (7)
p3: calculate partial count of prime numbers from 11 to 15 → Count = 2 (11, 13)
After reduction, P0 will have Count = 6 (2, 3, 5, 7, 11, 13)
```