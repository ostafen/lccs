#include <cartesian_tree.h>
#include <lccs.h>

void cartesian_tree_fix_rightmost(cartesian_tree_t *ct_min, cartesian_tree_t *ct_max)
{
    ct_min->l_childs[ct_min->rightmost] = ct_max->l_childs[ct_max->rightmost];
    cartesian_tree_rightmost_suffix_cut(ct_min);
    cartesian_tree_rightmost_suffix_cut(ct_max);
}

int parallel_insertion_left(cartesian_tree_t *ct_x, cartesian_tree_t *ct_y, int x, int y, int *len)
{
    int h = cartesian_tree_extend_left(ct_x, x);
    int k = cartesian_tree_extend_left(ct_y, y);

    if (h == k)
    {
        (*len)++;
        return 1;
    }
    return 0;
}

int parallel_insertion_right(cartesian_tree_t *ct_x, cartesian_tree_t *ct_y, int x, int y, int *len)
{
    int h = cartesian_tree_extend_right(ct_x, x);
    int k = cartesian_tree_extend_right(ct_y, y);

    if (h != k)
    {
        cartesian_tree_t *ct_min, *ct_max;

        if (h < k)
        {
            ct_min = ct_x;
            ct_max = ct_y;
        }
        else
        {
            ct_min = ct_y;
            ct_max = ct_x;
        }

        cartesian_tree_fix_rightmost(ct_min, ct_max);
        *len = ct_min->l_childs[ct_x->root] + 1;
        return 0;
    }
    (*len)++;
    return 1;
}

void pass_with_skips(int *x, int n, int *y, int m, cartesian_tree_t *ct_x, cartesian_tree_t *ct_y, int *lccs_size)
{
    int max_lccs_size = n < m ? n : m;

    if (max_lccs_size <= *lccs_size)
        return;

    cartesian_tree_reset(ct_x);
    cartesian_tree_reset(ct_y);

    int curr_len = 0;
    int i = 0;
    int res = 1;
    while (i < max_lccs_size)
    {
        if (curr_len + (max_lccs_size - i + 1) < *lccs_size + 1)
            break;

        if (res)
        {
            res = parallel_insertion_right(ct_x, ct_y, x[i], y[i], &curr_len);
            i += res;
        }
        else
        {
            int start_pos = i - curr_len + 1;
            int j = i + (*lccs_size + 1 - curr_len) - 1;

            if (j >= max_lccs_size)
                break;

            cartesian_tree_reset(ct_x);
            cartesian_tree_reset(ct_y);
            curr_len = 0;

            int temp = j;
            do
            {
                res = parallel_insertion_left(ct_x, ct_y, x[j], y[j], &curr_len);
                j--;
            } while (j >= start_pos && res);

            i = temp + res;
        }

        if (curr_len > *lccs_size)
            *lccs_size = curr_len;
    }
}

int lccs_pass_right(int *x, int n, int *y, int m, cartesian_tree_t *ct_x, cartesian_tree_t *ct_y, int *lccs_size)
{
    int max_lccs_size = n < m ? n : m;

    if (max_lccs_size <= *lccs_size)
        return 0;

    cartesian_tree_reset(ct_x);
    cartesian_tree_reset(ct_y);

    int curr_len = 0;
    for (int i = 0; i < max_lccs_size; i++)
    {
        parallel_insertion_right(ct_x, ct_y, x[i], y[i], &curr_len);

        if (curr_len >= *lccs_size)
            *lccs_size = curr_len;

        if (curr_len + (max_lccs_size - i + 1) < *lccs_size + 1)
            break;
    }
}

int _lccs(int *x, int n, int *y, int m, cartesian_tree_t *ct_x, cartesian_tree_t *ct_y)
{
    int lccs_size = 1;

    for (int i = 0; i < m; i++)
        lccs_pass_right(x, n, y + i, m - i, ct_x, ct_y, &lccs_size);

    for (int j = 1; j < n; j++)
        lccs_pass_right(x + j, n - j, y, m, ct_x, ct_y, &lccs_size);

    return lccs_size;
}

int run_lccs(int *x, int n, int *y, int m, clock_t *time)
{
    int max_lccs_size = n < m ? n : m;

    cartesian_tree_t ct_x, ct_y;
    cartesian_tree_init(&ct_x, max_lccs_size);
    cartesian_tree_init(&ct_y, max_lccs_size);

    *time = clock();
    int lcss_size = _lccs(x, n, y, m, &ct_x, &ct_y);
    *time = clock() - *time;

    // release memory
    cartesian_tree_free(&ct_x);
    cartesian_tree_free(&ct_y);

    return lcss_size;
}

int _lccs_with_skips(int *x, int n, int *y, int m, cartesian_tree_t *ct_x, cartesian_tree_t *ct_y)
{
    int lccs_size = 1;

    for (int i = 0; i < m; i++)
        pass_with_skips(x, n, y + i, m - i, ct_x, ct_y, &lccs_size);

    for (int j = 1; j < n; j++)
        pass_with_skips(x + j, n - j, y, m, ct_x, ct_y, &lccs_size);

    return lccs_size;
}

int run_lcss_with_skips(int *x, int n, int *y, int m, clock_t *time)
{
    int max_lccs_size = n < m ? n : m;

    cartesian_tree_t ct_x, ct_y;
    cartesian_tree_init(&ct_x, max_lccs_size);
    cartesian_tree_init(&ct_y, max_lccs_size);

    *time = clock();
    int lcss_size = _lccs_with_skips(x, n, y, m, &ct_x, &ct_y);
    *time = clock() - *time;

    // release memory
    cartesian_tree_free(&ct_x);
    cartesian_tree_free(&ct_y);

    return lcss_size;
}