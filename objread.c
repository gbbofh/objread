#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define error() while(1) {printf("Error 0x%08X: %s\n", errno, strerror(errno)), exit(EXIT_FAILURE);}
#define debug(str) printf("Debug: %s");

typedef struct obj_vertex {
    char* pos;
    char* norm;
    char* tex;
    struct obj_vertex* next;
} obj_vertex;

typedef struct {
    float position[3];
    float normal[3];
    float uv[2];
    int rgba;
} vertex;

typedef struct {
    int num_vert;
    int num_indx;
    vertex* vert;
    int* indx;
} meshdata;

struct global_data {
    int file_d;
    int file_len;
    void* file_addr;
    void* file_eof;
    meshdata mesh;
} globals;

bool is_eol(char c);

void load_vertex(char* str, vertex* vert);
void load_obj(char* path);
void scan_obj();
void load_meshdata();

int main(int argc, char* argv[]) {

    memset(&globals, 0, sizeof(struct global_data));

    if(argc < 2) {
        exit(EXIT_FAILURE);
    }

    vertex vert;
	char vstr[256];
	memcpy(vstr, "-143, 2, 3", 11);
    load_obj(argv[1]);
    load_vertex(vstr, &vert);
    printf("[%f, %f, %f]\n", vert.position[0], vert.position[1], vert.position[2]);
    return 0;
}

bool is_eol(char c) {
    switch(c) {
        case '\n':
        case '\r':
        return true;
        break;
        default:
        return false;
        break;
    }
}

void load_obj(char* path) {
    if(path == NULL) return;

    struct stat file_status;

    globals.file_d = open(path, O_RDONLY);

    if(globals.file_d < 0) {
        exit(EXIT_FAILURE);
    }

    fstat(globals.file_d, &file_status);
    globals.file_len = file_status.st_size;
    globals.file_addr = mmap(0, globals.file_len, PROT_READ,
                            MAP_PRIVATE, globals.file_d, 0);
    globals.file_eof = (char*)globals.file_addr + globals.file_len;

    if(globals.file_addr == MAP_FAILED) {
        close(globals.file_d);
        exit(EXIT_FAILURE);
    }
    
    // TODO: Call to functions to parse obj data
    load_meshdata();

    if(globals.mesh.vert != NULL) free(globals.mesh.vert); globals.mesh.vert = NULL;
    if(globals.mesh.indx != NULL) free(globals.mesh.indx); globals.mesh.indx = NULL;
    munmap(globals.file_addr, globals.file_len);
    close(globals.file_d);
}

// TODO: Move inside of do/while to own function for code reuse.
// TODO: Update to index locations of vertices into linked list.
//          Be sure to add handling for normals and texture coords.
void scan_obj() {
    obj_vertex vertex_list;
    obj_vertex* cvert = &vertex_list;
    char* tmp = globals.file_addr;
    do {
        if(*tmp == 'v' && isspace(*(tmp + 1))) {
            cvert->pos = tmp + 2;
            cvert->next = malloc(sizeof(obj_vertex));
            printf("%p\n", cvert);
            cvert = cvert->next;
            cvert->next = NULL;
            globals.mesh.num_vert++;
        } else if(*tmp == 'f' && isspace(*(tmp + 1))) {
            globals.mesh.num_indx++;
        }
        while(!is_eol(*(++tmp)) && tmp != (char*)globals.file_addr + globals.file_len);
    } while(tmp++ != globals.file_eof);

    globals.mesh.vert = malloc(globals.mesh.num_vert * sizeof(vertex));
    globals.mesh.indx = malloc(globals.mesh.num_indx * sizeof(int));
    
    // Clean up linked list to avoid memory leak.
    cvert = &vertex_list;
    obj_vertex* tvert;
    while(vertex_list.next != NULL) {
        while(cvert->next != NULL) tvert = cvert, cvert = cvert->next;
        free(cvert);
        tvert->next = NULL;
    }
}

void load_meshdata() {
    char* str = globals.file_addr;
    scan_obj();
}

/* Updated to not bork the input data
 * TODO: Need to add support for vertex normals and uv coords.
 *          Easiest way to do this may be to restructure and scan
 *          the file twice. Once to count vertices,
 *          and the second time to fill out offsets to related data.
 *          From then can fill out meshdata struct by parsing data
 *          from filled out offsets.
 * ALTERNATIVE: fill out a linked list containing offsets and expand
*           scan to include vertex normals and texture coords.
*/
void load_vertex(char* str, vertex* vert) {
    if(str == NULL || vert == NULL) return;
    char* tmp = str;
    char buf[256];
    
    for(int i = 0; i < 3; i++) {
        while(isspace(*tmp) || *tmp == ',') tmp++;
        if(isdigit(*tmp) || *tmp == '.' || *tmp == '-' || *tmp == '+') {
            str = tmp;
            tmp++;
            while(isdigit(*tmp)) tmp++;
            memset(buf, 0, 256);
            if((char*)tmp - (char*)str < 256)   memcpy(buf, str, ((char*)tmp - (char*)str));
            vert->position[i] = atof(buf);
        }
    }
}