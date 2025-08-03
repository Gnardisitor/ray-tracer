CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c99
OBJ = src/main.o src/camera.o src/object.o src/vector.o

ray-tracer: $(OBJ)
	$(CC) $(CFLAGS) -lm -o ray-tracer $(OBJ)

main.o: src/main.c src/main.h src/camera.h src/object.h src/vector.h
	$(CC) $(CFLAGS) -c main.c

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: ray-tracer
	./ray-tracer

clean:
	rm -f ray-tracer main.o src/*.o image.ppm
