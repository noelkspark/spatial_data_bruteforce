#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>

#include "r_tree.h"
#include "r_tree_query.h"

point* rtree_RQ_res;

int MySearchCallback(int id, void* arg) {
    /* Note: -1 to make up for the +1 when data was inserted */
    fprintf(stdout, "Hit data mbr %d \n", id - 1);
    return 1; /* keep going */
}

static double mid(double x1,double x2, double x3){

    if(x1 <= x2)
        if(x2 <= x3)
            return x2;  
        else if(x1 <= x3)
            return x3;  
        else 
            return x1;
    else
        if(x1 <= x3)
            return x1;  
        else if(x2 < x3)
            return x3;
        else 
            return x2;
}
double mbr_to_point_distance(RTREEMBR rec, point p) {
    if (p.x <= rec.bound[3] && p.x >= rec.bound[0]) {
        if (p.y <= rec.bound[1]) {
            return rec.bound[1] - p.y;
        }
        else if (p.y >= rec.bound[4]) {
            return p.y - rec.bound[4];
        }
        else {
            //printf("Point is inside the Rectangle\n");
            return 0.0;
        }
    }
    else if (p.y <= rec.bound[4] && p.y >= rec.bound[1]) {
        if (p.x <= rec.bound[1]) {
            return rec.bound[1] - p.x;
        }
        else if (p.x >= rec.bound[3]) {
            return p.x - rec.bound[3];
        }
        else {
            //printf("Point is inside the Rectangle\n");
            return 0.0;
        }
    }
    else if (p.x <= rec.bound[0] && p.y <= rec.bound[1]) {
        return sqrt(pow(p.x - rec.bound[0], 2) + pow(p.y - rec.bound[1], 2));
    }
    else if (p.x <= rec.bound[0] && p.y >= rec.bound[4]) {
        return sqrt(pow(p.x - rec.bound[0], 2) + pow(p.y - rec.bound[4], 2));
    }
    else if (p.x >= rec.bound[3] && p.y >= rec.bound[4]) {
        return sqrt(pow(p.x - rec.bound[3], 2) + pow(p.y - rec.bound[4], 2));
    }
    else if (p.x <= rec.bound[3] && p.y <= rec.bound[1]) {
        return sqrt(pow(p.x - rec.bound[3], 2) + pow(p.y - rec.bound[1], 2));
    }
    else {
        printf("Something is Wrong\n");
    }
}

int RTree_RangeQuery(RTREENODE* head, point qp, double radius){

    /* Fix not yet tested. */
    int obj_ref = 0;
    int i, j;

    assert(head);
    assert(head->level >= 0);

    if (head->level > 0) /* this is an internal node in the tree */
    {
        for (i = 0; i < MAXCARD; i++) {
            if (head->branch[i].child) {
                ++obj_ref;
                double min_x = mid(head->branch[i].mbr.bound[0], head->branch[i].mbr.bound[3], qp.x);
                double min_y = mid(head->branch[i].mbr.bound[1], head->branch[i].mbr.bound[4], qp.y);
                double dist = 0;

                dist = sqrt(pow(min_x - qp.x, 2) + pow(min_y - qp.y, 2));
                if (dist <= radius)
                    obj_ref += RTree_RangeQuery(head->branch[i].child, qp, radius);

            }
        }
    }
    else /* this is a leaf node */
    {
        for (i = 0; i < MAXCARD; i++)
        {
            if (head->branch[i].child) {
                ++obj_ref;
                double dist = sqrt(pow(head->branch[i].mbr.bound[0] - qp.x, 2) + pow(head->branch[i].mbr.bound[1] - qp.y, 2));
                if (dist <= radius) {
                    point* node = create_point(head->branch[i].mbr.bound[0], head->branch[i].mbr.bound[1]);
                    push_point(&rtree_RQ_res, node);
                }
            }
        }
    }
    return obj_ref;

	
}

r_heap_node* RTree_KNNQuery(RTREENODE* head, point qp, int K, int* heap_cnt) {
    struct r_candidate_node* r_st = NULL;
    point center;
    struct r_candidate_node* new_node;
    struct r_candidate_node* popped;
    int overlap_flag;
    double max_mindist = DBL_MAX;
    double tdist;

    r_heap_node* mheap = (r_heap_node*)malloc(sizeof(r_heap_node) * (K + 1));
    r_heap_node htmp;

     new_node = r_create_node(*head, head);
     rstack_push(&r_st, new_node); //root pushed
    

    while (popped = rstack_pop(&r_st)) {
        head = popped->ptr; //node where we should start from
        tdist = popped->distance;
        int cnt = head->count;
        for (int i = 0; i < cnt; i++) {  //check children
            if (!head)  //nothing to check
                break;

            double min_x = mid(head->branch[i].mbr.bound[0], head->branch[i].mbr.bound[3], qp.x);
            double min_y = mid(head->branch[i].mbr.bound[1], head->branch[i].mbr.bound[4], qp.y);
            tdist = sqrt(pow(min_x - qp.x, 2) + pow(min_y - qp.y, 2));

            if (max_mindist > tdist) {
                if (head->level == 0) { //if leaf node
                    htmp.distance = tdist;
                    htmp.node = *head;
                    if (*heap_cnt >= K) { //if KNN heap is full
                        r_heap_node poppedheapnode;
                        poppedheapnode = rmaxheap_pop(mheap, heap_cnt);
                    }
                    rmaxheap_push(mheap, htmp, heap_cnt);   //push new node
                    max_mindist = mheap[1].distance;
                }
                else {
                    new_node = r_create_node(*(head->branch[i].child), head->branch[i].child);
                    rstack_push(&r_st, new_node);                   
                }
            }
        }
        
    }

    return mheap;

}

