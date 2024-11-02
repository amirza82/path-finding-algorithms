#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "header.h"
#include "algorithm.h"

vertex *graph[N_VERTICES];

static void cleanall(void);

// static declerations
static void addq(node *n);
static node *getq(void);
static short headq = -1, tailq = 0;

static void adds(node *n);
static node *gets(void);
// static void truncates(void);
// static void truncateq(void);
// static void truncatepq(void);
static short heads = -1;

static node *getpq(void);
static void addpq(node * n, int priority);
typedef struct pqnode {
    int priority;
    node *n;
    struct pqnode *nextpqn;
} pqnode;
static struct pq {
    int size;
    struct pqnode *firstnodeptr;
} pq;

static int explored_vertices_id[N_VERTICES];
static int frontier_id[N_VERTICES];
static node *frontier[N_VERTICES];
static void trace_path(node *, struct log *);

#define h(NODE, DESTID) (sqrt(pow(NODE->state->xcord - graph[DESTID]->xcord, 2) + pow(NODE->state->ycord - graph[DESTID]->ycord, 2)))

static const char *get_vname(int id);

void graph_init()
{
    graph[ORADEA]         = vertex_new(180, 60, ORADEA, "oradea", 2, ZERIND, 71, SIBIU, 151);
    graph[ZERIND]         = vertex_new(140, 90, ZERIND, "zerind", 2, ARAD, 75, ORADEA, 71);
    graph[ARAD]           = vertex_new(80, 160, ARAD, "arad", 3, ZERIND, 75, SIBIU, 140, TIMISOARA, 118);
    graph[TIMISOARA]      = vertex_new(100, 270, TIMISOARA, "timisoara", 2, ARAD, 118, LUGOJ, 111);
    graph[LUGOJ]          = vertex_new(180, 330, LUGOJ, "lugoj", 2, TIMISOARA, 111, MEHADIA, 70);
    graph[MEHADIA]        = vertex_new(200, 390, MEHADIA, "mehadia", 2, LUGOJ, 70, DOBRETA, 75);
    graph[DOBRETA]        = vertex_new(160, 450, DOBRETA, "dobreta", 2, MEHADIA, 75, CRAIOVA, 120);
    graph[SIBIU]          = vertex_new(250, 160, SIBIU, "sibiu", 4, ORADEA, 151, ARAD, 140, RIMNICU_VILCEA, 80, FAGARAS, 99);
    graph[RIMNICU_VILCEA] = vertex_new(320, 230, RIMNICU_VILCEA, "rimnicu_vilcea", 3, SIBIU, 80, PITESTI, 97, CRAIOVA, 146);
    graph[CRAIOVA]        = vertex_new(320, 370, CRAIOVA, "craiova", 3, DOBRETA, 120, RIMNICU_VILCEA, 146, PITESTI, 138);
    graph[FAGARAS]        = vertex_new(320, 120, FAGARAS, "fagaras", 2, SIBIU, 99, BUCHAREST, 211);
    graph[PITESTI]        = vertex_new(390, 270, PITESTI, "pitesti", 3, RIMNICU_VILCEA, 97, CRAIOVA, 138, BUCHAREST, 101);
    graph[BUCHAREST]      = vertex_new(480, 280, BUCHAREST, "bucharest", 4, FAGARAS, 211, PITESTI, 101, GIURGIU, 90, URZICENI, 85);
    graph[GIURGIU]        = vertex_new(480, 390, GIURGIU, "giurgiu", 1, BUCHAREST, 90);
    graph[URZICENI]       = vertex_new(570, 250, URZICENI, "urziceni", 3, BUCHAREST, 85, HIRSOVA, 98, VASLUI, 142);
    graph[HIRSOVA]        = vertex_new(650, 270, HIRSOVA, "hirsova", 2, URZICENI, 98, EFORIE, 86);
    graph[EFORIE]         = vertex_new(660, 380, EFORIE, "eforie", 1, HIRSOVA, 86);
    graph[VASLUI]         = vertex_new(630, 160, VASLUI, "vaslui", 2, URZICENI, 142, IASI, 92);
    graph[IASI]           = vertex_new(580, 100, IASI, "iasi", 2, NEAMT, 87, VASLUI, 92);
    graph[NEAMT]          = vertex_new(550, 60, NEAMT, "neamt", 1, IASI, 87);
}

struct log *search(struct log *(*function) (int, int, struct log *), int origin_id, int destination_id)
{
    struct log *log = log_new();
    log->origin_id = origin_id;

    clock_t begin, end, t;
    begin = clock();
    function (origin_id, destination_id, log);
    end = clock();

    t = end - begin;
    log->time = (double) t / CLOCKS_PER_SEC;
    return log;
}


// BFS: breadth first search
struct log *BFS (int origin_id, int destination_id, struct log *log)
{
    printf("in BFS\n");
    node *pn, *pcn;
    action *pa;
    int i;
    addq(pcn = node_new(0, NULL, NULL, graph[origin_id]));
    frontier_id[origin_id] = 1;
    if (origin_id == destination_id) {
        trace_path(pcn, log);
        return log;
    }

