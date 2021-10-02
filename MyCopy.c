#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>


int file_block(const char *srcfile,int pNum)
{
	int sfd;
	int filesize;
	if((sfd = open(srcfile,O_RDWR)) == -1)
	{
		perror("open failed");
		exit(0);
	}
	if((filesize = lseek(sfd,0,SEEK_END)) ==-1)
	{
		perror("lseek failed");
		exit(0);
	}
	return (filesize/pNum==0) ? filesize/pNum : filesize/pNum + 1;
}

void process_create(const char*srcfile,const char*desfile,int prNum,int blocksize)
{
	pid_t pid;
	int i;
	for(i=0; i<prNum; i++)
	{
		pid = fork();
		if(pid==0)
			break;
	}
	if(pid>0)
	{
		printf("parent process\n");
	}
	else if(pid==0)
	{
		int pos=i*blocksize;
		char strpos[20];
		char strsize[20];
		bzero(strpos,sizeof(strpos));
		bzero(strsize,sizeof(strsize));
		sprintf(strpos,"%d",pos);
		sprintf(strsize,"%d",blocksize);

		printf("process %d, startpos %d, blocksize %d\n",getpid(),pos,blocksize);

		if(execl("/home/colin/1024周末班/process/process_copy/pcopy","pcopy",srcfile,desfile,strsize,strpos,NULL)==-1)
		{
			perror("failed");
			exit(0);
		}
	}
	else
	{
		perror("create failed");
		exit(0);
	}
}
int main(int argc,char **argv)
{
	int process_num=5;
	int blocksize;
	if(argc<3)
	{
		printf("few parameters\n");
		exit(0);
	}
	if((access(argv[1],F_OK)!=0))
	{
		printf("file no exits\n");
		exit(0);
	}
	if(argv[3]!=0)
	{
		process_num = atoi(argv[3]);
		if(process_num<0 || process_num>100)
		{
			printf("wrong number of processes\n");
			exit(0);
		}
	}
	blocksize=file_block(argv[1],process_num);
	process_create(argv[1],argv[2],process_num,blocksize);

	pid_t npid;
	while((npid = waitpid(-1,NULL,WNOHANG)) !=-1)
	{
		if(npid>0)
			printf("kill process success\n");
		else
			continue;
	}

	return 0;
}

