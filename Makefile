CLIENT:=Client
SERVER:=Server
BLD:=Build

$(shell  mkdir -p $(BLD))

all: client server

client: c_main.o client.o
	g++ -pthread $(BLD)/c_main.o $(BLD)/client.o -o $(BLD)/client

c_main.o: $(CLIENT)/main.cpp $(CLIENT)/Client.h
	g++ -c $(CLIENT)/main.cpp -o $(BLD)/c_main.o

client.o: $(CLIENT)/Client.cpp $(CLIENT)/Client.h
	g++ -c $(CLIENT)/Client.cpp -o $(BLD)/client.o

server: s_main.o server.o
	g++ $(BLD)/s_main.o $(BLD)/server.o -o $(BLD)/server

s_main.o: $(SERVER)/main.cpp $(SERVER)/Server.h
	g++ -c $(SERVER)/main.cpp -o $(BLD)/s_main.o

server.o: $(SERVER)/Server.cpp $(SERVER)/Server.h
	g++ -c $(SERVER)/Server.cpp -o $(BLD)/server.o

clean:
	rm -rf $(BLD)