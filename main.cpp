#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace sf;

#define WidthScreen 800
#define HeightScreen 700

#define PenSpeed -1.5

#define MinFruitSpeed 0.3
#define MaxFruitSpeed 0.8

class MySprite
{
private:
	float x, y;					//координаты рисунка на экране
	int x_rect, y_rect;			//координаты пр¤моугольника в картинке
	int width, height;			//width - ширина, height - высота пр¤моугольника
	float velocity;				//скорость перемещени¤
	float CurrentFrame = 0;
	String file;				//файл с рисунком
	Image image;
	Texture texture;
	Sprite sprite;				//непосредственно спрайт

public:
	MySprite(String F, int X, int Y, int W, int H)
	{
		width = W;
		height = H;
		x_rect = X;
		y_rect = Y;
		image.loadFromFile("images/" + F);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(x_rect, y_rect, width, height));
	}

	float GetX() { return x; }
	float GetY() { return y; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	Sprite GetSprite() { return sprite; }

	void SetSprite(String F, int X, int Y, int W, int H)
	{
		width = W;
		height = H;
		x_rect = X;
		y_rect = Y;
		image.loadFromFile("images/" + F);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(x_rect, y_rect, width, height));
	}

	void gotoxy(int X, int Y)
	{
		x = X;
		y = Y;
		sprite.setPosition(x, y);
	}

	void move(float dx, float dy)
	{
		x += dx;
		y += dy;

		sprite.setPosition(x, y);
	}

	void rotate(int angle)
	{
		sprite.setRotation(angle);
	}
};

