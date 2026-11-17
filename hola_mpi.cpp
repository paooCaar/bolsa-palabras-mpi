#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // quién soy yo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // cuántos procesos hay

    std::cout << "Hola desde el proceso " << rank << " de " << size << std::endl;

    MPI_Finalize();
    return 0;
}
