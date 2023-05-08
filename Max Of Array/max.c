#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc , char * argv[])
{
    int my_rank; /* rank of process */
    int p; /* number of process */
    int source; /* rank of sender */
    int dest; /* rank of reciever */
    int tag = 0; /* tag for messages */
    MPI_Status status; /* return status for */
    int arrSz , elemDiv , rem;
    /* recieve */
    /* Start up MPI */
    MPI_Init( &argc , &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank==0)
    {
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n\n",p-1);
        printf("Please enter size of the array...\n");
        scanf("%d",&arrSz);
        int *arr= (int*)malloc( sizeof(int)*arrSz);
        printf("\nPlease enter array elements...\n\n");
        for (int i = 0; i < arrSz; ++i) {
            scanf("%d",&arr[i]);
        }
        elemDiv = arrSz/(p-1);
        rem = arrSz%(p-1);
        for( source = 1; source < p-1 ; source++)
        {
            MPI_Send( &elemDiv, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            int partArr = (source-1)*elemDiv;
            MPI_Send( &arr[partArr], elemDiv, MPI_INT, source, tag, MPI_COMM_WORLD);
        }
        int lastElemDiv = elemDiv+rem;
        MPI_Send( &lastElemDiv, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
        int lastPartArr = (p-2)*elemDiv;
        MPI_Send( &arr[lastPartArr], lastElemDiv, MPI_INT, source, tag, MPI_COMM_WORLD);
    }else{
        dest = 0;
        int slaveElemDiv;
        MPI_Recv(&slaveElemDiv, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        int *arr= (int*)malloc( sizeof(int)*slaveElemDiv);
        MPI_Recv(arr, slaveElemDiv, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        int max = -100000000,maxIdx;
        for (int i = 0; i < slaveElemDiv; ++i) {
            if(arr[i]>max){
                max = arr[i];
                maxIdx = i;
            }
        }

        MPI_Send( &max, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Send( &maxIdx, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

    if (my_rank==0){

        int globalMx = -100000000;
        int globalMaxIdx;
        for( source = 1; source < p ; source++)
        {
            int mx=0,maxIdx;
            MPI_Recv(&mx, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&maxIdx, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            printf("Hello from slave#%d Max number in my partition is %d and index is %d.\n",source,mx,maxIdx);
            if(mx >globalMx){
                globalMx = mx;
                globalMaxIdx = (source-1)*elemDiv+maxIdx;
            }

        }
        printf("Master process announce the final max which is %d and its index is %d.\n\n",globalMx,globalMaxIdx);
        printf("Thanks for using our program\n");
    }
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}