#include <gtk/gtk.h>
#include "gui.h"

#include "i18n.h"
#include "settings.h"
#include "logging.h"

void gui_view_image(struct menu_bar_s *mb, char * filename){
  gtk_image_set_from_file (GTK_IMAGE(mb->image.image_widget), filename);
}

void gui_close_message(GtkWindow *parent, gchar *message){
 GtkWidget *dialog, *label, *content_area;
 GtkDialogFlags flags;

 // Create the widgets
 flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 dialog = gtk_dialog_new_with_buttons ("Message",
                                       parent,
                                       flags,
                                       "Yes",
                                       GTK_RESPONSE_ACCEPT,
                                       "No",
                                       GTK_RESPONSE_REJECT,
                                       NULL);
 content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
 label = gtk_label_new (message);

 // Ensure that the dialog box is destroyed when the user responds
 g_signal_connect_swapped (dialog,
                           "response",
                           G_CALLBACK (gtk_widget_destroy),
                           dialog);

 // Add the label, and show everything we’ve added

 gtk_container_add (GTK_CONTAINER (content_area), label);
 gtk_widget_show_all (dialog);
 gint result = gtk_dialog_run (GTK_DIALOG (dialog));
switch (result)
  {
    case GTK_RESPONSE_ACCEPT:
       LOG_INFO (_("Yes"));
       gtk_main_quit();
       break;
    default:
       LOG_INFO (_("No"));
       break;
  }

}

void gui_new_handler(GtkWidget *widget, gpointer data) {
  struct menu_bar_s *mb = (struct menu_bar_s *) data;

  gtk_image_clear(GTK_IMAGE(mb->image.image_widget));

}

void file_open_handler(GtkWidget *widget, gpointer data) {
  struct menu_bar_s *mb = (struct menu_bar_s *) data;

  mb->open_dialog.open_dialog = gtk_file_chooser_dialog_new ("Open File",
    (GtkWindow *) mb->window,
    mb->open_dialog.action,
    "_Cancel",
    GTK_RESPONSE_CANCEL,
    "_Open",
    GTK_RESPONSE_ACCEPT,
    NULL);
  mb->open_dialog.action = GTK_FILE_CHOOSER_ACTION_OPEN;

  g_print("opening\n");
  mb->open_dialog.res = gtk_dialog_run (GTK_DIALOG (mb->open_dialog.open_dialog));
  if (mb->open_dialog.res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (mb->open_dialog.open_dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    //open_file (filename);
    g_print("filename: %s\n", filename);
    gui_view_image(mb, filename);
    g_free (filename);

  }
  gtk_widget_destroy (mb->open_dialog.open_dialog);
}

void gui_quit_handler(GtkWidget *widget, gpointer data) {
  gui_close_message(GTK_WINDOW(widget), "Are you sure you want to quit?");
  //gtk_main_quit();
}

void open_file(struct menu_bar_s *mb){
  mb->open_dialog.res = gtk_dialog_run (GTK_DIALOG (mb->open_dialog.open_dialog));

}

void m_close(struct menu_bar_s *mb){
  // gtk_widget_destroy (mb->file_menu.file_menu);
  // gtk_widget_destroy (mb->file_menu.file_mi);
  // gtk_widget_destroy (mb->file_menu.quit_mi);
  // gtk_widget_destroy (mb->file_menu.open_mi);

}

int gui_start(struct i18n_h *i18n, Settings *settings){
  struct menu_bar_s mb;

  int buffer_size;

  gtk_init(NULL, NULL);

  mb.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(mb.window), GTK_WIN_POS_CENTER);

  //Get default window size from settings
  LOG_INFO("Getting window width and height.\n");
  int width = settings_get_int(settings, "window", "width");
  int height = settings_get_int(settings, "window", "height");
  LOG_INFO("width: %d, height: %d\n", width, height);

  gtk_window_set_default_size(GTK_WINDOW(mb.window), width, height);
  gtk_widget_set_size_request(mb.window, width, height);
  gtk_window_set_resizable (GTK_WINDOW(mb.window), FALSE);

  buffer_size = i18n_get_size(i18n, "title");
  char title_text[buffer_size];
  i18n_get(i18n, "title", title_text, buffer_size);
  gtk_window_set_title(GTK_WINDOW(mb.window), title_text);

  mb.vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(mb.window), mb.vbox);

  mb.menu_bar = gtk_menu_bar_new();
  mb.file_menu.file_menu = gtk_menu_new();
  mb.image.image_widget = gtk_image_new();

  //Create menu items
  buffer_size = i18n_get_size(i18n, "file");
  char file_text[buffer_size];
  i18n_get(i18n, "file", file_text, buffer_size);
  mb.file_menu.file_mi = gtk_menu_item_new_with_label(file_text);

  buffer_size = i18n_get_size(i18n, "quit");
  char quit_text[buffer_size];
  i18n_get(i18n, "quit", quit_text, buffer_size);
  mb.file_menu.quit_mi = gtk_menu_item_new_with_label(quit_text);

  buffer_size = i18n_get_size(i18n, "open");
  char open_text[buffer_size];
  i18n_get(i18n, "open", open_text, buffer_size);
  mb.file_menu.open_mi = gtk_menu_item_new_with_label(open_text);

  buffer_size = i18n_get_size(i18n, "new");
  char new_text[buffer_size];
  i18n_get(i18n, "new", new_text, buffer_size);
  mb.file_menu.new_mi = gtk_menu_item_new_with_label(new_text);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mb.file_menu.file_mi), mb.file_menu.file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.file_menu.file_menu), mb.file_menu.new_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.file_menu.file_menu), mb.file_menu.quit_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.file_menu.file_menu), mb.file_menu.open_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.menu_bar), mb.file_menu.file_mi);
  gtk_box_pack_start(GTK_BOX(mb.vbox), mb.menu_bar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(mb.vbox), mb.image.image_widget, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(mb.window), "destroy",
        G_CALLBACK(gui_quit_handler), NULL);

  g_signal_connect(G_OBJECT(mb.file_menu.quit_mi), "activate",
        G_CALLBACK(gui_quit_handler), NULL);

  g_signal_connect(G_OBJECT(mb.file_menu.open_mi), "activate",
      G_CALLBACK(file_open_handler), (gpointer) &mb);

  g_signal_connect(G_OBJECT(mb.file_menu.new_mi), "activate",
      G_CALLBACK(gui_new_handler), (gpointer) &mb);

  gtk_widget_show_all(mb.window);

  gtk_main();

  m_close(&mb);
  printf("done\n");
  return GUI_SUCCESS;
}
