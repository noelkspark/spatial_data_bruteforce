#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "bruteforce.h"
#include "config.h"


using namespace std;

extern point* head;

double distance(point p, Rect rec) {
	if (p.x <= rec.max_x && p.x >= rec.min_x) {
		if (p.y <= rec.min_y) {
			return rec.min_y - p.y;
		}
		else if (p.y >= rec.max_y) {
			return p.y - rec.max_y;
		}
		else {
			printf("Point is inside the Rectangle\n");
			return 0.0;
		}
	}
	else if (p.y <= rec.max_y && p.y >= rec.min_y) {
		if (p.x <= rec.min_x) {
			return rec.min_x - p.x;
		}
		else if (p.x >= rec.max_x) {
			return p.x - rec.max_x;
		}
		else {
			printf("Point is inside the Rectangle\n");
			return 0.0;
		}
	}
	else if(p.x <= rec.min_x && p.y <= rec.min_y){
		return sqrt(pow(p.x - rec.min_x, 2) + pow(p.y - rec.min_y, 2));
	}
	else if (p.x <= rec.min_x && p.y >= rec.max_y) {
		return sqrt(pow(p.x - rec.min_x, 2) + pow(p.y - rec.max_y, 2));
	}
	else if (p.x >= rec.max_x && p.y >= rec.max_y) {
		return sqrt(pow(p.x - rec.max_x, 2) + pow(p.y - rec.max_y, 2));
	}
	else if (p.x <= rec.max_x && p.y <= rec.min_y) {
		return sqrt(pow(p.x - rec.max_x, 2) + pow(p.y - rec.min_y, 2));
	}
	else {
		printf("Something is Wrong\n");
	}
}

point* rangeQuery(point* head, point qp, double r) {

	point* res = NULL;
	point* new_p;

	for (point* ptr = head; ptr != NULL; ptr = ptr->next) {
		if (sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2)) <= r) {
			new_p = create_point(ptr->x, ptr->y);
			push_point(&res, new_p);
		}
	}

	return res;
}


point* kNNQuery(point** head, point qp, int k) {
	point* res = NULL;
	point* tar = NULL;
	double tar_x, tar_y;

	point* new_p;
	point* tmp = *head;
	double min_dist = DBL_MAX;

	for (int i = 0; i < k; i++) {
		for (point* ptr = *head; ptr != NULL; ptr = ptr->next) {
			if (min_dist > sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2))) {
				min_dist = sqrt(pow(ptr->x - qp.x, 2) + pow(ptr->y - qp.y, 2));
				tar = ptr;
				tar_x = tar->x;	tar_y = tar->y;
				printf("cur (%lf, %lf) | tar (%lf, %lf)\n", ptr->x, ptr->y, tar->x, tar->y);
			}
		}
		
		if (pop_point(head, tar)) {
			new_p = create_point(tar_x, tar_y);
			push_point(&res, new_p);
		}
	}

	return res;
}




point* create_point(double x, double y) {
	point* new_p = (point*)malloc(sizeof(point));
	new_p->x = x;
	new_p->y = y;
	new_p->pre = NULL;
	new_p->next = NULL;
	printf("point (%lf %lf) created\n", x, y);
	return new_p;
}
void push_point(point** head, point* n_p) {

	point* tmp = *head;

	if (*head == NULL) {
		*head = n_p;
		printf("first element (%lf, %lf) pushed\n", n_p->x, n_p->y);
		return;
	}

	while ((*head)->next != NULL) {
		printf("moving, current element is (%lf ,%lf)\n", (*head)->x, (*head)->y);
		(*head) = (*head)->next;
		printf("moved to (%lf, %lf)\n", (*head)->x, (*head)->y);
	}

	(*head)->next = n_p;
	n_p->pre = *head;
	printf("element %lf %lf pushed\n", n_p->x, n_p->y);

	*head = tmp;
}
int pop_point(point** head, point* tar) {
	point* point_to_be_popped = tar;
	point* tmp = *head;

	printf("popping target point (%lf, %lf)\n", tar->x, tar->y);

	if ((*head) == tar) {
		if ((*head)->next) {
			(*head)->next->pre = NULL;
		}
		*head = (*head)->next;
		free(point_to_be_popped);

		///
		tmp = *head;
		while ((*head)) {
			printf("linked list from 1: (%lf ,%lf)\n", (*head)->x, (*head)->y);
			(*head) = (*head)->next;
		}
		*head = tmp;
		///

		return 1;
	}
	while ((*head)->next != NULL) {
		if ((*head) == tar) {
			(*head)->pre->next = (*head)->next;
			(*head)->next->pre = (*head)->pre;
			free(point_to_be_popped);
			*head = tmp;

			///
			while ((*head)) {
				printf("linked list from 2: (%lf ,%lf)\n", (*head)->x, (*head)->y);
				(*head) = (*head)->next;
			}
			*head = tmp;
			///
			return 1;
		}
		(*head) = (*head)->next;
	}

	if ((*head) == tar) {
		(*head)->pre->next = NULL;
		free(point_to_be_popped);
		*head = tmp;

		///
		while ((*head)) {
			printf("linked list from 3: (%lf ,%lf)\n", (*head)->x, (*head)->y);
			(*head) = (*head)->next;
		}
		*head = tmp;
		///

		return 1;
	}
	fprintf(stdout, " \nnothing to delete\n");

	return 0;
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
