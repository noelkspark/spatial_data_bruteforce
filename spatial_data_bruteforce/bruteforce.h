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
//����Ʈ�� �簢�� ������ �ּ� �Ÿ�

point* rangeQuery(point* head, point qp, double r);
//range query�� ���� ������ ���� ����Ʈ�� ���� �ݰ�

point* kNNQuery(point** head,point qp, int k);
//kNN query�� ���������� ���� ����Ʈ�� �ֱ����̿� ��



point* create_point(double x, double y);
void push_point(point** ptr, point* n_p);
int pop_point(point** ptr, point* tar);
void read_dataset_bf(point** head, const char* filename);