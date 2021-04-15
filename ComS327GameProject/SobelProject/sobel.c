#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* Do not modify write_pgm() or read_pgm() */
int write_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
  FILE *o;

  if (!(o = fopen(file, "w"))) {
    perror(file);

    return -1;
  }

  fprintf(o, "P5\n%u %u\n255\n", x, y);

  /* Assume input data is correctly formatted. *
   * There's no way to handle it, otherwise.   */

  if (fwrite(image, 1, x * y, o) != (x * y)) {
    perror("fwrite");
    fclose(o);

    return -1;
  }

  fclose(o);

  return 0;
}

/* A better implementation of this function would read the image dimensions *
 * from the input and allocate the storage, setting x and y so that the     *
 * user can determine the size of the file at runtime.  In order to         *
 * minimize complication, I've written this version to require the user to  *
 * know the size of the image in advance.                                   */
int read_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
  FILE *f;
  char s[80];
  unsigned i, j;

  if (!(f = fopen(file, "r"))) {
    perror(file);

    return -1;
  }

  if (!fgets(s, 80, f) || strncmp(s, "P5", 2)) {
    fprintf(stderr, "Expected P6\n");

    return -1;
  }

  /* Eat comments */
  do {
    fgets(s, 80, f);
  } while (s[0] == '#');

  if (sscanf(s, "%u %u", &i, &j) != 2 || i != x || j != y) {
    fprintf(stderr, "Expected x and y dimensions %u %u\n", x, y);
    fclose(f);

    return -1;
  }

  /* Eat comments */
  do {
    fgets(s, 80, f);
  } while (s[0] == '#');

  if (strncmp(s, "255", 3)) {
    fprintf(stderr, "Expected 255\n");
    fclose(f);

    return -1;
  }

  if (fread(image, 1, x * y, f) != x * y) {
    perror("fread");
    fclose(f);

    return -1;
  }

  fclose(f);

  return 0;
}

/*int acc(char* file, void *image, int K[][], int M[][], int n)
{
	int r;
	int c;
	int j = 0;
	int i = 0;
	int accumulator;
	for(r < 1024; r++)
	{
		for(c < 1024; c++)
		{
			accumulator = 0;
			for(j < n; j++)
			{
				for(i < n; i++)
				{
					accumulator = accumulator + K[j][i] * M[r + (j - ceil(n/2))][c + (i - ceil(n/2))];
				}
			}
			M'[r][c] = accumulator;
		}
	}
	return accumulator;
}*/

int main(int argc, char *argv[])
{
  int8_t image[1024][1024];
  int8_t out[1024][1024];
  
  /* Example usage of PGM functions */
  /* This assumes that motorcycle.pgm is a pgm image of size 1024x1024 */
  read_pgm("motorcycle.pgm", image, 1024, 1024);
  int kx[3][3];
  int ky[3][3];
  int8_t Ox[1024][1024];
  int8_t Oy[1024][1024];
  {
	  kx[1][1] = -1;
	  kx[1][2] = 0;
	  kx[1][3] = 1;
	  kx[2][1] = -2;
	  kx[2][2] = 0;
	  kx[2][3] = 2;
	  kx[3][1] = -1;
	  kx[3][2] = 0;
	  kx[3][3] = 1;
	  
	  ky[1][1] = -1;
	  ky[1][2] = -2;
	  ky[1][3] = -1;
	  ky[2][1] = 0;
	  ky[2][2] = 0;
	  ky[2][3] = 0;
	  ky[3][1] = 1;
	  ky[3][2] = 2;
	  ky[3][3] = 1;
  }
  
    int r = 0;
	int c = 0;
	int j = 0;
	int i = 0;
	int accumulator;
	for(r < 1024; r++)
	{
		for(c < 1024; c++)
		{
			accumulator = 0;
			for(j < 3; j++)
			{
				for(i < 3; i++)
				{
					accumulator = accumulator + kx[j][i] * image[r + (j - ceil(n/2))][c + (i - ceil(n/2))];
				}
			}
			Ox[r][c] = accumulator;
		}
	}
	
	r = 0;
	c = 0;
	j = 0;
	i = 0;
	for(r < 1024; r++)
	{
		for(c < 1024; c++)
		{
			accumulator = 0;
			for(j < 3; j++)
			{
				for(i < 3; i++)
				{
					accumulator = accumulator + ky[j][i] * image[r + (j - ceil(n/2))][c + (i - ceil(n/2))];
				}
			}
			Oy[r][c] = accumulator;
		}
	}
	r = 0;
	c = 0;
  for(r < 1024; r++)
  {
	  for(c < 1024; c++)
	  {
		  out[r][c] = ((Ox[r][c])^2 + (Oy[r][c])^2)^(1/2);
	  }
  }
  
  /* After processing the image and storing your output in "out", write *
   * to motorcycle.edge.pgm.                                            */
  write_pgm("motorcycle.edge.pgm", out, 1024, 1024);
  
  return 0;
}