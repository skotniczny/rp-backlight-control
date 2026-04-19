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
  GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *lbl = gtk_label_new (_("Screen brightness"));

  GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 1);
  GtkWidget *btn_day = gtk_button_new_with_label (_("Day"));
  GtkWidget *btn_night = gtk_button_new_with_label (_("Night"));

  gtk_range_set_value(GTK_RANGE(slider), get_actual_brightness());

  gtk_scale_set_draw_value(GTK_SCALE(slider), TRUE);
  gtk_scale_set_value_pos(GTK_SCALE(slider), GTK_POS_TOP);

  g_signal_connect (btn_day, "clicked", G_CALLBACK(on_day_clicked), slider);
  g_signal_connect (btn_night, "clicked", G_CALLBACK(on_night_clicked), slider);
  g_signal_connect (slider, "value-changed", G_CALLBACK(on_slider_changed), NULL);
  g_signal_connect (win, "delete_event", G_CALLBACK(end_program), NULL);

  gtk_box_pack_start (GTK_BOX (box), lbl, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), slider, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), btn_day, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), btn_night, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (win), box);
  gtk_container_set_border_width (GTK_CONTAINER (win), 5);
  gtk_widget_show_all (win);
  gtk_widget_set_size_request (win, 250, 10);
  gtk_main ();
  return 0;
}
