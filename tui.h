#pragma once

#include <sys/ioctl.h>
#include <string>
#include <utility>
#include <termios.h>
#include <poll.h>
#include <unistd.h>

#include "model.h"
#include "ui.h"

using namespace std;

class Tui : public Ui {
private:
	struct termios settings;
	struct winsize ws;
public:
	Tui();
	~Tui() {tcsetattr(1, TCSANOW, &settings);}
	pair<int, int> Getsize() const override {return {ws.ws_row, ws.ws_col};}
	void To_xy(const Point& p) const {
		printf("\e[%d;%dH", p.x, p.y);
		fflush(stdout);
	}
	void Clear() const {printf("\e[2J");}
	void Put_c(const Point& p, char c) const {
		printf("\e[%d;%dH%c", p.x, p.y, c);
		fflush(stdout);
	}
	void Draw_boarder() const override;
	void Draw(const Rabbit& r) const override {Put_c(r.p_, '$');}
	void Draw(const Snake& s) const override;
	char Get_key() override;
	void Draw(const Game& g) const override;
};

