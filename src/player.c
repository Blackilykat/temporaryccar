
#pragma once

#include "map.c"
#include "geometry.c"
#include <math.h>
#include <stdio.h>

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

Position get_collision_point(Car *car) {
	float angle_rad = PI * (car->rotation / 180);
	float mov_x = cos(angle_rad) * CAR_LENGTH / 2;
	float mov_y = sin(angle_rad) * CAR_LENGTH / 2;
	

	if(car->velocity < 0) {
		mov_x *= -1;
		mov_y *= -1;
	}

	return (Position){car->position.x + mov_x, car->position.y + mov_y};
}

Position get_reverse_collision_point(Car *car) {
	float angle_rad = PI * (car->rotation / 180);
	float mov_x = cos(angle_rad) * CAR_LENGTH / 2;
	float mov_y = sin(angle_rad) * CAR_LENGTH / 2;
	

	if(car->velocity >= 0) {
		mov_x *= -1;
		mov_y *= -1;
	}

	return (Position){car->position.x + mov_x, car->position.y + mov_y};
}

char collided(Position cpbm, Position cpam, RaceMap *map) {
	WallNode *cursor = map->firstWall;
	while(cursor != NULL) {

		// I don't even know https://stackoverflow.com/a/24392281
		float det = (cpam.x - cpbm.x) * (cursor->wall.end.y - cursor->wall.start.y) - (cursor->wall.end.x - cursor->wall.start.x) * (cpam.y - cpbm.y);
		if(det != 0) {
			float lambda = ((cursor->wall.end.y - cursor->wall.start.y) * (cursor->wall.end.x - cpbm.x) + (cursor->wall.start.x - cursor->wall.end.x) * (cursor->wall.end.y - cpbm.y)) / det;
			float gamma = ((cpbm.y - cpam.y) * (cursor->wall.end.x - cpbm.x) + (cpam.x - cpbm.x) * (cursor->wall.end.y - cpbm.y)) / det;
			if((0 <= lambda && lambda <= 1) && (0 <= gamma && gamma <= 1)) {
				return true;
				break;
			}
		}

		cursor = cursor->next;
	}
	return false;
}

void paint_car(Car *car) {
	DrawRectanglePro((Rectangle){car->position.x, car->position.y, CAR_WIDTH, CAR_LENGTH}, (Vector2){CAR_WIDTH / 2.0, CAR_LENGTH / 2.0}, car->rotation + 90, car->color);

	Position collisionPoint = get_collision_point(car);
	DrawPixel(collisionPoint.x, collisionPoint.y, RAYWHITE);
}


void tick_car(Car *car, float deltaTime, RaceMap *map) {
	if(car->velocity > 0) {
		car->velocity -= FRICTION * deltaTime * (1 + car->velocity / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity < 0.0) car->velocity = 0.0;
	}

	if(car->velocity < 0) {
		car->velocity += FRICTION * deltaTime * (1 - car->velocity / FRICTION_SPEED_MULTIPLIER);
		if(car->velocity > 0.0) car->velocity = 0.0;
	}

	Position cpbm = get_collision_point(car);

	float angle_rad = PI * (car->rotation / 180);
	float mov_x = cos(angle_rad) * car->velocity * deltaTime;
	float mov_y = sin(angle_rad) * car->velocity * deltaTime;

	car->position.x += mov_x;
	car->position.y += mov_y;

	Position cpam = get_collision_point(car);

	if(collided(cpbm, cpam, map)) {
		car->position.x -= mov_x;
		car->position.y -= mov_y;

		car->velocity *= -1;
	}
	
}


void tick_player(Car *car, Input input, float deltaTime, RaceMap *map) {
	if(input.forwards) {
		car->velocity += ACCELERATION * deltaTime;
	}
	if(input.backwards) {
		// TODO: different backwards speed
		car->velocity -= ACCELERATION * deltaTime;
	}
	char rotationMultiplier = 1;
	if(car->velocity < 0) rotationMultiplier = -1;
	float turnAmount = TURNING_SPEED * deltaTime * rotationMultiplier;
	if(input.right) {
		Position cpbm = get_collision_point(car);
		Position rcpbm = get_reverse_collision_point(car);
		car->rotation += turnAmount;
		Position cpam = get_collision_point(car);
		Position rcpam = get_reverse_collision_point(car);
		if(collided(cpbm, cpam, map) || collided(rcpbm, rcpam, map)) {
			car->rotation -= turnAmount;
		}
	}
	if(input.left) {
		Position cpbm = get_collision_point(car);
		Position rcpbm = get_reverse_collision_point(car);
		car->rotation -= turnAmount;
		Position cpam = get_collision_point(car);
		Position rcpam = get_reverse_collision_point(car);
		if(collided(cpbm, cpam, map) || collided(rcpbm, rcpam, map)) {
			car->rotation += turnAmount;
		}
	}
}

