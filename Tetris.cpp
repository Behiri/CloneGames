#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;
int nColor = 1;
int score = 0;
bool gameOver = false;

int tetromino[7][4] = 
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point {
	int x, y;
} a[4], b[4];

const int nW = 12;
const int nH = 18;

int nField[nH][nW] = { 0 };

float nScreenWidth = 400.f;
float nScreenRatio = 18/12;
float nS = nScreenWidth / nW;
float nScreenHeight = nS * nH;


bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= nW || a[i].y >= nH) return 0;
		else if (nField[a[i].y][a[i].x]) return 0;

	return 1;
}

void newTetromino()
{
	int n = std::rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = tetromino[n][i] % 2 + nW / 2 - 1;
		a[i].y = tetromino[n][i] / 2;
		nColor = 1 + rand() % 7;
	}

	for (int i = 0; i < 4; i++)
		if (nField[a[i].y][a[i].x]) gameOver = true;

	score += 25;
}

void lines()
{
	int k = nH - 1;
	for (int i = k; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < nW; j++)
		{
			if (nField[i][j]) count++;
			nField[k][j] = nField[i][j];
		}
		if (count < nW) k--;
	}

}

sf::Color getTileColor(int colorNum)
{
	switch (colorNum) {							
	case 1:  return sf::Color::Red;			 
	case 2:  return	sf::Color::Magenta;		 
	case 3:  return	sf::Color::Blue;		 
	case 4:  return	sf::Color::Yellow;		 
	case 5:  return	sf::Color::Green;	     
	case 6:  return	sf::Color::Cyan; 	     
	case 7:  return	sf::Color(160, 82, 45);  
	default: return sf::Color(200, 200, 200);
	}
}

int main()
{
	std::srand(std::time(0));

	sf::RenderWindow window(sf::VideoMode(nScreenWidth, nScreenHeight), "Tetris");
	
	int dx = nH;
	bool rotate = false;

	sf::Clock clock;
	float delay = 0.3f;
	float timer = 0.0f;

	newTetromino();

	while (window.isOpen())
	{
		if (!gameOver) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Right)	dx = 1;
					if (event.key.code == sf::Keyboard::Left)	dx = -1;
					if (event.key.code == sf::Keyboard::Up)	rotate = true;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	delay = 0.05f;

			/// Rotate
			if (rotate) {
				Point c = a[1]; // Center of rotation
				for (int i = 0; i < 4; i++)
				{
					Point p = a[i]; // Coordinates of The point we are rotating
					a[i].x = c.x + c.y - p.y;
					a[i].y = p.x + c.y - c.x;
				}

				if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
			}

			/// Move
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

			/// Tick
			if (timer > delay)
			{
				for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }





				if (!check()) {
					for (int i = 0; i < 4; i++) nField[b[i].y][b[i].x] = nColor;

					newTetromino();
					std::cout << score << std::endl;
				}
				timer = 0;
			}

			delay = 0.4f;
			dx = 0;
			rotate = false;
			timer += time;

			/// check lines
			lines();

			/// Draw
			window.clear(sf::Color::White);

			sf::RectangleShape shape(sf::Vector2f(nS, nS));
			shape.setOutlineThickness(-0.5f);
			shape.setOutlineColor(sf::Color::Black);

			for (int i = 0; i < nH; i++)
				for (int j = 0; j < nW; j++)
				{
					//if (nField[i][j] == 0) continue;
					shape.setFillColor(getTileColor(nField[i][j]));
					shape.setPosition(j*nS, i*nS);
					window.draw(shape);
				}

			for (int i = 0; i < 4; i++)
			{
				shape.setFillColor(getTileColor(nColor));
				shape.setPosition(a[i].x * nS, a[i].y * nS);
				window.draw(shape);
			}

			
		}
		else
		{
			sf::Font font;
			std::cout << "Game Over!\nYour Score: " << score << std::endl;
			std::cout << "Press enter to restart the game." << std::endl;
			std::cin.get();
			gameOver = false;
			for (int i = 0; i < nH; i++)
				for (int j = 0; j < nW; j++)
				{
					nField[i][j] = 0;
				}
			score = 0;


		}

		window.display();

		/// for optimization: every frame wait for 25ms
		std::this_thread::sleep_for(25ms);
	}

	return 0;
}

