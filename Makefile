objects = main.o efs.o descriptor.o

CPPFLAGS += -std=c++11 -lfsutil -Iinclude/

mkefs: $(addprefix src/, $(objects))
	g++ $(CPPFLAGS) $(addprefix src/, $(objects)) -o bin/mkefs
