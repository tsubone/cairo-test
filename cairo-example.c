/* compile with:
 * gcc $(pkg-config cairo-xlib-xrender --cflags --libs) cairo-example.c
 */

#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <stdio.h>

static Display *dpy;
static Window win;

static void
my_paint_to_cairo (cairo_t *cairo) {
  fprintf(stderr, "paint\n");
  cairo_set_line_width(cairo, 1);
  cairo_set_source_rgb(cairo, 128, 128, 0);
  cairo_rectangle(cairo, 20, 20, 50, 50);
  cairo_stroke(cairo);
}

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
        my_paint_to_cairo (cairo);
	if (e.type == Expose)
	  {
	    //	    fprintf (stderr, "send expose %d\n", (int)win);
	    XSendEvent(dpy, win, False, NoEventMask, &e);
	  }
        break;
    }
  }

  return 0;
}

int main() {
  cairo_t *cairo;
  cairo_surface_t *surface;

  surface = my_create_cairo_surface (200, 200);
  cairo = cairo_create(surface);

  my_event_loop (cairo);

  return 0;
}
