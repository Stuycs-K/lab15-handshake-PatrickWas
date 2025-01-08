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

    close(from_client);
    close(to_client);
  }

  return 0;
}
