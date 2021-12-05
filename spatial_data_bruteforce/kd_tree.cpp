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

    return sqrt(d);
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

struct candidate_node* kd_create_node(kd_node_t node, Rect r, struct kd_node_t* ptr, int mode) {
    struct candidate_node* new_node = (struct candidate_node*)malloc(sizeof(struct candidate_node));

    new_node->current_node = node;
    new_node->rec = r;
    new_node->pre = NULL;
    new_node->next = NULL;
    new_node->ptr = ptr;
    new_node->mode = mode;
    return new_node;
}

void kdstack_push(struct candidate_node** kdst, struct candidate_node* n_p) {
    printf("pushing %p\n", n_p);
    if (*kdst == NULL) {
        *kdst = n_p;
        return;
    }

    n_p->next = *kdst;
    (*kdst)->pre = n_p;
    *kdst = n_p;
}

struct candidate_node* kdstack_pop(struct candidate_node** kdt) {
   
    if (!kdt) {
        printf("somthing wrong\n\n\n\n");
        return NULL;
    }
    if ((*kdt) == NULL) {
        fprintf(stdout, "nothing to pop, EMPTY\n");
        return NULL;
    }

    struct candidate_node* point_to_be_popped = *kdt;
    struct candidate_node* res = (struct candidate_node*)malloc(sizeof(struct candidate_node));

    *res = **kdt;

    if ((*kdt)->next != NULL) {
        (*kdt) = (*kdt)->next;
        (*kdt)->pre = NULL;
    }
    else {
        (*kdt) = NULL;
    }
   
   // free(point_to_be_popped);

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
    int overlap_flag;

    point* res = (point*)malloc(sizeof(point));
    point* tp;

    map.max_x = map.max_y = DBL_MAX;    //init map
    map.min_x = map.min_y = 0.0;

    res = NULL;
    
    new_node = kd_create_node(*root, map, root, mode);   //coordinate date of tmp is used only
    kdstack_push(&kd_st, new_node); //root pushed
    
