
CC = gcc    
CXX = g++    
CFLAGS = -Wall -pipe -D_DEBUG -DDEBUG -g -O0    
RM = /bin/rm -f    
TARGET = client server 

all: $(TARGET)    

server: server.cpp    
	$(CXX) -o $@ $^ $(LDFLAGS)    


client: client.cpp   
	$(CXX) -o $@ $^ $(LDFLAGS)    

.PHONY:clean
clean:     
	${RM} $(TARGET)    

install:    
	cp -f $(TARGET) ../bin/   
