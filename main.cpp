#include <iostream>
#include <unistd.h> //for sleep

#include "tui.h"
#include "model.h"
#include "controller.h"

int main()
{
	Tui tui;
	Game g(tui.Getsize());
	for (;;) {
		tui.Draw(g);
		char key = tui.Get_key();
		Handle_key(g, key);
		Comp_control(g);
		if (g.Change())
			break;
		usleep(12500);
	}
}
