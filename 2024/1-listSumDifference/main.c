#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int getRowCount(FILE *file) {
    int count = 0;
    char line[32];
    int maxLineLength = sizeof(line);
    
    while (fgets(line, maxLineLength, file) != NULL) {
        count++;
    }

    rewind(file);
    return count;
}

void getLists(FILE *file, int* column1, int* column2, int rowCount) {
    char line[32];
    int lineIndex = 0;
    int maxLineLength = sizeof(line);
    
    while (fgets(line, maxLineLength, file) != NULL) {
        char* endPtr;

        column1[lineIndex] = strtol(line, &endPtr, 10);

        while (*endPtr == ' ' || *endPtr == '\t' || *endPtr == '\n') {
            endPtr++;
        }
        column2[lineIndex] = strtol(endPtr, &endPtr, 10);

        lineIndex++;
    }

    rewind(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    const int rowCount = getRowCount(file);

    int* column1 = (int*)malloc(rowCount * sizeof(int));
    int* column2 = (int*)malloc(rowCount * sizeof(int));

    getLists(file, column1, column2, rowCount);

    qsort(column1, rowCount, sizeof(int), compare);
    qsort(column2, rowCount, sizeof(int), compare);

    long listsSumDifference = 0;

    for (int i = 0; i < rowCount; i++) {
        listsSumDifference += abs(column1[i] - column2[i]);
    }

    printf("%ld\n", listsSumDifference);

    free(column1);
    free(column2);

    fclose(file);

    return 0;
}
