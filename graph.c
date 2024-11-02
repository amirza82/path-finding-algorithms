#include <stdarg.h>
#include <stdio.h>
#include "header.h"

vertex *vertex_new(double xcord, double ycord , int id, const char *name, int action_count, ...)
{
    int i;

    vertex *v = (vertex *) malloc(sizeof (vertex));
    v->xcord = xcord;
    v->ycord = ycord;
    v->id = id;
    v->action_count = action_count;
    v->name = name;

    action **actions = (action **) malloc(action_count * sizeof (action *));
    action *pa;
    va_list parg;
    va_start(parg, action_count);
    for (i = 0; i < action_count; ++i) {
        pa = (action *) malloc(sizeof (action));
        pa->dest_id = va_arg(parg, int);
        pa->length = va_arg(parg, int); va_end(parg);
        actions[i] = pa;
    }

    v->actions = actions;
    return v;
}

struct log *log_new(void)
{
    struct log *l = (struct log *) malloc(sizeof (struct log));
    *l = (struct log) {}; // Initialize everything to zero
    l->path = (action **) malloc(N_VERTICES * sizeof (action *));
    return l;
}

node *node_new(int cost, action *prev_action, node *prev_node, vertex *vertex)
{
    node *n = (node *) malloc(sizeof (node));
    *n = (node) {(prev_node) ? ++prev_node->depth:1, cost, prev_action, prev_node, vertex};
    return n;
}
