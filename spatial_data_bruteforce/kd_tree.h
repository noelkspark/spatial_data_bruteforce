#pragma once
//
//  kdtree.c
//  
//
//  Created by Bigdata LAB on 2019/11/05.
//

#define MAX_DIM 2
#define COMPARE(a, b) ((a > b) ? a : b)

// kdtree�� node �ڷᱸ�� ����.
struct kd_node_t
{
    double x[MAX_DIM];
    struct kd_node_t* left, * right;
};

// �Ÿ��Լ� ����.
inline double dist(struct kd_node_t* a, struct kd_node_t* b, int dim);

// swap �Լ� ����.
inline void swap(struct kd_node_t* x, struct kd_node_t* y);

// �߾Ӱ� ã�� �Լ� ����. �߾Ӱ��� kdtree�� node split point�� ã�� �� ���ȴ�.
struct kd_node_t* find_median(struct kd_node_t* start, struct kd_node_t* end, int idx);

// recursion���� kdtree�� build�ϴ� �Լ�.
struct kd_node_t* make_kdtree(struct kd_node_t* t, int len, int i, int dim);

void rangeQuery(struct kd_node_t* p, double radius);

void kNNquery(struct kd_node_t* p, int K);

int read_dataset_kd(struct kd_node_t**, const char* filename);

void print_preorder_kd(struct kd_node_t*);