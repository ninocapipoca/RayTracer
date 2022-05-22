vpath %.h include
vpath %.cpp src
vpath %.o obj

# Source files
SRCS := $(wildcard src/*.cpp)
# Headers
HEADERS := $(wildcard include/*.hpp)
# Object files once theyre compiled
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

# Compiler and flags
CXX := g++
CFLAGS := --std=c++17 -O3 -funroll-loops

# Rules
all: exec

debug: CFLAGS := --std=c++17 -g -lpthread
debug: clean exec

profile: CFLAGS := --std=c++17 -pg
profile: clean exec
	
verbose: CFLAGS := -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
				   -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs \
				   -Wnoexcept -Woverloaded-virtual -Wredundant-decls \
				   -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=3 \
				   -Wswitch-default -Wundef -Werror -Wno-unused
verbose: clean exec


obj/%.o: %.cpp ${HEADERS}
	$(CXX) $(CFLAGS) -c -o $@ $<

exec: ${OBJS} ${HEADERS}
	$(CXX) $(CFLAGS) -o exec main.cpp ${OBJS}

test: ${OBJS} ${HEADERS}
	$(CXX) $(CFLAGS) -o test test.cpp ${OBJS}

clean:
	rm -f ${OBJS} exec test
