#include <stdio.h>
#include <dirent.h>

#include <glib/gi18n.h>
#include "brightness.h"

#define BRIGHTNESS_PATH "/sys/class/backlight"

static char *backlight_path = NULL;

gboolean find_backlight_path(void) {
  DIR *dir;
  struct dirent *entry;
  if ((dir = opendir(BRIGHTNESS_PATH))) {
    while ((entry = readdir(dir))) {
      if (entry->d_name[0] != '.') {
        backlight_path = g_build_filename(BRIGHTNESS_PATH, entry->d_name, NULL);
        break;
      }
    }
    closedir(dir);
  }
  return backlight_path != NULL;
}

int get_max_brightness(void) {
  char *filename = g_build_filename(backlight_path, "max_brightness", NULL);
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
  char *filename = g_build_filename(backlight_path, "actual_brightness", NULL);
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

  char *filename = g_build_filename(backlight_path, "brightness", NULL);
  FILE *fp = fopen(filename, "w");
  if (fp) {
    fprintf(fp, "%d\n", actual_value);
    fclose(fp);
  } else {
    g_printerr(_("Cannot open brightness file: %s\n"), filename);
  }
  g_free(filename);
}