    while (1) {
        pn = getq();
        if (pn == NULL) {
            fprintf(stderr, "line %d error: no path found\n", __LINE__);
            return log;
        }
        frontier_id[pn->state->id] = 0;
        explored_vertices_id[pn->state->id] = 1;
        log->v_explored_count++;
        for (i = 0; i < pn->state->action_count; ++i) {
            pa = pn->state->actions[i];
            if (explored_vertices_id[pa->dest_id] == 0 && frontier_id[pa->dest_id] == 0) {
                addq(pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]));
                frontier_id[pa->dest_id] = 1;
                if (pa->dest_id == destination_id) {
                    trace_path(pcn, log);
                    return log;
                }
            }
        }
    }
}

// DFS: depth first search
struct log *DFS (int origin_id, int destination_id, struct log *log)
{
    printf("in DFS\n");
    node *pn, *pcn;
    action *pa;
    int i;
    adds(pcn = node_new(0, NULL, NULL, graph[origin_id]));
    frontier_id[origin_id] = 1;
    if (origin_id == destination_id) {
        trace_path(pcn, log);
        return log;
    }

    while (1) {
        pn = gets();
        if (pn == NULL) {
            fprintf(stderr, "line %d error: no path found\n", __LINE__);
            return log;
        }
        frontier_id[pn->state->id] = 0;
        explored_vertices_id[pn->state->id] = 1;
        log->v_explored_count++;
        for (i = 0; i < pn->state->action_count; ++i) {
            pa = pn->state->actions[i];
            if (explored_vertices_id[pa->dest_id] == 0 && frontier_id[pa->dest_id] == 0) {
                adds(pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]));
                frontier_id[pa->dest_id] = 1;
                if (pa->dest_id == destination_id) {
                    trace_path(pcn, log);
                    return log;
                }
            }
        }
    }
}

// UCS: uniform cost search
struct log *UCS (int origin_id, int destination_id, struct log *log)
{
    printf("in UCS\n");
    node *pn, *pcn;
    action *pa;
    int i;
    pcn = node_new(0, NULL, NULL, graph[origin_id]);
    addpq(pcn, -pcn->cost);
    frontier_id[origin_id] = 1;

    while (1) {
        pn = getpq();
        if (pn == NULL) {
            fprintf(stderr, "line %d error: no path found\n", __LINE__);
            return log;
        }
        frontier_id[pn->state->id] = 0;
        explored_vertices_id[pn->state->id] = 1;
        log->v_explored_count++;
        if (pn->state->id == destination_id) { // check if node is destination WHEN it's being expanded
            trace_path(pn, log);
            return log;
        }
        for (i = 0; i < pn->state->action_count; ++i) {
            pa = pn->state->actions[i];
            if (explored_vertices_id[pa->dest_id] == 0) {
                pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]);
                addpq(pcn, -pcn->cost);
                frontier_id[pa->dest_id] = 1;
            }
        }
    }
}

// DLS: depth limited search
struct log *DLS (int origin_id, int destination_id, struct log *log)
{
    printf("in DLS\n");
    int limit = 0;
    while (limit < N_VERTICES) {
        node *pn, *pcn;
        action *pa;
        int i;
        adds(pcn = node_new(0, NULL, NULL, graph[origin_id]));
        frontier_id[origin_id] = 1;
        if (origin_id == destination_id) {
            trace_path(pcn, log);
            return log;
        }

        while (1) {
            pn = gets();
            if (pn == NULL) {
                cleanall();
                printf("\tdepth: %d\n", limit);
                limit++;
                break;
            } else if (pn->depth > limit)
                continue;
            frontier_id[pn->state->id] = 0;
            explored_vertices_id[pn->state->id] = 1;
            log->v_explored_count++;
            for (i = 0; i < pn->state->action_count; ++i) {
                pa = pn->state->actions[i];
                if (explored_vertices_id[pa->dest_id] == 0 && frontier_id[pa->dest_id] == 0) {
                    adds(pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]));
                    frontier_id[pa->dest_id] = 1;
                    if (pa->dest_id == destination_id) {
                        trace_path(pcn, log);
                        return log;
                    }
                }
            }
        }
    }
    fprintf(stderr, "line %d error: no path found\n", __LINE__);
    return log;
}
// GS: gready search
struct log *GS (int origin_id, int destination_id, struct log *log)
{
    printf("in GS\n");
    node *pn, *pcn = node_new(0, NULL, NULL, graph[origin_id]);
    action *pa;
    int i;
    addpq(pcn, -(int) h(pcn, destination_id));
    frontier_id[origin_id] = 1;

