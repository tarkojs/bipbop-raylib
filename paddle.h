#pragma once

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, 430, 120, 20 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};