/* File:     mpi_vector_add_random.c
 * 
 * Purpose:  Create two vectors of at least 100,000 elements generated randomly in parallel using MPI.
 *           Add the vectors and print the first and last 10 elements for validation.
 *           Measure the execution time for the parallel vector addition.
 *
 * Compile:  mpicc -g -Wall -o mpi_vector_add_random mpi_vector_add_random.c
 * Run:      mpiexec -n <comm_sz> ./mpi_vector_add_random
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 100000

void Generate_vector(double local_a[], int local_n);
void Print_vector(double local_b[], int local_n, int n, char title[], int my_rank, MPI_Comm comm);

int main(void) {
    int local_n;
    int comm_sz, my_rank;
    double *local_x, *local_y, *local_z;
    MPI_Comm comm;
    double tstart, tend, total_time;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    local_n = N / comm_sz;

    // Allocate vectors
    local_x = (double*) malloc(local_n * sizeof(double));
    local_y = (double*) malloc(local_n * sizeof(double));
    local_z = (double*) malloc(local_n * sizeof(double));

    // Seed the random number generator
    srand(time(NULL) + my_rank);

    // Generate random vectors locally
    Generate_vector(local_x, local_n);
    Generate_vector(local_y, local_n);

    // Start measuring time
    tstart = MPI_Wtime();

    // Perform vector addition
    for (int i = 0; i < local_n; i++) {
        local_z[i] = local_x[i] + local_y[i];
    }

    // Stop measuring time
    tend = MPI_Wtime();
    total_time = tend - tstart;

    // Print first and last 10 elements for validation (only rank 0 prints)
    Print_vector(local_x, local_n, N, "Vector x", my_rank, comm);
    Print_vector(local_y, local_n, N, "Vector y", my_rank, comm);
    Print_vector(local_z, local_n, N, "Sum vector z", my_rank, comm);

    // Print execution time from rank 0
    if (my_rank == 0) {
        printf("Time taken for vector addition: %f seconds\n", total_time);
    }

    free(local_x);
    free(local_y);
    free(local_z);

    MPI_Finalize();
    return 0;
}

// Function to generate a local vector with random values
void Generate_vector(double local_a[], int local_n) {
    for (int i = 0; i < local_n; i++) {
        local_a[i] = ((double) rand() / (RAND_MAX)) * 100.0; // Random values between 0 and 100
    }
}

// Function to print the first and last 10 elements of the distributed vector
void Print_vector(double local_b[], int local_n, int n, char title[], int my_rank, MPI_Comm comm) {
    double* b = NULL;
    int i;
    if (my_rank == 0) {
        b = malloc(n * sizeof(double));
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);

        // Print first 10 elements
        printf("First 10 elements of %s:\n", title);
        for (i = 0; i < 10; i++) {
            printf("%f ", b[i]);
        }
        printf("\n");

        // Print last 10 elements
        printf("Last 10 elements of %s:\n", title);
        for (i = n - 10; i < n; i++) {
            printf("%f ", b[i]);
        }
        printf("\n");

        free(b);
    } else {
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
    }
}
