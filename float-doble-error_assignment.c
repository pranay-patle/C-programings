#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_ROWS 3  // Number of rows in the matrices
#define NUM_COLS 3  // Number of columns in the matrices
#define FILENAME "random_numbers.txt"

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate and save random double numbers to a file
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        double random_double = ((double)rand() / (double)RAND_MAX) * 100.0; // Generate random double between 0 and 100
        fprintf(file, "%.17lf\n", random_double); // Use "%.17lf" to print with full double precision
    }

    fclose(file);

    // Read the numbers from the file and store them in matrices
    double doubleMatrix[NUM_ROWS][NUM_COLS];
    float floatMatrix[NUM_ROWS][NUM_COLS];

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (fscanf(file, "%lf", &doubleMatrix[i][j]) != 1) {
                fprintf(stderr, "Error reading from file\n");
                fclose(file);
                return 1;
            }
            floatMatrix[i][j] = (float)doubleMatrix[i][j]; // Convert to float and store in the float matrix
        }
    }

    fclose(file);

    // Calculate and store the error matrix
    double errorMatrix[NUM_ROWS][NUM_COLS];

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            errorMatrix[i][j] = fabs(doubleMatrix[i][j] - (double)floatMatrix[i][j]);
        }
    }

    // Print the matrices for verification
    printf("Double Matrix:\n");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%.17lf\t", doubleMatrix[i][j]); // Use "%.17lf" to print with full double precision
        }
        printf("\n");
    }

    printf("\nFloat Matrix:\n");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%f\t", floatMatrix[i][j]);
        }
        printf("\n");
    }

    printf("\nError Matrix:\n");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%.17lf\t", errorMatrix[i][j]); // Use "%.17lf" to print with full double precision
        }
        printf("\n");
    }

    return 0;
}
