#include "objread.h"
#include "common.h"

/*
 * READ_OBJ(VOID* BASE, VOID* EOF, OBJ_FILE* OBJ)
 * Fills a buffer (obj_file struct) with offsets to
 * data within the file. These offsets point to:
 * Vertex position, texture, and normal data, as well
 * as information for face/triangle indices.
*/
void read_obj(void* base, void* eof, obj_file* obj) {
    char*       vp_base = base;
    char*       vt_base = base;
    char*       vn_base = base;
    char*       fi_base = base;
    char*       tmp;
    obj_vertex* cvert;
    obj_face*  cface;
    int i;

    // Locate initial offsets in file
    // Would like to come up with easier/shorter/prettier way to do this.
    // Can't seem to get it working though. Will come back and refactor.
    while(vp_base < (char*)eof) {
        if(*vp_base == 'v' && isspace(*(vp_base + 1))) {
            break;
        }
        while(vp_base < (char*)eof && *(vp_base++) != '\n');
    }
    while(vt_base < (char*)eof) {
        if(*vt_base == 'v' && *(vt_base + 1) == 't') {
            break;
        }
        while(vt_base < (char*)eof && *(vt_base++) != '\n');
    }
    while(vn_base < (char*)eof) {
        if(*vn_base == 'v' && *(vn_base + 1) == 'n') {
            break;
        }
        while(vn_base < (char*)eof && *(vn_base++) != '\n');
    }
    while(fi_base < (char*)eof) {
        if(*fi_base == 'f' && isspace(*(fi_base + 1))) {
            break;
        }
        while(fi_base < (char*)eof && *(fi_base++) != '\n');
    }

    msg_log(LOG_INFO, "Vertex Positions start at 0x%p", vp_base);
    msg_log(LOG_INFO, "Vertex UV Coords start at 0x%p", vt_base);
    msg_log(LOG_INFO, "Vertex Normals start at 0x%p", vn_base);
    msg_log(LOG_INFO, "Faces start at 0x%p", fi_base);

    // Once the initial offsets have been located
    // the real fun starts :D
    // Let's go through the file and add everything
    // that we plan on loading to a linked list.
    i= 0;
    tmp = vp_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        if(*tmp == 'v' && isspace(*(tmp + 1))) {
            cvert->pos = tmp + 2;
            cvert->next = malloc(sizeof(obj_vertex));
            cvert = cvert->next;
            i++;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }
    msg_log(LOG_INFO, "%d Vertices", i);
    obj->vert_count = i;

    tmp = vt_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        if(*tmp == 'v' && *(tmp + 1) == 't') {
            cvert->tex = tmp;
            cvert = cvert->next;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }

    tmp = vn_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        if(*tmp == 'v' && *(tmp + 1) == 'n') {
            cvert->norm = tmp;
            cvert = cvert->next;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }

    i = 0;
    tmp = fi_base;
    cvert = NULL;
    cface = &obj->faces;
    while(tmp < (char*)eof) {
        if(*tmp == 'f' && isspace(*(tmp + 1))) {
            cface->face = tmp;
            cface->next = malloc(sizeof(obj_face));
            cface = cface->next;
            i++;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }
    msg_log(LOG_INFO, "%d Faces", i);
    obj->face_count = i;
}

void free_obj(obj_file* obj) {
    obj_vertex* vert_root = &obj->verts;
    obj_face*  face_root = &obj->faces;
    obj_vertex* cvert;
    obj_face*  cface;

    while(vert_root->next != NULL) {
        cvert = vert_root;
        obj_vertex* tmp;
        while(cvert->next != NULL) {
            tmp = cvert;
            cvert = cvert->next;
        }
        free(cvert);
        tmp->next = NULL;
    }

    while(face_root->next != NULL) {
        cface = face_root;
        obj_face* tmp;
        while(cface->next != NULL) {
            tmp = cface;
            cface = cface->next;
        }
        free(cface);
        tmp->next = NULL;
    }
}
