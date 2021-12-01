
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include "config.h"
#include "context.h"
#include "util_functions.h"
#include "bruteforce.h"

int main() {
	point* head = NULL;

	read_dataset(&head, INPUT_FILE_NAME);
	
	if (MODE == BRUTE_FORCE) {
#if QUERY == DISTANCE
		/*distance query example*/
		point p;
		Rect r;

		p.x = 2;	p.y = 2;
		r.max_x = 1;	r.max_y = 1;	r.min_x = 0;	r.min_y = 0;

		fprintf(stdout, "\ncalculated distance between point (%lf, %lf) | Rectangle (%lf, %lf) (%lf, %lf): %lf\n", p.x, p.y, r.min_x, r.min_y, r.max_x, r.max_y, distance(p, r));
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
#if QUERY == DISTANCE

#endif
#if QUERY == RANGE

#endif
#if QUERY == KNN

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