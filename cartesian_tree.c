#include <stdlib.h>
#include <string.h>
#include <cartesian_tree.h>
#include <stdio.h>

void cartesian_tree_init(cartesian_tree_t *ct, int n)
{
    ct->tree = (int *)malloc(sizeof(int) * n);
    ct->parent = (int *)malloc(sizeof(int) * n);
    ct->l_childs = (int *)malloc(sizeof(int) * n);
    ct->r_childs = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
    {
        ct->parent[i] = NNULL;
        ct->l_childs[i] = ct->r_childs[i] = 0;
    }
    ct->root = ct->leftmost = ct->rightmost = NNULL;
    ct->l_path_len = ct->r_path_len = 0;
    ct->next_pos = 0;
    ct->size = n;
}

void cartesian_tree_free(cartesian_tree_t* ct) 
{
    free(ct->tree);
    free(ct->parent);
    free(ct->l_childs);
    free(ct->r_childs);
}

int cartesian_tree_extend_left(cartesian_tree_t *ct, int x)
{
    int nd = ct->next_pos;
    ct->next_pos = (ct->next_pos + 1) % ct->size;
    ct->tree[nd] = x;
    ct->l_path_len++;
    ct->l_childs[nd] = 0; // because x is always inserted as the leftmost node

    // tree is empty: add x as root node
    if (IS_NNULL(ct->root))
    {
        ct->parent[nd] = NNULL;
        ct->root = ct->leftmost = ct->rightmost = nd;
        ct->r_childs[nd] = 0;
        ct->r_path_len++;
        return ct->l_path_len - 1;
    }

    int right_childs = 0;
    int prev_node = NNULL;
    int curr_node = ct->leftmost;
    while (!IS_NNULL(curr_node) && x <= ct->tree[curr_node])
    {
        right_childs += ct->r_childs[curr_node] + 1;
        ct->l_path_len--;
        prev_node = curr_node;
        curr_node = ct->parent[curr_node];
    }

    ct->r_childs[nd] = right_childs;

    // case 1: x becomes the new root node
    if (IS_NNULL(curr_node))
    {
        ct->parent[nd] = NNULL;
        ct->parent[ct->root] = nd;
        ct->root = ct->leftmost = nd;
        ct->r_path_len++;
        return ct->l_path_len - 1;
    }

    ct->l_childs[ct->root]++;

    // case 3: x is inserted at the bottom of the leftmost path
    if (!IS_NNULL(prev_node))
    {
        ct->parent[prev_node] = nd;
        ct->l_childs[prev_node] = right_childs - ct->r_childs[prev_node] - 1;
    }

    // case 2: x is inserted in the middle the right path
    ct->parent[nd] = curr_node;
    ct->leftmost = nd;

    return ct->l_path_len - 1;
}

int cartesian_tree_extend_right(cartesian_tree_t *ct, int x)
{
    int nd = ct->next_pos;
    ct->next_pos = (ct->next_pos + 1) % ct->size;
    ct->tree[nd] = x;
    ct->r_path_len++;
    ct->r_childs[nd] = 0; // because x is always inserted as the rightmost node

    // tree is empty: add x as root node
    if (IS_NNULL(ct->root))
    {
        ct->parent[nd] = NNULL;
        ct->root = ct->leftmost = ct->rightmost = nd;
        ct->l_path_len++;
        ct->l_childs[nd] = 0;
        return ct->r_path_len - 1;
    }

    int left_childs = 0;
    int prev_node = NNULL;
    int curr_node = ct->rightmost;
    while (!IS_NNULL(curr_node) && x < ct->tree[curr_node])
    {
        left_childs += ct->l_childs[curr_node] + 1;
        ct->r_path_len--;
        prev_node = curr_node;
        curr_node = ct->parent[curr_node];
    }

    ct->l_childs[nd] = left_childs;

    // case 1: x becomes the new root node
    if (IS_NNULL(curr_node))
    {
        ct->parent[nd] = NNULL;
        ct->parent[ct->root] = nd;
        ct->root = ct->rightmost = nd;
        ct->l_path_len++;
        return ct->r_path_len - 1;
    }
    ct->r_childs[ct->root]++;

    // case 3: x is inserted at the bottom of the righmost path
    if (!IS_NNULL(prev_node))
        ct->parent[prev_node] = nd;

    // case 2: x is inserted in the middle the right path
    ct->parent[nd] = curr_node;
    ct->rightmost = nd;

    return ct->r_path_len - 1;
}

