// tree.h

typedef int Symbol;
typedef int *String;

#define HASHSIZE 4
#define PRIME 113
#define INITIAL 0

#define PARDIS(y, j, i) ((i) - ((y)[(i)]) >= (j) ? (y)[(i)] : 0)

typedef int Node;

struct _list
{                     /* a linked list of transitions */
  int startNode,      /* start node of the transition */
      targetNode;     /* target node of the transition */
  Symbol symbol;      /* first symbol of the label of the transition */
  struct _list *next; /* next cell */
};

typedef struct _list *List;

struct _sufTree
{                   /* a suffix tree */
  String y;         /* the string */
  int maxNode;      /* Maximal number of nodes of the tree */
  int nextNode;     /* Index of the next available node */
  List *transition; /* transitions */
  int *finite;      /* information for leaves */
  int *parent;      /* parent node (-1 for the root) */
  int *link;        /* suffix link */
  int *index;       /* label of the transition */
  int *position;    /* label of the transition */
  int *length;      /* label of the transition */
  int *depth;       /* depth of the node */
};

typedef struct _sufTree *SufTree;

Node newNode(SufTree T);
void setTarget(SufTree T, Node startNode, int index, int position, int length, Node targetNode);
void removeTarget(SufTree T, Node startNode, int index, int position);
Node getTarget(SufTree T, Node startNode, Symbol a, String y);
SufTree newSufTree(String y, int n);
void freeSufTree(SufTree st);
void writeSufTree(SufTree T);
size_t get_suff_size(int n);