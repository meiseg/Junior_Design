#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

int main(int argc, char* argv[]) {
  // Initialize GTK
  gtk_init(&argc, &argv);

  // Create the WebView
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  WebKitWebView* webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webview));
  gtk_widget_show_all(window);

  // Load a web page
  webkit_web_view_load_uri(webview, "https://www.youtube.com/watch?v=MqazV4hbu8E");

  // Start the main GTK loop
  gtk_main();

  return 0;
}
