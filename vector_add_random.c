/* File:     vector_add.c
 * 
 * Purpose:  Create two vectors of at least 100,000 elements generated randomly.
 *           Add the two vectors and print the first and last 10 elements for validation.
 *           Measure the execution time for the vector addition.
 *
 * Compile:  gcc -g -Wall -o vector_add_random vector_add_random.c
 * Run:      ./vector_add
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000

void Generate_vector(double a[], int n);
void Print_vector(double a[], int n);

int main(void) {
    double x[N], y[N], z[N];
    int i;
    clock_t start, end;  // Variables to measure time
    double cpu_time_used;

    // Seed the random number generator
    srand(time(NULL));

    // Generate random vectors x and y
    Generate_vector(x, N);
    Generate_vector(y, N);

    // Start measuring time before vector addition
    start = clock();

    // Add the vectors element-wise
    for (i = 0; i < N; i++) {
        z[i] = x[i] + y[i];
    }

    // Stop measuring time after vector addition
    end = clock();

    // Calculate the time in seconds
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to add vectors: %f seconds\n", cpu_time_used);

    // Print first and last 10 elements of each vector
    printf("First 10 elements of vector x:\n");
    Print_vector(x, 10);
    
    printf("Last 10 elements of vector x:\n");
    Print_vector(x + N - 10, 10);
    
    printf("First 10 elements of vector y:\n");
    Print_vector(y, 10);
    
    printf("Last 10 elements of vector y:\n");
    Print_vector(y + N - 10, 10);
    
    printf("First 10 elements of vector z (sum):\n");
    Print_vector(z, 10);
    
    printf("Last 10 elements of vector z (sum):\n");
    Print_vector(z + N - 10, 10);

    return 0;
}

// Function to generate a vector with random values
void Generate_vector(double a[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        a[i] = ((double) rand() / (RAND_MAX)) * 100.0; // Random values between 0 and 100
    }
}

// Function to print n elements of a vector
void Print_vector(double a[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%f ", a[i]);
    }
    printf("\n");
}
