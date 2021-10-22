#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector> 
#include <string>
#include <iomanip>
#include "Board.h"
#include "Textures.h"
#include "Random.h"
using namespace sf;
using namespace std;

//todo
//reveal all adjacent tiles that are NOT REVEALED NOT MINES NOT FLAGGED

//load config file
void loadConfig(string file, Board::Config& config){
    ifstream inFile(file);
    string line;

    string sColumns;
    int columns;
    string sRows;
    int rows;
    string sMines;
    int mines;
    int width;
    int height;
    int tiles;

    getline(inFile, sColumns);
    getline(inFile, sRows);
    getline(inFile, sMines);

    columns = stoi(sColumns);
    rows = stoi(sRows);
    mines = stoi(sMines);
    width = columns * 32.0f;
    height = (rows * 32.0f) + 88;
    tiles = rows * columns;

    config.columns = columns;
    config.rows = rows;
    config.mines = mines;
    config.width = width;
    config.height = height;
    config.tiles = tiles;
    config.clickedCount = 0;
    config.flagCount = 0;
    config.face.setTexture(Textures::GetTexture("face_happy"));
}

//sets numbers
void setNumbers(Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    int count = 0;
    for (int i = 0; i < config.rows; i++) { 
        for (int j = 0; j < config.columns; j++) { 
            if (theBoard.at(i).at(j).mine) {
                continue;
            }
            else if (i == 0 && j == 0) {  // top left corner
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j + 1).mine) {
                    count++;
                }
            }
            else if (i == 0 && j == config.columns - 1) { // bot left corner
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
            }
            else if (i == config.rows - 1 && j == 0) { // top right corner
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
            }
            else if (i == config.rows - 1 && j == config.columns - 1) { //bot riht corner
                if (theBoard.at(i - 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
            }
            else if (j == config.columns - 1) { // bottom edge
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j - 1).mine) {
                    count++;
                }
            }
            else if (i == 0) { // left edge
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j + 1).mine) {
                    count++;
                }
            }
            else if (j == 0) { //top edge
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
            }
            else if (i == config.rows - 1) { //right edge
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j + 1).mine) {
                    count++;
                }
            }

            else {
                if (theBoard.at(i - 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i - 1).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i).at(j + 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j - 1).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j).mine) {
                    count++;
                }
                if (theBoard.at(i + 1).at(j + 1).mine) {
                    count++;
                }
            }
            theBoard.at(i).at(j).mineCount = count;
            count = 0;
        }
    }
}

//creating a new board with random bombs
void newBoard(Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    theBoard.clear();
    config.flagCount = 0;
    config.face.setTexture(Textures::GetTexture("face_happy"));
    vector<Board::Tile> tiles;
    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            Board::Tile tile;
            tile.mine = false;
            tile.flag = false;
            tile.row = i;
            tile.column = j;
            tile.clicked = false;
            tile.mineCount = 0;
            if (tile.flag || tile.mine) {
                tile.blank = false;
            }
            else {
                tile.blank = true;
            }
            tiles.push_back(tile);
        }

        theBoard.push_back(tiles);
        tiles.clear();
    }

    int SetBombs = 0;
    int bombs = config.mines;
    for (int j = 0; j < bombs; j++) {

        int rrow = Random::Int(0, config.rows - 1);
        int rcol = Random::Int(0, config.columns - 1);

        if (!theBoard.at(rrow).at(rcol).mine) {
            theBoard.at(rrow).at(rcol).mine = true;
        }
        else {
            bombs++;
        }
    }
    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            theBoard.at(i).at(j).background.setTexture(Textures::GetTexture("tile_revealed"));
            theBoard.at(i).at(j).tile.setTexture(Textures::GetTexture("tile_hidden"));
            if (theBoard.at(i).at(j).flag || theBoard.at(i).at(j).mine) {
                theBoard.at(i).at(j).blank = false;
            }
            else {
                theBoard.at(i).at(j).blank = true;
            }
        }
    }

    setNumbers(config, theBoard);
}

