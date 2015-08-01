
#include "list.h"

#ifndef __FD_LIST
#define __FD_LIST

struct list_head head;
extern struct list_head *iterator;
typedef struct reg_fd_list{
        int fd;
        int socketfd;
        struct list_head node;
} reg_fd_list_t;

extern void list_fd_init();
extern void add_fd(int fd,int socket_fd);
extern int delete_fd(int fd);
extern int fd_test(int files);

extern void skimfd();
extern void add_fd_by_variable(int fd,int socket_fd);
extern void add_fd_by_pointer(int fd,int socket_fd);

extern ssize_t write_securely(int fildes, const void *buf, size_t nbyte);



#endif