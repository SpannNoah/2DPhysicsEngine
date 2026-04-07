#include "Widget.h"
#include "common.h"
#include "defs.h"
#include "Application.h"

static Widget widgetHead;
static Widget* widgetTail;

void Widget::InitWidets()
{
	memset(&widgetHead, 0, sizeof(Widget));
}

Widget* Widget::CreateWidget(char* name)
{
	Widget* w = new Widget();
	widgetTail->next = w;
	w->prev = widgetTail;
	widgetTail = w;

	STRCPY(w->name, name);

	return w;
}

void Widget::DoWidgets()
{
	
}
