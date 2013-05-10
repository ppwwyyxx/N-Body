// File: Gui.hh
// Date: Thu Aug 30 15:52:25 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#ifndef __HEAD__GUI
#define __HEAD__GUI
#include "NBody.hh"
#include "common.hh"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <climits>


gboolean delete_event( GtkWidget *, GdkEvent *, gpointer);
void destroy(GtkWidget *, gpointer);
gboolean cb_timeout(GtkWidget *widget);
gboolean da_expose_callback( GtkWidget *widget, GdkEventExpose *, gpointer);
void show_init();
void draw_all(GtkWidget *widget);
gboolean cb_clicked(GtkWidget *, GdkEventButton *event, gpointer);
gboolean cb_pressed(GtkWidget *, GdkEventKey *event, gpointer);

void gui_quit();

#endif //HEAD
