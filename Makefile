# raytracer: main.cpp vec3.h
# 	g++ -o raytracer main.cpp vec3.h -I .
CFLAGS = -O
CC = g++
SRC = main.cpp vec3.h ray.h hitable.h sphere.h hitable_list.h
OBJ = $(SRC:.cpp = .o)
RayTracer: $(OBJ)
	$(CC) $(CFLAGS) -o RayTracer $(OBJ)
clean:
	rm -f core *.o 
