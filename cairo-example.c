#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "npoc_meter.h"

static npoc_meter_t npoc_meter;

int
main (int argc, char *argv[])
{
  memset (&npoc_meter, 0, sizeof (npoc_meter));

  backend_init (750, 400);

  npoc_cairo_initialize (&npoc_meter);

  backend_event_loop (&npoc_meter);

  return 0;
}
