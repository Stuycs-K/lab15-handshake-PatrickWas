#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client;
  if(mkfifo(WKP, 0666) == -1){
    perror("Error creating the MARIO WKP");
    exit(1);
  }

  from_client = open(WKP, O_RDONLY);
  if(from_client == -1){
    perror("Error opening MARIO WKP pipe");
    exit(1);
  }

  unlink(WKP);

  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();

  char buffer[256];
  if(read(from_client, buffer, sizeof(buffer)) == -1){
    perror("Error reading from client");
    exit(1);
  }

  *to_client = open(buffer, O_WRONLY);
  if(*to_client == -1){
    perror("Error opening pipe");
    exit(1);
  }

  if(write(*to_client, *SYN_ACK, 4) == -1){
    perror("Error sending SYN_ACK to client");
    exit(1);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  char buffer[256]; 

  snprintf(buffer, sizeof(buffer), "/tmp/%d", getpid());

  if(mkfifo(buffer, 0666) == -1){
    perror("Error creating client's PP");
    exit(1);
  }

  *to_server = open(WKP, O_WRONLY);
  if(*to_server == -1){
    perror("Error opening MARIO WKP");
    exit(1);
  }

  if(write(*to_server, buffer, strlen(buffer) + 1) == -1){
    perror("Error communicating with client pipe");
    exit(1);
  }

  from_server = open(buffer, O_RDONLY);
  if(from_server == -1){
    perror("Error opening client's PP for reading");
    exit(1);
  }

  if(read(from_server, buffer, sizeof(buffer)) == -1){
    perror("Error reading from server");
    exit(1);
  }

  printf("Client got handshake: %s\n", buffer);

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client;
  char buffer[256];

  if(read(from_client, buffer, sizeof(buffer)) == -1){
    perror("Error reading client PP");
    exit(1);
  }

  printf("Server got client PP: %s\n", buffer);

  to_client = open(buffer, O_WRONLY);
  if(to_client == -1){
    perror("Error opening client's PP");
    exit(1);
  }

  return to_client;
}
