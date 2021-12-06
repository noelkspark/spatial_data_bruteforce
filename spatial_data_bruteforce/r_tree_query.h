#pragma once
#include "config.h"
#include "context.h"
#include "util.h"

int MySearchCallback(int id, void* arg);

int RTree_RangeQuery();
int RTree_KNNQuery();

void construct_rtree(RTREENODE** head, point* phead, int n);
int RTree_ReadData(point** phead, const char* file_name);