void cartesian_tree_build_left(cartesian_tree_t *ct, int *y, int n)
{
    for (int i = n - 1; i >= 0; i--)
        cartesian_tree_extend_left(ct, y[i]);
}

void cartesian_tree_build_right(cartesian_tree_t *ct, int *y, int n)
{
    for (int i = 0; i < n; i++)
        cartesian_tree_extend_right(ct, y[i]);
}

int _cartesian_tree_check_left_path(cartesian_tree_t *ct, int *l_path, int *r_childs, int *y, int n)
{
    if (n <= 0)
        return 1;

    int k = 0;
    int start_pos = n - 1;
    while (start_pos >= 0)
    {
        int min_pos = start_pos;
        for (int i = min_pos - 1; i >= 0; i--)
        {
            if (y[i] <= y[min_pos])
                min_pos = i;
        }

        if ((l_path[k] != y[min_pos] || r_childs[k] != (start_pos - min_pos)))
        {
            return 0;
        }
        k++;
        start_pos = min_pos - 1;
    }
    return 1;
}

int cartesian_tree_check_left_path(cartesian_tree_t *ct, int *y, int n)
{
    int l_path[ct->l_path_len];
    int r_childs[ct->l_path_len];

    int i = ct->l_path_len - 1;
    int curr_node = ct->leftmost;
    while (!IS_NNULL(curr_node))
    {
        r_childs[i] = ct->r_childs[curr_node];
        l_path[i] = ct->tree[curr_node];
        curr_node = ct->parent[curr_node];
        i--;
    }

    return _cartesian_tree_check_left_path(ct, l_path, r_childs, y, n);
}

int _cartesian_tree_check_right_path(cartesian_tree_t *ct, int *r_path, int *l_childs, int *y, int n)
{
    if (n <= 0)
        return 1;

    int k = 0;
    int start_pos = 0;
    while (start_pos < n)
    {
        int min_pos = start_pos;
        for (int i = min_pos + 1; i < n; i++)
        {
            if (y[i] < y[min_pos])
                min_pos = i;
        }

        if (r_path[k] != y[min_pos] || l_childs[k] != (min_pos - start_pos))
            return 0;

        k++;
        start_pos = min_pos + 1;
    }
    return 1;
}

int cartesian_tree_check_right_path(cartesian_tree_t *ct, int *y, int n)
{
    int r_path[ct->r_path_len];
    int l_childs[ct->r_path_len];

    int i = ct->r_path_len - 1;
    int curr_node = ct->rightmost;
    while (!IS_NNULL(curr_node))
    {
        l_childs[i] = ct->l_childs[curr_node];
        r_path[i] = ct->tree[curr_node];
        curr_node = ct->parent[curr_node];
        i--;
    }

    return _cartesian_tree_check_right_path(ct, r_path, l_childs, y, n);
}

void cartesian_tree_reset(cartesian_tree_t *ct)
{
    ct->root = ct->rightmost = ct->leftmost = NNULL;
    ct->l_path_len = ct->r_path_len = 0;
    ct->next_pos = (ct->next_pos + 1) % ct->size;
}

void cartesian_tree_rightmost_suffix_cut(cartesian_tree_t *ct)
{
    ct->root = ct->rightmost;
    ct->parent[ct->root] = NNULL;
    ct->r_path_len = 1;
}