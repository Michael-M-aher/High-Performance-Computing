# Matrix Multiplication Program

This program multiplies two matrices together and prints the result. The dimensions and values of the matrices are taken as input from either the console or a file, depending on the user's choice.

## Modes

This program has two modes:

1. File mode: Reads the input from a file.
1. Console mode: Reads the input from the console.


## Input Format

The input format for the matrices is as follows:

- The first line contains the dimensions of the first matrix, separated by a space. The dimensions are represented by two integers, `m` and `n`.
- The next `m` lines contain `n` integers each, representing the elements of the first matrix.
- The next line contains the dimensions of the second matrix, separated by a space. The dimensions are represented by two integers, `p` and `q`.
- The next `p` lines contain `q` integers each, representing the elements of the second matrix.

## Output Format

The output format for the result matrix is as follows:

- The dimensions of the result matrix are printed first, represented by two integers, `m` and `q`.
- The next `m` lines contain `q` integers each, representing the elements of the result matrix.


## Usage

To use the matrix multiplication program, follow these steps:

1. Make sure you installed mpi on your computer
1. Clone the repository to your local machine.
1. Compile the program using the following command:

```
mpicc matrix.c -o out
```

1. Run the program using the following command:

```
mpiexec -n <num of threads> ./out
```

1. Choose the mode you want to use: file mode or console mode.
1. Enter the necessary input values based on the mode you chose.
1. The program will then compute the result matrix and print it to the console.

## Example

Here's an example of how to use the matrix multiplication program in console mode:

```
Welcome to matrix multiplication program!
To read dimensions and values from file press 1
To read dimensions and values from console press 2
2
Please enter dimensions of the first matrix: 3 2
Please enter its elements:
1 2
5 8
1 5
Please enter dimensions of the second matrix: 2 2
Please enter its elements:
6 8
10 30

Result Matrix is (3x2):
26 68
110 280
56 158
```

In this example, the user chose console mode and entered the necessary input values. The program then computed the result matrix and printed it to the console.