#include <stdio.h>

#include <glib/gi18n.h>
#include "brightness.h"

#define BRIGHTNESS_PATH "/sys/class/backlight"

static char *brightness_dir = NULL;

gboolean find_brightness_dir(void) {
  GDir *dir = g_dir_open(BRIGHTNESS_PATH, 0, NULL);
  if (dir) {
    const char *name = g_dir_read_name(dir);
    if (name) brightness_dir = g_build_filename(BRIGHTNESS_PATH, name, NULL);
    g_dir_close(dir);
  }
  return brightness_dir != NULL;
}

int get_max_brightness(void) {
  char *filename = g_build_filename(brightness_dir, "max_brightness", NULL);
  FILE *fp = fopen(filename, "r");
  int max = -1;
  if (fp) {
    fscanf(fp, "%d", &max);
    fclose(fp);
  }
  g_free(filename);
  return max;
}

int get_actual_brightness(void) {
  char *filename = g_build_filename(brightness_dir, "actual_brightness", NULL);
  FILE *fp = fopen(filename, "r");
  int val = -1;
  if (fp) {
    fscanf(fp, "%d", &val);
    fclose(fp);
  }
  g_free(filename);

  int max = get_max_brightness();
  if (max == -1 || val == -1) return -1;

  int percentage = val * 100 / max;
  return percentage;
}

void set_brightness(int value) {
  int max = get_max_brightness();
  if (max == -1) return;

  int actual_value = value * max / 100;

  char *filename = g_build_filename(brightness_dir, "brightness", NULL);
  FILE *fp = fopen(filename, "w");
  if (fp) {
    fprintf(fp, "%d\n", actual_value);
    fclose(fp);
  } else {
    g_printerr(_("Cannot open brightness file: %s\n"), filename);
  }
  g_free(filename);
}
