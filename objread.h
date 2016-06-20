#ifndef OBJREAD_H
#define OBJREAD_H

typedef struct obj_vertex {
    struct obj_vertex* next;
    char* pos;
    char* norm;
    char* tex;
} obj_vertex;

typedef struct obj_face {
    struct obj_face* next;
    char* face;
} obj_face;

typedef struct {
    int vert_count;
    int face_count;
    obj_vertex verts;
    obj_face faces;
} obj_file;

void read_obj(void* base, void* eof, obj_file* obj);
void free_obj(obj_file* obj);

#endif
