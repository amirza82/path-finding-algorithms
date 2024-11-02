#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdio.h>
#include "header.h"

void graph_init(void);

struct log *BFS (int origin_id, int destination_id, struct log *log);
struct log *DFS (int origin_id, int destination_id, struct log *log);
struct log *UCS (int origin_id, int destination_id, struct log *log);
struct log *DLS (int origin_id, int destination_id, struct log *log);
struct log *AS (int origin_id, int destination_id, struct log *log);
struct log *GS (int origin_id, int destination_id, struct log *log);

#endif
