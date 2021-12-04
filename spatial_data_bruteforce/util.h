#pragma once

#include "context.h"

double rect_to_point_distance(point p, Rect rec);
point* create_point(double x, double y);
void push_point(point** ptr, point* n_p);
int pop_point(point** ptr, point* tar);

