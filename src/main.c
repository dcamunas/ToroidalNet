#include <stdio.h>
#include <mpi.h>
#include <ejemplos.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    
    //Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //Get the name of the process
    char name_process[256];


    printf("Hello world.\n");
    printf("%s\n", HOLA);
    MPI_Finalize();
    
    return 0;
}









/* int check_argv(int argc, char *argv){
    if(atoi(argv[argc-1])){
        return TRUE;
    }
    else{
        return FALSE;
    }
}
*/
