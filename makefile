compile: makeclient makeserver
makeclient: basic_client.o pipe_networking.o
	gcc -o progclient basic_client.o pipe_networking.o
makeserver: forking_server.o pipe_networking.o
	gcc -o progserver forking_server.o pipe_networking.o
basic_client.o: basic_client.c pipe_networking.h
	gcc -c basic_client.c
forking_server.o: forking_server.c pipe_networking.h
	gcc -c forking_server.c
pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c
client: progclient
	./progclient
server: progserver
	./progserver
#server: serverProgram
#	./server
#client: clientProgram
#	./client
#serverProgram:
clean:
	rm -f *.o
	rm -f progclient
	rm -f progserver
