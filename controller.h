#include "model.h"

using namespace std;

void Handle_key(Game& g, char key) {
	Direction current_dir = g.s_.dir_;
	switch(key) {
	case 'w':
		g.s_.dir_ = (current_dir == Direction::DOWN) ? current_dir : Direction::UP;
		break;
	case 's':
		g.s_.dir_ = (current_dir == Direction::UP) ? current_dir : Direction::DOWN;
		break;
	case 'a':
		g.s_.dir_ = (current_dir == Direction::RIGHT) ? current_dir : Direction::LEFT;
		break;
	case 'd':
		g.s_.dir_ = (current_dir == Direction::LEFT) ? current_dir : Direction::RIGHT;
		break;
	default:
		return;
	}
}

int Square_distance(const Point& a, const Point& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

Point Find_nearest_rabbit(const Point& a, const set<Rabbit>& rabbits) {
	Point res;
	int min_dist = 1000000000;
	for (const Rabbit& r : rabbits) {
		int sq_dist = Square_distance(a, r.p_);
		if (sq_dist < min_dist) {
			min_dist = sq_dist;
			res = r.p_;
		}
	}
	return res;
}

void Comp_control(Game& g) {
	Point head = g.comp_.Get_segments().back();
	Point aim = Find_nearest_rabbit(head, g.rabbits);
	if (aim.y != head.y)
		g.comp_.dir_ = (aim.y > head.y) ? Direction::RIGHT : Direction::LEFT;
	else g.comp_.dir_ = (aim.x > head.x) ? Direction::DOWN : Direction::UP;
}
