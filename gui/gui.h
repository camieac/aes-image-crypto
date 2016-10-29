#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <gtk/gtk.h>
#include "settings.h"
#include "i18n.h"

#define GUI_ERROR     0
#define GUI_SUCCESS   0

struct menu_bar_s {
  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *menu_bar;
  struct {
    GtkWidget *file_menu;
    GtkWidget *new_mi;
    GtkWidget *file_mi;
    GtkWidget *quit_mi;
    GtkWidget *open_mi;
  } file_menu;

  struct {
    GtkWidget *open_dialog;
    GtkFileChooserAction action;
    gint res;
  } open_dialog;

  struct {
    GtkWidget *image_widget;
  } image;


};

//Function Definitions
int gui_start(struct i18n_h *i18n, Settings *settings);

#endif
