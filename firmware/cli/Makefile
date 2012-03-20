TARGET = blipcli
CSRC = 
CPPSRC = blipcli.cpp Serial.cpp AsioSerial.cpp
LDFLAGS = -L/opt/local/lib -framework IOKit -framework CoreFoundation -lboost_system 
CPPFLAGS = -I/opt/local/include

CC = gcc
CPP = g++
REMOVE = rm -f

OBJ = $(CSRC:.c=.o) $(CPPSRC:.cpp=.o) $(ASRC:.S=.o) 

all: $(TARGET)

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $< -o $@ 

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@ 

$(TARGET): $(OBJ)
	g++ -Wall -g $(OBJ) $(LDFLAGS) -o $@

clean:
	$(REMOVE) $(TARGET)  \
	$(OBJ) $(CSRC:.c=.s) $(CSRC:.c=.d) $(CPPSRC:.cpp=.s) $(CPPSRC:.cpp=.d)
