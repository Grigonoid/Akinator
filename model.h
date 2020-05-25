#pragma once

#include <iostream>
#include <list>
#include <set>
#include <algorithm>
#include <utility> //for pair
#include <cstdlib> //for rand
#include <ctime>    //and srand

using namespace std;

enum class Direction {
	UP, DOWN, LEFT, RIGHT
};

struct Point {
	int x, y;
};

bool operator==(const Point& lhs, const Point& rhs);
Point Random_point (pair<int, int> size);

struct Rabbit {
	Rabbit(Point p) : p_(p) {}
	Rabbit(pair<int, int> size) {p_ = Random_point(size);}
	Point p_;
};

bool operator<(const Rabbit& lhs, const Rabbit& rhs); //for set

const int n_rabbits = 5;

class Snake {
public:
	Snake() {
		segments.push_back({16, 31}); //temporal solution
		segments.push_back({15, 31});
	}
	Snake(pair<int, int> size);
	const list<Point>& Get_segments() const {return segments;}
	void Move();
	void Grow();

	Direction dir_;
private:
	list<Point> segments;
}; 

class Game {
public:
	Game(pair<int, int> size);
	bool Check_rabbit_and_delete(const Snake&);
	void Add_rabbit();
	bool Check_intersect() const;
	bool Change();

	set<Rabbit> rabbits;
	Snake s_, comp_;
	pair<int, int> size_;
};
