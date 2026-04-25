#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <glib.h>

gboolean find_brightness_dir(void);
int get_max_brightness(void);
int get_actual_brightness(void);
void set_brightness(int value);

#endif
