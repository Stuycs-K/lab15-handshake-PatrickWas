#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  // just write/read a byte to/from the server and test if that works 
}
