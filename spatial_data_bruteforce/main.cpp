
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include "config.h"
#include "bruteforce.h"
#include "kd_tree.h"
//#include "r_tree.h"

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

		res = rangeQuery(head, qp, 3);
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
		struct point* kd_res = NULL;

		int len = read_dataset_kd(&kd_l, INPUT_FILE_NAME);
		if (!len) {
			fprintf(stdout, "file open error | nothing in the file\n\n");
		}

		kd_t = make_kdtree(kd_l, len, 0, 2);	//x first
		//print_preorder_kd(kd_t);			// TREE MADE
	
#if QUERY == DISTANCE

#endif
#if QUERY == RANGE
		input->x[0] = 7;	input->x[1] = 6;
		kd_res = rangeQuery_kd(kd_t, input, 2, 0);
		printf("stack : ");
		while (kd_res) {
			printf("(%lf, %lf)   ", kd_res->x, kd_res->y);
			kd_res = kd_res->next;
		}
#endif
#if QUERY == KNN
		int K = 1;
		input->x[0] = 7;	input->x[1] = 6;
		kd_res = kNNquery_kd(kd_t, input, K, 0);
		printf("stack : ");
		while (kd_res) {
			printf("(%lf, %lf)   ", kd_res->x, kd_res->y);
			kd_res = kd_res->next;
		}
		
#endif
	}
	else if (MODE == R_TREE) {
#if QUERY == DISTANCE

#endif
#if QUERY == RANGE

#endif
#if QUERY == KNN

#endif
	}
	else {
		fprintf(stdout, "WRONG MODE\n");
	}


	return 0;
}