#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mpi.h"

bool checkChar(char c) {
    if ((c >= 65 && c<=90) || (c >= 97 && c<=122))
        return true;
    else
        return false;
}

char encrypt(int key, char c) {
    if(c >= 97){
        c = c - 'a';
    }else{
        c = c - 'A';
    }
    c = (c + key) % 26;
    if(c >= 97){
        c = c + 'a';
    }else{
        c = c + 'A';
    }
    return c;
}

int main(int argc , char * argv[]){
    int my_rank; /* rank of process */
    int p; /* number of process */
    int source; /* rank of sender */
    int dest; /* rank of reciever */
    int tag = 0; /* tag for messages */
    MPI_Status status; /* return status for */
    int rem;
    /* recieve */
    /* Start up MPI */
    MPI_Init( &argc , &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    char unencryptedText[1000000];
    char encryptedText[1000000];
    int size_of_message=0,key,local_size;
    char* local_un;
    char* local_encrypted;

    //take input from txt file called input first line as int in variable called key second line called plain text
    if (my_rank==0) {

    FILE *fp = fopen("Plaintext.txt", "r");

    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
	char c = getc(fp);
	while(c!=EOF){
		unencryptedText[size_of_message++] = c;
		c = getc(fp);
	}

    fclose(fp);

    //Broadcast key value to all slaves
        printf("Enter KEY/SHIFT: ");
        scanf("%d",&key);

    }
    MPI_Bcast(&size_of_message,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&key,1,MPI_INT,0,MPI_COMM_WORLD);
    local_size=size_of_message/(p-1);
    rem=size_of_message%(p-1);
    if (rem >0)
    {
        local_size++;
    }
    int overhead= local_size*(p-1)-size_of_message;

    //local unencrypted array to receive the local message
    local_un=(char*)malloc(sizeof(char)*local_size);
    //local encrypted
    local_encrypted=(char*)malloc(sizeof(char)*local_size);
    MPI_Scatter(unencryptedText,local_size,MPI_CHAR,local_un,local_size,MPI_CHAR,0,MPI_COMM_WORLD);

    int length=0;
    if (overhead>0 && my_rank==(p-1))
    {
        length=local_size-overhead;
    }
    else {
        length = local_size;
    }
    for (int i=0; i<length; i++)
    {
        if (checkChar(local_un[i]))
            local_encrypted[i]= encrypt(key,local_un[i]);
        else
            local_encrypted[i]=local_un[i];
    }

    MPI_Gather(local_encrypted,local_size,MPI_CHAR,encryptedText,local_size,MPI_CHAR,0,MPI_COMM_WORLD);

    if (my_rank==0) {

        //print encrypted text
        printf("KEY/SHIFT: %d\n", key);
        printf("Plaintext.txt:  %s\n", unencryptedText);
        printf("Ciphertext.txt:  %s\n", encryptedText);

        //print encrypted text to output file
        FILE *output;
        output = fopen("Ciphertext.txt", "w");
        fprintf(output, "%s", encryptedText);
        fclose(output);
    }
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}