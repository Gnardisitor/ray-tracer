CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c99
OBJ = src/main.o src/ray.o src/vec3.o src/color.o src/hittable.o src/sphere.o

ray-tracer: $(OBJ)
	$(CC) $(CFLAGS) -lm -o ray-tracer $(OBJ)

main.o: src/main.c src/main.h src/vec3.h src/color.h src/ray.h src/hittable.h src/sphere.h
	$(CC) $(CFLAGS) -c main.c

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: ray-tracer
	./ray-tracer

clean:
	rm -f ray-tracer main.o src/*.o image.ppm
