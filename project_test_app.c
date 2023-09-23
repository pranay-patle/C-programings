#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROWS 100
#define COL 100
#define FILENAME "random_numbers.txt"
#define OUTPUT_FILENAME "average_errors.txt"

// Function to multiply two matrices
void matrixDoubleMultiply(double A[][COL], double B[][COL], double C[][COL]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < COL; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
void matrixFloatMultiply(float A[][COL], float B[][COL], float C[][COL]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < COL; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void generateRandNumbers()
{
    // Generate and save random double numbers to a file
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }

    for (int i = 0; i < ROWS * COL; i++) {
        double random_double = ((double)rand() / (double)RAND_MAX) * 2.0; // Generate random double between 0 and 100
        fprintf(file, "%.17lf\n", random_double); // Use "%.17lf" to print with full double precision
    }

    fclose(file);
    
    
}

void initializematrix(float floatMatrix[][COL],float tempMatrixFloat[][COL],double doubleMatrix[][COL],double tempMatrixDouble[][COL])
{
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            if (fscanf(file, "%lf", &doubleMatrix[i][j]) != 1) {
                fprintf(stderr, "Error reading from file\n");
                fclose(file);
                return;
            }
            floatMatrix[i][j] = (float)doubleMatrix[i][j]; // Convert to float and store in the float matrix
            tempMatrixFloat[i][j]= floatMatrix[i][j];
            tempMatrixDouble[i][j]= doubleMatrix[i][j]; 
        }
    }

    fclose(file);
    return;
}

void printMatrices(float floatMatrix[][COL],double doubleMatrix[][COL],double errorMatrix[][COL])
{
    printf("Double Matrix:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%.17lf\t", doubleMatrix[i][j]); // "%.17lf" to print double precision
        }
        printf("\n");
    }

    printf("\nFloat Matrix:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%f\t", floatMatrix[i][j]);
        }
        printf("\n");
    }

    printf("\nError Matrix:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%.17lf\t", errorMatrix[i][j]); // Use "%.17lf" to print with full double precision
        }
        printf("\n");
    }

    double errorSum = 0.0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            errorSum += errorMatrix[i][j];
        }
    }

    double averageError = errorSum / (ROWS * COL);

    printf("Average Error: %.17lf\n", averageError);
}

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    generateRandNumbers();

    double doubleMatrix[ROWS][COL],tempMatrixDouble[ROWS][COL];
    float floatMatrix[ROWS][COL],tempMatrixFloat[ROWS][COL];
    initializematrix(floatMatrix,tempMatrixFloat,doubleMatrix,tempMatrixDouble);
        
    // Calculate and store the error matrix
    double errorMatrix[ROWS][COL];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            errorMatrix[i][j] = fabs(doubleMatrix[i][j] - (double)floatMatrix[i][j]);
        }
    }

    // // Initial matrices
    printMatrices(floatMatrix,doubleMatrix,errorMatrix);
    
    // Create and open the output file for saving average errors
    FILE* outputFile = fopen(OUTPUT_FILENAME, "w");
    if (outputFile == NULL) {
        printf("Error opening output file");
        return 1;
    }
    double avg_error[15];
    // Perform matrix multiplication for both double and float matrices
    double resultDoubleMatrix[ROWS][COL] = {0};
    float resultFloatMatrix[ROWS][COL] = {0};

    //degree of multiplication
    for (int degree = 2; degree <= 8; degree++) {
        
        matrixDoubleMultiply(tempMatrixDouble, doubleMatrix, resultDoubleMatrix);
        matrixFloatMultiply(tempMatrixFloat, floatMatrix, resultFloatMatrix);

        // Update temp matrices for the next iteration
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COL; j++) {
                tempMatrixDouble[i][j] = resultDoubleMatrix[i][j];
                tempMatrixFloat[i][j] = resultFloatMatrix[i][j];
            }
        }
    
        // Calculate and store the error matrix for this degree
        double errorSum = 0.0;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COL; j++) {
                errorSum += fabs(resultFloatMatrix[i][j] - resultDoubleMatrix[i][j]);
                
            }
        }
        
        double averageError = errorSum / (ROWS * COL);

        printf("Degree %d - Average Error: %.17lf\n", degree, averageError);

        // Save the degree and average error to the output file
        //fprintf(outputFile, "Degree %d - Average Error: %.17lf\n", degree, averageError);
        fprintf(outputFile, "%d %.17lf\n", degree, averageError);
        avg_error[degree-2] = averageError;
    }
    // Using gnuplot to plot the data
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe == NULL) {
        printf("Error opening gnuplot pipe");
        return 1;
    }
    // Ploting the data
    fprintf(gnuplotPipe, "set title 'ERROR PLOT'\n");
    fprintf(gnuplotPipe, "set xlabel 'Degree of multiplication'\n");
    fprintf(gnuplotPipe, "set ylabel 'Average Error'\n");
    fprintf(gnuplotPipe, "plot 'average_errors.txt' with lines\n");
    // Closing gnuplot
    fclose(gnuplotPipe);

    fclose(outputFile);

    return 0;
}





