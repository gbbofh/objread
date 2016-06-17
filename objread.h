#ifndef OBJREAD_H
#define OBJREAD_H

typedef struct obj_vertex {
    struct obj_vertex* next;
    char* pos;
    char* norm;
    char* tex;
} obj_vertex;

typedef struct obj_index {
    struct obj_index* next;
    char* index;
} obj_index;

typedef struct {
    obj_vertex verts;
    obj_index inds;
} obj_file;

void read_obj(void* base, void* eof, obj_file* obj);
void free_obj(obj_file* obj);

#endif
