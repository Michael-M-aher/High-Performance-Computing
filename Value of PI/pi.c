#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

    int my_rank;       /* rank of process */
    int p;             /* number of process */
    int source;        /* rank of sender */
    int dest;          /* rank of reciever */
    int tag = 0;       /* tag for messages */
    MPI_Status status; /* return status for */
    int div, rem;
    /* recieve */
    /* Start up MPI */
    MPI_Init(&argc, &argv);
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    static long num_steps = 1000000;
    if (my_rank == 0)
    {
        div = (int)num_steps / (p - 1);
        rem = (int)num_steps % (p - 1);
    }
    MPI_Bcast(&div, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&rem, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double step, x, pi, sum, lsum = 0.0;
    step = 1.0 / (double)num_steps;

    if (my_rank != 0)
    {
        int i;
        int start = 0 + (my_rank - 1) * div;
        int end = start + div;
        if (my_rank == (p - 1))
            end += rem;
        for (i = start; i < end; i++)
        {
            x = ((double)(i + 0.5)) * step;
            lsum += 4.0 / (1.0 + x * x);
        }
    }

    MPI_Reduce(&lsum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        pi = step * sum;
        printf("Master process announce PI = %.20f\n", pi);
    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}