//loading test boards
void newTestBoard(string file, Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    config.flagCount = 0;
    config.clickedCount = 0;
    theBoard.clear();
    config.face.setTexture(Textures::GetTexture("face_happy"));
    ifstream inFile(file);
    string line;

    vector<Board::Tile> tiles;
    for (int i = 0; i <= config.rows; i++) {
        for (int j = 0; j <= config.columns; j++) {
            char mine;
            inFile.get(mine);
            Board::Tile tile;

            if (mine == '1') {
                tile.mine = true;
            }
            else {
                tile.mine = false;
            }
            if (tile.flag || tile.mine) {
                tile.blank = false;
            }
            else {
                tile.blank = true;
            }
            tile.flag = false;
            tile.row = i;
            tile.column = j;
            tile.clicked = false;
            tile.mineCount = 0;

            tiles.push_back(tile);
        }
        theBoard.push_back(tiles);
        tiles.clear();
        tiles.shrink_to_fit();
    }

    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            theBoard.at(i).at(j).background.setTexture(Textures::GetTexture("tile_revealed"));
            theBoard.at(i).at(j).tile.setTexture(Textures::GetTexture("tile_hidden"));
        }
    }

    setNumbers(config, theBoard);
}

//draws board to window
void drawBoard(Board::Config& config, vector<vector<Board::Tile>>& theBoard, map<string, sf::Sprite>& Sprites, sf::RenderWindow& window) {
    window.clear();

    sf::Texture zero;
    zero.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
    sf::Texture one;
    one.loadFromFile("images/digits.png", sf::IntRect(21, 0, 21, 32));
    sf::Texture two;
    two.loadFromFile("images/digits.png", sf::IntRect(21 * 2, 0, 21, 32));
    sf::Texture three;
    three.loadFromFile("images/digits.png", sf::IntRect(21 * 3, 0, 21, 32));
    sf::Texture four;
    four.loadFromFile("images/digits.png", sf::IntRect(21 * 4, 0, 21, 32));
    sf::Texture five;
    five.loadFromFile("images/digits.png", sf::IntRect(21 * 5, 0, 21, 32));
    sf::Texture six;
    six.loadFromFile("images/digits.png", sf::IntRect(21 * 6, 0, 21, 32));
    sf::Texture seven;
    seven.loadFromFile("images/digits.png", sf::IntRect(21 * 7, 0, 21, 32));
    sf::Texture eight;
    eight.loadFromFile("images/digits.png", sf::IntRect(21 * 8, 0, 21, 32));
    sf::Texture nine;
    nine.loadFromFile("images/digits.png", sf::IntRect(21 * 9, 0, 21, 32));
    sf::Texture negative;
    negative.loadFromFile("images/digits.png", sf::IntRect(21 * 10, 0, 21, 32));
    map<int, sf::Texture> Textures;
    Textures.emplace(0, zero);
    Textures.emplace(1, one);
    Textures.emplace(2, two);
    Textures.emplace(3, three);
    Textures.emplace(4, four);
    Textures.emplace(5, five);
    Textures.emplace(6, six);
    Textures.emplace(7, seven);
    Textures.emplace(8, eight);
    Textures.emplace(9, nine);
    Textures.emplace(10, negative);

    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            theBoard.at(i).at(j).background.setPosition(j * 32, i * 32);
            theBoard.at(i).at(j).tile.setPosition(j * 32, i * 32);

            window.draw(theBoard.at(i).at(j).background);
            window.draw(theBoard.at(i).at(j).tile);
        }
    }

    config.face.setPosition(sf::Vector2f(config.width / 2.f, config.height - 88.f));
    window.draw(config.face);

    Sprites["Test3"].setPosition(sf::Vector2f(config.width - 64.0f, config.height - 88.f));
    window.draw(Sprites["Test3"]);
    Sprites["Test2"].setPosition(sf::Vector2f(config.width - 128.0f, config.height - 88.0f));
    window.draw(Sprites["Test2"]);
    Sprites["Test1"].setPosition(sf::Vector2f(config.width - 192.0f, config.height - 88.0f));
    window.draw(Sprites["Test1"]);

    Sprites["debug"].setPosition(sf::Vector2f(config.width - 256.0f, config.height - 88.0f));
    window.draw(Sprites["debug"]);

    int winCondition = config.mines - config.flagCount;

    int digit1 = 0;
    int digit2 = 0;
    int digit3 = 0;

    if (winCondition < 0) {
        digit1 = 10;
        winCondition = winCondition * -1;
    }
    else {
        digit1 = winCondition / 100;
    }

    if (winCondition >= 100) {

        digit1 = winCondition / 100;
        digit2 = ((winCondition / 10) % 10);
        digit3  = winCondition % 10;

    }
    else {
        digit2 = winCondition / 10;
        digit3 = winCondition % 10;

    }

    sf::Sprite one1;
    one1.setTexture(Textures[digit1]);
    sf::Sprite two2;
    two2.setTexture(Textures[digit2]);
    sf::Sprite three3;
    three3.setTexture(Textures[digit3]);

    one1.setPosition((config.columns * 1) + 21, (config.rows * 32));
    window.draw(one1);
    two2.setPosition((config.columns * 1) + 21 * 2, (config.rows * 32));
    window.draw(two2);
    three3.setPosition((config.columns * 1) + 21 * 3, (config.rows * 32));
    window.draw(three3);

}

