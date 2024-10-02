
FILES = ./assignment2/main.cpp ./assignment2/object.cpp ./assignment2/robot.cpp
INCLUDES = -I ./assignment2/headers -I ./assignment2/include
OPTIONS = -D GL_SILENCE_DEPRECATION -g -std=c++20 -lglut -rpath @executable_path/ -framework OpenGL

mac: ./assignment2/main.cpp
	clang++ $(OPTIONS) $(INCLUDES) $(FILES) -o main
	./main

clean:
	rm -f *.out
	rm -f *.o
	rm -f main
