#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "bruteforce.h"
#include "config.h"
#include "context.h"
#include "util_functions.h"

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
