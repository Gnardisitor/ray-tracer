CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c99
OBJ = main.o src/ray.o src/vec3.o src/color.o

ray-tracer: $(OBJ)
	$(CC) $(CFLAGS) -lm -o ray-tracer $(OBJ)

main.o: main.c src/vec3.h src/color.h
	$(CC) $(CFLAGS) -c main.c

src/ray.o: src/ray.c src/ray.h src/vec3.h
	$(CC) $(CFLAGS) -c src/ray.c -o src/ray.o

src/vec3.o: src/vec3.c src/vec3.h
	$(CC) $(CFLAGS) -c src/vec3.c -o src/vec3.o

src/color.o: src/color.c src/color.h src/vec3.h
	$(CC) $(CFLAGS) -c src/color.c -o src/color.o

run: ray-tracer
	./ray-tracer

clean:
	rm -f ray-tracer main.o src/*.o image.ppm
