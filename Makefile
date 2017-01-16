SRC = main.c
FILE_NAME = main
SLIBS = lib/libmyhtml_static.a
FLAGS = -lcurl
all:
	gcc $(SRC) -o $(FILE_NAME) $(SLIBS) $(FLAGS)