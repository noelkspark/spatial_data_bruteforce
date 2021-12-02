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

double distance(point p, Rect rec); 
//포인트와 사각형 까지의 최소 거리

point* rangeQuery(point* head, point qp, double r);
//range query의 질의 조건인 질의 포인트와 질의 반경

point* kNNQuery(point** head,point qp, int k);
//kNN query의 질의조건인 질의 포인트와 최근접이웃 개



point* create_point(double x, double y);
void push_point(point** ptr, point* n_p);
int pop_point(point** ptr, point* tar);
void read_dataset_bf(point** head, const char* filename);