#include <iostream>
#include <fstream>
#include <gtkmm.h>
#include <webkit2/webkit2.h>
//#include<cstream>
using namespace std;

class Junior {
public:
	// widgets create here
    Gtk::Window window;
    Gtk::Box box{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label label;
	Gtk::MenuButton menuButton;
	WebKitWebView * one =  WEBKIT_WEB_VIEW( webkit_web_view_new() );
	Gtk::Widget * three = Glib::wrap( GTK_WIDGET( one ) );
	Gtk::Menu menu;
	Gtk::Label label1{"label1"};
	Gtk::Label label2{"label2"};
	Gtk::MenuItem item1,item2;
    Gtk::Button button1{"Karoake Mode"}, button2{"Warm Up"}, button3{"return"};

    int state;

	
    void hide() {
        auto children = box.get_children();
        for (auto &child : children)
            child->hide();
}

    void set_setup(){
		
		//set up for windows and title
        
		window.set_default_size(1920, 1000);
        window.set_title("Junior Design Project");
        
		
		
		//set up size for buttons and alignment
		button1.set_size_request(250, 500);
        button2.set_size_request(250, 500);
        button3.set_size_request(50, 50); 
		three->set_size_request(800, 800);
		button1.override_font(Pango::FontDescription("Arial 50"));
		button2.override_font(Pango::FontDescription("Arial 50"));
        label.override_font(Pango::FontDescription("Arial 50"));
        button3.set_valign(Gtk::ALIGN_END);
        button3.set_halign(Gtk::ALIGN_START);
		
		//setup for menu button
		item1.set_label("Song 1");
		item2.set_label("Song 2");
		menu.append(item1);
		menu.append(item2);
		menu.set_halign(Gtk::ALIGN_CENTER);
		menu.show_all();
		item1.set_size_request(150,150);
		item2.set_size_request(150,150);
		menu.set_size_request(300,300);
		menuButton.set_popup(menu);
		menuButton.set_size_request(500, 20);
		menuButton.set_direction(Gtk::ARROW_DOWN);
		// adding items to containter
		box.pack_start(menuButton, Gtk::PACK_SHRINK);
		box.pack_start(*three, Gtk::PACK_SHRINK);
		
        box.pack_start(button1, Gtk::PACK_SHRINK);
        box.pack_start(button2, Gtk::PACK_SHRINK);
        box.pack_end(button3, Gtk::PACK_SHRINK);
        box.set_center_widget(label);
}

    void initial_display() {
        window.add(box);
        box.show();
        button1.show();
        button2.show();
        button3.hide();
		three->hide();
        label.hide();
		//menuButton.hide();
}

    string read_ff() {
        fstream fs("file1.txt");
        string a;
        getline(fs, a);
        return a;
}

    void live_feed() {
		Glib::signal_timeout().connect([&]() {
		if (button3.get_visible()) {

		if (state == 5) {label.show();
		//system("arecord -f S16_LE -r 44100 -c 1 - | aubiopitch -i - -r 44100 -H 1024 -o file1.txt");
		string a = read_ff();
		label.set_text(a);}
		else{
		label.hide();}
		} 
		return true;
		}, 10);
}
void clicked_menubutton(){
	item1.signal_activate().connect([&](){
	WebKitSettings *settings = webkit_settings_new();
   g_object_set(settings, "enable-mediasource", TRUE, "enable-html5-video", TRUE, "enable-video", TRUE, "enable-webaudio", TRUE, NULL);
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(one), settings);
	webkit_web_view_load_uri(one, "https://www.youtube.com/watch?v=MqazV4hbu8E");
		three->show();
});
	item2.signal_activate().connect([&](){
		WebKitSettings *settings = webkit_settings_new();
    g_object_set(settings, "enable-mediasource", TRUE, "enable-html5-video", TRUE, "enable-video", TRUE, "enable-webaudio", TRUE, NULL);
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(one), settings);
	webkit_web_view_load_uri(one, "https://www.youtube.com/watch?v=qQzdAsjWGPg");
		three->show();
});
}
    void clicked_button3() {
        hide();
        button3.show();
		
		if(state == 1) {menuButton.show();clicked_menubutton();} else{menuButton.hide();three->hide();}
        button3.signal_clicked().connect([&]() {
		button1.show();
		button2.show();
		button3.hide();
		label.hide();
		menuButton.hide();
		three->hide();
		window.set_default_size(500, 500);
		});
}


void set_state(int x){(*this).state = x;}

};
	int main(int x, char ** v){
	
	Gtk::Main kit (x,v);
	Junior runt;
	runt.set_setup();
runt.button2.signal_clicked().connect([&](){
	runt.set_state(5);
	runt.live_feed();

	runt.clicked_button3();
});

runt.button1.signal_clicked().connect([&](){
	runt.set_state(1);
	runt.live_feed();	

	runt.clicked_button3();
	
});

	runt.initial_display();
	Gtk::Main::run(runt.window);

	return 0;


}