//shows all the bombs
void displayBombs(vector<vector<Board::Tile>>& theBoard, Board::Config& config) {
    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            if (theBoard.at(i).at(j).mine) {
                theBoard.at(i).at(j).tile.setTexture(Textures::GetTexture("mine"));
                theBoard.at(i).at(j).background.setTexture(Textures::GetTexture("tile_hidden"));
            }
        }
    }
}

//hides all the bombs
void hideBombs(vector<vector<Board::Tile>>& theBoard, Board::Config& config) {
    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            if (theBoard.at(i).at(j).mine) {
                theBoard.at(i).at(j).tile.setTexture(Textures::GetTexture("tile_hidden"));
                theBoard.at(i).at(j).background.setTexture(Textures::GetTexture("tile_revealed"));
            }
        }
    }
}

//checks for win
void checkWin(Board::Config& config, vector<vector<Board::Tile>>& theBoard, map<string, sf::Sprite>& Sprites, sf::RenderWindow& window) {
    int winTiles = config.tiles - config.mines;
    if (config.clickedCount == winTiles) {
        for (int i = 0; i < config.rows; i++) {
            for (int j = 0; j < config.columns; j++) {
                if (theBoard.at(i).at(j).mine) {
                    theBoard.at(i).at(j).tile.setTexture(Textures::GetTexture("flag"));
                    theBoard.at(i).at(j).background.setTexture(Textures::GetTexture("tile_hidden"));
                }
            }
        }

        config.face.setTexture(Textures::GetTexture("face_win"));

        for (int i = 0; i < config.rows; i++) {
            for (int j = 0; j < config.columns; j++) {
                theBoard.at(i).at(j).clicked = true;
            }
        }
    }
}

// ends tha game
void endGame(Board::Config& config, vector<vector<Board::Tile>>& theBoard, map<string, sf::Sprite>& Sprites, sf::RenderWindow& window) {
    displayBombs(theBoard, config);
    config.face.setTexture(Textures::GetTexture("face_lose"));
    for (int i = 0; i < config.rows; i++) {
        for (int j = 0; j < config.columns; j++) {
            theBoard.at(i).at(j).clicked = true;
        }
    }

}

