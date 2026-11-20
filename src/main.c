#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct
{
	Display *display;
	Window rootWindow;
	Window window;
	XEvent event;
	int screen;
	bool running;
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
	
	// I dont know if its a good idea do this, but at the end of this method I freed this variables and it works.
	int *maj = malloc(sizeof(int));
	int *min = malloc(sizeof(int));

	if (glXQueryVersion(app->display, maj, min))
	{
		if ((*maj == 1 && *min <3) || (*maj < 1))
		{
			fprintf(stderr, "Invalid GLX version. Required >=1.3\n");
		}
		printf("GLX Version: %d.%d\n", *maj, *min);
	}

	printf("%s\n", glXQueryExtensionsString(app->display, app->screen));

	free(maj);
	free(min);
	
	app->rootWindow = RootWindow(app->display, app->screen);
	app->window = XCreateSimpleWindow(app->display, app->rootWindow, 10, 10, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
	XSelectInput(app->display, app->window, ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask);
	XMapWindow(app->display, app->window);
	app->running = true;
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
	//char *msg = "Hello, boomer!";

	//App *app = create_app(WINDOW_WIDTH, WINDOW_HEIGHT);


	// Testing config loading and generating
	printf("Generating default config\n");

	config_generate_default("config_1.txt");
	
	printf("Done generating default config:\n");
	config_print(default_config, "config_1.txt");

	printf("Loading config...\n");

	config conf = config_load("config.txt");

	config_print(conf, "config.txt");

	//while (app->running == 1) 
	//	handle_app_event(app, msg);
		
	//close_app(app);
	return 0;
}
