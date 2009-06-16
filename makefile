CC=g++ -g3 -Wall
RELEASE_DIR=MySocket_release
RELEASE_LIB=$(RELEASE_DIR)/lib
RELEASE_INCLUDE=$(RELEASE_DIR)/include/MySocket

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
	mkdir -p $(RELEASE_INCLUDE)
	mkdir -p $(RELEASE_LIB)
	cp *.h $(RELEASE_INCLUDE)
	cp libSocket.a $(RELEASE_LIB)
	
install: lib
	mkdir -p /usr/local/include/MySocket
	cp *.h /usr/local/include/MySocket
	mkdir -p /usr/local/lib/MySocket
	cp *.a /usr/local/lib/MySocket

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
