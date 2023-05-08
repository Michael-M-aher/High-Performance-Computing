## Encrypting a Message Using Caesar Cipher

The program takes a message from a file, encrypts it using Caesar Cipher, and stores the output in a new file.

### Master Process

The master process performs the following steps:

1. Read the entire message from the given file and store it in a char array.
2. Read the key (shift) value from the user.
3. Broadcast the key value to all slave processes.
4. Scatter the message to all slave processes.
5. Handle the remaining part of the message (if exists).
6. Gather encrypted message parts from slaves.
7. Write the entire encrypted message to a file.

### Slave Processes

Each slave process performs the following steps:

1. Receive the key (shift) value from the master process.
2. Receive the part of the message to be encrypted using scatter.
3. Encrypt the received part using the given key value.
4. Send the encrypted part to the master process using gather.

Note: The size of the message may not be divisible by the number of processes, so the program must handle this case.

## Usage

To use the encryption program, follow these steps:

1. Make sure you have installed MPI on your computer.
2. Clone the repository to your local machine.
3. Compile the program using the following command:

```
mpicc encrypt.c -o out
```

4. Run the program using the following command:

```
mpiexec -n <num of threads> ./out 
```

5. Enter the necessary input values.

6. The program will then encrypt the message and write it to the output file.

## Example

Here's an example of how to use the program:

Input file (plaintext.txt):
```
THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG
```

Command:
```
mpiexec -n 4 ./out
```
Required Input
```
KEY/SHIFT: <shift value>
```

Output file (ciphertext.txt):
```
QEB NRFZH YOLTK CLU GRJMP LSBO QEB IXWV ALD
```

In this example, the user entered the necessary input values. The program then encrypted the message using Caesar Cipher with a key value of 23 and wrote the result to the output file.