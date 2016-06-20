#include "common.h"
#include "mesh.h"

void parse_vertices(obj_vertex* ovtx, meshdata* mesh);
void parse_faces(obj_face* oface, meshdata* mesh);
int num_inds(obj_face* oface);

void create_mesh(obj_file* objdata, meshdata* mesh) {
    if(mesh == NULL || objdata == NULL) {
        msg_log(LOG_ERROR, "create_mesh: Bad parameters!");
    }
    memset(mesh, 0, sizeof(meshdata));
    parse_faces(&objdata->faces, mesh);
}

void delete_mesh(meshdata* mesh) {
}

void parse_vertices(obj_vertex* ovtx, meshdata* mesh) {
}

void parse_faces(obj_face* oface, meshdata* mesh) {
    obj_face* cur = oface;
    char* facedata;
    mesh->num_indx = 0;
    while(cur->next != NULL) {
        mesh->num_indx += num_inds(cur);
        cur = cur->next;
    }
    msg_log(LOG_INFO, "%d Triangle Indices", mesh->num_indx);
    mesh->indx = malloc(mesh->num_indx * sizeof(int));

    cur = oface;

/*    while(cur->next != NULL) {
        facedata = cur->face;
        while(*facedata != '\n') {
            facedata++;
        }
        cur = cur->next;
    }*/
}

int num_inds(obj_face* oface) {
    char* cur = oface->face;
    int num_verts = 0;
    while(*cur != '\n') {
        if(isdigit(*cur)) num_verts++;
        while(isdigit(*cur) && *(cur + 1) != '\n') cur++;
        cur++;
    }
    return (num_verts - 2) * 3;
}
