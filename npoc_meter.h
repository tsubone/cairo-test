#ifndef _NPOC_METER_H_
#define _NPOC_METER_H_

#include <cairo.h>

/* this file should not include cnode/xlib headers */

typedef struct npoc_meter
{
  cairo_surface_t *bg_img;
  cairo_surface_t *meter_img;
  cairo_surface_t *speed_needle_img;
  cairo_surface_t *tacho_needle_img;

  cairo_t *speed_needle_ctx;
  cairo_t *tacho_needle_ctx;

  char *speed_needle_data;
  char *tacho_needle_data;

} npoc_meter_t;

int
npoc_cairo_initialize (npoc_meter_t *npoc);

int
npoc_cairo_paint (npoc_meter_t *npoc, int count);

int
backend_init (int w, int h);

int
backend_event_loop (npoc_meter_t *npoc);

#endif /* _NPOC_METER_H_ */
