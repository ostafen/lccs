// computepd.c

#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Computes the Parent Distance Array 
 */
int *computePD(int *x, int m)
{
  int *PD, *stackV, *stackI;
  int i, top;

  PD = (int *)malloc(m * sizeof(int));
  if (PD == NULL)
    exit(1);
  stackV = (int *)malloc(m * sizeof(int));
  if (stackV == NULL)
    exit(1);
  stackI = (int *)malloc(m * sizeof(int));
  if (stackI == NULL)
    exit(1);

  top = -1;
  for (i = 0; i < m; ++i)
  {
    while (top >= 0)
    {
      if (stackV[top] <= x[i])
        break;
      --top;
    }
    PD[i] = ((top < 0) ? (0) : (i - stackI[top]));
    stackV[++top] = x[i];
    stackI[top] = i;
  }
  free(stackV);
  free(stackI);
  return (PD);
}
