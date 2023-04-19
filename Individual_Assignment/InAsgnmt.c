#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int sig){
     printf("The process has been interupt!\n.");
}

int main(void) {
      void sigint_handler(int sig);

      int pipefds[2];
      char msg[200];
      char buffer[200];

      if(signal(SIGINT, sigint_handler)==SIG_ERR){
         perror("signal");
         exit(1);
      }

      for(int i=0;i<3;i++){
          pipe(pipefds);

          pid_t pid = fork();

          if(pid == 0) {
               close(pipefds[1]);
               read(pipefds[0], &buffer, sizeof(buffer));
               close(pipefds[0]);
               printf("Child, received message: %s", buffer);
               exit(EXIT_SUCCESS);
          }
          else{
               close(pipefds[0]);
               printf("\nParent, send message: ");
               fgets(msg, sizeof(msg), stdin);
               write(pipefds[1], &msg, sizeof(msg));
               close(pipefds[1]);
               wait(NULL);
          }
      }
      printf("\nProcess is done.\n");
      return EXIT_SUCCESS;
}
