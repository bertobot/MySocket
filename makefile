CC=g++ -g3 -Wall
DIR=`pwd`

OBJ=Socket.o \
ClientSocket.o \
ServerSocket.o

LIBNAME=libSocket.a

all: lib server client

server: server.o $(OBJ)
	$(CC) -o server server.o -L$(DIR) -lSocket

client: client.o $(OBJ)
	$(CC) -o client client.o -L$(DIR) -lSocket

lib: $(OBJ)
	ar rs $(LIBNAME) $(OBJ)

lib_release: lib
	mkdir -p MySocket/include
	mkdir -p MySocket/lib
	cp *.h MySocket/include
	cp libSocket.a MySocket/lib
	
Socket.o: Socket.cpp
	$(CC) -c Socket.cpp

ClientSocket.o: ClientSocket.cpp
	$(CC) -c ClientSocket.cpp

ServerSocket.o: ServerSocket.cpp
	$(CC) -c ServerSocket.cpp

server.o: server.cpp
	$(CC) -c server.cpp

client.o: client.cpp
	$(CC) -c client.cpp

clean:
	rm -f *.o

mrproper: clean
	rm -f $(LIBNAME) client server 
