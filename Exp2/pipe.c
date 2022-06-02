#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main( )
{
    pid_t pid1,pid2;//创建进程
	int pipe_fd[2];
	char w_buffer[100],r_buffer[100];
	int	len1,len2;
	/*创建一个管道*/
	if(pipe(pipe_fd)<0)
	{
		printf("Pipe creation error!\n");
		return -1;
	}
	/*创建子进程*/
	if((pid1=fork( ))==0)	// 子进程1执行序列
    {
        close(pipe_fd[0]);	// 子进程先关闭管道的读段
		dup2(pipe_fd[1], STDOUT_FILENO);
		printf("Child process 1 is sending message!\n");    //向管道写字符串
		exit(0);
    }
    else if(pid1>0)	// 父进程执行序列
    {
        waitpid(pid1,NULL,0);	// 等子进程1先写完
		if((pid2=fork( ))==0)	// 子进程2执行序列
		{
            close(pipe_fd[0]);	// 子进程先关闭管道的读段
			dup2(pipe_fd[1], STDOUT_FILENO);
			printf("Child process 2 is sending message!\n");
			exit(0);
		 }
		 else if(pid2>0)	// 父进程执行序列
		 {
            waitpid(pid2,NULL,0);	// 再等子进程2写完
			close(pipe_fd[1]);	// 父进程先关闭管道的写端
			/*从管道中读固定长度字节的串*/
			dup2(pipe_fd[0], 0);
			if(fgets(r_buffer, 50, stdin) != NULL)
                printf("%s\n", r_buffer);
            if(fgets(r_buffer, 50, stdin) != NULL)
                printf("%s\n", r_buffer);
			exit(0);
		}
	  }
	  return 0;
}

