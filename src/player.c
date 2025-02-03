#include "raylib.h"
#include "geometry.c"

typedef struct Velocity {
	int x;
	int y;
} Velocity;

typedef struct Car {
	Position position;
	Velocity velocity;
	float rotation;
	Color color;
} Car;
