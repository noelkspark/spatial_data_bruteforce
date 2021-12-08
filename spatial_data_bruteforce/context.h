#pragma once

typedef struct point {
	double x;
	double y;
	struct point* pre;
	struct point* next;
}point;

typedef struct Rect {
	double min_x, min_y;
	double max_x, max_y;
}Rect;


// kdtree의 node 자료구조 정의.

#define MAX_DIM 2
#define COMPARE(a, b) ((a > b) ? a : b)
struct kd_node_t
{
	double x[MAX_DIM];
	struct kd_node_t* left, * right;
};

struct candidate_node{
	struct kd_node_t current_node;
	struct candidate_node* next;
	struct candidate_node* pre;
	struct kd_node_t* ptr;
	int mode;
};

typedef struct kd_heap_node {
	struct kd_node_t node;
	double distance;
}kd_heap_node;
