#include <stdint.h>

typedef struct _GtkWindow GtkWindow;
typedef struct _GtkWidget GtkWidget;
typedef struct _GdkWindow GdkWindow;
typedef struct _GdkDisplay GdkDisplay;
typedef struct _XDisplay Display;

typedef unsigned long XID;

GdkWindow* gtk_widget_get_window(GtkWidget* widget);
XID gdk_x11_window_get_xid(GdkWindow* window);
GdkDisplay* gdk_window_get_display(GdkWindow* window);
Display* gdk_x11_display_get_xdisplay(GdkDisplay* display);

void* get_x11_window_from_gtk(void* gtk_window)
{
    GtkWindow* win = (GtkWindow*)gtk_window;
    GdkWindow* gdk_win = gtk_widget_get_window((GtkWidget*)win);
    if (!gdk_win) return 0;
    return (void*)(uintptr_t)gdk_x11_window_get_xid(gdk_win);
}

void* get_x11_display_from_gtk(void* gtk_window)
{
    GtkWindow* win = (GtkWindow*)gtk_window;
    GdkWindow* gdk_win = gtk_widget_get_window((GtkWidget*)win);
    if (!gdk_win) return 0;
    GdkDisplay* display = gdk_window_get_display(gdk_win);
    return gdk_x11_display_get_xdisplay(display);
}
