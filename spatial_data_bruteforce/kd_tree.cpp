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
#include<algorithm>
#include "config.h"
#include "kd_tree.h"
#include "util.h"
    
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

struct candidate_node* kd_create_node(kd_node_t node, Rect r, struct kd_node_t* ptr) {
    struct candidate_node* new_node = (struct candidate_node*)malloc(sizeof(struct candidate_node));

    new_node->current_node = node;
    new_node->rec = r;
    new_node->pre = NULL;
    new_node->next = NULL;
    new_node->ptr = ptr;
 
    return new_node;
}

void kdstack_push(struct candidate_node** kdt, struct candidate_node* n_p) {
    if (*kdt == NULL) {
        *kdt = n_p;
        return;
    }
    struct candidate_node* tmp = (*kdt);
    (*kdt) = n_p;
    n_p->next = tmp;
    tmp->pre = n_p;
}

struct candidate_node* kdstack_pop(struct candidate_node** kdt) {
    if ((*kdt) == NULL) {
        fprintf(stdout, "nothing to pop, EMPTY\n");
        return NULL;
    }

    struct candidate_node* point_to_be_popped = *kdt;
    struct candidate_node* res = (struct candidate_node*)malloc(sizeof(struct candidate_node));
    struct candidate_node tmp;

    tmp = **kdt;
    *res = tmp;

    if ((*kdt)->next != NULL) {
        (*kdt) = (*kdt)->next;
        (*kdt)->pre = NULL;
    }
    else {
        (*kdt) = NULL;
    }
    free(point_to_be_popped);

    return res;
}

int check_map_overlap(point center, double radius, Rect map0, Rect map1) {
    int flag = -1;
    int cnt = 0;

    if (rect_to_point_distance(center, map0) <= radius) {
        flag = 0;
        cnt++;
        
    }
    if (rect_to_point_distance(center, map1) <= radius) {
        flag = 1;
        cnt++;
    }

    if (cnt == 2)
        flag = 2;

    return flag;
}

//range query의 질의 조건인 질의 포인트와 질의 반경
point* rangeQuery_kd(struct kd_node_t* root, struct kd_node_t* node, double radius, int mode){
    struct candidate_node* kd_st = NULL;
    Rect map;
    Rect map0, map1;
    point center;
    struct candidate_node* new_node;
    struct candidate_node* popped;
    struct kd_node_t* ptr[2];
    int overlap_flag;

    point* res = (point*)malloc(sizeof(point));
    point* tp;

    map.max_x = map.max_y = DBL_MAX;    //init map
    map.min_x = map.min_y = 0.0;

    res = NULL;
    
    new_node = kd_create_node(*root, map, root);   //coordinate date of tmp is used only
    kdstack_push(&kd_st, new_node); //root pushed
    
    while (popped = kdstack_pop(&kd_st)) {

        map = popped->rec;  //current map
        root = popped->ptr; //node where we should start from

        if (dist(&(popped->current_node), node, 2) < radius) {   //if candidate is in the query range
            tp = create_point(popped->current_node.x[0], popped->current_node.x[1]);
            push_point(&res, tp);
        }
        //then, divide the map
        if (mode == 0) {    //divide x
            map0.max_x = node->x[0];   map0.max_y = map.max_y;    //left
            map0.min_x = map.min_x;    map0.min_y = map.min_y;
            map1.max_x = map.max_x;    map1.max_y = map.max_y;    //right
            map1.min_x = node->x[0];   map1.min_y = map.min_y;
            center.x = node->x[0];   center.y = node->x[1];
            overlap_flag = check_map_overlap(center, radius, map0, map1);
            mode = 1;
        }
        else {  //divide y
            map0.max_x = map.max_x;   map0.max_y = node->x[1];  //bottom
            map0.min_x = map.min_x;   map0.min_y = map.min_y;
            map1.max_x = map.max_x;   map1.max_y = map.max_y;    //top
            map1.min_x = map.min_x;   map1.min_y = node->x[1];
            center.x = node->x[0];   center.y = node->x[1];
            overlap_flag = check_map_overlap(center, radius, map0, map1);
            mode = 0;
        }

        if (overlap_flag == -1) {
            printf("no overlap\n");
        }
        else if (overlap_flag == 0) {
            printf("check only map0\n");
            new_node = kd_create_node((*root), map0, root->left);
            kdstack_push(&kd_st, new_node);
        }
        else if (overlap_flag == 1) {
            printf("check only map1\n");
            new_node = kd_create_node((*root), map1, root->right);
            kdstack_push(&kd_st, new_node);
            ptr[0] = NULL;
            ptr[1] = root->right;
        }
        else if (overlap_flag == 2) {
            printf("check both rectangles\n");
            new_node = kd_create_node((*root), map0, root->left);
            kdstack_push(&kd_st, new_node);
            new_node = kd_create_node((*root), map1, root->right);
            kdstack_push(&kd_st, new_node);
            ptr[0] = root->left;
            ptr[1] = root->right;
        }
        else {
            printf("something wrong with function check_map_overlap\n");
        }

        /*test start : suppose to end with nothing in stack and two area divided by (7,2)*/
        //printf("map0 : (%lf, %lf)~(%lf, %lf)\n\n\nmap1 : (%lf, %lf)~(%lf, %lf)\n", map0.min_x, map0.min_y, map0.max_x, map0.max_y, map1.min_x, map1.min_y, map1.max_x, map1.max_y);
        //return res;
        /*test done*/
    }
    
    return res;
}

//kNN query의 질의조건인 질의 포인트와 최근접이웃 개수
void kNNquery_kd(struct kd_node_t* p, int K)
{
   

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

void print_preorder_kd(struct kd_node_t* t) {
    if (t != NULL) {
        printf("%lf %lf\n", t->x[0], t->x[1]);
        print_preorder_kd(t->left);
        print_preorder_kd(t->right);
    }
}
