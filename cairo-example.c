/* compile with:
 * gcc $(pkg-config cairo-xlib-xrender --cflags --libs) cairo-example.c
 */

#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <stdio.h>

static cairo_surface_t *surface;
static Display *dpy;

void
paint (cairo_t *cairo) {
  fprintf(stderr, "paint\n");
  cairo_set_line_width(cairo, 1);
  cairo_set_source_rgb(cairo, 128, 128, 0);
  cairo_rectangle(cairo, 20, 20, 50, 50);
  cairo_stroke(cairo);
}

int main() {
  static cairo_t *cairo;
  
  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Error: Can't open display. Is DISPLAY set?\n");
    return 1;
  }

  Window w;
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
                          0, 0, 200, 200, 0, 0, BlackPixel(dpy, 0));
  XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
  XMapWindow(dpy, w);

  surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), 200, 200);
  cairo = cairo_create(surface);

  while (1) {
    XEvent e;
    XNextEvent(dpy, &e);
    //printf("Got event: %d\n", e.type);

    switch (e.type) {
      case MapNotify:
      case Expose:
      case ConfigureNotify:
        paint(cairo);
	if (e.type == Expose)
	  XSendEvent(dpy, w, False, NoEventMask, &e);
        break;
    }
  }

  return 0;
}
