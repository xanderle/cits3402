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

struct appropriatecells {
  float lowerbound;
  float upperbound;
  int cell[1000];
}

struct appropriatecells cells[1000000][500]; //One point for each solumn and each range.

struct newblocks {
  long signature;
  int column;
  int elements[4];
}

struct newblocks newblocks[1000000];
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

/*This method will be used in order to calculate the blocks for
  the various dia */
  // THIS WAS THE OLD ONE, PROBS NOT THAT GREAT
int generateblocks(float[] matrix, long[] keymatrix){
  for(int i=0;i<500;i++){//Does each column one at a time
    int currentcolumn = i;
    for(int j=0;j<4400;j++){//Chooses the first element
      float FirstElement = matrix[j][currentcolumn];
      for(int k=0;k<4400;k++){//Chooses the second element
        if(j==k) continue;
        float SecondElement = matrix[k][currentcolumn];
        if(SecondElement >= FirstElement && SecondElement <= FirstElement + dia){
          SecondElement = matrix[k][currentcolumn];
        }else if(SecondElement <= FirstElement && SecondElement >= FirstElement - dia){
          SecondElement = matrix[k][currentcolumn];
        } else continue;
        for(int l=0;l<4400;l++){//Chooses the third element
          float ThirdElemnt = matrix[l][currentcolumn];
          for(int m=0;m<4400;m++){//Chooses the final element
            float LastElement = matrix[m][currentcolumn];
          }
        }
      }
    }
  }
}

/* This method will allow us to find the given elements within the specified dia,
  from here we will call another method in order to calculate the blocks */
  //I KNOW THIS WILL TAKE AGES TO RUN THE FIRST TIME, I DONT CARE
int FindElementsWithinDia(float[] matrix)
{
  for(int i=0; i < 1000000; i++)//Cover all dia distances
  {
    float currentlow = i*dia;
    float currenthigh = (i+1)*dia;
    for(int j=0; j<500; j++)//Iterate over each column
    {
      int currentcolumn = j;
      int cellindex = 0;
      cells[i][currentcolumn].lowerbound = currentlow;
      cells[i][currentcolumn].upperbound = currenthigh;
      for(int k=0; k<4400; k++)
      {
        float currentcell = matrix[k][currentcolumn];
        if(currentcell <= currenthigh && currentcell >= currentlow)
        {
          cells[i][currentcolumn].cell[cellindex] = k;
          cellindex++;
        }
      }
    }
  }
  return 0;
}

int findblocksfromelements(appropriatecells[][] cells)
{
  for(int i=0; i<1000000; i++)
  {
    for(int j=0; j<500; j++)
    {
      for(int k=0; k<10; k++)
      {
        int firstelement = cells[i][j].cell[k];
        for(int l=0; l<10; l++)
        {
          if(l == k) continue;
          int secondelement = cells[i][j].cell[l];
          for(int m=0; m<10;m++)
          {
            if(m ==l || m == k) continue;
            int thirdelement = cells[i][j];
            for(int n=0;n<10; n++)
            {
              if(n == m || n == l || n == k) continue;
              int fourthelement = cells[i][j].cell[n];
              newblocks[i].column = j;
              newblocks[i].signature = keymatrix[firstelement] + keymatrix[secondelement] + keymatrix[thirdelement] + keymatrix[fourthelement];
              newblocks[i].elements[0] = firstelement;
              newblocks[i].elements[1] = secondelement;
              newblocks[i].elements[2] = thirdelement;
              newblocks[i].elements[3] = fourthelement;
            }
          }
        }
      }
    }
  }
}
