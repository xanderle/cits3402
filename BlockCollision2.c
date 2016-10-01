#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float matrix[4400][500]; //The matrix that will store all the initiail data
long keymatrix[4400]; //The matrix that will store all the key data

float dia = 1*10^-6 //The dia (distance) between compatiible cells

struct block {
  long signature[15];
  char columns[100];
};

struct block blocks[10000]; //The structure in which all block data will be stored

/* Reads in the text file containing the data, from here
   it will populate the array that we have designated. */
int readingmatrix()
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
       keymatrix[i] = atol(token);
       token = strtok(NULL, s);
       i++;
     }
   }
   fclose(fp);
   if(line) free(line);
   return 0;
}
