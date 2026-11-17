#include <GL/gl.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


void handle_event(Display *d, Window w, XEvent e, int s, const char *msg)
{
	XNextEvent(d, &e);
	if (e.type == Expose)
		XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, strlen(msg));
	if (e.type == KeyPress)
	{
		KeySym key = XLookupKeysym(&e.xkey, 0);
		if (&key != NULL)
			printf("Keyboard event.\n");
	}
	if (e.type == ButtonPressMask || e.type == PointerMotionMask)
	{
		printf("Mouse event.\n");
	}
}

int main(void)
{
	Display *d;
	Window w;
	Window rootWindow;
	XEvent e;
	const char *msg = "Hello, boomer!";
	int s;

	d = XOpenDisplay(NULL);
	if (d == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}

	s = DefaultScreen(d);
	rootWindow = RootWindow(d, s);
	/* 
	 *		extern Window XCreateSimpleWindow(
    			Display*								// display,
    			Window									// parent,
    			int											// x,
    			int											// y,
    			unsigned int						// width,
    			unsigned int						// height,
    			unsigned int						// border_width,
    			unsigned long						// border,
    			unsigned long						// background
				);
	 */
	w = XCreateSimpleWindow(d, rootWindow, 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask);
	XMapWindow(d, w);

	while (1)
	{
		handle_event(d, w, e, s, msg);
	}

	XCloseDisplay(d);
	return 0;
}
