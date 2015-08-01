#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "list.h"
#include "fd_list.h"
#include "util_by_wei.h"
//struct list_head head;
reg_fd_list_t *list_fds;
struct list_head *iterator;

void list_fd_init(){

	INIT_LIST_HEAD(&head);//初始化头部    //定义存放文件内容的链表
	list_fds = (reg_fd_list_t *)malloc(sizeof(reg_fd_list_t));
}



void add_fd(int fd,int socket_fd){
	volatile reg_fd_list_t *add ;
	add = malloc(sizeof(reg_fd_list_t));
	add->fd =fd;
	add->socketfd = socket_fd;
	reg_fd_list_t *entry;
	ULdbg();
    list_for_each(iterator,&head)//遍历  
    {
    	ULdbg();
        entry=list_entry(iterator,reg_fd_list_t,node);//读取某个值  
        if(entry->fd == fd){
           Udbg("the fd is registered!\n");
           return ;
        }
    }
    printf(" entry fd is %d,socket fd is %d\n",fd,socket_fd);
	list_add((struct list_head *)&(add->node),&head);
	unsigned char *testBuf="abcd";

    list_for_each(iterator,&head)//遍历  
    {  
        entry = list_entry(iterator,reg_fd_list_t,node);//读取某个值  
        printf("socketfd :%d\n",entry->socketfd);
        ULdbg();
        printf("head addr is %08x\n",iterator);
    }
    printf("head addr is %08x\n",&head);
	//write_securely(socket_fd,testBuf,4);
	/*sleep(1);
	//test
	sleep(2);
	write_securely(socket_fd,testBuf,4);*/
}


void skimfd(){
	reg_fd_list_t *entry;
	list_for_each(iterator,&head)//遍历  
    {  
        entry = list_entry(iterator,reg_fd_list_t,node);//读取某个值  
        printf("socketfd :%d\n",entry->socketfd);
        ULdbg();
        printf("head addr is %08x\n",iterator);
    }
    printf("head addr is %08x\n",iterator);
}

int delete_fd(int fd){
	reg_fd_list_t *entry;
    list_for_each(iterator,&head)//遍历  
    {
        entry=list_entry(iterator,reg_fd_list_t,node);//读取某个值 
        if(entry->socketfd == fd){
           Udbg("deleting..\n");
           list_del(&entry->node);
           return 1;
        }
    }
    Udbg("fd not found\n");
    return 0;
}
fd_set rfds;
fd_set exp_fds;
int fd_test(int files){
	int ret;
	//unsigned char tmp[2] = {2,2};
	FD_ZERO(&rfds);
	FD_SET(files,&rfds);
	//ret = write(files,tmp,2);
	struct timeval tv = {
		2,
		0,
	};
	ULdbg();
	ret = select(files+1,&rfds,NULL,&exp_fds,NULL);
	if(FD_ISSET(files,&exp_fds)){
		printf("invailable files\n");
	}
	Udbgv(ret);
	if(ret <0){
		printf("invailable fd, removing it...\n");
		delete_fd(files);
		Udbgv(errno);
		return 0;
	}else{
		Udbg("right fd\n",ret);
		return 1;
	}
}

ssize_t write_securely(int fildes, const void *buf, size_t nbyte){
	ssize_t  ret = write(fildes, buf, nbyte);
	if(ret < 0 ){
		delete_fd(fildes);//socket fd..
		Udbgv(errno);
	}
	return ret;
}