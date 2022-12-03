#include <iostream>

#include "raylib.h"
#include "raymath.h"

#include "timer.h"
#include "ball.h"
#include "paddle.h"
#include "bricks.h"
#include "AI_agent.h"

/*

SET ai_active to false if you want to control the paddle yourself.

TO DO:

1. window --> done, ball --> done, board --> done, bricks --> done

2. ball movement --> done

3. movement of board --> done

4. 1) when brick hit by ball, reduce integrity by one (full integrity is 10) --> done (currently set to 3 for testing)
   2) ball bounce according to angles --> done
   3) scoring mechanism (full lives is 3) --> done (currently set to 3 lives)
   4) timer --> done (currently set to 30 seconds)

5. make game smart --> done (please specify the ai_active boolean)

6. report up to 5 pages --> done

*/

// compiler flags:
// clang -std=c++11 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libraylib.a main.cpp -o BipBop




int main() 
{

	InitWindow(640, 480, "BipBop");
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 10;
	ball.x_speed = 250;
	ball.y_speed = 250;

	Paddle b_paddle;
	b_paddle.x = 320;
	b_paddle.y = 450;
	b_paddle.width = 120;
	b_paddle.height = 20;
	b_paddle.speed = 500;

	Bricks bricks;
	bricks.brick_width = 40;
	bricks.brick_height = 40;
	bricks.extraSpace = 10; // padding
	bricks.brick [15][15];
	bricks.integrity [15][15];
	bricks.brick_cols = GetScreenWidth() / (bricks.brick_width + bricks.extraSpace + 4);
    bricks.brick_rows = (GetScreenHeight() / 2) / (bricks.brick_height + bricks.extraSpace);
    bricks.brick_center = (GetScreenWidth() - bricks.brick_cols * bricks.brick_width - bricks.brick_cols * bricks.extraSpace + bricks.extraSpace) / 4;

	for (int i = 0; i < 15; i++){ // fill an array with brick integrity values
		for (int j = 0; j < 15; j++)
		{
        	bricks.integrity[i][j] = 3; // specify the number of "lives" each brick has
		}
	}

	Agent agent;

	const char* gameOverText = nullptr;
	const char* time_gameOverText = nullptr;
	int lives = 3; // number of lives (3 according to the instructions)
	int score = 0; // initialize the score as 0
	float ballLife = 30.0f; // how long the player will have to destroy all the bricks before losing
	bool ai_active = true; // choose whether we want to play ourselves or let the AI play


	Timer timer_ball = { 0 };
	startTimer(&timer_ball, ballLife);

	while (!WindowShouldClose())
	{

		ball.x += ball.x_speed * GetFrameTime();
		ball.y += ball.y_speed * GetFrameTime();

		//agent.bruteForce(ball.x, ball.y, ball.x_speed, ball.y_speed, b_paddle.x); NOT USED, just having it in the main loop works better.

		if (ai_active)
		{
			if (ball.y_speed > 0) // if ball is falling, change the position of the paddle
			{
				b_paddle.x = ball.x;
			}
		}

		if (ball.y < 0) // upper part of the screen
		{
			ball.y = 0;
			ball.y_speed *= -1;
		}


		if (ball.y > GetScreenHeight()) // bottom part of the screen
		{
			lives -= 1;

			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.x_speed = 250;
			ball.y_speed = 250;

		}

		if (ball.x < 0) // left part of the screen
		{
			ball.x = 0;
			ball.x_speed *= -1;
		}

		if (ball.x > GetScreenWidth()) // right part of the screen
		{
			ball.x = GetScreenWidth();
			ball.x_speed *= -1;
		}

		if (IsKeyDown(KEY_A))
		{
			b_paddle.x -= b_paddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_D))
		{
			b_paddle.x += b_paddle.speed * GetFrameTime();
		}

		// collisions::


		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, b_paddle.GetRect())) // ball & paddle collision
		{
			if (ball.y_speed > 0)
			{
				ball.y_speed *= -1.0f;
				ball.x_speed *= 1.0f;
			}
		}

		//bricks.Collide(5, 3, bricks.brick_center, bricks.brick_height, bricks.brick_width, bricks.x, bricks.y, ball.x, ball.y, ball.radius, bricks.extraSpace);

		for(int i = 0; i < bricks.brick_cols; i++) // ball & bricks collision
    	{
			for(int j = 0; j < bricks.brick_rows; j++)
			{
				if(bricks.brick[i][j])
				{
					int x = bricks.brick_center + (bricks.extraSpace * 2) + i * bricks.brick_width + i * bricks.extraSpace;
					int y = bricks.extraSpace * 2 + j * bricks.brick_height + j * bricks.extraSpace;
					if(CheckCollisionCircleRec(Vector2{ (float) ball.x, (float) ball.y }, (float) ball.radius, 
														{(float) x, (float) y, (float) bricks.brick_width, (float) bricks.brick_height}))
					{
						if (ball.y_speed < 0 && ball.x_speed < 0)
						{
							ball.y_speed *= -1.0f;
							ball.x_speed *= 1.0f; // *1.0f just in case
							bricks.integrity[i][j] -= 1;
						}
						else if (ball.y_speed < 0 && ball.x_speed > 0)
						{
							ball.y_speed *= -1.0f;
							ball.x_speed *= 1.0f;
							bricks.integrity[i][j] -= 1;
						}
						else if (ball.y_speed > 0 && ball.x_speed < 0)
						{
							ball.y_speed *= 1.0f;
							ball.x_speed *= -1.0f;
							bricks.integrity[i][j] -= 1;
						}
						else if (ball.y_speed > 0 && ball.x_speed > 0)
						{
							ball.y_speed *= 1.0f;
							ball.x_speed *= -1.0f;
							bricks.integrity[i][j] -= 1;
						}
		
						if (bricks.integrity[i][j] == 0)
						{
							bricks.brick[i][j] = 0;
							score += 1; // add to the score if a brick is destroyed
						}
                	}	
            	}
     		}
		}



		if (lives <= 0)
		{
			gameOverText = "No lives left, press R to restart..";
		}

		if (timer_ball.Lifetime <= 0)
		{
			time_gameOverText = "Ran out of time, press R to restart..";
		}


		if (gameOverText && IsKeyPressed(KEY_R))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.x_speed = 250;
			ball.y_speed = 250;
			stopTimer(&timer_ball);
			startTimer(&timer_ball, ballLife);
			gameOverText = nullptr;
		}

		if (time_gameOverText && IsKeyPressed(KEY_R))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.x_speed = 250;
			ball.y_speed = 250;
			stopTimer(&timer_ball);
			startTimer(&timer_ball, ballLife);
			time_gameOverText = nullptr;
		}

		updateTimer(&timer_ball);

		BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			b_paddle.Draw();
			bricks.Draw();
			
			if (gameOverText)
			{
				int textWidth = MeasureText(gameOverText, 30);
				DrawText(gameOverText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 30, RED);
				ball.x_speed = 0, ball.y_speed = 0;
				lives = 3;
				CloseWindow(); // remove this and the next statement to see the game over text, otherwise the game will automatically restart
				main();
			}

			if (time_gameOverText)
			{
				int textWidth = MeasureText(time_gameOverText, 30);
				DrawText(time_gameOverText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 30, RED);
				ball.x_speed = 0, ball.y_speed = 0;
				lives = 3;
				CloseWindow(); // remove this and the next statement to see the game over text, otherwise the game will automatically restart
				main();
			}

			DrawText(TextFormat("Score: %d", score), 10, 450, 30, RED);
			DrawText(TextFormat("Lives: %d", lives), 10, 420, 30, RED);
			DrawText(TextFormat("Total Time (s): %d", 30), 10, 400, 20, RED);
		

		EndDrawing();
	}


	CloseWindow();

	return 0;	
}
