#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while(fgets(buf, MAXLINE, stdin) != NULL)
    {   
		//change '\n' to '\0'
        buf[strlen(buf) - 1] = '\0';
     
        if((pid = fork()) < 0)
            err_sys("fork error");
		
        if(pid == 0)//child
        {
            char *argv[50];//to store the address of each arguement
            int i = 0; 
            int index = 1;
            int len = strlen(buf);
            //delete the 'space' ahead of the cmd
            while(buf[i] == ' ')
                i++;
            argv[0] = &buf[i];
            //store the address of each string in argv[]
            for(; i < len; i++)
            {
                if(buf[i] == ' ')
                {
                    buf[i] = 0;
                    if(buf[i+1] != ' ') //skip the 'space'
                        argv[index++] = &buf[i+1];
                }
            }
            //the last pointer must be NULL
            argv[index] = NULL;
			
			//acquire Path Enviorment
			char *path_env = getenv("PATH");
			if(NULL == path_env)
			{
				err_sys("couldn't get PATH");
				exit(0);
			}
			else if(NULL != strstr(argv[0], "/"))//absolute path
			{
				execve(argv[0], argv, NULL);
			}
			else//relative path
			{
				char *path_item;
				char path[100];
				path_item = strtok(path_env, ":");
				while(NULL != path_item)
				{
					strcpy(path, path_item);
					strcat(path, "/");
					strcat(path, argv[0]);// merge 
					execve(path, argv, NULL);
					path_item = strtok(NULL, ":");//next path
				}
			}
			//if succeed above, the code below will not work
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }
		
        //parent
        if((pid = waitpid(pid, &status, 0)) < 0)
        {
            err_sys("waitpid error");
        }
        //next round
        printf("%% ");
    }
    exit(0);
}
