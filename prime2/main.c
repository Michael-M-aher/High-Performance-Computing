#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int isPrime(int x){
    if(x<=1) return 0;
    for (int i = 2; i <= x/2; ++i) {
        if(x%i == 0){
            return 0;
        }
    }
    return 1;
}
int main(int argc , char * argv[])
{
    int my_rank; /* rank of process */
    int p; /* number of process */
    int source; /* rank of sender */
    int dest; /* rank of reciever */
    int tag = 0; /* tag for messages */
    MPI_Status status; /* return status for */
    int arrSz , r , rem,x,y,cnt;
    /* recieve */
    /* Start up MPI */
    MPI_Init( &argc , &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0)
    {
        x =1;
        y = 16;
        r = (y-x)/(p-1);
        rem = (y-x)%(p-1);
    }
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&rem, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int* arr = malloc(sizeof(int)*r);
    int start = x+(my_rank-1)*r;
    int end = start + r-1;
    if(my_rank == (p-1)) end+=rem;
    int count = 0;
    for (int i = start; i <= end; ++i) {
        if(isPrime(i) ==1){
            arr[count] = i;
            count++;
        }
    }
    MPI_Reduce(&count, &cnt, 1, MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);
    if(my_rank == 0)
    {
        printf("Count = %i\n", cnt);
    }

//    if (my_rank==0){
//        int cnt=0;
//        int* arr = malloc(sizeof(int)*(upper-lower));
//        for( source = 1; source < p ; source++)
//        {
//            int c;
//            int* arrLocal = malloc(sizeof(int)*elemDiv);
//            MPI_Recv( &c, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
//            MPI_Recv( arrLocal, c, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
//            for (int i = 0; i < c; ++i) {
//                arr[cnt+i] =  arrLocal[i];
//            }
//            int start = lower+(source-1)*elemDiv;
//            int end = start + elemDiv-1;
//            printf("p%d: calculate partial count of prime numbers from %d to %d --> Count = %d (",source,start,end,c);
//            for (int i = 0; i < c-1; ++i) {
//                printf("%d, ",arrLocal[i]);
//            }
//            printf("%d)\n",arrLocal[c-1]);
//            cnt+=c;
//        }
//        printf("After reduction, P%d will have Count = %d (",my_rank,cnt);
//        for (int i = 0; i < cnt-1; ++i) {
//            printf("%d, ",arr[i]);
//        }
//        printf("%d)\n",arr[cnt-1]);
//
//    }
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}