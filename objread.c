#include "objread.h"
#include "common.h"

void read_obj(void* base, void* eof, obj_file* obj) {
    char*       vp_base = base;
    char*       vt_base = base;
    char*       vn_base = base;
    char*       fi_base = base;
    char*       tmp;
    obj_vertex* cvert;
    obj_index*  cind;
    int i;

    // Locate initial offsets in file
    //while(vp_base < (char*)eof && *vp_base != 'v' && !isspace(*(vp_base + 1))) vp_base++;
    //while(vt_base < (char*)eof && *vt_base != 'v' && *(vt_base + 1) != 't') vt_base++;
    //while(vn_base < (char*)eof && *vn_base != 'v' && *(vn_base + 1) != 'n') vn_base++;
    //while(fi_base < (char*)eof && *fi_base != 'f' && !isspace(*(fi_base + 1))) fi_base++;

    while(vp_base < (char*)eof) {
        if(*vp_base == 'v' && isspace(*(vp_base + 1))) {
            break;
        }
        while(*(vp_base++) != '\n' && vp_base < (char*)eof);
    }
    while(vt_base < (char*)eof) {
        if(*vt_base == 'v' && *(vt_base + 1) == 't') {
            break;
        }
        while(*(vt_base++) != '\n' && vt_base < (char*)eof);
    }
    while(vn_base < (char*)eof) {
        if(*vn_base == 'v' && *(vn_base + 1) == 'n') {
            break;
        }
        while(*(vn_base++) != '\n' && vn_base < (char*)eof);
    }
    while(fi_base < (char*)eof) {
        if(*fi_base == 'f' && isspace(*(fi_base + 1))) {
            break;
        }
        while(*(fi_base++) != '\n' && fi_base < (char*)eof);
    }

    msg_log(LOG_INFO, "Vertex Positions start at 0x%p", vp_base);
    msg_log(LOG_INFO, "Vertex UV Coords start at 0x%p", vt_base);
    msg_log(LOG_INFO, "Vertex Normals start at 0x%p", vn_base);
    msg_log(LOG_INFO, "Face Indices start at 0x%p", fi_base);

    i= 0;
    tmp = vp_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        //printf("position tmp=%p\n", tmp);
        if(*tmp == 'v' && isspace(*(tmp + 1))) {
            cvert->pos = tmp + 2;
            cvert->next = malloc(sizeof(obj_vertex));
            cvert = cvert->next;
            i++;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }
    msg_log(LOG_INFO, "%d Vertices", i);

    tmp = vt_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        //printf("texture tmp=%p\n", tmp);
        if(*tmp == 'v' && *(tmp + 1) == 't') {
            cvert->tex = tmp;
            cvert = cvert->next;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }

    tmp = vn_base;
    cvert = &obj->verts;
    while(tmp < (char*)eof) {
        //printf("normal tmp=%p\n", tmp);
        if(*tmp == 'v' && *(tmp + 1) == 'n') {
            cvert->norm = tmp;
            cvert = cvert->next;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }

    i = 0;
    tmp = fi_base;
    cvert = NULL;
    cind = &obj->inds;
    while(tmp < (char*)eof) {
        //printf("index tmp=%p\n", tmp);
        if(*tmp == 'f' && isspace(*(tmp + 1))) {
            cind->index = tmp;
            cind->next = malloc(sizeof(obj_index));
            cind = cind->next;
            i++;
        }
        while(*(tmp++) != '\n' && tmp < (char*)eof);
    }
    msg_log(LOG_INFO, "%d Indices", i);
}

void free_obj(obj_file* obj) {
    obj_vertex* vert_root = &obj->verts;
    obj_index*  ind_root = &obj->inds;
    obj_vertex* cvert;
    obj_index*  cind;

    while(vert_root->next != NULL) {
        cvert = vert_root;
        obj_vertex* tmp;
        while(cvert->next != NULL) {
            tmp = cvert;
            cvert = cvert->next;
        }
        //printf("Freeing Vertex %p\n", cvert);
        free(cvert);
        tmp->next = NULL;
    }

    while(ind_root->next != NULL) {
        cind = ind_root;
        obj_index* tmp;
        while(cind->next != NULL) {
            tmp = cind;
            cind = cind->next;
        }
        //printf("Freeing Index %p\n", cind);
        free(cind);
        tmp->next = NULL;
    }
}
