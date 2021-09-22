
#ifndef CARTESIAN_TREE_H
#define CARTESIAN_TREE_H

#define NNULL -1
#define IS_NNULL(x) (x < 0)

typedef struct cartesian_tree
{
    int root;
    int *tree;
    int *parent;
    int *l_childs, *r_childs;
    int l_path_len, r_path_len;
    int rightmost, leftmost;
    int next_pos;
    int size;
} cartesian_tree_t;

void cartesian_tree_init(cartesian_tree_t *ct, int n);
void cartesian_tree_free(cartesian_tree_t *ct);

void cartesian_tree_build_left(cartesian_tree_t *ct, int *y, int n);
void cartesian_tree_build_right(cartesian_tree_t *ct, int *y, int n);
void cartesian_tree_reset(cartesian_tree_t *ct);
int cartesian_tree_check_right_path(cartesian_tree_t *ct, int *x, int n);
int cartesian_tree_check_left_path(cartesian_tree_t *ct, int *x, int n);
int cartesian_tree_extend_left(cartesian_tree_t *ct, int x);
int cartesian_tree_extend_right(cartesian_tree_t *ct, int x);
void cartesian_tree_rightmost_suffix_cut(cartesian_tree_t *ct);

int cartesian_tree_extend_parallel(cartesian_tree_t *ct_x, cartesian_tree_t *ct_y, int x, int y);

#endif