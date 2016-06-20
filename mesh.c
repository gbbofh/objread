#include "common.h"
#include "mesh.h"

void parse_vertices(obj_vertex* ovtx, meshdata* mesh);
void parse_faces(obj_face* oface, meshdata* mesh);

void create_mesh(obj_file* objdata, meshdata* mesh) {
    if(mesh == NULL || objdata == NULL) {
        msg_log(LOG_ERROR, "create_mesh: Bad parameters!");
    }
    mesh->verts = malloc(sizeof(vertex) * objdata->vert_count);
    // Obj faces are ngons. Need to first convert them to triangles.
    // For each ngon, can subtract 2 from number of vertices in it
    // to get number of triangles. Num triangles * 3 is num triangle indices
}

void delete_mesh(meshdata* mesh) {
}

void parse_vertices(obj_vertex* ovtx, meshdata* mesh) {
}

void parse_faces(obj_face* oface, meshdata* mesh) {
    obj_face* cur = oface;
    while(cur->next != NULL) {
        cur = cur->next;
    }
}


void parse_face(obj_face* oface) {
    char* cur = oface->face;
    int num_verts = 0;
    int num_inds = 0;
    while(*cur != '\n') {
        if(isdigit(*cur)) num_verts++, while(isdigit(*(cur++));
        cur++;
    }

    num_inds = (num_verts - 2) * 3;
}
