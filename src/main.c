#include <GL/gl.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct
{
	Display *display;
	Window rootWindow;
	Window window;
	XEvent event;
	int screen;
	int running;
} App;

App *create_app(int width, int height)
{
	App *app = NULL;
	app = calloc(1, sizeof(App));
	if (app == NULL)
	{
		fprintf(stderr, "Allocating App struct failed.");
		exit(1);
	}

	app->display = XOpenDisplay(NULL);
	if (app->display == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}

	app->screen = DefaultScreen(app->display);
	app->rootWindow = RootWindow(app->display, app->screen);
	app->window = XCreateSimpleWindow(app->display, app->rootWindow, 10, 10, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
	XSelectInput(app->display, app->window, ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask);
	XMapWindow(app->display, app->window);
	app->running = 1;
	return app;
}

void draw_string(App *app, int x, int y, char *msg)
{
	XDrawString(app->display, app->window, DefaultGC(app->display, app->screen), x, y, msg, strlen(msg));
}

void handle_app_event(App *app, char *msg)
{
	XNextEvent(app->display, &app->event);
	if (app->event.type == Expose) 
	{
		for (int i = 0; i < 10; i++)
			draw_string(app, 10, i * 10, msg);
	}
	if (app->event.type == KeyPress)
	{
		KeySym key = XLookupKeysym(&app->event.xkey, 0);
		if (key != 0) // I dont know why in tutorial used a NULL if KeySym is just unsigned long.
			printf("Keyboard event.\n");
	}
	if (app->event.type == ButtonPressMask || app->event.type == PointerMotionMask)
		printf("Mouse event.\n");
}

void close_app(App *app)
{
	XCloseDisplay(app->display);
	free(app);
}

int main(void)
{
	char *msg = "Hello, boomer!";

	App *app = create_app(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (app->running == 1) 
		handle_app_event(app, msg);
		

	close_app(app);
	return 0;
}
