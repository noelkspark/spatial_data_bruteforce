#include "context.h"


double distance(point p, Rect rec); 
//����Ʈ�� �簢�� ������ �ּ� �Ÿ�

point* rangeQuery(point* head, point qp, double r);
//range query�� ���� ������ ���� ����Ʈ�� ���� �ݰ�

point* kNNQuery(point** head,point qp, int k);
//kNN query�� ���������� ���� ����Ʈ�� �ֱ����̿� ��