#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void CalcularNumero(int index, int numero, int *resultados);

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Definir la cantidad de números en la serie
    const int cantidadNumeros = 100000;

    // Calcular la cantidad de números por proceso
    const int numerosPorProceso = cantidadNumeros / size;

    // Inicializar arreglos para almacenar los números y los resultados
    int *numeros = (int *)malloc(numerosPorProceso * sizeof(int));
    int *resultados = (int *)malloc(numerosPorProceso * sizeof(int));

    // Crear y ejecutar tareas en paralelo utilizando MPI_Scatter
    MPI_Scatter(NULL, 0, MPI_INT, numeros, numerosPorProceso, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < numerosPorProceso; i++) {
        // Asignar valores a los números
        numeros[i] = rank * numerosPorProceso + i + 1;

        // Calcular y almacenar el resultado para cada número
        CalcularNumero(i, numeros[i], resultados);
    }

    // Recopilar resultados en el proceso raíz
    MPI_Gather(resultados, numerosPorProceso, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Mostrar toda la serie en el proceso raíz
        printf("Serie generada con 100000 posiciones:\n");
        for (int i = 0; i < cantidadNumeros; i++) {
            printf("%d", resultados[i]);
            if (i < cantidadNumeros - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    // Liberar memoria
    free(numeros);
    free(resultados);

    MPI_Finalize();
    return 0;
}

// Método para calcular y asignar el resultado según si el número es par o impar
void CalcularNumero(int index, int numero, int *resultados) {
    resultados[index] = (numero % 2 == 0) ? numero * 2 : numero * 2 + 1;
}
