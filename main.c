#include "header.h"
#include "algorithm.h"

int main (int argc, char **argv)
{
    graph_init();
    struct log *l;
    /******** ONLY ONE METHOD SHOULD BE USED ********/
    l = search(AS, ARAD, BUCHAREST);
    log_print(l);
    // l = search(DFS, ARAD, BUCHAREST);
    // log_print(l);
    // l = search(DLS, ARAD, BUCHAREST);
    // log_print(l);
    // l = search(BFS, ARAD, BUCHAREST);
    // log_print(l);
    // l = search(UCS, ARAD, BUCHAREST);
    // log_print(l);
    // l = search(GS, ARAD, BUCHAREST);
    // log_print(l);
}

