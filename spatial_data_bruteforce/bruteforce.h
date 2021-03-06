#include "context.h"

double distance(point p, Rect rec); 
//포인트와 사각형 까지의 최소 거리

void rangeQuery(point** res, point* head, point qp, double r);
//range query의 질의 조건인 질의 포인트와 질의 반경

point* kNNQuery(point** head,point qp, int k);
//kNN query의 질의조건인 질의 포인트와 최근접이웃 개

void read_dataset_bf(point** head, const char* filename);