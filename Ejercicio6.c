#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    const int cantidadNumeros = 100000;
    int *numeros = (int *)malloc(cantidadNumeros * sizeof(int));
    int *resultados = (int *)malloc(cantidadNumeros * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Dividir el trabajo entre los procesos
    int inicio = rank * (cantidadNumeros / size);
    int fin = (rank + 1) * (cantidadNumeros / size);
    for (int i = inicio; i < fin; i++) {
        numeros[i] = i + 1;
        resultados[i] = (numeros[i] % 2 == 0) ? numeros[i] * 2 : numeros[i] * 2 + 1;
    }

    // Recopilar los resultados en el proceso 0
    MPI_Gather(resultados + inicio, cantidadNumeros / size, MPI_INT, resultados, cantidadNumeros / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Mostrar la serie generada
        printf("Serie generada con 100000 posiciones: ");
        for (int i = 0; i < cantidadNumeros; i++) {
            printf("%d", resultados[i]);
            if (i < cantidadNumeros - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free(numeros);
    free(resultados);

    MPI_Finalize();
    return 0;
}