    while (popped = kdstack_pop(&kd_st)) {

        map = popped->rec;  //current map
        root = popped->ptr; //node where we should start from
        mode = popped->mode;//current mode
        /*for test*/
        double test = dist(root, node, 2);
        printf("distance between (%lf, %lf) and (%lf, %lf) = %lf\n\n", root->x[0], root->x[1], node->x[0], node->x[1], test);
        /*test done*/

        if (dist(root, node, 2) <= radius) {   //if candidate is in the query range
            tp = create_point(root->x[0], root->x[1]);
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
            if (root->left != NULL) {
                new_node = kd_create_node((*root), map0, root->left, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else if (overlap_flag == 1) {
            printf("check only map1\n");
            if (root->right != NULL) {
                new_node = kd_create_node((*root), map1, root->right, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else if (overlap_flag == 2) {
            printf("check both rectangles\n");
            if (root->left != NULL) {
                new_node = kd_create_node((*root), map0, root->left, mode);
                kdstack_push(&kd_st, new_node);
            }
            if (root->right != NULL) {
                new_node = kd_create_node((*root), map1, root->right, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else {
            printf("something wrong with function check_map_overlap\n");
        }

        /*test start : suppose to end with nothing in stack and two area divided by (7,2)*/
        //printf("map0 : (%lf, %lf)~(%lf, %lf)\n\n\nmap1 : (%lf, %lf)~(%lf, %lf)\n", map0.min_x, map0.min_y, map0.max_x, map0.max_y, map1.min_x, map1.min_y, map1.max_x, map1.max_y);
        //return res;
        /*test done*/
    
        //free(popped);
    }
    
    return res;
}

//kNN query의 질의조건인 질의 포인트와 최근접이웃 개수
kd_heap_node* kNNquery_kd(struct kd_node_t* root, struct kd_node_t* node, int K, int mode, int* heap_cnt) {
    struct candidate_node* kd_st = NULL;
    Rect map;
    Rect map0, map1;
    point center;
    struct candidate_node* new_node;
    struct candidate_node* popped;
    int overlap_flag;
    double max_mindist = DBL_MAX;

    kd_heap_node* mheap = (kd_heap_node*)malloc(sizeof(kd_heap_node) * (K + 1)); 
    kd_heap_node htmp;
    kd_node_t ttmp;

    map.max_x = map.max_y = DBL_MAX;    //init map
    map.min_x = map.min_y = 0;

    new_node = kd_create_node(*root, map, root, mode);   //coordinate date of tmp is used only
    kdstack_push(&kd_st, new_node); //root pushed
    while (popped = kdstack_pop(&kd_st)) {
        map = popped->rec;  //current map
        root = popped->ptr; //node where we should start from
        mode = popped->mode;//current mode
        if (!root)
           continue;
        /*for test*/
        double test = dist(root, node, 2);
        printf("distance between (%lf, %lf) and (%lf, %lf) = %lf\n\n",root->x[0], root->x[1], node->x[0], node->x[1], test);
        /*test done*/
        //printf("max_mindist : %lf\n", max_mindist);
        if (dist(root, node, 2) <= max_mindist) {   //if NN candidate
            htmp.distance = dist(root, node, 2);
            ttmp.x[0] = root->x[0];
            ttmp.x[1] = root->x[1];
            htmp.node = ttmp;
            fprintf(stdout, "pushing (%lf %lf) in main\n", ttmp.x[0], ttmp.x[1]);
            if (*heap_cnt >= K) { //if KNN heap is full
                kd_heap_node poppedheapnode;
                poppedheapnode = maxheap_pop(mheap, heap_cnt);
                fprintf(stdout, "popped (%lf %lf) in main\n", poppedheapnode.node.x[0], poppedheapnode.node.x[1]);
            }
            //fprintf(stdout, "pushing in main (%lf)\n", htmp.distance);
            maxheap_push(mheap, htmp, heap_cnt);   //push new node
            max_mindist = mheap[1].distance;
        }
        //then, divide the map
        if (mode == 0) {    //divide x
            map0.max_x = node->x[0];   map0.max_y = map.max_y;    //left
            map0.min_x = map.min_x;    map0.min_y = map.min_y;
            map1.max_x = map.max_x;    map1.max_y = map.max_y;    //right
            map1.min_x = node->x[0];   map1.min_y = map.min_y;
            center.x = node->x[0];   center.y = node->x[1];
            overlap_flag = check_map_overlap(center, max_mindist, map0, map1);
            mode = 1;
        }
        else {  //divide y
            map0.max_x = map.max_x;   map0.max_y = node->x[1];  //bottom
            map0.min_x = map.min_x;   map0.min_y = map.min_y;
            map1.max_x = map.max_x;   map1.max_y = map.max_y;    //top
            map1.min_x = map.min_x;   map1.min_y = node->x[1];
            center.x = node->x[0];   center.y = node->x[1];
            overlap_flag = check_map_overlap(center, max_mindist, map0, map1);
            mode = 0;
        }
        if (overlap_flag == -1) {
            printf("no overlap\n");
        }
        else if (overlap_flag == 0) {
            printf("check only map0\n");
            if (root->left != NULL) {
                new_node = kd_create_node((*root), map0, root->left, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else if (overlap_flag == 1) {
            printf("check only map1\n");
            if (root->right != NULL) {
                new_node = kd_create_node((*root), map1, root->right, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else if (overlap_flag == 2) {
            printf("check both rectangles\n");
            if (root->left != NULL) {
                new_node = kd_create_node((*root), map0, root->left, mode);
                kdstack_push(&kd_st, new_node);
            }
            if (root->right != NULL) {
                new_node = kd_create_node((*root), map1, root->right, mode);
                kdstack_push(&kd_st, new_node);
            }
        }
        else {
            printf("something wrong with function check_map_overlap\n");
        }

       
        //free(popped);
        /*test start : suppose to end with nothing in stack and two area divided by (7,2)*/
        //printf("map0 : (%lf, %lf)~(%lf, %lf)\n\n\nmap1 : (%lf, %lf)~(%lf, %lf)\n", map0.min_x, map0.min_y, map0.max_x, map0.max_y, map1.min_x, map1.min_y, map1.max_x, map1.max_y);
        //return res;
        /*test done*/
    }

    return mheap;
}

void h_swap(kd_heap_node* a, kd_heap_node* b) {
    kd_heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxheap_push(kd_heap_node* heap, kd_heap_node node, int* cnt) {
    (*cnt)++;
    heap[(*cnt)] = node;

    int child = *cnt;
    int parent = child / 2;

    while (child > 1 && heap[child].distance > heap[parent].distance) {
        h_swap(&heap[child], &heap[parent]);
        child = parent;
        parent = child / 2;
    }
    printf("pushed heap : (%lf)\n",node.distance);
}

kd_heap_node maxheap_pop(kd_heap_node* heap, int* cnt) {
    kd_heap_node ret = heap[1];
    printf("heap popping (%lf)\n", ret.distance);
    h_swap(&heap[1], &heap[*cnt]);
    (*cnt)--;

    int parent = 1;
    int child = parent * 2;

    if (child + 1 <= *cnt) {
        child = (heap[child].distance > heap[child + 1].distance) ? child : child + 1;
    }

    while (child <= *cnt && heap[child].distance> heap[parent].distance) {
        h_swap(&heap[child], &heap[parent]);
        parent = child;
        child = parent * 2;

        if (child + 1 <= *cnt) {
            child = (heap[child].distance > heap[child + 1].distance) ? child : child + 1;
        }
    }

    return ret;
}

void minheap_push(kd_heap_node* heap, kd_heap_node node, int* cnt) {
    (*cnt)++;
    heap[*cnt] = node;

    int child = *cnt;
    int parent = child / 2;

    while (child > 1 && heap[child].distance < heap[parent].distance) {
        h_swap(&heap[child], &heap[parent]);
        child = parent;
        parent = child / 2;
    }

}

kd_heap_node minheap_pop(kd_heap_node* heap, int* cnt) {
    kd_heap_node ret = heap[1];

    h_swap(&heap[1], &heap[*cnt]);
    (*cnt)--;

    int parent = 1;
    int child = parent * 2;

    if (child + 1 <= *cnt) {
        child = (heap[child].distance < heap[child + 1].distance) ? child : child + 1;
    }

    while (child <= *cnt && heap[child].distance < heap[parent].distance) {
        h_swap(&heap[child], &heap[parent]);
        parent = child;
        child = parent * 2;

        if (child + 1 <= *cnt) {
            child = (heap[child].distance < heap[child + 1].distance) ? child : child + 1;
        }
    }

    return ret;
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
