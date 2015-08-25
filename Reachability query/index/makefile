CC	= g++
CPPFLAGS= -Wno-deprecated -O2 -c
LDFLAGS	= -O2
SOURCES	= indexing.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=index

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(Include) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	-rm -f *.o