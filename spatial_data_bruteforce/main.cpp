
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <Windows.h>

#include "config.h"
#include "bruteforce.h"
#include "kd_tree.h"
#include "r_tree.h"
#include "r_tree_query.h"

extern point* rtree_RQ_res;
__int64 _start, _freq, _end;
float compute_time;

#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))

int main() {
	
	if (MODE == BRUTE_FORCE) {

		point* head = NULL;
		
		read_dataset_bf(&head, INPUT_FILE_NAME);

#if QUERY == DISTANCE
		/*distance query example*/
		point p;
		Rect r;

		p.x = 2;	p.y = 2;
		r.max_x = 1;	r.max_y = 1;	r.min_x = 0;	r.min_y = 0;

		fprintf(stdout, "\ncalculated distance between point (%lf, %lf) | Rectangle (%lf, %lf) (%lf, %lf): %lf\n", p.x, p.y, r.min_x, r.min_y, r.max_x, r.max_y, rect_to_point_distance(p, r));
#endif
#if QUERY == RANGE
		/*range query example*/
		point qp;
		point* res;
		qp.x = 2.3;	qp.y = 2.3;
		int num = 0;

		CHECK_TIME_START(_start, _freq);
		res = rangeQuery(head, qp, 3);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		fprintf(stdout, "\nrangeQuery result : \n\n");
		while (res) {
			printf("(%lf, %lf)\n", res->x, res->y);
			res = res->next;
			num++;
		}
		fprintf(stdout, "got %d result(s)\n", num);
#endif
#if QUERY == KNN
		point qp;
		point* res;
		qp.x = 2.3;	qp.y = 2.3;
		int num = 0;

		res = kNNQuery(&head, qp, 3);
		fprintf(stdout, "\nknnQuery result : \n\n");
		while (res) {
			printf("%d: (%lf, %lf)\n", num, res->x, res->y);
			res = res->next;
			num++;
		}  
		fprintf(stdout, "got %d result(s)\n", num);
#endif
	}
	else if (MODE == KD_TREE) {
		struct kd_node_t* kd_l = NULL;
		struct kd_node_t* kd_t = NULL;
		struct kd_node_t* input = (struct kd_node_t*)malloc(sizeof(struct kd_node_t));

		int len = read_dataset_kd(&kd_l, INPUT_FILE_NAME);
		if (!len) {
			fprintf(stdout, "file open error | nothing in the file\n\n");
		}

		kd_t = make_kdtree(kd_l, len, 0, 2);	//x first
		//print_preorder_kd(kd_t);			// TREE MADE
	
#if QUERY == DISTANCE

#endif
#if QUERY == RANGE
		struct point* kd_rangeQres = NULL;
		input->x[0] = 7;	input->x[1] = 6;
		kd_rangeQres = rangeQuery_kd(kd_t, input, 2, 0);
		printf("stack : ");
		while (kd_rangeQres) {
			printf("(%lf, %lf)   ", kd_rangeQres->x, kd_rangeQres->y);
			kd_rangeQres = kd_rangeQres->next;
		}
#endif
#if QUERY == KNN
		kd_heap_node* kd_KNNres = NULL;
		kd_heap_node tmp;
		int heap_cnt = 0;
		int K = 2;
		input->x[0] = 7;	input->x[1] = 6;
		kd_KNNres = kNNquery_kd(kd_t, input, K, 0, &heap_cnt);
		printf("\n%d elements in stack : ", K);
		for (int i = 0; i < K; i++) {
			tmp = maxheap_pop(kd_KNNres, &heap_cnt);
			printf("(%lf, %lf)   ", tmp.node.x[0], tmp.node.x[1]);
		}
		
#endif
	}
	else if (MODE == R_TREE) {
		
#if QUERY == DISTANCE

#endif
#if QUERY == RANGE
		RTREENODE* root = RTreeCreate();
		RTREEMBR search_rect = {
			{0, 0, 0, 3, 3, 0}   /* search will find above rects that this one overlaps */
		};
		point* head = NULL;
		int len;
		int nhits;
		len =  RTree_ReadData(&head, INPUT_FILE_NAME);

		construct_rtree(&root, head, len);
		
		/*
		nhits = RTreeSearch(root, &search_rect, MySearchCallback, 0);
		printf("%d\n", nhits);
		*/
		point qp;
		qp.x = 7;	qp.y = 6;
	
		nhits = RTree_RangeQuery(root, qp, 2);
		printf("%d hits\n", nhits);

		while (rtree_RQ_res) {
			printf("%lf %lf\n", rtree_RQ_res->x, rtree_RQ_res->y);
			rtree_RQ_res = rtree_RQ_res->next;
		}
		
#endif
#if QUERY == KNN

#endif
	}
	else {
		fprintf(stdout, "WRONG MODE\n");
	}


	return 0;
}