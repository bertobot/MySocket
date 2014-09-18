CC=g++ -g3 -Wall -ffor-scope
RELEASE_DIR=MySocket_release
RELEASE_LIB=$(RELEASE_DIR)/lib
RELEASE_INCLUDE=$(RELEASE_DIR)/include/MySocket

OBJ=Socket.o \
ClientSocket.o \
ServerSocket.o \
DatagramPacket.o \
DatagramServer.o \
BufferedReader.o


LIBNAME=libSocket.a

#all: lib server client
all: lib

server: server.o $(OBJ)
	$(CC) -o server server.o -L$(DIR) -lSocket

client: client.o $(OBJ)
	$(CC) -o client client.o -L$(DIR) -lSocket

udp: udpclient.o udpserver.o $(OBJ)
	$(CC) -o udpclient udpclient.o $(OBJ) -lSocket
	$(CC) -o udpserver udpserver.o $(OBJ) -lSocket

lib: $(OBJ)
	ar rs $(LIBNAME) $(OBJ)

lib_release: lib
	mkdir -p $(RELEASE_INCLUDE)
	mkdir -p $(RELEASE_LIB)
	cp *.h $(RELEASE_INCLUDE)
	cp libSocket.a $(RELEASE_LIB)
	
install: lib
	mkdir -p /usr/local/include/MySocket
	install -m 666 *.h /usr/local/include/MySocket
	install -m 666 *.a /usr/local/lib

.cpp.o:
	$(CC) -c $?

clean:
	rm -f *.o

mrproper: clean
	rm -f $(LIBNAME) client server udpclient udpserver
	rm -f *~
