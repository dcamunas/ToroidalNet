#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <math.h>
#include <mpi.h>

FILE *open_file(const char * path, const char *mode);
int load_data(double *data);
int check_size(int size, int numbers_n);
void add_numbers(double *data, int size, int rank);
void get_knowns(int rank, int *knowns);

int main(int argc, char *argv[])
{

    /*Variables*/
    int rank, size, numbers_n, finish;
    double number;
    double *data = malloc(DATA_SIZE);
    int *knows = malloc(4*sizeof(double));
    MPI_Status status;

    /* Initialize MPI program */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    /* Get side's value */
    /*const int L = (int)sqrt(size);*/

    if(rank == 0)
    {
        /*Get quantity of numbers*/
        numbers_n = load_data(data);

        finish = check_size(size, numbers_n);

        if(finish != TRUE)
        {
          add_numbers(data, size, rank); 
        }
    }

    MPI_Bcast(&finish, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(finish != TRUE)
    {
        MPI_Recv(&number, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        
        get_knowns(rank, knows);
        printf("[X] RANK[%d]: NORTH[%d] | SOUTH[%d] | WEST[%d] | EAST[%d]\n", 
            rank, knows[NORTH], knows[SOUTH], knows[WEST], knows[EAST]);
        
    }

    /* Finalize MPI program */
    MPI_Finalize();

    return EXIT_SUCCESS;
}

/*Get rank's neighbours*/
void get_knowns(int rank, int *knowns)
{
    int row = rank/L;
    int column = rank%L;

    switch (row)
    {
    case 0:
        knowns[NORTH] = L *(L - 1) + rank;
        knowns[SOUTH] = rank + L;
        break;

    case L - 1: 
        knowns[NORTH] = rank - L;
        knowns[SOUTH] = rank % L;
        break;

    default:
        knowns[NORTH] = rank - L;
        knowns[SOUTH] = rank + L;
        break;
    }


    switch (column)
    {
    case 0:
        knowns[WEST] = rank + (L-1);
        knowns[EAST] = rank + 1;
        break;
    
    case L - 1:
        knowns[WEST] = rank - 1;
        knowns[EAST] = rank - (L - 1); 
        break;

    default:
        knowns[WEST] = rank - 1;
        knowns[EAST] = rank + 1;
        break;
    }

}

/* Add number to nodes (ranks) */
void add_numbers(double *data, int size, int rank)
{
    int i;
    double number;

    for (i = 0; i < size; i++)
    {
        number = data[i];
        MPI_Send(&number, 1, MPI_DOUBLE, i, rank, MPI_COMM_WORLD);
    }

    free(data);
}

/* Check toroid's size (nº nodes) */
int check_size(int size, int numbers_n)
{
    int finish = FALSE;
    if(size != numbers_n)
    {
        fprintf(stderr, "[X] PROCESS[0]: Error, quantity of numbers (%d) is diferent at toroid's size (%d | L = %d)\n", 
            numbers_n, size, L);
        finish = TRUE;
    }

    return finish;
}

/* Load data (numbers) from datos.dat */
int load_data(double *data)
{
    
    FILE *file = open_file(DATA_PATH, READ_MOD);
    char line[MAX_SIZE];
    char *token;
    int i;

    fgets(line, sizeof(line), file);
    fclose(file);

    data[0] = atof(strtok(line, SEPARATOR));

    for(i = 1; (token = strtok(NULL, SEPARATOR)) != NULL; i++)
    {
        data[i] = atof(token);
    }

    return i;
}

/* Open file */
FILE *open_file(const char * path, const char *mode)
{
    FILE *file;
    if ((file = fopen(path, mode)) == NULL)
    {
        fprintf(stderr, "[X] PROCESS[0]: Error opening file.\n");        
        exit(EXIT_FAILURE);
    }
    return file;
}
