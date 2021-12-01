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