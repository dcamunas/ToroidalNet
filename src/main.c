#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <math.h>
#include <mpi.h>

FILE *open_file(const char * path, const char *mode);
int load_data(double *data);
void check_size(int size, int L, int numbers_n, int *finish);

int main(int argc, char *argv[]){

    /*Variables*/
    int rank, size, L, numbers_n;
    double *data = malloc(DATA_SIZE);
    int finish = FALSE;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    /* Get side's value */
    L = (int)sqrt(size);

    if(rank == 0)
    {
        /*Get quantity of numbers*/
        numbers_n = load_data(data);

        check_size(size, L, numbers_n, &finish);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}

void check_size(int size, int L, int numbers_n, int *finish)
{
    if(size != numbers_n)
    {
        fprintf(stderr, "[X] PROCESS[0]: Error, quantity of numbers (%d) is diferent at toroid's size (%d | L = %d)\n", numbers_n, size, L);
        *finish = TRUE;
        /*MPI_Bcast()*/
    }
}

int load_data(double *data){
    
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

    return i+1;
}


/*Open file*/
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
