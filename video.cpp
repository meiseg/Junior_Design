#include <gtkmm.h>
#include <webkit2/webkit2.h>

int main( int argc
        , char **argv
        )
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "" );

  Gtk::Window window;
  window.set_default_size( 800, 600 );

  WebKitWebView * one =  WEBKIT_WEB_VIEW( webkit_web_view_new() );
  /*
   * the next line does some tricks :
   * GTK_WIDGET( one ) -> convert WebKitWebView to GtkWidget (one->two)
   * Glib::wrap( GTK_WIDGET( one ) ) -> convert GtkWidget to Gtk::Widget (two->three)
   */
  Gtk::Widget * three = Glib::wrap( GTK_WIDGET( one ) );

  window.add( *three );
  webkit_web_view_load_uri(one, "http://stackoverflow.com/questions/17039942/example-of-using-webkitgtk-with-gtkmm-3-0");

  window.show_all();

  app->run( window );  
  exit( 0 );
}
