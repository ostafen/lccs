// suftree.c

#include <stdio.h>
#include <stdlib.h>
#include <tree.h>

void slowScan(SufTree T, Node p, int index, int k, Node *resp, int *resk, String y, int n)
{
   Node r, q;
   int i, j, ell, indexq;

   //printf("begin slowScan\n");
   while (k < n && (q = getTarget(T, p, PARDIS(y, index, k), y)) != -1)
   {
      indexq = T->index[q];
      j = T->position[q];
      ell = T->length[q];
      i = j;
      do
      {
         ++i;
         ++k;
      } while (i < j + ell && k < n && PARDIS(y, indexq, i) == PARDIS(y, index, k));
      if (i < j + ell)
      {
         removeTarget(T, p, indexq, j);
         r = newNode(T);
         setTarget(T, p, indexq, j, i - j, r);
         setTarget(T, r, indexq, i, ell - i + j, q);
         *resp = r;
         *resk = k;
         //printf("end slowScan\n");
         return;
      }
      p = q;
   }
   *resp = p;
   *resk = k;
   //printf("end slowScan\n");
}

Node fastScan(SufTree T, Node r, int index, int j, int k, String y)
{
   Node p, q;
   int jp, ell, indexq;

   //printf("begin fastScan\n");
   // Computation of delta(r, y[j..k - 1])
   if (j >= k)
   {
      //printf("end fastScan\n");
      return (r);
   }
   else
   {
      q = getTarget(T, r, PARDIS(y, index, j), y);
      indexq = T->index[q];
      jp = T->position[q];
      ell = T->length[q];
      if (j + ell <= k)
         return (fastScan(T, q, index, j + ell, k, y));
      else
      {
         removeTarget(T, r, indexq, jp);
         p = newNode(T);
         setTarget(T, r, index, j, k - j, p);
         setTarget(T, p, indexq, jp + k - j, ell - k + j, q);
         //printf("end fastScan\n");
         return (p);
      }
   }
}

/*******************************************************************************
 * Builds the Cartesian Suffix Tree of y which is in Parent Distance representation
 * Actually y is a concatenation of the PD representation of 2 strings
 * the first one has length m (included the -1 at the end)
 * y has length n (included the -2 at the end)
*/
int buildSufTree(String y, int n, int m)
{
   SufTree T;
   Node fork, q, t;
   int i, j, k, ell;
   int maxdepth;
   Node maxnode;

   //printf("n = %d, m = %d\n", n, m);
   //for (i = 0; i < n; ++i) printf("%d ", y[i]); printf("\n");
   T = newSufTree(y, 3 * n);
   T->link[INITIAL] = INITIAL;
   fork = INITIAL;
   maxdepth = 0;
   k = 0;
   for (i = 0; i < m; ++i)
   {
      //printf("i = %d\n", i);
      if (k < i)
         k = i;
      if (T->link[fork] == -1)
      {
         // change with McCreight algo: go up until finding a suffix link
         // the root has always one
         ell = 0;
         t = fork;
         do
         {
            ell += T->length[t];
            t = T->parent[t];
         } while (T->link[t] == -1);
         if (t == INITIAL)
            --ell;
         T->link[fork] = fastScan(T, T->link[t], i, k - ell, k, y);
      }
      slowScan(T, T->link[fork], i, k, &fork, &k, y, n);
      q = newNode(T);
      setTarget(T, fork, i, k, n - k, q);
      T->finite[q] = i;
   }
   //for (i = m; i < n; ++i) {
   for (; i < n; ++i)
   {
      //printf("i = %d\n", i);
      if (k < i)
         k = i;
      if (T->link[fork] == -1)
      {
         // change with McCreight algo: go up until finding a suffix link
         // the root has always one
         ell = 0;
         t = fork;
         do
         {
            ell += T->length[t];
            t = T->parent[t];
         } while (T->link[t] == -1);
         if (t == INITIAL)
            --ell;
         T->link[fork] = fastScan(T, T->link[t], i, k - ell, k, y);
      }
      slowScan(T, T->link[fork], i, k, &fork, &k, y, n);
      /*******************************************************************************/
      /* computation of the longest common Cartesian substring                       */
      if (T->index[fork] < m && T->depth[fork] > maxdepth)
      {
         maxdepth = T->depth[fork];
         maxnode = fork;
      }
      /*******************************************************************************/
      //q = newNode(T);
      //setTarget(T, fork, i, k, n-k,  q);
      //T->finite[q] = i;
   }
   freeSufTree(T);
   return (maxdepth);
}

void freeSufTree(SufTree st)
{
   free(st->transition);
   free(st->finite);
   free(st->parent);
   free(st->link);
   free(st->index);
   free(st->position);
   free(st->length);
}