#include "tui.h"

Tui::Tui() {
	ioctl(0, TIOCGWINSZ, &ws);
	tcgetattr(1, &settings);
	struct termios new_settings = settings;
	//cfmakeraw(&new_settings);
	new_settings.c_lflag &= ~ ICANON;
	new_settings.c_lflag &= ~ ECHO;
	tcsetattr(1, TCSANOW, &new_settings);
}

char Tui::Get_key() {
	struct pollfd fds[1];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	int ret = poll(fds, 1, 100);
	if (ret)
		return getchar();
	else
		return 0;
}

void Tui::Draw_boarder() const {
	string vline(ws.ws_col, '#');
	string line(ws.ws_col - 2, ' ');
	string vempty = "#" + line + "#";
	printf("\e[H%s", vline.c_str());
	for (int i = 2; i < ws.ws_col; i++)
		printf("\e[%dH%s", i, vempty.c_str());
	printf("\e[%dH%s", ws.ws_row, vline.c_str());
	fflush(stdout);
}

void Tui::Draw(const Snake& s) const {
	for (const Point& p : s.Get_segments())
		Put_c(p,'+');
}

void Tui::Draw(const Game& g) const {
	Draw_boarder();
	Draw(g.s_);
	Draw(g.comp_);
	for (const Rabbit& r : g.rabbits)
		Draw(r);
	To_xy({1,1});
}
