#pragma once

#include <iostream>

#include "raylib.h"
#include "raymath.h"
#include "ball.h"


struct Bricks
{

    public:
        int brick [15][15];          
        int brick_width, brick_height; 
        int brick_cols, brick_rows;           
        int brick_center;  
        int integrity [15][15];
        int extraSpace;
        int x, y;
        
        void Draw();
        void Collide(int c, int r, int brick_center, int brick_height, int brick_width, int brick_x, int brick_y, int ball_x, int ball_y, int ball_radius, int extraSpace);
};


void Bricks::Draw()
{
    for(int i = 0; i < brick_cols; i++)
    {
        for(int j = 0; j < brick_rows; j++)
        {
            if(brick[i][j])
            {
                DrawRectangle(brick_center + (extraSpace * 2) + i * brick_width + i * extraSpace, extraSpace * 2 + j * brick_height + j * extraSpace, brick_width, brick_height, BLUE);
            }
        }
    }
	
}


void Bricks::Collide(int c, int r, int brick_center, int brick_height, int brick_width, int brick_x, int brick_y, int ball_x, int ball_y, int ball_radius, int extraSpace)
{
for(int i = 0; i < c; i++)
    {
        for(int j = 0; j < r; j++)
        {
            if(brick[i][j])
            {
                int x = brick_center + (extraSpace * 2) + i * brick_width + i * extraSpace;
                int y = extraSpace * 2 + j * brick_height + j * extraSpace;
                if(CheckCollisionCircleRec(Vector2{ (float) ball_x, (float) ball_y }, (float) ball_radius, {(float) brick_x, (float) brick_y, (float) brick_width, (float) brick_height}))
                {
                    brick[i][j] = 0;
                }
            }
        }
    }

}
