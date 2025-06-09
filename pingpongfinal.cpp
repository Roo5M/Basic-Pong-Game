#include <raylib.h>
// Ping Pong Game
class Ball
{
public:
	float ballposx;
	float ballposy;
	float ballspeedx;
	int ballRadius;
	float ballsppedy;

	Ball()
	{
		ballposx = GetScreenWidth() / 2;
		ballposy = GetScreenHeight() / 2;
		ballspeedx = 7;
		ballsppedy = 8;
		ballRadius = 20;
	}


	void Reset()
	{
		ballposx = GetScreenWidth() / 2;
		ballposy = GetScreenHeight() / 2;
		ballspeedx = 7;
		ballsppedy = 8;
	}
};

class Paddles
{
public:
	float paddle1posx;
	float paddle1posy;
	float paddle1speedy;
	float paddle1width;
	float paddle1height;


	float paddle2posx;
	float paddle2posy;
	float paddle2speedy;
	float paddle2width;
	float paddle2height;

	Paddles()
	{
		paddle1posx = 10;
		paddle1posy = 290;
		paddle1width = 20;
		paddle1height = 100;
		paddle1speedy = 10;

		paddle2posx = 10;
		paddle2posy = 290;
		paddle2width = 20;
		paddle2height = 100;
		paddle2speedy = 10;
	}

	void Reset()
	{
		paddle1posx = 10;
		paddle1posy = 290;

		paddle2posx = 10;
		paddle2posy = 290;
	}
};

void Draw(Ball& ball, Paddles& paddle, int& startX, int startY, int endX, int endY)
{

	DrawLine(startX, startY, endX, endY, WHITE);

	DrawCircle(ball.ballposx, ball.ballposy, ball.ballRadius, WHITE);

	DrawRectangle(paddle.paddle1posx, paddle.paddle1posy, paddle.paddle1width, paddle.paddle1height, RED);

	DrawRectangle(GetScreenWidth() - paddle.paddle2posx - paddle.paddle2width, paddle.paddle2posy, paddle.paddle2width, paddle.paddle2height, BLUE);
}


void Move(Ball& ball, Paddles& paddle)
{
	ball.ballposx += ball.ballspeedx;
	ball.ballposy += ball.ballsppedy;

	if (IsKeyDown(KEY_W) && paddle.paddle1posy >= 0)
	{
		paddle.paddle1posy -= paddle.paddle1speedy;
	}
	if (IsKeyDown(KEY_S) && paddle.paddle1posy + paddle.paddle1height <= GetScreenHeight())
	{
		paddle.paddle1posy += paddle.paddle1speedy;
	}


	if (IsKeyDown(KEY_UP) && paddle.paddle2posy >= 0)
	{
		paddle.paddle2posy -= paddle.paddle2speedy;
	}
	if (IsKeyDown(KEY_DOWN) && paddle.paddle2posy + paddle.paddle2height <= GetScreenHeight())
	{
		paddle.paddle2posy += paddle.paddle2speedy;
	}
}

void TeamScore(Ball& ball, int& Team2, int& Team1, bool& gamePasued)
{
	if (gamePasued)
	{
		return;
	}


	if (ball.ballposx - ball.ballRadius <= 0)
	{
		ball.ballposx = GetScreenWidth() / 2;
		ball.ballposy = GetScreenHeight() / 2;
		Team2++;
	}

	if (ball.ballposx + ball.ballRadius >= GetScreenWidth())
	{
		ball.ballposx = GetScreenWidth() / 2;
		ball.ballposy = GetScreenHeight() / 2;
		Team1++;
	}
}

void checkForCollisions(Ball& ball, Paddles& paddle, Sound& bgmusic)
{
	if (ball.ballposx - ball.ballRadius <= 0 || ball.ballposx + ball.ballRadius >= GetScreenWidth())
	{
		ball.ballspeedx *= -1;
	}

	if (ball.ballposy - ball.ballRadius <= 0 || ball.ballposy + ball.ballRadius >= GetScreenHeight())
	{
		ball.ballsppedy *= -1;
	}


	if (CheckCollisionCircleRec(Vector2{ ball.ballposx, ball.ballposy }, ball.ballRadius, Rectangle{ paddle.paddle1posx, paddle.paddle1posy, paddle.paddle1width, paddle.paddle1height }))
	{
		ball.ballspeedx *= -1;
		PlaySound(bgmusic);
		ball.ballposx = paddle.paddle1posx + paddle.paddle1width + ball.ballRadius;
	}

	if (CheckCollisionCircleRec(Vector2{ ball.ballposx, ball.ballposy }, ball.ballRadius, Rectangle{ GetScreenWidth() - paddle.paddle2posx - paddle.paddle2width, paddle.paddle2posy, paddle.paddle1width, paddle.paddle2height }))
	{
		ball.ballspeedx *= -1;
		PlaySound(bgmusic);
		ball.ballposx = GetScreenWidth() - paddle.paddle2posx - paddle.paddle2width - ball.ballRadius;
	}
}

int main()
{
	InitWindow(600, 600, "Ping Pong");

	InitAudioDevice();

	Ball ball;
	Paddles paddle;

	int Team1 = 0;
	int Team2 = 0;
	int Winning_Score = 5;

	bool gamePaused = false;

	int startX = GetScreenWidth() / 2;
	int startY = 0;


	Sound bgmusic = LoadSound("sounds/ball.wav");

	int endX = startX;
	int endY = GetScreenHeight();

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		if (Team1 == Winning_Score || Team2 == Winning_Score)
		{
			gamePaused = true;
			int Text = MeasureText("Press Enter to Restart", 20);
			DrawText(TextFormat("Press Enter to Restart"), GetScreenWidth() / 2 - Text / 2, 300, 20, WHITE);

			if (IsKeyDown(KEY_ENTER))
			{
				gamePaused = false;
				Team1 = 0;
				Team2 = 0;
				ball.Reset();
				paddle.Reset();
			}
		}
		else
		{
			Draw(ball, paddle, startX, startY, endX, endY);
			Move(ball, paddle);
			checkForCollisions(ball, paddle, bgmusic);
			TeamScore(ball, Team2, Team1, gamePaused);
		}
		DrawText(TextFormat("%d", Team1), GetScreenWidth() / 4, 20, 40, RED);
		DrawText(TextFormat("%d", Team2), 3 * GetScreenWidth() / 4, 20, 40, BLUE);
		EndDrawing();
	}
	CloseAudioDevice();
	CloseWindow();
}