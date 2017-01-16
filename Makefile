SRC = main.c parson/parson.c
FILE_NAME = main
SLIBS = lib/libmyhtml_static.a
FLAGS = -lcurl
all:
	gcc $(SRC) -o $(FILE_NAME) $(SLIBS) $(FLAGS)

clean:
	rm -rf *.o $(FILE_NAME)