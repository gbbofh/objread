#include "common.h"
#include "objread.h"
#include "mesh.h"

/* ========================================
 * struct file_desc
 * ========================================
 * Wraps around a file path, file descriptor,
 * file status, and the address that it is mapped to.
*/
typedef struct {
    char* path;
    int fdesc;
    struct stat fstat;
    void* addr;
} file_desc;

bool open_file(file_desc* fd);
void close_file(file_desc* fd);

int main(int argc, char* argv[]) {
    open_log();

    obj_file of;
    meshdata md;

    file_desc fd;
    memset(&fd, 0, sizeof(file_desc));
    fd.path = "airboat.obj";

    if(open_file(&fd) != true) {
        fatal();
    }

    read_obj(fd.addr, (char*)fd.addr + fd.fstat.st_size, &of);
    create_mesh(&of, &md);
    delete_mesh(&md);
    free_obj(&of);

    close_file(&fd);
    close_log();

    return 0;
}

/* ========================================
 * bool open_file(file_desc* fd)
 * ========================================
 * Opens a file on the disk, and maps it to
 * memory.
*/
bool open_file(file_desc* fd) {
    fd->fdesc = open(fd->path, O_RDONLY);

    msg_log(LOG_INFO, "Opening %s...", fd->path);

    if(fd->fdesc < 0) {
        return false;
    }

    fstat(fd->fdesc, &fd->fstat);

    msg_log(LOG_INFO, "Length: %d Bytes", fd->fstat.st_size);

    fd->addr = mmap(0, fd->fstat.st_size, PROT_READ,
                    MAP_PRIVATE, fd->fdesc, 0);
    if(fd->addr == MAP_FAILED) {
        return false;
    }

    msg_log(LOG_INFO, "Mapped to 0x%p", fd->addr);

    return true;
}

/* ========================================
 * void close_file(file_desc* fd)
 * ========================================
 * Unmaps an open file from memory and closes
 * the file descriptor.
*/
void close_file(file_desc* fd) {
    msg_log(LOG_INFO, "Unmapping %s from 0x%p", fd->path, fd->addr);
    munmap(fd->addr, fd->fstat.st_size);
    close(fd->fdesc);
    memset(fd, 0, sizeof(file_desc));
}
