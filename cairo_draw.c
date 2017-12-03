#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cairo.h>
#include "npoc_meter.h"

#define NEEDLE_WIDTH  300
#define NEEDLE_HEIGHT 300

int
npoc_cairo_initialize (npoc_meter_t *npoc)
{
  npoc->bg_img    = cairo_image_surface_create_from_png("meter.png");
  npoc->meter_img = cairo_image_surface_create_from_png("meter.png");

  npoc->speed_needle_data = malloc (NEEDLE_WIDTH * NEEDLE_HEIGHT * 4);

  npoc->speed_needle_img =
    cairo_image_surface_create_for_data (npoc->speed_needle_data,
					   CAIRO_FORMAT_ARGB32,
					   NEEDLE_WIDTH,
					   NEEDLE_HEIGHT,
					   NEEDLE_WIDTH * 4);
  
  npoc->speed_needle_ctx = cairo_create (npoc->speed_needle_img);

  npoc->tacho_needle_data = malloc (NEEDLE_WIDTH * NEEDLE_HEIGHT * 4);
  npoc->tacho_needle_img = 
    cairo_image_surface_create_for_data (npoc->tacho_needle_data,
					 CAIRO_FORMAT_ARGB32,
					 NEEDLE_WIDTH,
					 NEEDLE_HEIGHT,
					 NEEDLE_WIDTH * 4);
  
  npoc->tacho_needle_ctx = cairo_create (npoc->tacho_needle_img);

  //  cairo_surface_destroy (meter_img);

  return 0;
}

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

int
npoc_cairo_paint (npoc_meter_t *npoc, int count) {

  fprintf(stderr, "paint\n");

  draw_speed_needle(npoc->speed_needle_ctx, count);

  draw_tacho_needle(npoc->tacho_needle_ctx, count);

  return 0;
}
