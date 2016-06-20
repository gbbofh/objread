#include "common.h"
#include "mesh.h"

void parse_vertex(obj_vertex* ovtx, vertex* vtx);

void create_mesh(obj_file* objdata, meshdata* mesh) {
    if(mesh == NULL || objdata == NULL) {
        msg_log(LOG_ERROR, "create_mesh: Bad parameters!");
    }
    mesh->vert_count = malloc(sizeof(vertex) * obj->vert_count);
    // Obj faces are ngons. Need to first convert them to triangles.
}

void delete_mesh(meshdata* mesh) {
}
