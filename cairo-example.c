/* compile with:
 * gcc $(pkg-config cairo-xlib-xrender --cflags --libs) cairo-example.c
 */

#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static cairo_surface_t *bg_img;
static cairo_surface_t *meter_img;
static cairo_surface_t *speed_needle_img;
static cairo_surface_t *tacho_needle_img;

static cairo_t *speed_needle_ctx;
static cairo_t *tacho_needle_ctx;

static char *speed_needle_data;
static char *tacho_needle_data;

#define NEEDLE_WIDTH  300
#define NEEDLE_HEIGHT 300

static int
my_cairo_initialize (cairo_t *cairo)
{
  bg_img    = cairo_image_surface_create_from_png("meter.png");
  meter_img = cairo_image_surface_create_from_png("meter.png");

  speed_needle_data = malloc (NEEDLE_WIDTH * NEEDLE_HEIGHT * 4);

  speed_needle_img = cairo_image_surface_create_for_data (speed_needle_data,
							  CAIRO_FORMAT_ARGB32,
							  NEEDLE_WIDTH,
							  NEEDLE_HEIGHT,
							  NEEDLE_WIDTH * 4);
  speed_needle_ctx = cairo_create (speed_needle_img);

  tacho_needle_data = malloc (NEEDLE_WIDTH * NEEDLE_HEIGHT * 4);
  tacho_needle_img = cairo_image_surface_create_for_data (tacho_needle_data,
							  CAIRO_FORMAT_ARGB32,
							  NEEDLE_WIDTH,
							  NEEDLE_HEIGHT,
							  NEEDLE_WIDTH * 4);
  tacho_needle_ctx = cairo_create (tacho_needle_img);

  //  cairo_surface_destroy (meter_img);

  return 0;
}

static int my_count = 0;

static int
draw_speed_needle(cairo_t *ctx, int count)
{
  cairo_set_source_rgba (ctx, 1.0, 0, 0, 0.5);
  cairo_paint (ctx);
  
  cairo_set_line_width(ctx, 1);
  cairo_set_source_rgb(ctx, 1.0, 1.0, 0);
  cairo_rectangle(ctx, 20+count%450, 20+count%350, 50, 50);
  cairo_stroke(ctx);

  return 0;
}

static int
draw_tacho_needle(cairo_t *ctx, int count)
{
  cairo_set_source_rgba (ctx, 0, 0, 1.0, 0.5);
  cairo_paint (ctx);
  
  cairo_set_line_width(ctx, 1);
  cairo_set_source_rgb(ctx, 1.0, 1.0, 0);
  cairo_rectangle(ctx, 20+count%450, 20+count%350, 50, 50);
  cairo_stroke(ctx);

  return 0;
}

static int
my_cairo_paint (cairo_t *cairo) {

  fprintf(stderr, "paint\n");

  my_count++;

  draw_speed_needle(speed_needle_ctx, my_count);

  draw_tacho_needle(tacho_needle_ctx, my_count);

  return 0;
}

#define SPEED_POS_X 30.0
#define SPEED_POS_Y 30.0

#define TACHO_POS_X 450.0
#define TACHO_POS_Y 30.0

static int
my_composite (cairo_t *cairo) {
  fprintf(stderr, "composite \n");

  cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface(cairo, bg_img, 0, 0);
  cairo_paint(cairo);

  cairo_set_operator(cairo, CAIRO_OPERATOR_OVER);
  cairo_set_source_surface(cairo, meter_img, 0, 0);
  cairo_paint(cairo);

  cairo_set_source_surface(cairo, speed_needle_img, SPEED_POS_X, SPEED_POS_Y);
  cairo_paint(cairo);

  cairo_set_source_surface(cairo, tacho_needle_img, TACHO_POS_X, TACHO_POS_Y);
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


static cairo_surface_t*
my_event_loop (cairo_t *cairo)
{
  while (1) {
    XEvent e;
    XNextEvent(dpy, &e);
    //printf("Got event: %d\n", e.type);

    switch (e.type) {
      case MapNotify:
      case Expose:
      case ConfigureNotify:
        my_cairo_paint (cairo);
	my_composite (cairo);

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

  surface = my_create_cairo_surface (750, 400);
  cairo = cairo_create(surface);
  my_cairo_initialize (cairo);

  my_event_loop (cairo);

  return 0;
}
