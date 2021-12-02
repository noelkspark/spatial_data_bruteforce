#pragma once
//
//  kdtree.c
//  
//
//  Created by Bigdata LAB on 2019/11/05.
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "config.h"
#include "kd_tree.h"

#define MAX_DIM 2
#define COMPARE(a, b) ((a > b) ? a : b)

// kdtree의 node 자료구조 정의.

// 거리함수 정의.
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

// swap 함수 정의.
inline void swap(struct kd_node_t* x, struct kd_node_t* y)
{
    double tmp[MAX_DIM];
    memcpy(tmp, x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp, sizeof(tmp));
}

// 중앙값 찾는 함수 정의. 중앙값은 kdtree의 node split point를 찾을 때 사용된다.
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

// recursion으로 kdtree를 build하는 함수.
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
    //range query의 질의 조건인 질의 포인트와 질의 반경
}

void kNNquery(struct kd_node_t* p, int K)
{
    //kNN query의 질의조건인 질의 포인트와 최근접이웃 개수

}

int read_dataset_kd(struct kd_node_t** t, const char* file_name) {
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stdout, "FILE OPEN ERROR");
        return 0;
    }
    int line_num = 0;
    char tmp_buf[20], tmp_x[10], tmp_y[10], dummy[5];
    double dx, dy;

    while (!feof(fp)) {
        fgets(tmp_buf, sizeof(tmp_buf), fp);
        line_num++;
    }

    *t = (kd_node_t*)malloc(sizeof(kd_node_t) * line_num);

    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < line_num; i++) {
        fgets(tmp_buf, sizeof(tmp_buf), fp);
        sscanf(tmp_buf, "%[^',']%[^' ']%s", tmp_x, dummy, tmp_y);
        //printf("%s | %s\n", tmp_x, tmp_y);
        dx = atof(tmp_x);
        dy = atof(tmp_y);
        
        (*t)[i].x[0] = dx; (*t)[i].x[1] = dy;
    }

    /*
    point* tmp = *phead;
    while ((*phead) != NULL) {
        printf("head is %lf %lf\n", (*phead)->x, (*phead)->y);
        *phead = (*phead)->next;
    }
    *phead = tmp;
    */

    fclose(fp);
    
    return line_num;
}

