#include <stdio.h>
#include <locale.h>
#include <dirent.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#define BRIGHTNESS_PATH "/sys/class/backlight"

typedef enum {
  NIGHT = 20,
  DAY = 200
} BrightnessLevel;

char *backlight_path = NULL;

gboolean find_backlight (void) {
  DIR *dir;
  struct dirent *entry;
  if ((dir = opendir (BRIGHTNESS_PATH))) {
    while ((entry = readdir (dir)))
    {
      if (entry->d_name[0] != '.') {
        backlight_path = g_build_filename (BRIGHTNESS_PATH, entry->d_name, NULL);
        break;
      }
    }
    closedir (dir);
  }
  return backlight_path != NULL;
}

int get_actual_brightness() {
  char *filename = g_build_filename (backlight_path, "actual_brightness", NULL);
  FILE *fp = fopen (filename, "r");
  int val = DAY;
  if (fp) {
    fscanf (fp, "%d", &val);
    fclose (fp);
  }
  g_free (filename);
  return val;
}

void set_brightness(int value) {
  char *filename = g_build_filename (backlight_path, "brightness", NULL);
  FILE *fp = fopen (filename, "w");
  if (fp) {
    fprintf (fp, "%d\n", value);
    fclose (fp);
  } else {
    g_printerr ("Cannot open brightness file: %s\n", filename);
  }
  g_free (filename);
}

void on_day_clicked(GtkWidget *widget, gpointer data) {
    GtkRange *slider = GTK_RANGE(data);
    gtk_range_set_value(slider, DAY);
    set_brightness(DAY);
}

void on_night_clicked(GtkWidget *widget, gpointer data) {
    GtkRange *slider = GTK_RANGE(data);
    gtk_range_set_value(slider, NIGHT);
    set_brightness(NIGHT);
}

void on_slider_changed(GtkRange *range, gpointer data) {
  int value = (int)gtk_range_get_value(range);
  set_brightness(value);
}

void end_program (GtkWidget *wid, gpointer ptr)
{
  gtk_main_quit ();
}

int main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  gtk_init (&argc, &argv);

  if (!find_backlight ()) {
    g_printerr ("No backlight device found\n");
    return 1;
  }

  GtkBuilder *builder = gtk_builder_new_from_file (PACKAGE_UI_DIR "/backlight.ui");
  GtkWidget *win = (GtkWidget *) gtk_builder_get_object(builder, "main_window");
  GObject *btn_day = gtk_builder_get_object(builder, "btn_day");
  GObject *btn_night = gtk_builder_get_object(builder, "btn_night");
  GObject *slider = gtk_builder_get_object (builder, "slider");

  gtk_range_set_value(GTK_RANGE(slider), get_actual_brightness());

  g_signal_connect (btn_day, "clicked", G_CALLBACK(on_day_clicked), slider);
  g_signal_connect (btn_night, "clicked", G_CALLBACK(on_night_clicked), slider);
  g_signal_connect (slider, "value-changed", G_CALLBACK(on_slider_changed), NULL);
  g_signal_connect (win, "destroy", G_CALLBACK(end_program), NULL);

  gtk_widget_show_all (win);
  gtk_main ();
  gtk_widget_destroy (win);
  return 0;
}
