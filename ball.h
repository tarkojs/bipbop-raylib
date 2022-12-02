#pragma once

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

struct Ball
{
	float x, y;
	float x_speed, y_speed;
	float radius;

	void Draw()
	{
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
};