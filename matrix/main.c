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
    int rows1, cols1,rows2,cols2 , elemDiv , rem;
    int **mat1,**mat2;

    /* recieve */
    /* Start up MPI */
    MPI_Init( &argc , &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank==0) {
        printf("Welcome to Matrix multiplication program!\n");
        printf("To read dimensions and values from file press 1\n");
        printf("To read dimensions and values from console press 2\n");

        int input_mode;
        scanf("%d", &input_mode);

        if (input_mode == 1) {
            // Read input from file
            char filename[100];
            printf("Please enter the filename: ");
            scanf("%s", filename);

            FILE* fp = fopen(filename, "r");

            fscanf(fp, "%d %d", &rows1, &cols1);
            mat1 = (int **) malloc(sizeof(int *) * rows1);
            for (int i = 0; i < rows1; i++) {
                mat1[i] = (int*)malloc(cols1 * sizeof(int));
                for (int j = 0; j < cols1; j++) {
                    fscanf(fp, "%d", &mat1[i][j]);
                }
            }

            fscanf(fp, "%d %d", &rows2, &cols2);
            mat2 = (int **) malloc(sizeof(int *) * rows2);
            for (int i = 0; i < cols2; i++) {
                mat2[i] = (int *)malloc(cols2 * sizeof(int));
                for (int j = 0; j < cols2; j++) {
                    fscanf(fp, "%d", &mat2[i][j]);
                }
            }

            fclose(fp);
        }else if (input_mode == 2) {
            // Read input from console
            printf("Please enter dimensions of the first matrix: ");
            scanf("%d%d", &rows1, &cols1);
            mat1 = (int **) malloc(sizeof(int *) * rows1);
            for (int i = 0; i < rows1; ++i) {
                mat1[i] = (int *) malloc(sizeof(int) * cols1);
            }

            printf("Please enter its elements:\n");
            for (int i = 0; i < rows1; ++i) {
                for (int j = 0; j < cols1; ++j) {
                    scanf("%d", &mat1[i][j]);
                }
            }
            printf("Please enter dimensions of the second matrix: ");
            scanf("%d%d", &rows2, &cols2);
            mat2 = (int **) malloc(sizeof(int *) * rows2);

            for (int i = 0; i < rows2; ++i) {
                mat2[i] = (int *) malloc(sizeof(int) * cols2);
            }
            printf("Please enter its elements:\n");
            for (int i = 0; i < rows2; ++i) {
                for (int j = 0; j < cols2; ++j) {
                    scanf("%d", &mat2[i][j]);
                }
            }

        }else {
            printf("Invalid input mode. Exiting.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if(cols1 != rows2){
            printf("Columns of matrix 1 must be equal to rows of matrix 2. Exiting.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        elemDiv = rows1/(p-1);
        rem = rows1%(p-1);
        for( source = 1; source < p ; source++){
            MPI_Send(&rows2, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            MPI_Send(&cols2, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            for (int i = 0; i < rows2; i++) {
                MPI_Send(mat2[i], cols2, MPI_INT, source, tag, MPI_COMM_WORLD);
            }
        }
        for( source = 1; source < (p-1) ; source++){
            MPI_Send(&elemDiv, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            MPI_Send(&cols1, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            int startRow = (source-1)*elemDiv;
            for (int i = startRow; i < (startRow+elemDiv); i++) {
                MPI_Send(mat1[i], cols1, MPI_INT, source, tag, MPI_COMM_WORLD);
            }
        }
        int lastElemDiv = elemDiv+rem;
        MPI_Send(&lastElemDiv, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
        MPI_Send(&cols1, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
        int startRow = (p-2)*elemDiv;
        for (int i = startRow; i < (startRow+lastElemDiv); i++) {
            MPI_Send(mat1[i], cols1, MPI_INT, source, tag, MPI_COMM_WORLD);
        }
    }else{

        dest = 0;
        int rows1, cols1,rows2,cols2;
        MPI_Recv(&rows2, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&cols2, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        int **mat2 = (int **) malloc(sizeof(int *) * rows2);
        for (int i = 0; i < rows2; ++i) {
            int *row= (int*)malloc( sizeof(int)*cols2);
            MPI_Recv(row, cols2, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
            mat2[i] = row;
        }

        MPI_Recv(&rows1, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&cols1, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        int **mat1= (int**)malloc( sizeof(int*)*rows1);
        int **resMat= (int**)malloc( sizeof(int*)*rows1);
        for (int i = 0; i < rows1; ++i) {
            resMat[i] = (int*)malloc( sizeof(int)*cols2);
            int *row= (int*)malloc( sizeof(int)*cols1);
            MPI_Recv(row, cols1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
            mat1[i] = row;
        }


        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                int sum = 0;
                for (int k = 0; k < cols1; ++k) {
                    sum+=(mat1[i][k]*mat2[k][j]);
                }
                resMat[i][j] = sum;
            }
        }

        MPI_Send(&rows1, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

        for (int i = 0; i < rows1; ++i) {
            MPI_Send(resMat[i], cols2, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }


//        printf("mat 1\n");
//        for (int i = 0; i < rows1; ++i) {
//            for (int j = 0; j < cols1; ++j) {
//                printf("%d",mat1[i][j]);
//            }
//            printf("\n");
//        }
//        printf("mat 2\n");
//        for (int i = 0; i < rows2; ++i) {
//            for (int j = 0; j < cols2; ++j) {
//                printf("%d",mat2[i][j]);
//            }
//            printf("\n");
//        }

    }

    if (my_rank==0){
        int **resMat= (int**)malloc( sizeof(int*)*rows1);
        for( source = 1; source < p ; source++){
            int rows;
            MPI_Recv(&rows, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            int startRow = (source-1)*elemDiv;
            for (int i = 0; i < rows; ++i) {
                int *row= (int*)malloc( sizeof(int)*cols2);
                MPI_Recv(row, cols2, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
                resMat[startRow+i] = row;
            }
        }
        printf("\n\nResult Matrix is (%dx%d):\n",rows1,cols2);
        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                printf("%d ",resMat[i][j]);
            }
            printf("\n");
        }
    }


    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}