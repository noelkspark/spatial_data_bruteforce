#pragma once
//
//  kdtree.c
//  
//
//  Created by Bigdata LAB on 2019/11/05.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "config.h"
#include "kd_tree.h"

#define MAX_DIM 2
#define COMPARE(a, b) ((a > b) ? a : b)

// kdtree�� node �ڷᱸ�� ����.

// �Ÿ��Լ� ����.
inline double dist(struct kd_node_t* a, struct kd_node_t* b, int dim)
{
    double t, d = 0;
    while (dim--)
    {
        t = a->x[dim] - b->x[dim];
        d = d + t * t;
    }

    return d;
}

// swap �Լ� ����.
inline void swap(struct kd_node_t* x, struct kd_node_t* y)
{
    double tmp[MAX_DIM];
    memcpy(tmp, x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp, sizeof(tmp));
}

// �߾Ӱ� ã�� �Լ� ����. �߾Ӱ��� kdtree�� node split point�� ã�� �� ���ȴ�.
struct kd_node_t* find_median(struct kd_node_t* start, struct kd_node_t* end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;

    struct kd_node_t* p, * store, * med;
    med = start + (end - start) / 2;
    double pivot;

    while (1)
    {
        pivot = med->x[idx];

        swap(med, end - 1);
        for (store = p = start; p < end; p++)
            if (p->x[idx] < pivot)
            {
                if (p != store)
                    swap(p, store);
                store++;
            }
        swap(store, end - 1);

        if (store->x[idx] == med->x[idx])
            return med;
        if (store > med) end = store;
        else start = store;
    }
}

// recursion���� kdtree�� build�ϴ� �Լ�.
struct kd_node_t* make_kdtree(struct kd_node_t* t, int len, int i, int dim)
{
    struct kd_node_t* n;

    if (!len) return NULL;

    if ((n = find_median(t, t + len, i)))
    {
        i = (i + 1) % dim;
        n->left = make_kdtree(t, n - t, i, dim);
        n->right = make_kdtree(n + 1, t + len - (n + 1), i, dim);
    }

    return n;
}

void rangeQuery(struct kd_node_t* p, double radius)
{
    //range query�� ���� ������ ���� ����Ʈ�� ���� �ݰ�
}

void kNNquery(struct kd_node_t* p, int K)
{
    //kNN query�� ���������� ���� ����Ʈ�� �ֱ����̿� ����

}

