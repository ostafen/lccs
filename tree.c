// tree.c

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define ABS(i) ((i) >= 0 ? (i) : (-i))

int hashing(SufTree T, Node node, Symbol symbol)
{
   //printf("begin hashing n = %d\n", T->maxNode);
   //printf("  %d %d %d\n", node+1, symbol+1, PRIME);
   return (((node + 1) * ABS((symbol + 1)) * PRIME) % (HASHSIZE * T->maxNode));
}

Node newNode(SufTree T)
{
   //printf("begin newNode\n");
   T->link[T->nextNode] = -1;
   //printf("end newNode\n");
   return (T->nextNode++);
}

void setTarget(SufTree T, Node node, int index, int position, int length, Node targetNode)
{
   List cell;
   int i;
   Symbol c;

   //printf("begin setTarget\n");
   cell = (List)malloc(sizeof(struct _list));
   if (cell == NULL)
      exit(1);

   c = PARDIS(T->y, index, position);
   i = hashing(T, node, c);
   //printf("      setTarget 1 i=%d\n", i);
   cell->startNode = node;
   //printf("      setTarget 2\n");
   cell->symbol = c;
   //printf("      setTarget 3\n");
   cell->targetNode = targetNode;
   //printf("      setTarget 4\n");
   cell->next = T->transition[i];
   //printf("      setTarget 5\n");
   T->transition[i] = cell;
   //printf("      setTarget 6\n");
   T->index[targetNode] = index;
   //printf("      setTarget 7\n");
   T->position[targetNode] = position;
   //printf("      setTarget 8\n");
   T->length[targetNode] = length;
   //printf("      setTarget 9\n");
   T->parent[targetNode] = node;
   //printf("      setTarget 10\n");
   T->depth[targetNode] = T->depth[node] + length;
   //printf("end setTarget\n");
}

void removeTarget(SufTree T, Node node, int index, int position)
{
   List del, cell;
   int i;
   Symbol c;
   Node targetNode;
   // a transition exists from node labelled by PARDIS(y,index,position)

   //printf("begin removeTarget\n");
   c = PARDIS(T->y, index, position);
   i = hashing(T, node, c);
   cell = T->transition[i];
   if (cell->startNode == node &&
       cell->symbol == c)
   {
      targetNode = cell->targetNode;
      T->transition[i] = cell->next;
      free(cell);
   }
   else
   {
      while (cell->next->startNode != node ||
             cell->next->symbol != c)
         cell = cell->next;
      targetNode = cell->next->targetNode;
      del = cell->next;
      cell->next = del->next;
      free(del);
   }
   T->index[targetNode] = -1;
   T->position[targetNode] = -1;
   T->length[targetNode] = -1;
   //printf("end removeTarget\n");
}

Node getTarget(SufTree T, Node node, Symbol a, String y)
{
   List cell;
   int i;

   //printf("begin removeTarget\n");
   i = hashing(T, node, a);
   //printf("i = %d\n", i);
   cell = T->transition[i];
   //printf("cell = %p\n", cell);
   while (cell != NULL)
   {
      //printf("cell = %p\n", cell);
      if (cell->startNode == node &&
          cell->symbol == a)
      {
         //printf("end removeTarget\n");
         return (cell->targetNode);
      }
      else
         cell = cell->next;
   }
   //printf("end removeTarget\n");
   return (-1);
}

SufTree newSufTree(String y, int n)
{
   SufTree T;

   //printf("begin newSufTree\n");
   T = (SufTree)malloc(sizeof(struct _sufTree));
   if (T == NULL)
      exit(1);
   T->y = y;
   T->maxNode = n;
   T->nextNode = 1;
   T->transition = (List *)calloc(HASHSIZE * n, sizeof(List));
   if (T->transition == NULL)
      exit(1);
   T->finite = (int *)malloc(n * sizeof(int));
   if (T->finite == NULL)
      exit(1);
   T->parent = (int *)malloc(n * sizeof(int));
   if (T->parent == NULL)
      exit(1);
   T->parent[0] = -1;
   T->link = (int *)malloc(n * sizeof(int));
   if (T->link == NULL)
      exit(1);
   T->index = (int *)malloc(n * sizeof(int));
   if (T->index == NULL)
      exit(1);
   T->position = (int *)malloc(n * sizeof(int));
   if (T->position == NULL)
      exit(1);
   T->length = (int *)malloc(n * sizeof(int));
   if (T->length == NULL)
      exit(1);
   T->depth = (int *)malloc(n * sizeof(int));
   if (T->depth == NULL)
      exit(1);
   T->depth[0] = 0;

   //printf("end newSufTree\n");
   return (T);
}

size_t get_suff_size(int n)
{
   return sizeof(struct _sufTree) + (HASHSIZE * n * sizeof(List)) + 7 * sizeof(int) * n;
}

void writeSufTree(SufTree T)
{
   int i;
   List cell;

   for (i = 0; i < T->nextNode; ++i)
   {
      printf("state=%d, finite=%d, parent=%d, link=%d\n", i, T->finite[i], T->parent[i], T->link[i]);
      printf("state=%d, index=%d, position=%d, length=%d\n", i, T->index[i], T->position[i], T->length[i]);
      printf("state=%d, depth=%d\n\n", i, T->depth[i]);
   }
   for (i = 0; i < HASHSIZE * T->maxNode; ++i)
   {
      cell = T->transition[i];
      while (cell != NULL)
      {
         printf("startNode=%d, symbol=%d, targetNode=%d\n", cell->startNode, cell->symbol, cell->targetNode);
         cell = cell->next;
      }
   }
}
