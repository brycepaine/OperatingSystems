/*Taoh Green (tmgreen@ucsc.edu)
 *Sajan Patel (savipate@ucsc.edu)
 *NAME: sortPrint.c
 *DESCRIPTION: creates two arrays of numbers by reading them
    from a text file. It then passes these arrays to a heapsort 
    and insertionSort algorithm, which returns the same array but in
    sorted order. It then prints the sorted arrays.
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "insertionSort.h"
#include "heapSort.h"


int main(int argc, char **argv) {
    FILE *file = fopen(argv[1], "r");
    if(file==NULL) {
        printf("Need to give program input file.\n");
        return EXIT_FAILURE;
    }
    char str[60];

    int num;
    double testIn;
    int numKeys = 0;  
    if(fgets(str,60,file) != NULL) {
        int test = sscanf (str, "%d%lg", &numKeys, &testIn);
        if(test!=1) {
            fprintf(stderr, "bad input\n");
            return EXIT_FAILURE;
        }
    }

    int i = 0;
    int insertKeys[numKeys];
    int heapKeys[numKeys];
    
    while(fgets(str,60,file) != NULL) {
        int test = sscanf (str, "%d%lg", &num, &testIn);
        if(test!=1) {
            fprintf(stderr, "bad input\n");
            return EXIT_FAILURE;
        }
        heapKeys[i+1] = num;
        insertKeys[i] = num;
        ++i;
    }
    
    fclose(file); 
    heapSort(heapKeys, numKeys);
    insertionSort (insertKeys, numKeys);

    printf("Heap sorted: %d keys\n", numKeys);
    for(int i = 1; i < numKeys; ++i) {
        if (i != numKeys-1) assert(heapKeys[i] <= heapKeys[i+1]);
        printf("%d\n", heapKeys[i]);
    }
    printf("Insertion sorted: %d keys\n", numKeys);
    for(int i = 0; i < numKeys; ++i) {
        if (i != numKeys-1) assert(insertKeys[i] <= insertKeys[i+1]);
        printf("%d\n", insertKeys[i]);
    }
    return EXIT_SUCCESS;
}




