#ifndef UTIL_H
#define UTIL_H

struct table_node {
    char *key;
    void *value;
    struct table_node *next;
}

typedef struct table_node* hashEntry;

hashEntry *hashInit(int hashsize);

#endif