#ifndef MESH_H
#define MESH_H

typedef struct {
    float position[3];
    float normal[3];
    float texture[2];
} vertex;

typedef struct {
    int num_verts;
    vertex* verts;
    int num_indx;
} meshdata;

#endif
