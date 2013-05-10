// File: Gui.cc
// Date: Sat Oct 06 01:20:13 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include "Gui.hh"
using namespace std;
extern NBody world;
extern real_t time_step;
extern int FPS;
bool all_pause = false;
extern int myid, nproc;
int nowstep = 0;

gboolean delete_event(GtkWidget *, GdkEvent *, gpointer) {
	return FALSE;
}

void destroy(GtkWidget *, gpointer) {
	gtk_main_quit();
}

gboolean cb_timeout(GtkWidget *widget) {
	if (widget->window == NULL)
		return FALSE;
	gtk_widget_queue_draw_area(widget, 0, 0, widget->allocation.width, widget->allocation.height);
	return TRUE;
}

void draw_all(GtkWidget *widget){
	cairo_t *cr = gdk_cairo_create(widget->window);

	// clear 
	cairo_save (cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint (cr);
	cairo_restore (cr);

	for (int i = 0; i < MAXN; i ++) {
		Body body = world.b[i];

		cairo_save(cr);

		cairo_set_source_rgb(cr, body.rr, body.g,body.b);
		cairo_new_path(cr);
		cairo_arc(cr, body.pos.x, body.pos.y, body.r, 0, 2 * M_PI);
		cairo_close_path(cr);
		cairo_fill_preserve(cr);
		cairo_stroke(cr);
		cairo_restore(cr);
	}
	cairo_destroy(cr);
}

gboolean da_expose_callback( GtkWidget *widget, GdkEventExpose *, gpointer) {
	if (!all_pause)
		world.step();
	draw_all(widget);
	return FALSE;
}

void show_init() {
	GtkWidget *window;
	int border_width = 0;

	int window_width = SIZE_X,
		window_height = SIZE_Y;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), border_width);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), window_width, window_height);
	gtk_widget_set_size_request(window, window_width, window_height);
	gtk_window_set_decorated (GTK_WINDOW(window), FALSE);
	GtkWidget *da = gtk_drawing_area_new();
	gtk_widget_set_size_request(da, window_width, window_height);

	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

	gtk_widget_add_events(da, GDK_BUTTON_PRESS_MASK);
	g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(cb_clicked), NULL);

	gtk_widget_add_events(da, GDK_KEY_PRESS_MASK || GDK_KEY_RELEASE_MASK);
	g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(cb_pressed), NULL);

	g_signal_connect(da, "expose_event", G_CALLBACK(da_expose_callback), NULL);

	g_timeout_add(1000 / FPS, (GSourceFunc)cb_timeout, da);

	gtk_container_add(GTK_CONTAINER(window), da);

	gtk_widget_show_all(window);
}

gboolean cb_clicked(GtkWidget *, GdkEventButton *event, gpointer){
	static const unsigned LEFT = 1,
				 RIGHT = 3;
	int cx = event->x, cy = event->y;
	Body& body = world.get_body(Vec(cx, cy));
	if (event->button == LEFT){
		cout << "pos -- " << body.pos << endl;
	} else if (event->button == RIGHT){
		cout << "vel -- " << body.v << endl;
	} else {
		cout << "radius: " << body.r << "; mass: " << body.m << endl;
	}
	return TRUE;
}

gboolean cb_pressed(GtkWidget *, GdkEventKey *event, gpointer){
	int key = event->keyval;
	switch (key){
		case GDK_Escape:
			gui_quit();
			break;
		case GDK_space:
			all_pause = !all_pause;
			break;
		default:
			break;
	}
	return TRUE;
}

void gui_quit(){
#ifdef USE_MPI
	for (int i = 1; i < nproc; i ++)
		Send(&i, 1, i, QUIT);
#endif
	gtk_main_quit();
}
