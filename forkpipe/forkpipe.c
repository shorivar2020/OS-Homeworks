#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#include<sys/wait.h>

int flag = 0;

void sh();
void status_pid(int status);

int main () 
{
    pid_t GEN, NSD; 
    int pipe1[2];
    int gen_status = 0;
    int nsd_status = 0;
    if (pipe(pipe1) < 0){
        return 2;
    }

    GEN = fork();
    if (GEN < 0){
        exit(2);
    }
    if (GEN == 0)
    {
        setbuf(stdout, NULL);
        close(pipe1[0]); 
        if (dup2(pipe1[1], STDOUT_FILENO) < 0){
            exit(2);
        }
        close(pipe1[1]); 
        signal(SIGTERM, sh);
        while(flag == 0)
        {
            printf("%d %d\n", rand() % 4096, rand() % 4096);
            sleep(1);
        }
        fprintf(stderr, "GEN TERMINATED\n");
        return 0;
    }
    NSD = fork();
    if (NSD < 0){
        exit(2);
    }
    if (NSD == 0)
    {
        close(pipe1[1]);
        if (dup2(pipe1[0], STDIN_FILENO) < 0){
            exit(2);
        }
        close(pipe1[0]);
        if (execl("nsd", "nsd", NULL) == -1){
            exit(2);
        }
        
    }
    if ( GEN && NSD ) 
    {
        close(pipe1[0]);
        close(pipe1[1]);
        sleep(5);
        kill(GEN, SIGTERM);
        waitpid(GEN, &gen_status, 0);
        status_pid(nsd_status);
        waitpid(NSD, &nsd_status, 0); 
        status_pid(nsd_status);
        printf("OK\n");
        exit(0);
    }
}

void sh() {
  flag = 1;
}

void status_pid(int status){
    if(WIFEXITED(status) == 0){ 
        printf("ERROR\n"); 
        exit(1); 
    } 
    else if( WEXITSTATUS(status) != 0)
    { 
        printf("ERROR\n"); 
        exit(1); 
    } 
}