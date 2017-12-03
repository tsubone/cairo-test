/* compile with:
 * gcc $(pkg-config cairo-xlib-xrender --cflags --libs) cairo-example.c
 */

#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "npoc_meter.h"

static npoc_meter_t npoc_meter;

#if 0
static cairo_surface_t *bg_img;
static cairo_surface_t *meter_img;
static cairo_surface_t *speed_needle_img;
static cairo_surface_t *tacho_needle_img;

static cairo_t *speed_needle_ctx;
static cairo_t *tacho_needle_ctx;

static char *speed_needle_data;
static char *tacho_needle_data;
#endif

#define SPEED_POS_X 30.0
#define SPEED_POS_Y 30.0

#define TACHO_POS_X 450.0
#define TACHO_POS_Y 30.0

static int
my_composite (npoc_meter_t *npoc, cairo_t *cairo)
{
  fprintf(stderr, "composite \n");

  cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface(cairo, npoc->bg_img, 0, 0);
  cairo_paint(cairo);

  cairo_set_operator(cairo, CAIRO_OPERATOR_OVER);
  cairo_set_source_surface(cairo, npoc->meter_img, 0, 0);
  cairo_paint(cairo);

  cairo_set_source_surface(cairo, npoc->speed_needle_img, SPEED_POS_X, SPEED_POS_Y);
  cairo_paint(cairo);

  cairo_set_source_surface(cairo, npoc->tacho_needle_img, TACHO_POS_X, TACHO_POS_Y);
  cairo_paint(cairo);

  return 0;
}

static Display *dpy;
static Window win;

static cairo_surface_t*
my_create_cairo_surface (int w, int h)
{
  cairo_surface_t *csurface;

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Error: Can't open display. Is DISPLAY set?\n");
    return NULL;
  }

  win = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
                          0, 0, w, h, 0, 0, BlackPixel(dpy, 0));
  XSelectInput(dpy, win, StructureNotifyMask | ExposureMask);
  XMapWindow(dpy, win);

  csurface = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), w, h);

  return csurface;
}

static int my_count = 0;

static cairo_surface_t*
my_event_loop (npoc_meter_t *npoc, cairo_t *cairo)
{
  while (1) {
    XEvent e;
    XNextEvent(dpy, &e);
    //printf("Got event: %d\n", e.type);

    switch (e.type) {
      case MapNotify:
      case Expose:
      case ConfigureNotify:
        npoc_cairo_paint (npoc, my_count++);
	my_composite (npoc, cairo);

	if (e.type == Expose)
	  {
	    //	    fprintf (stderr, "send expose %d\n", (int)win);
	    XSendEvent(dpy, win, False, NoEventMask, &e);
	    usleep (16666); /* 16msec */
	  }
        break;
    }
  }

  return 0;
}

int main() {
  cairo_t *cairo;
  cairo_surface_t *surface;

  memset (&npoc_meter, 0, sizeof (npoc_meter));

  surface = my_create_cairo_surface (750, 400);
  cairo = cairo_create(surface);
  npoc_cairo_initialize (&npoc_meter);

  my_event_loop (&npoc_meter, cairo);

  return 0;
}
