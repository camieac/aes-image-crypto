#include <gtk/gtk.h>
#include "menu.h"

#ifdef NOP

if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    open_file (filename);
    g_free (filename);
  }

gtk_widget_destroy (dialog);
#endif

void file_open_handler(GtkWidget *widget, gpointer data) {
  struct menu_bar_s *mb = (struct menu_bar_s *) data;
  g_print("opening\n");
  mb->open_dialog.res = gtk_dialog_run (GTK_DIALOG (mb->open_dialog.open_dialog));
  if (mb->open_dialog.res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (mb->open_dialog.open_dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    open_file (filename);
    g_free (filename);
  }
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

int gui_start(){
  GtkWidget *window;
  GtkWidget *vbox;
  struct menu_bar_s mb;
  GtkWidget *editMenu;
  GtkWidget *editPreferencesMi;


  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Simple menu");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  mb.menu_bar = gtk_menu_bar_new();
  mb.file_menu.file_menu = gtk_menu_new();

  mb.file_menu.file_mi = gtk_menu_item_new_with_label("File");
  mb.file_menu.quit_mi = gtk_menu_item_new_with_label("Quit");
  mb.file_menu.open_mi = gtk_menu_item_new_with_label("Open");

  mb.open_dialog.open_dialog = gtk_file_chooser_dialog_new ("Open File",
    window,
    mb.open_dialog.action,
    "_Cancel",
    GTK_RESPONSE_CANCEL,
    "_Open",
    GTK_RESPONSE_ACCEPT,
    NULL);
  mb.open_dialog.action = GTK_FILE_CHOOSER_ACTION_OPEN;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mb.file_menu.file_mi), mb.file_menu.file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.file_menu.file_menu), mb.file_menu.quit_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.file_menu.file_menu), mb.file_menu.open_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(mb.menu_bar), mb.file_menu.file_mi);
  gtk_box_pack_start(GTK_BOX(vbox), mb.menu_bar, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(mb.file_menu.quit_mi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(mb.file_menu.open_mi), "activate",
      G_CALLBACK(file_open_handler), (gpointer) &mb);

  gtk_widget_show_all(window);

  gtk_main();

  m_close(&mb);
  printf("done\n");
}

int main(int argc, char *argv[]) {

  gui_start();

  return 0;
}
