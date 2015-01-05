#include "apue.h"
#include <fcntl.h>
#include <sys/times.h>

int main(int argc, const char *argv[])
{
    if(argc < 2 || argc > 3)
    {
        printf("input error");
        exit(1);
    }

    int fd;    
    if(argc == 2){
        if((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0){
			err_sys("open error");
        }

    }
	if(argc == 3){//sync
		if(strcmp(argv[2], "sync") != 0){
			printf("input error");
			exit(1);
		}
		if((fd = open(argv[1], O_RDWR | O_CREAT | O_SYNC, FILE_MODE)) < 0){
			err_sys("open error");
		}
			
	}	

	int file_length;
	char* buf;
	
	if((file_length = lseek(STDIN_FILENO, 0, SEEK_END)) < 0){
		printf("lseek error");
	}
    printf("%d\n", file_length);

	if((lseek(STDIN_FILENO, 0, SEEK_SET)) == -1){//orient to the head
		printf("lseek error");
	}
	if((buf = (char*)malloc(sizeof(char) * file_length)) == NULL){
		printf("malloc error");
		exit(1);
	}
	if(read(STDIN_FILENO, buf, file_length) < 0){
		err_sys("read error");
	}
	
	printf("BUFFSIZE\tUser CPU(seconds)\tSystem CPU(seconds)\tClock time(seconds)\t#loops\n");
		
	struct tms tmp_start;
	struct tms tmp_end;
	float user_time = 0, sys_time = 0, clock_time = 0;
	clock_t time_start, time_end;
	int buffsize, n, loops, i;
	int ticker;
	
	for(buffsize = 1; buffsize <= 131072; buffsize *= 2)
	{
		lseek(fd, 0, SEEK_SET);
		time_start = times(&tmp_start);
//      printf("%lu\n", time_start);
		n = file_length / buffsize;
		loops = 0;
		
		for(i = 1; i <= n; i++)
		{
			loops++;
			if(write(fd, buf + (i-1) * buffsize, buffsize) != buffsize){
				err_sys("write error");
			}
		}
		if(write(fd, buf + n * buffsize, file_length % buffsize) != file_length % buffsize){
			err_sys("write error");
		}
		loops++;
		time_end = times(&tmp_end);
//      printf("%lu\n", time_end);
		ticker = sysconf(_SC_CLK_TCK);
		clock_time = (float)(time_end - time_start);
		user_time = (float)(tmp_end.tms_utime - tmp_start.tms_utime);
		sys_time = (float)(tmp_end.tms_stime - tmp_start.tms_stime);
		printf("%d\t\t%.2f\t\t\t%.2f\t\t\t%.2f\t\t\t%d\n", buffsize, user_time/ticker, sys_time/ticker, clock_time/ticker, loops);
		
	}
	

    return 0;
}
