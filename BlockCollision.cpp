#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


float matrix[4400][500]; //The matrix that will store all the initiail data
long keymatrix[4400]; //The matrix that will store all the key data

float dia = 1*10^-6; //The dia (distance) between compatiible cells

int elements[1000];

struct blocks{
  float signature;
  int elements;
};

/* Reads in the text file containing the data, from here
it will populate the array that we have designated. */
int readingMatrix()
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int i = 0;

  fp = fopen("data.txt", "r");
  if(fp == NULL) exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1 )
  {
    const char s[2] = ",";
    char *token;
    token = strtok(line, s);
    int j = 0;

    while (token != NULL)
    {
      matrix[i][j] = atof(token);
      //printf("%f \n", matrix[i][j]);
      //printf("%d,%d\n",i,j);
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

int readingKeys()
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
    while(token != NULL){
      {
        keymatrix[i] = atol(token);
        printf("%d\n",i);
        printf("token is %s\n",token);
        token = strtok(NULL, s);
        printf("%d\n",i);
        i++;
        printf("%d\n",i);
      }
    }
  }
  fclose(fp);
  if(line) free(line);
  return 0;
}

int generateBlocks(){
  int index = 0;
  for(int i =0; i<500;i++){
    for(int j =0; j < 1000000;j++){
      for(int z =0;z<44000;z++){
        float lowerbound = i*dia;
        float upperbound = (i+1)*dia;
        if(lowerbound < matrix[i][j] && upperbound > matrix[i][j]){
          elements[index]= z;
          index++;
        }
      }
    }
    for(int k =0;k< index;k++){
      for(int l=0;l<index;l++){
        if(l == k) continue;
        for(int m = 0; m<index;m++){
          if(m == l || m == k) continue;
          for(int n = 0; n<index; n++){
            if(n == l || n == k || n == m) continue;
            long first = elements[k];
            long second = elements[l];
            long third = elements[m];
            long fourth = elements[n];
            printf("First element %l",first);
            printf(" Second element %l",second);
            printf(" Third element %l",third);
            printf(" Fourth element %l \n",fourth);

          }
        }
      }
    }
  }
}
int main()
{
  printf("Starting \n");
  readingKeys();
  printf("readingMatrix");
  readingMatrix();
  printf("Blocks");
  generateBlocks();
  return 0;
}
