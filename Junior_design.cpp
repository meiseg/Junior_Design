#include <iostream>
#include <fstream>
#include <gtkmm.h>

using namespace std;

class Junior {
public:
	// widgets create here
    Gtk::Window window;
    Gtk::Box box{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label label;
	Gtk::MenuButton menuButton;
	Gtk::Menu menu;
	Gtk::Label label1{"label1"};
	Gtk::Label label2{"label2"};
	Gtk::MenuItem item1,item2;
    Gtk::Button button1{"Karoake Mode"}, button2{"Warm Up"}, button3{"return"};

    int state;

	void set_state(int);
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
		button1.override_font(Pango::FontDescription("Arial 50"));
		button2.override_font(Pango::FontDescription("Arial 50"));
        label.override_font(Pango::FontDescription("Arial 50"));
        button3.set_valign(Gtk::ALIGN_END);
        button3.set_halign(Gtk::ALIGN_START);
		
		//setup for menu button
		item1.set_label("option 1");
		item2.set_label("option2");
		menu.append(item1);
		menu.append(item2);
		menu.set_halign(Gtk::ALIGN_CENTER);
		menu.show_all();
		menuButton.set_popup(menu);
		menuButton.set_size_request(500, 20);
		menuButton.set_direction(Gtk::ARROW_DOWN);
		// adding items to containter
		box.pack_start(menuButton, Gtk::PACK_SHRINK);
		//box.pack_start(menuButton,Gtk::PACK_SHRINK);
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
		string a = read_ff();
		label.set_text(a);
		if (state == 5)
		label.show();
		else
		label.hide();
		}
		return true;
		}, 10);
}

    void clicked_button3() {
        hide();
        button3.show();
		if(state == 1) menuButton.show(); else menuButton.hide();
        button3.signal_clicked().connect([&]() {
		button1.show();
		button2.show();
		button3.hide();
		label.hide();
		menuButton.hide();
		window.set_default_size(500, 500);
		});
}



  
};
void Junior::set_state(int x){(*this).state = x;}


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