inline double rdist(RTREENODE* a, point qp)
{
    double t1, t2, sqr;
  
    t1 = (a->branch->mbr.bound[0] - qp.x);
    t2 = (a->branch->mbr.bound[1] - qp.y); 

    sqr = t1 * t1 + t2 * t2;

    return sqrt(sqr);
}

void rstack_push(struct r_candidate_node** rst, struct r_candidate_node* n_p) {
    //printf("pushing %p\n", n_p);
    r_candidate_node* tmp = *rst;

    if (*rst == NULL) {
        *rst = n_p;
        return;
    }
    
    n_p->next = *rst;
    (*rst)->pre = n_p;
    *rst = n_p;
}
void rstack_push_inorder(struct r_candidate_node** head, struct r_candidate_node* n_p) {
    r_candidate_node* tmp = *head;

    if (*head == NULL) {
        *head = n_p;
        return;
    }

    if ((*head)->next == NULL && (*head)->distance > n_p->distance) {   //if only one node
        //push infront of head
        n_p->next = *head;
        (*head)->pre = n_p;
        *head = n_p;
        return;
    }

    while ((*head)->next) { //in while if more than one node
        if ((*head)->distance > n_p->distance) {
            //push infront of head
            n_p->next = *head;
            (*head)->pre = n_p;
            *head = tmp;
            return;
        }
        (*head) = (*head)->next;
    }
    //at last node
    if ((*head)->distance > n_p->distance) {
        //push infront of head
        n_p->next = *head;
        (*head)->pre = n_p;
    }
    else {
        //push end
        (*head)->next = n_p;
        n_p->pre = *head;
    }

    *head = tmp;
}

struct r_candidate_node* rstack_pop(struct r_candidate_node** rdt) {
    if (!rdt) {
        printf("somthing wrong\n\n\n\n");
        return NULL;
    }
    if ((*rdt) == NULL) {
        //fprintf(stdout, "nothing to pop, EMPTY\n");
        return NULL;
    }

    struct r_candidate_node* point_to_be_popped = *rdt;
    struct r_candidate_node* res = (struct r_candidate_node*)malloc(sizeof(struct r_candidate_node));

    *res = **rdt;

    if ((*rdt)->next != NULL) {
        (*rdt) = (*rdt)->next;
        (*rdt)->pre = NULL;
    }
    else {
        (*rdt) = NULL;
    }

    // free(point_to_be_popped);

    return res;
}
struct r_candidate_node* r_create_node(RTREENODE r_node, RTREENODE* ptr) {
    struct r_candidate_node* new_node = (struct r_candidate_node*)malloc(sizeof(struct r_candidate_node));

    new_node->current_node = r_node;
    new_node->pre = new_node->next = NULL;
    new_node->ptr = ptr;

    return new_node;
}

void r_swap(struct r_heap_node* a, struct r_heap_node* b) {
    struct r_heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}
void rmaxheap_push(struct r_heap_node* heap, struct r_heap_node node, int* cnt) {
    (*cnt)++;
    heap[(*cnt)] = node;

    int child = *cnt;
    int parent = child / 2;

    while (child > 1 && heap[child].distance > heap[parent].distance) {
        r_swap(&heap[child], &heap[parent]);
        child = parent;
        parent = child / 2;
    }
    //printf("pushed heap : (%lf)\n", node.distance);
}
 struct r_heap_node rmaxheap_pop(struct r_heap_node* heap, int* cnt) {
     struct r_heap_node ret = heap[1];
     //printf("heap popping (%lf)\n", ret.distance);
     r_swap(&heap[1], &heap[*cnt]);
     (*cnt)--;

     int parent = 1;
     int child = parent * 2;

     if (child + 1 <= *cnt) {
         child = (heap[child].distance > heap[child + 1].distance) ? child : child + 1;
     }

     while (child <= *cnt && heap[child].distance > heap[parent].distance) {
         r_swap(&heap[child], &heap[parent]);
         parent = child;
         child = parent * 2;

         if (child + 1 <= *cnt) {
             child = (heap[child].distance > heap[child + 1].distance) ? child : child + 1;
         }
     }

     return ret;
}

void  construct_rtree(RTREENODE** root, point* phead, int n) {
    RTREEMBR* rects = (RTREEMBR*)malloc(sizeof(RTREEMBR) * (n+1));
	
	int i = 0;
	

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

	fclose(fp);
	return line_num;


}



