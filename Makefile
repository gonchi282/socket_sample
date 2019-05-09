PROGRAM = client
OBJDIR = obj
SRCDIR = src
CFLAGS = -c -Wall -g -O0 -lssl -lcrypt

$(PROGRAM): socket.o 
	$(CXX) -o  $@ -Wall -lssl -lcrypt $^

socket.o: socket.cpp
	$(CXX) -o $@ $(CFLAGS) $<