int getAdjTiles(Board::Tile& tile, Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    vector<Board::Tile> adjTiles;
    if (tile.row == 0 && tile.column == 0) {  // top left corner
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column + 1));
    }
    else if (tile.row == 0 && tile.column == config.columns - 1) { // bot left corner
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column - 1));
    }
    else if (tile.row == config.rows - 1 && tile.column == 0) { // top right corner
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column + 1));
    }
    else if (tile.row == config.rows - 1 && tile.column == config.columns - 1) { //bot riht corner
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column - 1));
    }
    else if (tile.column == config.columns - 1) { // bottom edge
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
    }
    else if (tile.row == 0) { // left edge
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
    }
    else if (tile.column == 0) { //top edge
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
    }
    else if (tile.row == config.rows - 1) { //right edge
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
    }

    else {
        adjTiles.push_back(theBoard.at(tile.row - 1).at(tile.column));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column - 1));
        adjTiles.push_back(theBoard.at(tile.row).at(tile.column + 1));
        adjTiles.push_back(theBoard.at(tile.row + 1).at(tile.column));
    }
    
        int count = 0;
    for (int i = 0; i < adjTiles.size(); i++) {
        if (adjTiles[i].mine) {
            count++;
        }
    }
    return count;
}

bool checkBounds(int x, int y, Board::Config& config) {
    if (x > config.rows || y > config.columns) {
        return false;
    }
    else {
        return true;
    }
}


void revealAdjacent(Board::Tile& tile, int x, int y, Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    // if out of bounds **or** if already revealed, return
    if (!theBoard.at(x).at(y).blank || !checkBounds(x, y, config)) {
        return;
    }

    if (theBoard.at(x).at(y).mineCount == 0) {
        theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("tile_hidden"));
        theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
    }


    
}

//shows the tile under 
void revealTile(int x, int y, Board::Config& config, vector<vector<Board::Tile>>& theBoard, map<string, sf::Sprite>& Sprites, sf::RenderWindow& window) {
    if (!theBoard.at(x).at(y).flag) { //makes sure theres no flag
        if (!theBoard.at(x).at(y).clicked) { //makes sure its not already clicked
            if (theBoard.at(x).at(y).mine) { //clicked a mine
                theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("mine"));
                theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                endGame(config, theBoard, Sprites, window);
            }
            else if (theBoard.at(x).at(y).mineCount > 0) { // clicked a number tile
                if (theBoard.at(x).at(y).mineCount == 1) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_1"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 2) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_2"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 3) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_3"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 4) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_4"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 5) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_5"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 6) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_6"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 7) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_7"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
                else if (theBoard.at(x).at(y).mineCount == 8) {
                    theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("number_8"));
                    theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                }
            }
            else { 
                
                theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("tile_hidden"));
                theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
                checkWin(config, theBoard, Sprites, window);
                
            }
            theBoard.at(x).at(y).clicked = true;
            config.clickedCount++;
        }
    }
}

//flag
void flagTile(int x, int y, Board::Config& config, vector<vector<Board::Tile>>& theBoard) {
    if (!theBoard.at(x).at(y).flag && !theBoard.at(x).at(y).clicked) {
        theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("flag"));
        theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_hidden"));
        theBoard.at(x).at(y).flag = true;
        config.flagCount++;
    }
    else if (theBoard.at(x).at(y).flag && !theBoard.at(x).at(y).clicked) {
        theBoard.at(x).at(y).tile.setTexture(Textures::GetTexture("tile_hidden"));
        theBoard.at(x).at(y).background.setTexture(Textures::GetTexture("tile_revealed"));
        theBoard.at(x).at(y).flag = false;
        config.flagCount--;

    }
}

