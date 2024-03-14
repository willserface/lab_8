#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
    // Will Serface's Implementation

    if (r - l <= 0) { // Recursive Exit Case
        return; // List with 1 or fewer elements is sorted
    } else {
        int mid = l + (r - l) / 2; // Find middle index of sort
        mergeSort(pData, l, mid); // Sort lower half of index
        mergeSort(pData, mid + 1, r); // Sort upper half of index

        int left = mid - l + 1, right = r - mid; // Calculate sizes of left and right arrays to be allocated

        int *leftArray = Alloc(left * sizeof(int)), // Allocate memory for left (lower) half of array
        *rightArray = Alloc(right * sizeof(int)); // Allocate memory for right (upper) half of array

        for (int i = l; i <= r; ++i) { // Repeat for each item in l to r range
            if (i <= mid) { // Check which array to populate
                leftArray[i - l] = pData[i]; // Save item to lower array
            } else {
                rightArray[i - mid - 1] = pData[i]; // Save item to upper array
            }
        }

        int lIndex = 0, rIndex = 0; // Tracks the current merge index of lower and upper arrays

        for (int i = l; i <= r; ++i) { // Loop through all numbers to be merged
            if (lIndex >= left) { // Check if lower array is empty
                pData[i] = rightArray[rIndex++]; // Load current index of Data from upper array
            } else if (rIndex >= right) { // Check if upper array is empty
                pData[i] = leftArray[lIndex++]; // Load current index of Data from lower array
            } else { // Both arrays have items remaining
                if (leftArray[lIndex] < rightArray[rIndex]) { // Compare current indexes of lower and upper arrays
                    pData[i] = leftArray[lIndex++]; // Load current index of Data from lower array
                } else {
                    pData[i] = rightArray[rIndex++]; // Load current index of Data from upper array
                }
            }
        }

        // Deallocate memory for lower and upper arrays
        DeAlloc(leftArray);
        DeAlloc(rightArray);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}