#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void AskName(){
   char name[20];
   printf("Enter your name: ");
   fgets(name, sizeof(name), stdin);
   printf("Your name is: %s", name);
}

int main(void) {
      for(int i = 0; i < 4; i++) {
          pid_t pid = fork();
          if(pid == 0) {
              printf("Child process => PPID=%d, PID=%d\n", getppid(), getpid());
              AskName();
              exit(0);
          }
          else{
              printf("Parent process => PID=%d\n", getpid());
              printf("Waiting for child processes to finish...\n");
              wait(NULL);
              printf("Child process finished.\n");
          }
          printf("\n"); 
      }
      printf("Job is done.\n");
      return EXIT_SUCCESS;
}
