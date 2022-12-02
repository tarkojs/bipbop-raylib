#pragma once

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#include "paddle.h"
#include "ball.h"

struct Agent{

    int x, y;

    void bruteForce(float ball_x, float ball_y, float ball_speedX, float ball_speedY, float paddle_x);


};


void Agent::bruteForce(float ball_x, float ball_y, float ball_speedX, float ball_speedY, float paddle_x)
{
    if ( (ball_speedY > 0) && (ball_y > 100) ){
        paddle_x = ball_x;
    }
}



