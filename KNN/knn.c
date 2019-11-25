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

void swap(double *xp, double *yp)
{
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void swapi(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

double
distance (double *one, double *two, int n)
{
  int i = 0;
  double b = 0;
  for (i = 1; i < n; ++i)
    b = b + ((one[i] - two[i]) * (one[i] - two[i]));
  b = sqrt(b);
  return b;
}

int
classify (double *instance, double **data, int c, int r, int n)
{
  double candidates[87];
  int i = 0;
  double alpha = -1.4;
  int t = 0;
  double euler = 2.7182818284;
  int j = 0;
  int ii = 0;
  int jj = 0;
  int min_idx = 0;
  int cluster[87] = { 0 };
  for (i = 0; i <= n; ++i)
    candidates[i] = 999999.0;
  double zc = 0;
  double oc = 0;
  double tc = 0;
  for (i = 0; i < r; ++i)
    {
      t = distance (instance, data[i], c);
      if (t < candidates[n - 1])
	{

		  candidates[n-1] = t;
		  cluster[n-1] = i;

      for (ii = 0; ii < n-1; ii++)
    {

        min_idx = ii;
        for (jj = ii+1; jj < n; jj++)
          if (candidates[jj] < candidates[min_idx])
            min_idx = jj;


        swap(&candidates[min_idx], &candidates[ii]); swapi(&cluster[min_idx], &cluster[ii]);
    }


	}
    }
  for (i = 0; i < n; ++i)
    {
      if (data[cluster[i]][0] == 1) oc=oc+pow(euler,(alpha*pow(candidates[i],1))); //oc=oc+(10/candidates[i]);
	//++oc;
      else if (data[cluster[i]][0] == 2) tc=tc+pow(euler,(alpha*pow(candidates[i],1))); //tc=tc+(10/candidates[i]);
  //++tc;
      else zc=zc+pow(euler,(alpha*pow(candidates[i],1))); //zc=zc+(10/candidates[i]);
	//++zc;
    }
  if (zc > oc && zc > tc)
    return 0;
  else if (oc > zc && oc > tc)
    return 1;
  else
    return 2;
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
  int corclass = 0;
  fclose (fp);
  int confuze[3][3]={0,0,0,0,0,0,0,0,0};
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
      if(classifications[i] != instances[i][0])
	{
	  ++misclass;
	  printf ("Misclassified instance number %d\n", i + 2);
	}
      else {printf ("Correctly classified instance number %d\n", i + 2); ++corclass;}
    }
  float accuracy =
    (float) (100 -
	     (float) (((float) (misclass) / (float) (ninstances)) * 100));
  printf ("\nOut of %d instances, %d were classified correctly and %d were misclassified.\n\n", ninstances,
	  corclass,misclass);

  for(i=0;i<ninstances;++i){
    if(classifications[i]==0 && instances[i][0]==0) ++confuze[0][0];
    else if(classifications[i]==0 && instances[i][0]==1) ++confuze[0][1];
    else if(classifications[i]==0 && instances[i][0]==2) ++confuze[0][2];
    else if(classifications[i]==1 && instances[i][0]==0) ++confuze[1][0];
    else if(classifications[i]==1 && instances[i][0]==1) ++confuze[1][1];
    else if(classifications[i]==1 && instances[i][0]==2) ++confuze[1][2];
    else if(classifications[i]==2 && instances[i][0]==0) ++confuze[2][0];
    else if(classifications[i]==2 && instances[i][0]==1) ++confuze[2][1];
    else ++confuze[2][2];
  }
  printf("Precision for Class 0 is %.2f%%\n",100*(float)(confuze[0][0])/(float)(confuze[0][0]+confuze[0][1]+confuze[0][2]));
  printf("Recall for Class 0 is %.2f%%\n\n",100*(float)(confuze[0][0])/(float)(confuze[0][0]+confuze[1][0]+confuze[2][0]));
  printf("Precision for Class 1 is %.2f%%\n",100*(float)(confuze[1][1])/(float)(confuze[1][0]+confuze[1][1]+confuze[1][2]));
  printf("Recall for Class 1 is %.2f%%\n\n",100*(float)(confuze[1][1])/(float)(confuze[0][1]+confuze[1][1]+confuze[2][1]));
  printf("Precision for Class 2 is %.2f%%\n",100*(float)(confuze[2][2])/(float)(confuze[2][0]+confuze[2][1]+confuze[2][2]));
  printf("Recall for Class 2 is %.2f%%\n\n",100*(float)(confuze[2][2])/(float)(confuze[0][2]+confuze[1][2]+confuze[2][2]));
  printf ("The overall accuracy is %.2f%%\n", accuracy);
  return 0;
}
