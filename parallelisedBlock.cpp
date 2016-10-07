#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <omp.h>
#include <sys/time.h>
float dia = 0.000001; //The dia (distance) between compatiible cells
struct elements{
  int index;
  float cellElement;
};

struct collisions{
  long signature;
  int blocks[100];
  int column;
};

struct Blocks{
  long signature;
  int Elements[4];
  int column;
};
collisions colli[120000];
Blocks block[123235];
elements elementMatrix[4400][499];
elements flippedMatrix[499][4400];
long keyArray[4400] = {0};//The matrix that will store all the key data
int blockIndex = 0;

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

  while ((read = getline(&line, &len, fp)) != -1)
  {
    const char s[2] = ",";
    char *token;
    token = strtok(line, s);
    j = 0;

    while (token != NULL && j != 499)
    {
      if(j == 499) continue;
      elementMatrix[i][j].index = i;
      elementMatrix[i][j].cellElement = atof(token);
      token = strtok(NULL, s);
      j++;
    }
    i++;
  }
  fclose(fp);
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
       token = strtok(NULL, s);
       i++;
     }
   }
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
  int i = 0;
  #pragma omp parallel
  {
    #pragma omp for
    for(i = 0; i < 499;i++){
      mergeSort(flippedMatrix[i],0,4400);
    }
  }
}

void flipMatrix(){
  int i =0;
  #pragma omp parallel
  {
    #pragma omp for
  for(i = 0;i<4400;i++){
    for(int j =0; j<499;j++){
      flippedMatrix[j][i] = elementMatrix[i][j];
    }
  }
  }
}

void flipBack(){
  int i =0;
  #pragma omp parallel
  {
    #pragma omp for
  for(int i =0 ;i<499;i++){
    for(int j = 0;j<4400;j++){
      elementMatrix[j][i] = flippedMatrix[i][j];
    }
  }
}
}


void printMatrix(){
  for(int i =0; i < 4400;i++){
    for(int j = 0;j< 499;j++){
      printf("%f",elementMatrix[i][j].cellElement);
    }
    printf("\n");
  }
}

void printthematrix(){
  for(int i = 0; i < 4400; i++){
    printf("%f\n", elementMatrix[i][0].cellElement);
  }
}

void blockCreation(){
int i =0;
#pragma omp parallel
{
#pragma omp single
{
  for(i =0; i<499; i++){
    for(int j =0;j<4400;j++){
      int k = 0;
      elements currElement;
      currElement = elementMatrix[j][i];
      int index = j + 1;
      elements potentialBlocks[4400];
      potentialBlocks[k] = currElement;
      k++;

      while((std::abs(currElement.cellElement - elementMatrix[index][i].cellElement) <= dia)){
        potentialBlocks[k] = elementMatrix[index][i];
        index++;
        k++;
    }
      //#pragma omp task firstprivate(k)
     //{
      if(k > 3){
        for(int a =0;a<=k;a++){
          for(int b = 0;b<=k;b++){
            if(a == b || b < a) continue;
            for(int c =0;c<=k;c++){
              if(c == b || c == a || c < b) continue;
              for(int d= 0;d<=k;d++){
                if(d == a || d == b || d == c || d < c) continue;
                Blocks tmpBlock;
                tmpBlock.column = i;
                tmpBlock.Elements[0] = potentialBlocks[a].index;
                tmpBlock.Elements[1] = potentialBlocks[b].index;
                tmpBlock.Elements[2] = potentialBlocks[c].index;
                tmpBlock.Elements[3] = potentialBlocks[d].index;
                tmpBlock.signature = keyArray[potentialBlocks[a].index]+keyArray[potentialBlocks[b].index]+keyArray[potentialBlocks[c].index]+keyArray[potentialBlocks[d].index];
                block[blockIndex] = tmpBlock;
                blockIndex++;
              }
              }
          //  }
            }
          }
          }
        }
    }
}
}
}



void mergeBlock(Blocks arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    Blocks L[n1], R[n2];

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
        if (L[i].signature <= R[j].signature)
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

void mergeSortBlock(Blocks arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSortBlock(arr, l, m);
        mergeSortBlock(arr, m+1, r);
        mergeBlock(arr, l, m, r);
    }
}

void sortBlocks()
{
    mergeSortBlock(block, 0, 123235);
}

//Calculates the collisions that have been produced.
void collisionDetection(){
 int indexOfCollision =0;
 int collisionIndex = 0;
 for(int i = 0; i< blockIndex ;i++){
   if(block[i].signature == block[i+1].signature){
    //  if(indexOfCollision ==0 ){
    //    printf("Signature: %ld {",block[i].signature);
    //  }
     colli[collisionIndex].signature = block[i].signature;
     colli[collisionIndex].blocks[indexOfCollision] = i;
     colli[collisionIndex].column = block[i].column;
     indexOfCollision++;
     //printf("%d ", block[i].column);
   }
 else{

  // if(indexOfCollision!=0){
  //   printf("}\n");
  // }
  if(colli[collisionIndex].signature != 0){
    printf("Signature: %ld - { ", colli[collisionIndex].signature);
    for(int i = 0; i < indexOfCollision ;  i++){
      printf(" M%d ", colli[collisionIndex].blocks[i]);
    }
    printf("}\n");
  }
   collisionIndex++;
   indexOfCollision = 0;
   }
 }
}


int main()
{
  struct timeval start, end;
    gettimeofday(&start, NULL);
  printf("Starting...\n");

  #pragma omp parallel
  {
    #pragma omp single
    {
      readingkeys();
    }

    #pragma omp single
    {
      readingMatrix();
    }
  }
  //printf("Data Read!\n");
  flipMatrix();
  //printf("Sorting Data...\n");
  sortColums();
  //printf("Data Sorted!\nGenerating Blocks...\n");
  flipBack();
  //printf("Creation..\n");
  blockCreation();
  //printf("Blocks Generated!\nNow Sorting...\n");
  sortBlocks();
  collisionDetection();
  //printf("Collisions Detected!\n");
  gettimeofday(&end,NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
    end.tv_usec - start.tv_usec) / 1.e6;
    printf("time=%12.10f\n",delta);
  return 1;
}
