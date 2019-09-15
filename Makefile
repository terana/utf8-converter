CXX=g++
CPPFLAGS=-g -Wall -std=c++17

SRCS=main.cpp utf8_converter.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: converter

converter: $(OBJS)
	$(CXX) $(CPPFLAGS) -o converter $(OBJS)

clean:
	rm -f $(OBJS)

