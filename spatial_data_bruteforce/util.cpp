#include "config.h"
#include "context.h"
#include "util.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

point* create_point(double x, double y) {
	point* new_p = (point*)malloc(sizeof(point));
	new_p->x = x;
	new_p->y = y;
	new_p->pre = NULL;
	new_p->next = NULL;
	//printf("point (%lf %lf) created\n", x, y);
	return new_p;
}
void push_point(point** head, point* n_p) {
	if (!head)
		return;
	if (*head == NULL) {
		*head = n_p;
		//printf("first element (%lf, %lf) pushed\n", n_p->x, n_p->y);
		return;
	}
	n_p->next = *head;
	(*head)->pre = n_p;
	*head = n_p;
	//printf("element %lf %lf pushed\n", n_p->x, n_p->y);
}

int pop_point(point** head, point* tar) {
	point* point_to_be_popped = tar;
	point* tmp = *head;

	//printf("popping target point (%lf, %lf)\n", tar->x, tar->y);

	if ((*head) == tar) {
		if ((*head)->next) {
			(*head)->next->pre = NULL;
		}
		*head = (*head)->next;
		free(point_to_be_popped);

		return 1;
	}
	while ((*head)->next != NULL) {
		if ((*head) == tar) {
			(*head)->pre->next = (*head)->next;
			(*head)->next->pre = (*head)->pre;
			free(point_to_be_popped);
			*head = tmp;

			return 1;
		}
		(*head) = (*head)->next;
	}

	if ((*head) == tar) {
		(*head)->pre->next = NULL;
		free(point_to_be_popped);
		*head = tmp;

		return 1;
	}
	fprintf(stdout, " \nnothing to delete\n");

	return 0;
}




