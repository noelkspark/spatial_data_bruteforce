#pragma once
//
//  kdtree.c
//  
//
//  Created by Bigdata LAB on 2019/11/05.
//

#include "context.h"

// �Ÿ��Լ� ����.
inline double dist(struct kd_node_t* a, struct kd_node_t* b, int dim);

// swap �Լ� ����.
inline void swap(struct kd_node_t* x, struct kd_node_t* y);

// �߾Ӱ� ã�� �Լ� ����. �߾Ӱ��� kdtree�� node split point�� ã�� �� ���ȴ�.
struct kd_node_t* find_median(struct kd_node_t* start, struct kd_node_t* end, int idx);

// recursion���� kdtree�� build�ϴ� �Լ�.
struct kd_node_t* make_kdtree(struct kd_node_t* t, int len, int i, int dim);

void kdstack_push(struct candidate_node** head, struct candidate_node* n_p);
struct candidate_node* kdstack_pop(struct candidate_node** head);
struct candidate_node* kd_create_node(struct kd_node_t kd_node, Rect r);

int check_map_overlap(point center, double radius, Rect map1, Rect map2);

point* rangeQuery_kd(struct kd_node_t* root, struct kd_node_t* p, double radius, int mode);

void kNNquery_kd(struct kd_node_t* p, int K);

int read_dataset_kd(struct kd_node_t**, const char* filename);

void print_preorder_kd(struct kd_node_t*);

