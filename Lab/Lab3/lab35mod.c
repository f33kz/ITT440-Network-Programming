#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void primeNum(int n){
     int flag = 0;

     if (n == 0 || n == 1){
         flag = 1;
     }
     for (int i = 2; i <= n / 2; ++i) {
         if (n % i == 0) {
            flag = 1;
            break;
         }
     }

     if (flag == 0){
        printf("%d is a prime number.\n", n);
     }
     else{
        printf("%d is not a prime number.\n", n);
     }
}

void sigint_handler(int sig){
     printf("The is special signal handler for SIGINT.");
}

int main(void) {
      void primeNum(int n);
      void sigint_handler(int sig);

      int pipefds[2];
      int num, buffer;

      pipe(pipefds);

      pid_t pid = fork(); 

      if(signal(SIGINT, sigint_handler)==SIG_ERR){
         perror("signal");
         exit(1);
      }

      if(pid == 0) {
         close(pipefds[0]);
         printf("Child, enter positive integer number: ");
         scanf("%d", &num);
         write(pipefds[1], &num, sizeof(num));
         exit(EXIT_SUCCESS);
      }
      else{
         wait(NULL);
         close(pipefds[1]);
         read(pipefds[0], &buffer, sizeof(buffer));
         close(pipefds[0]);
         printf("Parent, received number: %d\n", buffer);
         primeNum(buffer);
      }
      printf("Process is done.\n");
      return EXIT_SUCCESS;
}