int main()
{
	RenderWindow window(VideoMode(WidthScreen, HeightScreen), "PPAP");

TryAgain:
	int Score = 0, HighScore;
	char TextScore[50], EndGameText[] = { "Press Tab to restart game" };
	float velocity, velocity_bonus = 0.4;
	bool PenMove = 0;
	bool fruit = 1; // 0 - ананас, 1 - ¤блоко
	bool PositionX; //0 - права¤ сторона, 1 - лева¤ сторона
	int PositionY, GameMode = 2, Bonus = 0;
	float currentframe = 0;
	Text text, EndGame;
	Font font;
	Clock clock;
	MySprite plant("apple.png", 0, 0, 120, 120);
	MySprite plant_bonus("apple.png", 0, 0, 120, 120);
	MySprite background("background1.png", 0, 0, WidthScreen, HeightScreen);
	MySprite pen("pens.png", 405, 510, 45, 170);
	MySprite pen_bonus("pens.png", 405, 680, 45, -170);
	FILE *HighScoreFile;

	fopen_s(&HighScoreFile, "HighScore.a", "r");
	fscanf_s(HighScoreFile, "%d", &HighScore);
	fclose(HighScoreFile);

	velocity = 0.001 * (rand() % int(1001 * (MaxFruitSpeed - MinFruitSpeed))) + MinFruitSpeed;
	PositionX = rand() % 2;
	PositionY = rand() % (HeightScreen / 2);
	text.setFont(font);

	printf("v = %f\n", velocity);
	printf("fruit = %d\n", fruit);
	printf("PositionX = %d\n", PositionX);
	printf("***********************HighScore= %d\n", HighScore);

	sprintf_s(TextScore, "Score: %d\nHighScore: %d", Score, HighScore);
	font.loadFromFile("13162.otf");
	text.setString(TextScore);
	text.setCharacterSize(30);
	text.setFillColor(Color::Red);

	EndGame.setFont(font);
	EndGame.setString(EndGameText);
	EndGame.setCharacterSize(40);
	EndGame.setFillColor(Color::Red);

	background.gotoxy(0, 0);
	pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
	plant.gotoxy(PositionX * (WidthScreen - plant.GetWidth() - 1), PositionY);
	text.setPosition(0, 0);
	EndGame.setPosition(WidthScreen / 2 - 200, HeightScreen / 2);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 2000;

		if (Score >= HighScore)
			HighScore = Score;

		window.clear();
		window.draw(background.GetSprite());
		window.draw(pen.GetSprite());
		window.draw(plant.GetSprite());
		window.draw(text);

		if (GameMode == 3)
			window.draw(EndGame);

		if (GameMode == 1 || (Bonus == 1 && GameMode == 3))
		{
			window.draw(plant_bonus.GetSprite());
			window.draw(pen_bonus.GetSprite());
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) && GameMode != 3)
			PenMove = 1;

		if (Keyboard::isKeyPressed(Keyboard::Tab))
			goto TryAgain;

		if (pen.GetY() < plant.GetY() + 2 && pen.GetY() > plant.GetY() - 2 && GameMode != 1 && GameMode != 3)
			if ((pen.GetX() + pen.GetWidth() / 2) > plant.GetX() - 20 && pen.GetX() < plant.GetX() + plant.GetWidth() + 20)
			{
				Score++;
				sprintf_s(TextScore, "Score: %d\nHighScore: %d", Score, HighScore);
				text.setString(TextScore);

				if ((pen.GetX() + pen.GetWidth() / 2) > (plant.GetX() + plant.GetWidth() - 40) && (pen.GetX() + pen.GetWidth() / 2) < (plant.GetX() + plant.GetWidth() + 40))
				{
					printf("GOAL!!!\n");
					if (Bonus < 1)
						Bonus++;
					else
					{
						GameMode = 1;

						plant_bonus.SetSprite("pineapple.png", 120, 230, 110, 175);
						plant.SetSprite("apple.png", 0, 0, 120, 120);

						plant.gotoxy(1, (HeightScreen / 2) - 170);
						plant_bonus.gotoxy(WidthScreen - plant_bonus.GetWidth() - 1, (HeightScreen / 2) - 30);
						pen_bonus.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, 0);

						PenMove = 0;
						pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
					}
				}
				else
					Bonus = 0;

				if (GameMode != 1)
				{
					printf("APPLE!!!\n");
					fruit = rand() % 2;
					PositionX = rand() % 2;
					PositionY = rand() % (HeightScreen / 2);
					velocity = 0.001 * (rand() % int(1001 * (MaxFruitSpeed - MinFruitSpeed))) + MinFruitSpeed;

					PenMove = 0;
					pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());

					if (fruit)
						plant.SetSprite("apple.png", 0, 0, 120, 120);
					else
						plant.SetSprite("pineapple.png", 120, 230, 110, 175);

					plant.gotoxy(PositionX * (WidthScreen - plant.GetWidth() - 1), PositionY);

					printf("fruit = %d\n", fruit);
					printf("PositionX = %d\n", PositionX);
					printf("velocity = %f\n", velocity);

					GameMode = 2;
				}
			}
			else
			{
				GameMode = 3;

				printf("LOOSE!!!\n");

				Bonus = 0;

				fopen_s(&HighScoreFile, "HighScore.a", "w");
				fprintf_s(HighScoreFile, "%d", HighScore);
				fclose(HighScoreFile);

				if (fruit)
					plant.SetSprite("apple.png", 120, 120, 120, 120);
				else
					plant.SetSprite("pineapple.png", 235, 235, 110, 175);
			}

		switch (GameMode)
		{
		case 1:

			if (PenMove == 1)
			{
				pen.move(0, PenSpeed);
				pen_bonus.move(0, -PenSpeed);
			}

			if (pen.GetY() < plant.GetY() + 2 && pen.GetY() > plant.GetY() - 2)
				if ((pen.GetX() + pen.GetWidth() / 2) > plant_bonus.GetX() && pen.GetX() < plant_bonus.GetX() + plant_bonus.GetWidth())
				{
					if ((pen_bonus.GetX() + pen_bonus.GetWidth() / 2) > plant.GetX() && pen_bonus.GetX() < plant.GetX() + plant.GetWidth())
					{
						Score += 2;
						sprintf_s(TextScore, "Score: %d\nHighScore: %d", Score, HighScore);
						text.setString(TextScore);

						PenMove = 0;
						pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
						printf("*******PPAP\n");
						GameMode = 2;
						Bonus = 0;
						PositionX = rand() % 2;
						PositionY = rand() % (HeightScreen / 2);
						plant.gotoxy(PositionX * (WidthScreen - plant.GetWidth() - 1), PositionY);
						break;
					}
				}
				else
				{
					fopen_s(&HighScoreFile, "HighScore.a", "w");
					fprintf_s(HighScoreFile, "%d", HighScore);
					fclose(HighScoreFile);

					Bonus = 1;
					GameMode = 3;
					plant_bonus.SetSprite("pineapple.png", 235, 235, 110, 175);
					plant.SetSprite("apple.png", 120, 120, 120, 120);
					break;
				}

			if (plant.GetX() + plant.GetWidth() > WidthScreen - 1 || plant.GetX() < 0)
				velocity_bonus = -velocity_bonus;

			if (plant_bonus.GetX() + plant_bonus.GetWidth() > WidthScreen - 1 || plant_bonus.GetX() < 0)
				velocity_bonus = -velocity_bonus;

			plant.move(-velocity_bonus, 0);
			plant_bonus.move(velocity_bonus, 0);

			break;

		case 2:
			if (PenMove == 1)
				pen.move(0, PenSpeed);

			if (pen.GetY() < -pen.GetHeight())
			{
				PenMove = 0;
				pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
			}

			if (plant.GetX() + plant.GetWidth() > WidthScreen - 1 || plant.GetX() < 0)
				velocity = -velocity;

			plant.move(velocity, 0);
			break;

		case 3:
			if (Bonus != 0)
			{
				if (PenMove == 1)
				{
					pen.move(0, PenSpeed);
					pen_bonus.move(0, -PenSpeed);
				}
				if (pen.GetY() < -pen.GetHeight())
				{
					PenMove = 0;
					pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
					pen_bonus.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, 0);
				}

				plant.move(-velocity_bonus, 0);
				plant_bonus.move(velocity_bonus, 0);

				if (plant.GetX() + plant.GetWidth() > WidthScreen - 1 || plant.GetX() < 0)
					velocity_bonus = -velocity_bonus;

				if (plant_bonus.GetX() + plant_bonus.GetWidth() > WidthScreen - 1 || plant_bonus.GetX() < 0)
					velocity_bonus = -velocity_bonus;
			}
			else
			{
				if (PenMove == 1)
					pen.move(0, PenSpeed);

				if (pen.GetY() < -pen.GetHeight())
				{
					PenMove = 0;
					pen.gotoxy(WidthScreen / 2 - pen.GetWidth() / 2, HeightScreen - pen.GetHeight());
				}

				if (plant.GetX() + plant.GetWidth() > WidthScreen - 1 || plant.GetX() < 0)
					velocity = -velocity;

				plant.move(velocity, 0);
			}

			GameMode = 3;

			break;
		}
		window.display();
	}
	return 0;
}
