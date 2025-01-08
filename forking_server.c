#include "pipe_networking.h"

void signum(int signum){
  unlink(WKP);
  printf("SIGINT initiated");
  exit(0);
}

int main() {

  signal(SIGINT, signum);

  while(1){
    int to_client;
    int from_client;

    from_client = server_handshake( &to_client );
    unlink(WKP);
    
    pid_t pid = fork();
    if(pid == -1){
      printf("my fork has only 3 prongs");
      exit(1);
    }

    if(pid == 0){
      close(from_client);
      close(to_client);
      exit(0);
    } else{
      close(to_client);
      close(from_client);
      waitpid(pid, NULL, 0);
    }

  }

  return 0;
}
