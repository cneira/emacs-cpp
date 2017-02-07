# path to the emacs source dir
# (you can provide it here or on the command line)
ROOT    = /home/cnb/emacs
CC      = g++
LD      = g++
CFLAGS  = -std=c++11 -O2 -Wall -Wextra  -fpermissive
LDFLAGS = `llvm-config --cxxflags --ldflags` -lstdc++ -lclang $(CFLAGS)
TARGET=CodeComplete
CXX=g++
FLAGS=`llvm-config --cxxflags --ldflags` -lclang $(CFLAGS)

all: complete mymod.dll
lib:  $(TARGET)

complete: $(TARGET).cc
	$(CXX) -D_DEBUG -c $(TARGET).cc  $(FLAGS) 


# make shared library out of the object file
%.dll: %.o
	$(LD) $(TARGET).o  -shared $(LDFLAGS) -o $@ $<


# compile source file to object file
%.o: %.c  
	$(CC) $(CFLAGS) -I$(ROOT)/src -I./ -fPIC -c $<

$(TARGET): $(TARGET).cc
	$(CXX)  $(TARGET).cc -o $(TARGET) $(FLAGS)




