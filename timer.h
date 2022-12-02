#pragma once

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"


typedef struct
{
    float Lifetime;
}Timer;

void startTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}

void updateTimer(Timer* timer)
{
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

bool stopTimer(Timer* timer)
{
    if (timer != NULL)
        return timer->Lifetime <= 0;

	return false;
}
