#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float matrix[4400][500];
long keymatrix[4400];
float dia = 1*10^-6

struct blocks {
  long signature[15];
  char columns[100];
};

blocks *blockArray = malloc(1 *sizeof(struct *blocks));

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
        printf("%f \n", matrix[i][j]);
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

int blockgeneration(float matrix, long keymatrix)
{
  for(int j=0;i<500;j++)//colms
  {
    for (int i =0; i < 4400; i++)//rows
    {
      float val = matrix[i][j]
      for(int k = 0; k<4400;k++)
      {
        if(j == k)
        {
          continue;
        }
        if(abs(matrix[i][j] - matrix[i][k]) =< dia)
        {

        }
      }
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  readingkeys();
  readingmatrix();
}