    while (1) {
        pn = getpq();
        if (pn == NULL) {
            fprintf(stderr, "line %d error: no path found\n", __LINE__);
            return log;
        }
        frontier_id[pn->state->id] = 0;
        explored_vertices_id[pn->state->id] = 1;
        log->v_explored_count++;
        if (pn->state->id == destination_id) { // check if node is destination WHEN it's being expanded
            trace_path(pn, log);
            return log;
        }
        for (i = 0; i < pn->state->action_count; ++i) {
            pa = pn->state->actions[i];
            if (explored_vertices_id[pa->dest_id] == 0) {
                pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]);
                addpq(pcn, -(int) h(pcn, destination_id));
                frontier_id[pa->dest_id] = 1;
            }
        }
    }
}

// AS: A* search
struct log *AS (int origin_id, int destination_id, struct log *log)
{
    printf("in AS\n");
    node *pn, *pcn = node_new(0, NULL, NULL, graph[origin_id]);
    action *pa;
    int i;
    addpq(pcn, -((int) h(pcn, destination_id) + pcn->cost));
    frontier_id[origin_id] = 1;

    while (1) {
        pn = getpq();
        if (pn == NULL) {
            fprintf(stderr, "line %d error: no path found\n", __LINE__);
            return log;
        }
        frontier_id[pn->state->id] = 0;
        explored_vertices_id[pn->state->id] = 1;
        log->v_explored_count++;
        if (pn->state->id == destination_id) { // check if node is destination WHEN it's being expanded
            trace_path(pn, log);
            return log;
        }
        for (i = 0; i < pn->state->action_count; ++i) {
            pa = pn->state->actions[i];
            if (explored_vertices_id[pa->dest_id] == 0) {
                pcn = node_new(pa->length + pn->cost, pa, pn, graph[pa->dest_id]);
                addpq(pcn, -((int) h(pcn, destination_id) + pcn->cost));
                frontier_id[pa->dest_id] = 1;
            }
        }
    }
}

// trace_path: trace the path to origin and set the cost
static void trace_path(node *n, struct log *l)
{
    *l->path = NULL;
    l->cost = n->cost;
    while (n) {
        *++l->path = n->prev_action;
        n = n->prev_node;
    }
}


static void cleanall(void)
{
    node *n;
    for (int i = 0; i < N_VERTICES; ++i) {
        explored_vertices_id[i] = frontier_id[i] = 0;
    }
    while (n = gets())
        node_free(n);

}
// ------------------------- queue functions -------------------------

static void addq(node *n)
{
    if (tailq == N_VERTICES) {
        fprintf(stderr, "line %d error: queue full\n", __LINE__);
        return;
    }
    frontier[tailq++] = n;
}

static node *getq(void)
{
    if (++headq == tailq) {
        fprintf(stderr, "line %d error: queue empty\n", __LINE__);
        return NULL;
    }
    return frontier[headq];
}

// ------------------------- stack functions -------------------------
static void adds(node *n)
{
    if (++heads == N_VERTICES) {
        fprintf(stderr, "line %d error: stack full\n", __LINE__);
        return;
    }
    frontier[heads] = n;
    printf("adds: %s\n", n->state->name);
}

static node *gets(void)
{
    if (heads == -1) {
        fprintf(stderr, "line %d error: stack empty\n", __LINE__);
        return NULL;
    }
    return frontier[heads--];
}

// ------------------------- priority queue -------------------------
static void addpq(node * n, int priority)
{
    printf("addpq:\tcost: %d, name: %s, priority: %d\n", n->cost, n->state->name, priority);
    pqnode **pqnptr = &pq.firstnodeptr;
    pqnode *newpqnptr = (pqnode *) malloc (sizeof (pqnode));
    pq.size++;
    *newpqnptr = (pqnode) {priority, n, NULL};
    while (*pqnptr) {
        if (newpqnptr->priority > (*pqnptr)->priority) {
            newpqnptr->nextpqn = *pqnptr;
            *pqnptr = newpqnptr;
            return;
        }
        pqnptr = &((*pqnptr)->nextpqn);
    }
    // we have reached the end of the queue
    *pqnptr = newpqnptr;
}
static node *getpq(void)
{
    node *pn;
    pqnode *pqn;
    if (pq.size == 0) {
        fprintf(stderr, "line %d error: Priority queue is empty\n", __LINE__);
        return NULL;
    }
    pq.size--;
    pqn = pq.firstnodeptr;
    pn = pqn->n;
    pq.firstnodeptr = pqn->nextpqn;
    free(pqn);
    printf("getpq:\tcost: %d, name: %s\n", pn->cost, pn->state->name);
    return pn;
}


void log_print(struct log *l)
{
    printf("|---------- log object ----------|\n");
    printf("\tcpu time: %lf\n\tcost: %d\n\texplored vertices: %d\n\npath:\n", l->time, l->cost, l->v_explored_count);

    printf("\t%s", get_vname(l->origin_id));
    while (*--l->path)
        printf(" ---> %s", get_vname((*l->path)->dest_id));
    putchar('\n');
}

static const char *get_vname(int id)
{
    if (id < 0 || id >= N_VERTICES)
         return NULL;
    return graph[id]->name;
}
