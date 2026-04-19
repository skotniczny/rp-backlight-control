#include <stdio.h>
#include <locale.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#define BRIGHTNESS_PATH "/sys/class/backlight/10-0045/brightness"

typedef enum {
  NIGHT = 20,
  DAY = 200
} BrightnessLevel;

int get_actual_brightness() {
  FILE *fp = fopen(BRIGHTNESS_PATH, "r");
  if (fp == NULL) return DAY;
  int val;
  fscanf(fp, "%d", &val);
  fclose(fp);
  return val;
}

void set_brightness(int value) {
    FILE *fp = fopen(BRIGHTNESS_PATH, "w");
    if (fp == NULL) {
        perror("Cannot open brightness file");
        return;
    }
    fprintf(fp, "%d\n", value);
    fclose(fp);
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

  GtkBuilder *builder = gtk_builder_new_from_file (PACKAGE_UI_DIR "/backlight.ui");
  GObject *win = gtk_builder_get_object(builder, "main_window");
  GObject *btn_day = gtk_builder_get_object(builder, "btn_day");
  GObject *btn_night = gtk_builder_get_object(builder, "btn_night");
  GObject *slider = gtk_builder_get_object (builder, "slider");

  gtk_range_set_value(GTK_RANGE(slider), get_actual_brightness());

  g_signal_connect (btn_day, "clicked", G_CALLBACK(on_day_clicked), slider);
  g_signal_connect (btn_night, "clicked", G_CALLBACK(on_night_clicked), slider);
  g_signal_connect (slider, "value-changed", G_CALLBACK(on_slider_changed), NULL);
  g_signal_connect (win, "delete_event", G_CALLBACK(end_program), NULL);

  gtk_widget_show_all (GTK_WIDGET(win));
  gtk_main ();
  return 0;
}