int main()
{
    //tiles
    sf::Sprite hiddenTile(Textures::GetTexture("tile_hidden"));
    sf::Sprite revealedTile(Textures::GetTexture("tile_revealed"));
    sf::Sprite mine(Textures::GetTexture("mine"));
    sf::Sprite flag(Textures::GetTexture("flag"));
    sf::Sprite number1(Textures::GetTexture("number_1"));
    sf::Sprite number2(Textures::GetTexture("number_2"));
    sf::Sprite number3(Textures::GetTexture("number_3"));
    sf::Sprite number4(Textures::GetTexture("number_4"));
    sf::Sprite number5(Textures::GetTexture("number_5"));
    sf::Sprite number6(Textures::GetTexture("number_6"));
    sf::Sprite number7(Textures::GetTexture("number_7"));
    sf::Sprite number8(Textures::GetTexture("number_8"));

    sf::Sprite happyFace(Textures::GetTexture("face_happy"));
    sf::Sprite sadFace(Textures::GetTexture("face_lose"));
    sf::Sprite winFace(Textures::GetTexture("face_win"));

    sf::Sprite Test1(Textures::GetTexture("test_1"));
    sf::Sprite Test2(Textures::GetTexture("test_2"));
    sf::Sprite Test3(Textures::GetTexture("test_3"));

    sf::Sprite debug(Textures::GetTexture("debug"));

    map<string, sf::Sprite> Sprites;
    Sprites.emplace("happyFace", happyFace);
    Sprites.emplace("sadFace", sadFace);
    Sprites.emplace("winFace", winFace);
    Sprites.emplace("Test1", Test1);
    Sprites.emplace("Test2", Test2);
    Sprites.emplace("Test3", Test3);
    Sprites.emplace("debug", debug);

    Board board;
    Board::Config config;
    loadConfig("boards/config.cfg", config);
    sf::RenderWindow window(sf::VideoMode(config.width, config.height), "Minesweeper");

    vector<vector<Board::Tile>> theBoard;
    newBoard(config, theBoard);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int xMouse = sf::Mouse::getPosition(window).x;
                    int yMouse = sf::Mouse::getPosition(window).y;
                    int column = xMouse / 32;
                    int row = yMouse / 32;

                    auto test1Bounds = Sprites["Test1"].getGlobalBounds();
                    if (test1Bounds.contains(xMouse, yMouse)) {
                        newTestBoard("boards/testboard1.brd", config, theBoard);
                    }

                    auto test2Bounds = Sprites["Test2"].getGlobalBounds();
                    if (test2Bounds.contains(xMouse, yMouse)) {

                        newTestBoard("boards/testboard2.brd", config, theBoard);
                    }

                    auto test3Bounds = Sprites["Test3"].getGlobalBounds();
                    if (test3Bounds.contains(xMouse, yMouse)) {
                        newTestBoard("boards/testboard3.brd", config, theBoard);
                    }



                    if (((row + 1) <= config.rows) && ((column + 1) <= config.columns)) {
                        revealTile(row, column, config, theBoard, Sprites, window);
                    }

                    auto sadBounds = config.face.getGlobalBounds();
                    if (sadBounds.contains(xMouse, yMouse)) {
                        newBoard(config, theBoard);
                    }
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                int xMouse = sf::Mouse::getPosition(window).x;
                int yMouse = sf::Mouse::getPosition(window).y;
                int column = xMouse / 32;
                int row = yMouse / 32;

                auto debugBounds = Sprites["debug"].getGlobalBounds();
                if (debugBounds.contains(xMouse, yMouse)) {
                    if (config.debug == true) {
                        hideBombs(theBoard, config);
                        config.debug = false;
                    }
                    else if (config.debug == false) {
                        displayBombs(theBoard, config);
                        config.debug = true;
                    }
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                int xMouse = sf::Mouse::getPosition(window).x;
                int yMouse = sf::Mouse::getPosition(window).y;
                int column = xMouse / 32;
                int row = yMouse / 32;

                if (((row + 1) <= config.rows) && ((column + 1) <= config.columns)) {
                    flagTile(row, column, config, theBoard);
                }
            }
        }

        window.clear();
        drawBoard(config, theBoard, Sprites, window);
        window.display();
        
    }

    Textures::Clear();
    return 0;
    
}