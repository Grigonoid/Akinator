#include "model.h"

bool operator==(const Point& lhs, const Point& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool operator<(const Rabbit& lhs, const Rabbit& rhs) {
	if (lhs.p_.x != rhs.p_.x)
		return lhs.p_.x < rhs.p_.x;
	else
		return lhs.p_.y < rhs.p_.y;
}

Point Random_point (pair<int, int> size) {
	int x = (rand() % (size.first - 2)) + 2;
	int y = (rand() % (size.second - 2)) + 2;
	return {x, y};
}

Snake::Snake(pair<int, int> size) {
	segments.push_back({size.first / 2, size.second / 2});
	segments.push_back({size.first / 2 - 1, size.second / 2});
	dir_ = Direction::UP;
}

void Snake::Move() {
	segments.pop_front();	
	switch(dir_) {
	case Direction::UP:
		segments.push_back({segments.back().x - 1, segments.back().y});
		break;
	case Direction::DOWN:
		segments.push_back({segments.back().x + 1, segments.back().y});
		break;
	case Direction::LEFT:
		segments.push_back({segments.back().x, segments.back().y - 1});
		break;
	case Direction::RIGHT:
		segments.push_back({segments.back().x, segments.back().y + 1});
		break;
	}
}

void Snake::Grow() {
	Point last = segments.front();
	Point prelast = *(++segments.begin());
	if (last.x == prelast.x)
		if (last.y < prelast.y)
			segments.push_front({last.x, last.y - 1});
		else
			segments.push_front({last.x, last.y + 1});
	else
		if (last.x < prelast.x)
			segments.push_front({last.x - 1, last.y});
		else
			segments.push_front({last.x + 1, last.y});
}

Game::Game(pair<int, int> size) : s_(size), size_(size) {
		srand(static_cast<unsigned int>(time(0)));
		while (rabbits.size() < n_rabbits)
			rabbits.insert(Rabbit(size)); //can spawn rabbit on snake
	}

bool Game::Check_intersect() const {
	Point head = s_.Get_segments().back();
	if (head.x == 1 || head.x == size_.first || head.y == 1 || head.y == size_.second
		|| count(s_.Get_segments().begin(), s_.Get_segments().end(), head) > 1)
		return true;
	return false;
}

bool Game::Check_rabbit_and_delete(const Snake& s) {
	for (const Rabbit& r : rabbits)
		if (s.Get_segments().back() == r.p_) {
			rabbits.erase(r);
			return true;
		}
	return false;
}

void Game::Add_rabbit() {
	Point p = Random_point(size_);
	Rabbit r(p);
	while (count(s_.Get_segments().begin(), s_.Get_segments().end(), p) > 0
		   || rabbits.count(r) > 0) {
		p = Random_point(size_);
		r.p_ = p;
	}
	rabbits.insert(r);
}

bool Game::Change() {
	if (Check_rabbit_and_delete(s_)) {
		s_.Grow();
		s_.Move();
		Add_rabbit();
	}
	else 
		s_.Move();
	if (Check_rabbit_and_delete(comp_)) {
		comp_.Grow();
		comp_.Move();
		Add_rabbit();
	}
	else 
		comp_.Move();
	if (Check_intersect())
		return true;
	return false;
}
