INCLUDE=-I. \
		-I/usr/local/include \


#-L/usr/local/lib \

LIB=-L. \
	-lpthread \
	-lgflags \
	-lglog \
#	-lgtest \

CXXFLAGS+=-Wall -Wreorder -Wswitch -Wno-multichar -Wconversion -Wpointer-arith  \
			-Wformat -pipe -Wno-invalid-offsetof $(INCLUDE) -g -rdynamic -fPIC

CC = g++ $(CXXFLAGS)
EXECUTABLES=m

all: $(EXECUTABLES) output

src/%.o:%.cpp
	$(CC) -c $^ -o $@
src/%.o:%.c
	$(CC) -c $^ -o $@
src/%o:%.cc
	$(CC) -c $^ -o $@

output : $(EXECUTABLES) 
	rm -rf output ;

clean:
	rm -rf  src/*.o $(EXECUTABLES);

m : \
	src/main.o \
	src/graph.o \
	src/shortest_path.o \

	$(CC) $^ -o $@ $(LIB)
	

