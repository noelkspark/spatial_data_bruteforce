
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
#define offset 0.01
#define rad 100

int main() {
	
	if (MODE == BRUTE_FORCE) {

		point* head = NULL;
		
		read_dataset_bf(&head, INPUT_FILE_NAME);


#if QUERY == RANGE
		/*range query example*/
		point qp;
		point* res = NULL;
		qp.x = 250.0;	qp.y = 250.0;

		CHECK_TIME_START(_start, _freq);
		rangeQuery(&res, head, qp, rad * offset);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		int cnt = 0;

		while (res) {
			cnt++;
			res = res->next;
		}
		printf("%d data\n", cnt);
		
#endif
#if QUERY == KNN
		point qp;
		point* res;
		qp.x = 250.0;	qp.y = 250.0;
		int num = 0;
		
		CHECK_TIME_START(_start, _freq);
		res = kNNQuery(&head, qp, 10);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		
		//fprintf(stdout, "\nknnQuery result : \n\n");
		while (res) {
			//printf("%d: (%lf, %lf)\n", num, res->x, res->y);
			res = res->next;
			num++;
		}  
		fprintf(stdout, "got %d result(s)\n", num);
#endif
		printf("%f seconds past\n\n", compute_time);
	}
	else if (MODE == KD_TREE) {
		struct kd_node_t* kd_l = NULL;
		struct kd_node_t* kd_t = NULL;
		struct kd_node_t* input = (struct kd_node_t*)malloc(sizeof(struct kd_node_t));

		int len = read_dataset_kd(&kd_l, INPUT_FILE_NAME);
		if (!len) {
			fprintf(stdout, "file open error | nothing in the file\n\n");
		}
		printf("len %d\n", len);
		kd_t = make_kdtree(kd_l, len, 0, 2);	//x first
		//print_preorder_kd(kd_t);			// TREE MADE
	

#if QUERY == RANGE
		struct point* kd_rangeQres = NULL;
		input->x[0] = 250.0;	input->x[1] = 250.0;
		int cnt = 0;
		CHECK_TIME_START(_start, _freq);
		rangeQuery_kd(&kd_rangeQres, kd_t, input, rad * offset, 0);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		
		while (kd_rangeQres) {
			cnt++;
			kd_rangeQres = kd_rangeQres->next;
		}
		printf("%d data\n", cnt);
		
#endif
#if QUERY == KNN
		kd_heap_node* kd_KNNres = NULL;
		kd_heap_node tmp;
		int heap_cnt = 0;
		int K = 10;
		input->x[0] = 250.0;	input->x[1] = 250.0;
		CHECK_TIME_START(_start, _freq);
		kd_KNNres = kNNquery_kd(kd_t, input, K, 0, &heap_cnt);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		printf("\n%d elements in stack : ", K);
#endif
		printf("%5.3f seconds past\n\n", compute_time);
	}
	else if (MODE == R_TREE) {
		RTREENODE* root = RTreeCreate();
		RTREEMBR search_rect = {
			{0, 0, 0, 3, 3, 0}   /* search will find above rects that this one overlaps */
	};
		point* head = NULL;
		int len;
		int nhits;
		len = RTree_ReadData(&head, INPUT_FILE_NAME);

		construct_rtree(&root, head, len);

#if QUERY == RANGE
		
		
		/*
		nhits = RTreeSearch(root, &search_rect, MySearchCallback, 0);
		printf("%d\n", nhits);
		*/
		point qp;
		qp.x = 250.0;	qp.y = 250.0;
		int cnt = 0;
		nhits = RTree_RangeQuery(root, qp, rad * offset);
		printf("%d hits\n", nhits);

		while (rtree_RQ_res) {
			cnt++;
			rtree_RQ_res = rtree_RQ_res->next;
		}
		printf("%d\n", cnt);
		
#endif
#if QUERY == KNN
		point qp;
		r_heap_node* res;
		qp.x = 250.0;	qp.y = 250.0;
		int cnt = 0;
		CHECK_TIME_START(_start, _freq);
		res = RTree_KNNQuery(root, qp, 10, &cnt);
		CHECK_TIME_END(_start, _end, _freq, compute_time);
		
		printf("%d\n", cnt);
		printf("%lf seconds\n", compute_time);
#endif
	}
	else {
		fprintf(stdout, "WRONG MODE\n");
	}


	return 0;
}