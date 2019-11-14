#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void
show2darray (double **arr, int rows, int columns)
{
  int i = 0;
  int j = 0;
  for (i = 0; i < rows; ++i)
    {
      for (j = 0; j < columns; ++j)
	{
	  printf ("arr[%d][%d] = %f\n", i, j, arr[i][j]);
	}
    }
  return;
}

double
distance (double *one, double *two, int n)
{
  int i = 0;
  double b = 0;
  for (i = 1; i < n; ++i)
    b = b + ((one[i] - two[i]) * (one[i] - two[i]));
  return sqrt (b);
}

int
classify (double *instance, double **data, int c, int r, int n)
{
  double candidates[27];
  int i = 0;
  int t = 0;
  int j = 0;
  int cluster[27] = { 0 };
  for (i = 0; i < n; ++i)
    candidates[i] = 999999.0;
  int zc = 0;
  int oc = 0;
  for (i = 0; i < r; ++i)
    {
      t = distance (instance, data[i], c);
      if (t < candidates[n - 1])
	{
	  for (j = 0; j < n; ++j)
	    {
	      if (t < candidates[j])
		{
		  candidates[j] = t;
		  cluster[j] = i;
		  break;
		}
	    }
	}
    }
  for (i = 0; i < n; ++i)
    {
      if (data[cluster[i]][0] == 0)
	++zc;
      else
	++oc;
    }
  if (zc > oc)
    return 0;
  else
    return 1;
}

int
main (int argc, char **argv)
{
  int ncount = 0;
  int ccount = 0;
  int ninstances = 0;
  printf ("Enter the number of columns to process.\n");
  scanf ("%d", &ccount);
  printf ("Enter the n value to use in KNN learning.\n");
  scanf ("%d", &ncount);
  if (argc < 3)
    {
      printf ("Must specify files containing training data and instances\n");
      return 0;
    }
  char buf[1024];
  char token[1024];
  int n = 0;
  int m = 0;
  int row_count = 0;
  int in_double_quotes = 0;
  int token_pos = 0;
  int i = 0;
  double **data = (double **) malloc (9000 * sizeof (double *));
  for (i = 0; i < 9000; ++i)
    data[i] = (double *) malloc ((ccount) * sizeof (double));
  double **instances = (double **) malloc (9000 * sizeof (double *));
  for (i = 0; i < 9000; ++i)
    instances[i] = (double *) malloc ((ccount) * sizeof (double));
  FILE *fp = fopen (argv[1], "r");
  if (!fp)
    {
      printf ("Can't open file\n");
      return 0;
    }
  FILE *fpp = fopen (argv[2], "r");
  if (!fpp)
    {
      printf ("Can't open file\n");
      return 0;
    }
  while (fgets (buf, 1024, fp))
    {
      row_count++;
      if (row_count == 1)
	continue;
      i = 0;
      do
	{
	  token[token_pos++] = buf[i];
	  if (!in_double_quotes && (buf[i] == ',' || buf[i] == '\n'))
	    {
	      token[token_pos - 1] = 0;
	      token_pos = 0;
	      if (m < ccount)
		data[n][m] = atof (token);
	      ++m;
	    }
	  if (buf[i] == '"' && buf[i + 1] != '"')
	    {
	      token_pos--;
	      in_double_quotes = !in_double_quotes;
	    }
	  if (buf[i] == '"' && buf[i + 1] == '"')
	    i++;
	}
      while (buf[++i]);
      ++n;
      m = 0;
    }
  i = 0;
  m = 0;
  in_double_quotes = 0;
  token_pos = 0;
  row_count = 0;
  while (fgets (buf, 1024, fpp))
    {
      row_count++;
      if (row_count == 1)
	continue;
      i = 0;
      do
	{
	  token[token_pos++] = buf[i];
	  if (!in_double_quotes && (buf[i] == ',' || buf[i] == '\n'))
	    {
	      token[token_pos - 1] = 0;
	      token_pos = 0;
	      if (m < ccount)
		instances[ninstances][m] = atof (token);
	      ++m;
	    }
	  if (buf[i] == '"' && buf[i + 1] != '"')
	    {
	      token_pos--;
	      in_double_quotes = !in_double_quotes;
	    }
	  if (buf[i] == '"' && buf[i + 1] == '"')
	    i++;
	}
      while (buf[++i]);
      ++ninstances;
      m = 0;
    }
  fclose (fpp);
  int misclass = 0;
  fclose (fp);
//show2darray(data,n,ccount);
//show2darray(instances,ninstances-1000,ccount);
//printf("n = %d\n",n);
//printf("ninstances = %d\n",ninstances);
//double one[9]={0,5,123,245,0,0,0,0,0};
//double two[9]={0,34,2,311,0,0,0,0,0};
//printf("%f\n",distance(one,two,9));
//classify(data[5],data,ccount,n,ncount);
  int *classifications = malloc (sizeof (int) * ninstances);
  for (i = 0; i < ninstances; ++i)
    {
      classifications[i] = classify (instances[i], data, ccount, n, ncount);
      if (classifications[i] != instances[i][0])
	{
	  ++misclass;
	  printf ("Misclassified instance number %d\n", i + 2);
	}
    }
  float accuracy =
    (float) (100 -
	     (float) (((float) (misclass) / (float) (ninstances)) * 100));
  printf ("Out of %d instances, %d were misclassified.\n", ninstances,
	  misclass);
  printf ("The overall accuracy is %.2f%%\n", accuracy);

  return 0;
}
