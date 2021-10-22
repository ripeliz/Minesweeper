#pragma once
#include <iostream>
#include "Textures.h"
#include <vector>

using namespace std;

class Board
{
public:
	struct Config {
	int columns;
	int rows;
	int mines;
	int width;
	int height;
	int tiles;
	sf::Sprite face;
	int flagCount;
	int clickedCount;
	bool debug = false;
	};

	struct Tile{
		bool flag;
		bool mine;
		bool clicked;
		int mineCount;
		bool blank; //aka not a flag and not a mine
		sf::Sprite tile;
		sf::Sprite background;
		int row;
		int column;
	};

	Board();

};

