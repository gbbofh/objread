#ifndef MESH_H
#define MESH_H

#include "objread.h"

typedef struct {
    float position[3];
    float normal[3];
    float texture[2];
} vertex;

typedef struct {
    int num_verts;
    vertex* verts;
    int num_indx;
    int* indx;
} meshdata;

void create_mesh(obj_file* objdata, meshdata* mesh);
void delete_mesh(meshdata* mesh);
void parse_faces(obj_face* objdata, meshdata* mesh);

#endif
