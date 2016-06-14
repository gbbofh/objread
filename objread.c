#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

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
    //load_vertex(vstr, &vert);
    printf("[%f, %f, %f]\n", vert.position[0], vert.position[1], vert.position[2]);
    return 0;
}

bool is_eol(char c) {
    switch(c) {
        case '\n':
        case '\r':
        // case '\0':
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
    printf("%s\n", (char*)globals.file_addr);
    // TODO: Call to functions to parse obj data
    load_meshdata();

    if(globals.mesh.vert != NULL) free(globals.mesh.vert); globals.mesh.vert = NULL;
    if(globals.mesh.indx != NULL) free(globals.mesh.indx); globals.mesh.indx = NULL;
    munmap(globals.file_addr, globals.file_len);
    close(globals.file_d);
}

// TODO: Move inside of do/while to own function for code reuse.
void scan_obj() {
    char* tmp = globals.file_addr;
    do {
        if(*tmp == 'v' && isspace(*(tmp + 1))) {
            globals.mesh.num_vert++;
        } else if(*tmp == 'f' && isspace(*(tmp + 1))) {
            globals.mesh.num_indx++;
        }
        while(!is_eol(*(++tmp)) && tmp != (char*)globals.file_addr + globals.file_len);
    } while(tmp++ != globals.file_eof);
    printf("vertices:\t%d\n", globals.mesh.num_vert);
    printf("indices:\t%d\n", globals.mesh.num_indx);

    globals.mesh.vert = malloc(globals.mesh.num_vert * sizeof(vertex));
    globals.mesh.indx = malloc(globals.mesh.num_indx * sizeof(int));
}

void load_vertex(char* str, vertex* vert) {
    if(str == NULL || vert == NULL) return;
    char* tmp = str;

    for(int i = 0; i < 3; i++) {
        printf("i=%d\n", i);
        while(isspace(*tmp)) printf("whitespace: %c\n", *tmp), tmp++;
        if(isdigit(*tmp) || *tmp == '.' || *tmp == '-' || *tmp == '+') {
            printf("digit/pnct: %c\n", *tmp);
            str = tmp;
            tmp++;
            while(isdigit(*tmp)) printf("digit: %c\n", *tmp), tmp++;
            *tmp = '\0';
            vert->position[i] = atof(str);
            tmp++;
        }
    }
}

void load_meshdata() {
    scan_obj();
}
