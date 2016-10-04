#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>

float dia = 0.000001; //The dia (distance) between compatiible cells
struct elements{
  int index;
  float cellElement;
};


struct collisions{
  long signature;
  elements blocks[10];
};
struct Blocks{
  long signature;
  int Elements[4];
  int column;
};

Blocks block[40000];
elements potentialBlocks[100];
elements elementMatrix[4400][500];
elements flippedMatrix[500][4400];
long keyArray[4400] = {0};//The matrix that will store all the key data

/* Reads in the text file containing the data, from here
   it will populate the array that we have designated. */

int readingMatrix()
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int i = 0;
  int j = 0;
  fp = fopen("data.txt", "r");
  if(fp == NULL) exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1 )
  {
    const char s[2] = ",";
    char *token;
    token = strtok(line, s);
    j = 0;

    while (token != NULL)
    {
      elementMatrix[i][j].index = i;
      elementMatrix[i][j].cellElement = atof(token);
      //printf("%f \n", elementMatrix[i][j].cellElement);
      token = strtok(NULL, s);
      j++;
    }
    i++;
  }
  fclose(fp);
  //printf("i is %d j is %d\n",i,j);
  if(line) free(line);
  return 0;
}

/* Read in the keys data and populate the key matrix
   this will allow the key matrix to be accessed and
   used throughout the program */

int readingkeys()
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int i = 0;

  fp = fopen("keys.txt", "r");
  if(fp == NULL) exit(EXIT_FAILURE);
   while((read = getline(&line, &len, fp)) != -1)
   {
     const char s[2] = " ";
     char *token;
     token = strtok(line, s);
     while(token != NULL)
     {
       keyArray[i] = atol(token);
       //printf("token %ld\n",keyArray[i]);
       token = strtok(NULL, s);
       i++;
     }
   }
   printf("Finished Reading Keys\n");
   fclose(fp);
   if(line) free(line);
   return 0;
}

void merge(elements arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    elements L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i].cellElement <= R[j].cellElement)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(elements arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}
void printArray(elements A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%f\n", A[i].cellElement);
    printf("\n");
}

void sortColums(){
  for(int i = 0; i < 500;i++){
      mergeSort(flippedMatrix[i],0,4400);
  }
}

void flipMatrix(){
  for(int i = 0;i<4400;i++){
    for(int j =0; j<500;j++){
      flippedMatrix[j][i] = elementMatrix[i][j];
    }
  }
}
void flipBack(){
  for(int i =0 ;i<500;i++){
    for(int j = 0;j<4400;j++){
      elementMatrix[j][i] = flippedMatrix[i][j];
    }
  }
}


void printMatrix(){
  for(int i =0; i < 4400;i++){
    for(int j = 0;j< 500;j++){
      printf("%f",elementMatrix[i][j].cellElement);
    }
    printf("\n");
  }
}
int main()
{

  printf("Starting\n");
  readingkeys();
  readingMatrix();
  flipMatrix();
  sortColums();
  printArray(flippedMatrix[0],4400);
  //blockCreation();

  return 1;
}
