
#pragma once

#include "raylib.h"
#include "geometry.c"
#include <math.h>

#define CAR_WIDTH 10
#define CAR_LENGTH 15

#define ACCELERATION 100.0
#define TURNING_SPEED 180.0
#define FRICTION 20.0
#define FRICTION_SPEED_MULTIPLIER 75.0

typedef struct Car {
	Position position;
	float velocity;
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
	if(car->velocity > 0) {
		car->velocity -= FRICTION * deltaTime * (1 + car->velocity / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity < 0.0) car->velocity = 0.0;
	}

	if(car->velocity < 0) {
		car->velocity += FRICTION * deltaTime * (1 - car->velocity / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity > 0.0) car->velocity = 0.0;
	}

	float angle_rad = PI * (car->rotation / 180);
	float mov_x = cos(angle_rad) * car->velocity * deltaTime;
	float mov_y = sin(angle_rad) * car->velocity * deltaTime;

	car->position.x += mov_x;
	car->position.y += mov_y;
}


void tick_player(Car *car, Input input, float deltaTime) {
	if(input.forwards) {
		car->velocity += ACCELERATION * deltaTime;
	}
	if(input.backwards) {
		// TODO: different backwards speed
		car->velocity -= ACCELERATION * deltaTime;
	}
	char rotationMultiplier = 1;
	if(car->velocity < 0) rotationMultiplier = -1;
	if(input.right) {
		car->rotation += TURNING_SPEED * deltaTime * rotationMultiplier;
	}
	if(input.left) {
		car->rotation -= TURNING_SPEED * deltaTime * rotationMultiplier;
	}
}
