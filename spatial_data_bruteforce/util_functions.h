#include<stdio.h>
#include<stdlib.h>
#include "context.h"

point* create_point(double x, double y);
void push_point(point** ptr, point* n_p);
int pop_point(point** ptr, point* tar);
void read_dataset(point** head, const char* filename);
