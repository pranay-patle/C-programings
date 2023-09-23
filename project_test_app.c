#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_ROWS 10// Number of rows in the matrices
#define NUM_COLS 10 // Number of columns in the matrices
#define FILENAME "random_numbers.txt"
#define OUTPUT_FILENAME "average_errors.txt"

// Function to multiply two matrices
void matrixDoubleMultiply(double A[][NUM_COLS], double B[][NUM_COLS], double C[][NUM_COLS]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < NUM_COLS; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
void matrixFloatMultiply(float A[][NUM_COLS], float B[][NUM_COLS], float C[][NUM_COLS]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < NUM_COLS; k++) {
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
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        double random_double = ((double)rand() / (double)RAND_MAX) * 2.0; // Generate random double between 0 and 100
        fprintf(file, "%.17lf\n", random_double); // Use "%.17lf" to print with full double precision
    }

    fclose(file);
    
    
}

void initializematrix(float floatMatrix[][NUM_COLS],float tempMatrixFloat[][NUM_COLS],double doubleMatrix[][NUM_COLS],double tempMatrixDouble[][NUM_COLS])
{
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
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

void printMatrices(float floatMatrix[][NUM_COLS],double doubleMatrix[][NUM_COLS],double errorMatrix[][NUM_COLS])
{
    printf("Double Matrix:\n");
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%.17lf\t", doubleMatrix[i][j]); // "%.17lf" to print double precision
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

    double errorSum = 0.0;

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            errorSum += errorMatrix[i][j];
        }
    }

    double averageError = errorSum / (NUM_ROWS * NUM_COLS);

    printf("Average Error: %.17lf\n", averageError);
}

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    generateRandNumbers();

    double doubleMatrix[NUM_ROWS][NUM_COLS],tempMatrixDouble[NUM_ROWS][NUM_COLS];
    float floatMatrix[NUM_ROWS][NUM_COLS],tempMatrixFloat[NUM_ROWS][NUM_COLS];
    initializematrix(floatMatrix,tempMatrixFloat,doubleMatrix,tempMatrixDouble);
        
    // Calculate and store the error matrix
    double errorMatrix[NUM_ROWS][NUM_COLS];

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            errorMatrix[i][j] = fabs(doubleMatrix[i][j] - (double)floatMatrix[i][j]);
        }
    }

    // // Initial matrices
    printMatrices(floatMatrix,doubleMatrix,errorMatrix);
    
    // Create and open the output file for saving average errors
    FILE* outputFile = fopen(OUTPUT_FILENAME, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }
    double avg_error[15];
    // Perform matrix multiplication for both double and float matrices
    double resultDoubleMatrix[NUM_ROWS][NUM_COLS] = {0};
    float resultFloatMatrix[NUM_ROWS][NUM_COLS] = {0};

    //degree of multiplication
    for (int degree = 2; degree <= 10; degree++) {
        
        matrixDoubleMultiply(tempMatrixDouble, doubleMatrix, resultDoubleMatrix);
        matrixFloatMultiply(tempMatrixFloat, floatMatrix, resultFloatMatrix);

        // Update temp matrices for the next iteration
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                tempMatrixDouble[i][j] = resultDoubleMatrix[i][j];
                tempMatrixFloat[i][j] = resultFloatMatrix[i][j];
            }
        }
    
        // Calculate and store the error matrix for this degree
        double errorSum = 0.0;

        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                errorSum += (resultFloatMatrix[i][j] - resultDoubleMatrix[i][j]);
                
            }
        }
        
        double averageError = errorSum / (NUM_ROWS * NUM_COLS);

        printf("Degree %d - Average Error: %.17lf\n", degree, averageError);

        // Save the degree and average error to the output file
        //fprintf(outputFile, "Degree %d - Average Error: %.17lf\n", degree, averageError);
        fprintf(outputFile, "%d %.17lf\n", degree, averageError);
        avg_error[degree-2] = averageError;
    }
    // Using gnuplot to plot the data
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe == NULL) {
        perror("Error opening gnuplot pipe");
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





