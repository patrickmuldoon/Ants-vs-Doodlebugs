/*
Author: Patrick Muldoon
Date: 12/9/2016
Time Spent: 15 hours
Purpose: This program creates a 2d prey simulation where there are two species,
ants and doodlbebugs. These are of type organism which is a child of Tile. Tile
has two children, organism (ants and doodblebugs) and empty. The doodlbebugs try
to eat the ants and if they do not eat in 3 turns, they die off. Ants try to survive
and if they do for 3 turns they breed. Doodlebugs can also breed, but they must survive
8 turns in able to do that.
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

void gotoxy(int x, int y);
const int ANT_BREED = 3;
const int STARVE = 3;
const int DOODBLEBUG_BREED = 8;
class World;

enum Direction { UP, DOWN, LEFT, RIGHT, NUMBER_OF_DIRECTIONS };
enum Tile_type { EMPTY, DOODLEBUG, ANT, NUMBER_OF_TILE_TYPES };

struct Position
{
Position(int a, int b) : x(a), y(b) {}
  int x;
  int y;
};

class Tile;

typedef Tile* Tile_ptr;

class Tile
{
public:
  Tile() :position(0, 0) {}
  Tile(int a, int b) : position(a, b) {}
  Position get_position() { return position; }
  void set_position(Position pos) { position = pos; }
  virtual void draw() = 0;
  virtual void breed(World& w) {}
  void position_swap(Tile& other);
  virtual void move(World& w) = 0;
  virtual Tile_type who() = 0;
  bool moved = false;
private:
  Position position;

};





class Organism : public Tile
{
public:
  Organism() {}
  Organism(int a, int b) : Tile(a, b) {}
  virtual void move(World& w) {
    cout << "moving";
  }
protected:
  int life = 0;
  int starve_counter = 0;
private:
};

class Empty : public Tile
{
public:
  Empty() {}
  Empty(int a, int b) : Tile(a, b) {}
  virtual void draw();
  virtual void move(World& w) {}
  virtual Tile_type who() { return EMPTY; }
private:
};

class Doodlebug : public Organism
{
public:
  Doodlebug() { life = 0; starve_counter = 0; moved = false; }
  Doodlebug(int a, int b) : Organism(a, b) {}
  virtual void draw();
  virtual void move(World& w);
  virtual void breed(World& w);
  bool starve();
  virtual Tile_type who() { return DOODLEBUG; }
private:
};

class Ant : public Organism
{
public:
  Ant() { life = 0; moved = false; }
  Ant(int a, int b) : Organism(a, b) {}
  virtual void draw();
  virtual void move(World& w);
  virtual void breed(World& w);
  virtual Tile_type who() { return ANT; }
private:
};

class World
{
public:
  World();
  explicit World(int side);
  World(int height, int width);
  void critter_swap(Tile_ptr& first, Tile_ptr& second);
  int get_height() { return height; }
  int get_width() { return width; }
  void run();
  Tile_ptr* operator[](int index);
private:
  int width;
  int height;
  void shuffle();

public:
  Tile_ptr** grid;
};

int main()
{
  World world;

  for (;;)
    {
        world.run();
        cout << "Press enter to continue";
        cin.get();
    }

  return 0;
}

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Empty::draw()
{
  gotoxy(get_position().x, get_position().y);
  cout << " ";
}

void Doodlebug::draw()
{
  gotoxy(get_position().x, get_position().y);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "X";
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED |
  FOREGROUND_GREEN);
}

void Ant::draw()
{
  gotoxy(get_position().x, get_position().y);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN |
  FOREGROUND_INTENSITY);
  cout << "O";
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN |
  FOREGROUND_GREEN);
}

World::World()
{
  int ants = 100;
  int doodlebugs = 5;

  width = 20;
  height = 20;
  grid = new Tile_ptr*[height];
  for (int i = 0; i < height; i++)
  {
    grid[i] = new Tile_ptr[width];
  }

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (ants > 0)
      {
        grid[i][j] = new Ant(j, i);
        ants--;
      }
      else if (doodlebugs > 0)
      {
        grid[i][j] = new Doodlebug(j, i);
        doodlebugs--;
      }
      else
      {
        grid[i][j] = new Empty(j, i);
      }
    }
  }
shuffle();

}

World::World(int side)
{

}

World::World(int height, int width)
{

}

void World::shuffle()
{
  for (int i = 0; i < 10000; i++)
  {
    int x1 = rand() % width;
    int y1 = rand() % height;
    int x2 = rand() % width;
    int y2 = rand() % height;

    critter_swap(grid[y1][x1], grid[y2][x2]);
  }

}

void Tile::position_swap(Tile& other)
{
  swap(position, other.position);
}

void World::critter_swap(Tile_ptr& first, Tile_ptr& second)
{
  first->position_swap(*second);
  swap(first, second);
}

void Ant::move(World& w)
{
  Direction direction;
  int x1, y1, x2 = -1, y2 = -1;

  direction = static_cast<Direction>(rand() % NUMBER_OF_DIRECTIONS);
  x1 = get_position().x;
  y1 = get_position().y;
  switch (direction)
  {
    case UP:
    if (y1 > 0)
    {
      if (w.grid[y1 - 1][x1]->who() == EMPTY)
      {
        x2 = x1;
        y2 = y1 - 1;
      }
    }
    break;
    case DOWN:
    if (y1 < w.get_height() - 1)
    {
      if (w.grid[y1 + 1][x1]->who() == EMPTY)
      {
        x2 = x1;
        y2 = y1 + 1;
      }
    }
    break;
    case LEFT:
    if (x1 > 0)
    {
      if (w.grid[y1][x1 - 1]->who() == EMPTY)
      {
        x2 = x1 - 1;
        y2 = y1;
      }
    }
    break;
    case RIGHT:
    if (x1 < w.get_width() - 1)
    {
      if (w.grid[y1][x1 + 1]->who() == EMPTY)
      {
        x2 = x1 + 1;
        y2 = y1;
      }
    }
    break;
  }
  if (x2 != -1)
  {
    w.critter_swap(w.grid[y1][x1], w.grid[y2][x2]);
  }
}

void Doodlebug::move(World& arg)
{
  int x1, y1, x2 = -1, y2 = -1;

  x1 = get_position().x;
  y1 = get_position().y;
//case up
  if ((y1 > 0) && (arg.grid[y1 - 1][x1]->who() == ANT)) {
    delete arg.grid[y1 - 1][x1];
    arg.grid[y1 - 1][x1] = new Empty(x1, y1 - 1);
    arg.critter_swap(arg.grid[y1][x1], arg.grid[y1 - 1][x1]);
    starve_counter = 0;
    return;
  }
//case down
  else if ((y1 < arg.get_height() - 1) && (arg.grid[y1 + 1][x1]->who() == ANT)) {
    delete arg.grid[y1 + 1][x1];
    arg.grid[y1 + 1][x1] = new Empty(x1, y1 + 1);
    arg.critter_swap(arg.grid[y1][x1], arg.grid[y1 + 1][x1]);
    starve_counter = 0;
    return;
  }
//case left
  else if ((x1 > 0) && (arg.grid[y1][x1 - 1]->who() == ANT)) {
    delete arg.grid[y1][x1 - 1];
    arg.grid[y1][x1 - 1] = new Empty(x1 - 1, y1);
    arg.critter_swap(arg.grid[y1][x1], arg.grid[y1][x1 - 1]);
    starve_counter = 0;
    return;
  }
//case right
  else if ((x1 < arg.get_width() - 1) && (arg.grid[y1][x1 + 1]->who() == ANT)) {
    delete arg.grid[y1][x1 + 1];
    arg.grid[y1][x1 + 1] = new Empty(x1 + 1, y1);
    arg.critter_swap(arg.grid[y1][x1], arg.grid[y1][x1 + 1]);
    starve_counter = 0;
    return;
  }

  int direction = rand() % 4;
  if (direction == 0) {
    if ((y1 > 0) && arg.grid[y1 - 1][x1]->who() == EMPTY) {
      x2 = x1;
      y2 = y1 - 1;
    }
  }
  else if (direction == 1) {
    if ((y1 < arg.get_height() - 1) && arg.grid[y1 + 1][x1]->who() == EMPTY) {
      x2 = x1;
      y2 = y1 + 1;
    }
  }
  else if (direction == 2) {
    if ((x1 > 0) && arg.grid[y1][x1 - 1]->who() == EMPTY) {
      x2 = x1 - 1;
      y2 = y1;
    }
  }
  else if (direction == 3) {
    if ((x1 < arg.get_width() - 1) && arg.grid[y1][x1 + 1]->who() == EMPTY) {
      x2 = x1 + 1;
      y2 = y1;
    }
  }

  if (x2 != -1) {
    arg.critter_swap(arg.grid[y1][x1], arg.grid[y2][x2]);
    starve_counter++;
  }
  if (this->starve())
  {
    delete arg.grid[y2][x2];
    arg.grid[y2][x2] = new Empty(x2, y2);
  }
}

void Ant::breed(World& w)
{
  life++;
  if (life == ANT_BREED) {
    life = 0;
    int x1, y1, x2, y2;
    x1 = get_position().x;
    y1 = get_position().y;

    if ((y1 > 0) && (w.grid[y1 - 1][x1]->who() == EMPTY)) {
      y2 = y1 - 1;
      x2 = x1;
      w.grid[y2][x2] = new Ant(x2, y2);
    }
    else if ((y1 < w.get_height() - 1) && (w.grid[y1 + 1][x1]->who() == EMPTY)) {
      y2 = y1 + 1;
      x2 = x1;
      w.grid[y2][x2] = new Ant(x2, y2);
    }
    else if ((x1 > 0) && (w.grid[y1][x1 - 1]->who() == EMPTY)) {
      y2 = y1;
      x2 = x1 - 1;
      w.grid[y2][x2] = new Ant(x2, y2);
    }
    else if ((x1 < w.get_width() - 1) && (w.grid[y1][x1 + 1]->who() == EMPTY)) {
      y2 = y1;
      x2 = x1 + 1;
      w.grid[y2][x2] = new Ant(x2, y2);
    }
  }
}

bool Doodlebug::starve()
{
  if (starve_counter == 3) {
    starve_counter = 0;
    return true;
  }
  else
  return false;
}

void Doodlebug::breed(World& w)
{
  life++;
  if (life == DOODBLEBUG_BREED) {
    life = 0;
    int x1, y1;
    x1 = get_position().x;
    y1 = get_position().y;

    if ((y1 > 0) && (w.grid[y1 - 1][x1]->who() == EMPTY)) {
      w.grid[y1 - 1][x1] = new Doodlebug(x1, y1 - 1);
    }
    else if ((y1 < w.get_height() - 1) && (w.grid[y1 + 1][x1]->who() == EMPTY)) {
      w.grid[y1 + 1][x1] = new Doodlebug(x1, y1 + 1);
    }
    else if ((x1 > 0) && (w.grid[y1][x1 - 1]->who() == EMPTY)) {
      w.grid[y1][x1 - 1] = new Doodlebug(x1 - 1, y1);
    }
    else if ((x1 < w.get_width() - 1) && (w.grid[y1][x1 + 1]->who() == EMPTY)) {
      w.grid[y1][x1 + 1] = new Doodlebug(x1 + 1, y1);
    }
  }
}

Tile_ptr* World::operator[](int index)
{
  return grid[index];
}

void World::run()
{
  for (int i = 0; i < this->get_height(); i++)
  {
    for (int j = 0; j < this->get_width(); j++)
    {
      grid[i][j]->draw();
    }
  }
  vector<int> random;
  for (int i = 0; i < 400; i++) {
    random.push_back(i);
  }

  random_shuffle(random.begin(), random.end());
  for (int i = 0; i < this->get_height(); i++)
  {
    for (int j = 0; j < this->get_width(); j++)
    {
      if (grid[i][j]->who() != EMPTY)
      {
        grid[i][j]->moved = false;
      }
    }
  }

  for (auto p = random.begin(); p != random.end(); p++)
  {
    int a = *p / 20;
    int b = *p % 20;

    if (grid[a][b]->who() != EMPTY && grid[a][b]->who() == DOODLEBUG)
    {
      if (grid[a][b]->moved == false)
      {
        grid[a][b]->moved = true;
        grid[a][b]->move(*this);
      }
    }
  }

  for (auto p = random.begin(); p != random.end(); p++)
  {
    int a = *p / 20;
    int b = *p % 20;
    if (grid[a][b]->who() != EMPTY && grid[a][b]->who() == ANT)
    {
      if (grid[a][b]->moved == false)
      {
        grid[a][b]->moved = true;
        grid[a][b]->move(*this);
      }
    }
  }

  for (int i = 0; i < this->get_height(); i++) {
    for (int j = 0; j < this->get_width(); j++) {
      if (grid[i][j]->who() != EMPTY && grid[i][j]->moved == true) {
        grid[i][j]->breed(*this);
      }
    }
  }

}
