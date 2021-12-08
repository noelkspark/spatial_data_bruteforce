#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "bruteforce.h"
#include "config.h"
#include "util.h"


using namespace std;

//extern point* head;

void rangeQuery(point** res, point* head, point qp, double r) {

	point* new_p;

	for (point* ptr = head; ptr != NULL; ptr = ptr->next) {
		if (sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2)) <= r) {
			new_p = create_point(ptr->x, ptr->y);
			push_point(res, new_p);
		}
	}

}

point* kNNQuery(point** head, point qp, int k) {
	point* res = NULL;
	point* tar = NULL;
	double tar_x, tar_y;

	point* new_p;
	point* tmp;
	double min_dist = DBL_MAX;

	for (int i = 0; i < k; i++) {
		tmp = *head;
		for (point* ptr = *head; ptr != NULL; ptr = ptr->next) {
			min_dist = DBL_MAX;
			if (min_dist > sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2))) {
				min_dist = sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2));
				tar = ptr;
				
				//printf("cur (%lf, %lf) | tar (%lf, %lf)\n", ptr->x, ptr->y, tar->x, tar->y);
			}
		}
		tar_x = tar->x;	tar_y = tar->y;
		
		if (pop_point(head, tar)) {
			new_p = create_point(tar_x, tar_y);
			push_point(&res, new_p);
		}
		
	}

	return res;
}

void read_dataset_bf(point** phead, const char* file_name) {

	FILE* fp = fopen(file_name, "r");
	if (!fp) {
		fprintf(stdout, "FILE OPEN ERROR");
		return;
	}
	int line_num = 0;
	char tmp_buf[20], tmp_x[10], tmp_y[10], dummy[5];
	double dx, dy;
	point* new_p;

	while (!feof(fp)) {
		fgets(tmp_buf, sizeof(tmp_buf), fp);
		line_num++;
	}
	printf("%d lines read\n", line_num);
	fseek(fp, 0, SEEK_SET);
	for (int i = 0; i < line_num; i++) {
		fgets(tmp_buf, sizeof(tmp_buf), fp);
		sscanf(tmp_buf, "%[^',']%[^' ']%s", tmp_x, dummy, tmp_y);
		//printf("%s | %s\n", tmp_x, tmp_y);
		dx = atof(tmp_x);
		dy = atof(tmp_y);

		new_p = create_point(dx, dy);

		//printf("%lf %lf\n", new_p->x, new_p->y);

		push_point(phead, new_p);

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


}
