objects = main.o efs.o descriptor.o

CPPFLAGS += -std=c++11 -Iinclude/

mkefs: $(addprefix src/, $(objects))
	g++ $(CPPFLAGS) $(addprefix src/, $(objects)) -lfsutil -o bin/mkefs

install:
	cp bin/mkefs /bin
