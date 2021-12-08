#pragma once
#define _NO_CRT_STDIO_INLINE
#include "config.h"
#include "context.h"
#include "util.h"
#include "r_tree.h"

//rtree의 node 자료구조 정의

struct r_candidate_node {
	struct r_candidate_node* next;
	struct r_candidate_node* pre;
	RTREENODE* ptr;
	double distance;
};

typedef struct r_heap_node {
	RTREENODE node;
	double distance;
}r_heap_node;

int MySearchCallback(int id, void* arg);
double mid(double a, double b, double c);

void RTree_RangeQuery(RTREENODE* head, point qp, double radius);
r_heap_node* RTree_KNNQuery(RTREENODE* root, point qp, int K, int* heap_cnt);

double rdist(RTREENODE* a, point b);

void rstack_push(struct r_candidate_node** head, struct r_candidate_node* n_p);
void rstack_push_inorder(struct r_candidate_node** head, struct r_candidate_node* n_p);
struct r_candidate_node* rstack_pop(struct r_candidate_node** head);
struct r_candidate_node* r_create_node(RTREENODE* ptr);

void r_swap(struct r_heap_node* a, struct r_heap_node* b);
void rmaxheap_push(struct r_heap_node* heap, struct r_heap_node val, int* cnt);
struct r_heap_node rmaxheap_pop(struct r_heap_node* heap, int* cnt);

void construct_rtree(RTREENODE** head, point* phead, int n);
int RTree_ReadData(point** phead, const char* file_name);


