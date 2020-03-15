#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <math.h>
#include <mpi.h>

FILE *open_file(const char * path, const char *mode);
int load_data(long double *data);
int check_size(int size, int numbers_n);
void add_numbers(long double *data, int size, int rank);
void get_neighbors(int rank, int *neighbors);
long double calculate_min(int rank, long double my_number, int *neighbors);
void print_min_number(int rank, long double min_number);

int main(int argc, char *argv[])
{

    /*Variables*/
    int rank, size, numbers_n, finish;
    long double number, min_number;
    long double *data = malloc(DATA_SIZE);
    int *neighbors = malloc(NEIGHBORS_SIZE);
    MPI_Status status;

    /* Initialize MPI program */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    /* Get side's value */
    /*const int L = (int)sqrt(size);*/

    if(rank == FIRST_RANK)
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
        MPI_Recv(&number, 1, MPI_LONG_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        get_neighbors(rank, neighbors);
        
        printf("[X] RANK[%d] --> %.2Lf\n", rank, number); 
    
        min_number = calculate_min(rank, number, neighbors);

        print_min_number(rank, min_number);
        
    }

    /* Finalize MPI program */
    MPI_Finalize();

    return EXIT_SUCCESS;
}


long double calculate_min(int rank, long double my_number, int *neighbors)
{
    int i;
    long double his_number;


    /* Calculate rows */
    for(i = 1; i < L; i++)
    {
        MPI_Send(&my_number,1,MPI_LONG_DOUBLE, neighbors[SOUTH], SEND_TAG, MPI_COMM_WORLD);
        MPI_Recv(&his_number, 1, MPI_LONG_DOUBLE, neighbors[NORTH], MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        my_number = (his_number > my_number ? my_number : his_number);
    }

    for(i = 1; i < L; i++)
    {
        MPI_Send(&my_number,1,MPI_LONG_DOUBLE, neighbors[EAST], SEND_TAG, MPI_COMM_WORLD);
        MPI_Recv(&his_number, 1, MPI_LONG_DOUBLE, neighbors[WEST], MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        my_number = (his_number > my_number ? my_number : his_number);
    }

    free(neighbors);

    return my_number;
}

void print_min_number(int rank, long double min_number)
{
    if(rank == FIRST_RANK){
        printf("\n[X] RANK[%d]: The minium value is: %.2Lf\n", rank, min_number);
    
    /*End message*/
    printf("----------------------------------------------------------------------------------------\n");
    printf("\t\t\t***** PROGRAM FINALIZED *****\n\n");
    }
}


/*Get rank's neighbours*/
void get_neighbors(int rank, int *neighbors)
{
    int row = rank/L;
    int column = rank%L;

    switch (row)
    {
    case 0:
        neighbors[NORTH] = L *(L - 1) + rank;
        neighbors[SOUTH] = rank + L;
        break;

    case L - 1: 
        neighbors[NORTH] = rank - L;
        neighbors[SOUTH] = rank % L;
        break;

    default:
        neighbors[NORTH] = rank - L;
        neighbors[SOUTH] = rank + L;
        break;
    }


    switch (column)
    {
    case 0:
        neighbors[WEST] = rank + (L-1);
        neighbors[EAST] = rank + 1;
        break;
    
    case L - 1:
        neighbors[WEST] = rank - 1;
        neighbors[EAST] = rank - (L - 1); 
        break;

    default:
        neighbors[WEST] = rank - 1;
        neighbors[EAST] = rank + 1;
        break;
    }

}

/* Add number to nodes (ranks) */
void add_numbers(long double *data, int size, int rank)
{
    int i;
    long double number;

    for (i = 0; i < size; i++)
    {
        number = data[i];
        MPI_Send(&number, 1, MPI_LONG_DOUBLE, i, SEND_TAG, MPI_COMM_WORLD);
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
int load_data(long double *data)
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
