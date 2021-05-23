#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h> //time(), localtime(), asctime()

#define MAX 255
#define LEN_HOSTNAME    30
#define MAX_LEN_LINE    100

void *prompt(char xBuf[])
{
    time_t rawtime;
    struct tm *timeinfo;
    char *now;
    void *ret;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    now = asctime(timeinfo);
    
    now[strlen(now)-1] = 0;
    
    printf("Time: [%s]", now);
    
    ret = fgets(xBuf, MAX, stdin);
    
    if(xBuf[strlen(xBuf)-1] == '\n')
        xBuf[strlen(xBuf)-1] = 0;
    
    return ret;
}

int main(void)
{
    char xBuf[MAX], yBuf[MAX];
    char hostname[LEN_HOSTNAME + 1];
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    memset(hostname, 0x00, sizeof(hostname));
    printf("username: %s\n", getpwuid(getuid())->pw_name);
    gethostname(hostname, LEN_HOSTNAME);
    printf("hostname: %s\n", hostname);
    getcwd(yBuf, MAX);
    printf("directoryname: %s\n", yBuf);
    
    while(prompt(xBuf))
    {
    while (true) {
        char *s;
        int len;
        char bin[50] = "/bin/";
        
        printf("%c[1;31mMyShell $ ",27);  //빨간색으로 변경
        printf("%c[1;36m",27); //밝은 파랑색으로 변경
        s = fgets(command, MAX_LEN_LINE, stdin);
        
        //exit\n과 같으면 프로그램 종료
        if (strcmp(command, "exit\n")==0)
        {
            printf("%c[1;32m",27); // 녹색으로 변경, 27=아스키코드 ESC
            printf("Exit MyShell\n");
            printf("%c[0m\n",27); //원래의 색()으로 변경
            break;
        }
        
        
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
      
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            strcat(bin, command); // /bin/command로 문자열 합침
            ret = execve(bin, args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    exit(0);
    }
    return 0;
}
