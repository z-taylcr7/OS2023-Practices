#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
    
    pid_t pid;
    // OPEN FILES
    int fd = open("test.txt" ,O_RDWR | O_CREAT |O_TRUNC);
    
    printf("fd = %d\n", fd);

    char* mmaped=NULL;
    struct stat sb;
    if (fd == -1)
    {
        /* code */
    }

   
    //write 'hello fcntl!' to file
	mmaped = (char *)mmap(NULL, sb.st_size,PROT_WRITE,MAP_SHARED, fd, 0);
    char* str="hello fcntl!";
    write(fd,str,13);
    printf("okay\n");
    
    /* code */


    // DUPLICATE FD
    int fd2;
    fd2=fcntl(fd, F_DUPFD);
    if(fd2==-1)printf("error in fcntlaa\n");
    // if(fcntl(fd, F_DUPFD, fd2)==-1)printf("error in fcntlaa\n");
    /* code */
    
    

    pid = fork();

    if(pid < 0){
        // FAILS
        printf("error in fork");
        return 1;
    }
    struct flock fl;

    if(pid > 0){
        // PARENT PROCESS
        //set the lock
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_END;//SEEK_CUR,SEEK_SET,SEEK_END
        fl.l_len = 1;
        fl.l_pid = getpid();


        int res=fcntl(fd2, F_SETLK, &fl);
        if(res==-1)printf("error in fcntlbb\n");
        //append 'b'
        char*st="b";
        write(fd2,st,1);
        //unlock

        
        sleep(3);
        fl.l_type = F_UNLCK;
        if(fcntl(fd2, F_SETLK, &fl)==-1)printf("error in fcntlbbb\n");
        printf("%s", mmaped); //the feedback should be 'hello fcntl!ba'
        // mine is "hello fcntl!\0ba"
        
        exit(0);

    } else {
        // CHILD PROCESS
        sleep(2);
        //get the lock
        
        if(fcntl(fd2, F_GETLK, &fl)==-1)printf("error in fcntlaba\n");
        //append 'a'
        char* st="a";
        write(fd2,st,1);

        exit(0);
    }
    close(fd);
    munmap(mmaped, sb.st_size);
    return 0;
}