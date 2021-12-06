#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>

#include "r_tree.h"
#include "r_tree_query.h"


int RTree_RangeQuery() {
	return 0;
}

int RTree_KNNQuery() {
	return 0;
}

void  construct_rtree(RTREENODE** root, point* phead, int n) {
    RTREEMBR* rects = (RTREEMBR*)malloc(sizeof(RTREEMBR) * (n+1));
	//RTREEMBR* init = (RTREEMBR*)malloc(sizeof(RTREEMBR));
	/*
	init->bound[0] = init->bound[1] = init->bound[2] = init->bound[5] = 0;
	init->bound[3] = init->bound[4] = DBL_MAX;
    int i = 1;
	*/
	int i = 0;
	//RTreeInsertRect(init, 1, root, 0);

    while (phead) {
        rects[i].bound[0] = rects[i].bound[3] = phead->x;
        rects[i].bound[1] = rects[i].bound[4] = phead->y;
        rects[i].bound[2] = rects[i++].bound[5] = 0;
        phead = phead->next;
    }

	for (i = 0; i < n; i++) {
		RTreeInsertRect(&rects[i], i + 1, root, 0);
	}
}

int RTree_ReadData(point** phead, const char* file_name) {


	FILE* fp = fopen(file_name, "r");
	if (!fp) {
		fprintf(stdout, "FILE OPEN ERROR");
		return 0;
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
	return line_num;


}

int MySearchCallback(int id, void* arg) {
	/* Note: -1 to make up for the +1 when data was inserted */
	fprintf(stdout, "Hit data mbr %d \n", id - 1);
	return 1; /* keep going */
}

