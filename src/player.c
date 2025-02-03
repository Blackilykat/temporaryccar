#include "raylib.h"
#include "geometry.c"
#include <math.h>

#define CAR_WIDTH 10
#define CAR_LENGTH 15

#define ACCELERATION 5.0
#define TURNING_SPEED 180.0
#define FRICTION 1.0
#define FRICTION_SPEED_MULTIPLIER 5.0

typedef struct Velocity {
	float magnitude;
	float direction;
} Velocity;

typedef struct Car {
	Position position;
	Velocity velocity;
	float rotation;
	Color color;
} Car;

typedef struct Input {
	char forwards;
	char backwards;
	char left;
	char right;
} Input;

void paint_car(Car *car) {
	DrawRectanglePro((Rectangle){car->position.x, car->position.y, CAR_WIDTH, CAR_LENGTH}, (Vector2){CAR_WIDTH / 2.0, CAR_LENGTH / 2.0}, car->rotation + 90, car->color);
}

void tick_car(Car *car, float deltaTime) {
	if(car->velocity.magnitude > 0) {
		car->velocity.magnitude -= FRICTION * deltaTime * (1 + car->velocity.magnitude / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity.magnitude < 0.0) car->velocity.magnitude = 0.0;
	}

	if(car->velocity.magnitude < 0) {
		car->velocity.magnitude += FRICTION * deltaTime * (1 + car->velocity.magnitude / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity.magnitude > 0.0) car->velocity.magnitude = 0.0;
	}

	float angle_rad = PI * (car->rotation / 180);
	float mov_x = cos(angle_rad) * car->velocity.magnitude;
	float mov_y = sin(angle_rad) * car->velocity.magnitude;

	car->position.x += mov_x;
	car->position.y += mov_y;
}

void tick_player(Car *car, Input input, float deltaTime) {
	if(input.forwards) {
		car->velocity.magnitude += ACCELERATION * deltaTime;
	}
	if(input.backwards) {
		// TODO: different backwards speed
		car->velocity.magnitude -= ACCELERATION * deltaTime;
	}
	char rotationMultiplier = 1;
	if(car->velocity.magnitude < 0) rotationMultiplier = -1;
	if(input.right) {
		car->rotation += TURNING_SPEED * deltaTime * rotationMultiplier;
	}
	if(input.left) {
		car->rotation -= TURNING_SPEED * deltaTime * rotationMultiplier;
	}
}
