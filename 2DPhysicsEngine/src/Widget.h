#ifndef WIDGET_H
#define WIDGET_H

#include "defs.h"
#include "SDL.h"

struct Widget
{
	char	name[MAX_NAME_LENGTH];
	int		x;
	int		y;
	char	label[MAX_NAME_LENGTH];
	Widget* prev;
	Widget* next;

	void InitWidets();
	Widget* CreateWidget(char* name);
	void DoWidgets();
};

#endif