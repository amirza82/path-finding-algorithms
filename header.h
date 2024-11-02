#ifndef MY_HEADER
#define MY_HEADER

#include <stdlib.h>
typedef struct action action;
typedef struct vertex vertex;
// apparently math.h has a log definition which conflicts with this one
// typedef struct log log;
typedef struct node node;

enum {
    ORADEA,
    ZERIND,
    ARAD,
    TIMISOARA,
    LUGOJ,
    MEHADIA,
    DOBRETA,
    SIBIU,
    RIMNICU_VILCEA,
    CRAIOVA,
    FAGARAS,
    PITESTI,
    BUCHAREST,
    GIURGIU,
    URZICENI,
    HIRSOVA,
    EFORIE,
    VASLUI,
    IASI,
    NEAMT,
    N_VERTICES
};

struct vertex {
    double xcord;
    double ycord;
    int id;
    int action_count;
    const char *name;
    action **actions;
};

struct action {
    int dest_id;
    int length;
};

struct log {
    int origin_id;
    int cost;
    int v_explored_count;
    double time;
    action **path; // array of action(s)
};

struct node {
    int depth;
    int cost;
    action *prev_action;
    node *prev_node;
    vertex *state;
};

// vertex_new: create and initialize a new vertex object
vertex *vertex_new (double xcord, double ycord, int id, const char *name, int action_count, ...);

// log_new: create log object and initialize to zero/null
struct log *log_new (void);
// log_print: print log info to stdout
void log_print(struct log *l);

// node_new: create and initialize a node object
node *node_new(int cost, action *prev_action, node *prev_node, vertex *vertex);
#define node_free(NODEPTR) (free(NODEPTR))

// search: search the graph for a path with the specified method
struct log *search (struct log *(*function) (int, int, struct log *), int origin_id, int destination_id);

#endif
