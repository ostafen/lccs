// sol1.c

#include <stdio.h>
#include <stdlib.h>
#include "computepd.h"
#include "sol1.h"

#define PARDIS(y, j, i) ((i) - ((y)[(i)]) >= (j) ? (y)[(i)] : 0)

char ch;

/*
 * Assume that m <= n
 */
int sol1(int *x, int m, int *y, int n)
{
  int i, j, d;
  int maxlength;
  int ell;
  int nMinusm, xi, yj;
  int *xl, *yl;
  int startx, starty, xa, ya;

  xl = computePD(x, m);
  yl = computePD(y, n);

  /* the long diagonals */
  maxlength = 1;
  nMinusm = n - m;
  for (d = 0; d <= nMinusm; ++d)
  {
    //printf("d = %d\n", d);
    startx = 0;
    starty = d;
    i = 1;
    j = d + 1;
    while (i < m && j < n)
    {
      //printf("   startx = %d, starty = %d\n", startx, starty);
      //printf("   i = %d, j = %d\n", i, j);
      //scanf("%c", &ch);
      if (m - startx <= maxlength)
        break;
      xa = PARDIS(xl, startx, i);
      ya = PARDIS(yl, starty, j);
      //while (xa != ya) {
      //if (xa > ya) {
      //startx = i-xa+1;
      //starty = j-xa+1;
      //xa = PARDIS(xl, startx, i);
      //}
      //else {
      //startx = i-ya+1;
      //starty = j-ya+1;
      //ya = PARDIS(yl, starty, j);
      //}
      //}
      if (xa != ya)
      {
        if (xa == 0)
        {
          startx = i - ya + 1;
          starty = j - ya + 1;
        }
        else
        {
          if (ya == 0)
          {
            startx = i - xa + 1;
            starty = j - xa + 1;
          }
          else
          {
            ell = ((xa < ya) ? xa : ya);
            startx = i - ell + 1;
            starty = j - ell + 1;
          }
        }
      }
      if (i - startx + 1 > maxlength)
        maxlength = i - startx + 1;
      //printf("==> maxlength = %d\n", maxlength);
      ++i;
      ++j;
    }
  }

  /* short diagonals with suffix of y */
  for (d = nMinusm + 1; d < n - 1; ++d)
  {
    //printf("d = %d\n", d);
    startx = 0;
    starty = d;
    i = 1;
    j = d + 1;
    while (i < m && j < n)
    {
      //printf("   startx = %d, starty = %d\n", startx, starty);
      //printf("   i = %d, j = %d\n", i, j);
      //scanf("%c", &ch);
      if (n - starty <= maxlength)
        break;
      xa = PARDIS(xl, startx, i);
      ya = PARDIS(yl, starty, j);
      //while (xa != ya) {
      //if (xa > ya) {
      //startx = i-xa+1;
      //starty = j-xa+1;
      //xa = PARDIS(xl, startx, i);
      //}
      //else {
      //startx = i-ya+1;
      //starty = j-ya+1;
      //ya = PARDIS(yl, starty, j);
      //}
      //}
      if (xa != ya)
      {
        if (xa == 0)
        {
          startx = i - ya + 1;
          starty = j - ya + 1;
        }
        else
        {
          if (ya == 0)
          {
            startx = i - xa + 1;
            starty = j - xa + 1;
          }
          else
          {
            ell = ((xa < ya) ? xa : ya);
            startx = i - ell + 1;
            starty = j - ell + 1;
          }
        }
      }
      if (i - startx + 1 > maxlength)
        maxlength = i - startx + 1;
      //printf("==> maxlength = %d\n", maxlength);
      ++i;
      ++j;
    }
  }

  /* short diagonals with prefix of y */
  for (d = -m + 2; d < 0; ++d)
  {
    //printf("d = %d\n", d);
    startx = -d;
    starty = 0;
    i = -d + 1;
    j = 1;
    while (i < m && j < n)
    {
      //printf("   startx = %d, starty = %d\n", startx, starty);
      //printf("   i = %d, j = %d\n", i, j);
      //scanf("%c", &ch);
      if (m - startx <= maxlength)
        break;
      xa = PARDIS(xl, startx, i);
      ya = PARDIS(yl, starty, j);
      //while (xa != ya) {
      //if (xa > ya) {
      //startx = i-xa+1;
      //starty = j-xa+1;
      //xa = PARDIS(xl, startx, i);
      //}
      //else {
      //startx = i-ya+1;
      //starty = j-ya+1;
      //ya = PARDIS(yl, starty, j);
      //}
      //}
      if (xa != ya)
      {
        if (xa == 0)
        {
          startx = i - ya + 1;
          starty = j - ya + 1;
        }
        else
        {
          if (ya == 0)
          {
            startx = i - xa + 1;
            starty = j - xa + 1;
          }
          else
          {
            ell = ((xa < ya) ? xa : ya);
            startx = i - ell + 1;
            starty = j - ell + 1;
          }
        }
      }
      else if (i - startx + 1 > maxlength)
        maxlength = i - startx + 1;
      //printf("==> maxlength = %d\n", maxlength);
      ++i;
      ++j;
    }
  }

  free(xl);
  free(yl);
  return (maxlength);
}
