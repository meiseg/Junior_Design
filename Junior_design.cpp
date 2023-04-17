#include <gtkmm.h>
#include <fstream>
#include <iostream>

using namespace std;

void hide(Gtk::Box& box){
	auto children = box.get_children();
    for (auto& child : children) child->hide();
}
void set_setup(Gtk::Window& window, Gtk::Button& button1,Gtk::Button& button2,Gtk::Button& button3,Gtk::Label& label,Gtk::Box& box,Gtk::Menu& item, Gtk::MenuButton& menu){

window.set_default_size(500,500);
	window.set_title("Junior Design Project");button1.set_size_request(250,250);
	button2.set_size_request(250,250);
	button3.set_size_request(50,50);
	label.override_font(Pango::FontDescription("Arial 50"));
	button3.set_valign(Gtk::ALIGN_END);
	button3.set_halign(Gtk::ALIGN_START);
	
	Gtk::MenuItem item1("option 1"), item2("option2");
	
	Gtk::Popover as;
	as.add(item1);
	as.add(item2);
	menu.set_popover(as);
	menu.set_margin_start(10);
	menu.set_margin_end(10);
	menu.set_margin_top(10);
	menu.set_margin_bottom(10);
	
	box.pack_start(menu,Gtk::PACK_SHRINK);
	
	box.pack_start(button1,Gtk::PACK_SHRINK);
	box.pack_start(button2,Gtk::PACK_SHRINK);
	box.pack_end(button3,Gtk::PACK_SHRINK);
	box.set_center_widget(label);

}
void initial_display(Gtk::Window& window, Gtk::Button& button1,Gtk::Button& button2,Gtk::Button& button3,Gtk::Label& label,Gtk::Box& box){

window.add(box);
box.show();
button1.show();
button2.show();
button3.hide();
label.hide();


}
string read_ff(){fstream fs("file1.txt");
			string a;
			getline(fs,a);
				   return a;}
void live_feed(Gtk::Label& label,Gtk::Button& button3,int& state){

	
	 Glib::signal_timeout().connect([&](){	
		if(button3.get_visible()){
			
		string a =read_ff();
		
		label.set_text(a);
			if(state == 5) label.show();
			else label.hide();
		}
			
		;return true;},10);
	
}

void clicked_button3(Gtk::Button& button3,Gtk::Button& button1,Gtk::Button& button2,Gtk::Window& window,Gtk::Label& label,Gtk::Box& box,Gtk::MenuButton& menu){
hide(box);
button3.show();


button3.signal_clicked().connect([&](){

button1.show();
button2.show();
button3.hide();
label.hide();
menu.hide();
window.set_default_size(500,500);

});}
void men(Gtk::MenuButton& menu){
	menu.signal_clicked().connect([&]() {
   
});

}

int main(int x, char ** a){

	Gtk::Main kit (x,a);
	auto state = 5;

	Gtk::Window window;
	Gtk::Box box(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label label;
	Gtk::Menu item;
	Gtk::MenuButton menu;
	
	Gtk::Button button1( "Karoake Mode"),button2("Warm Up"),button3("return");

	set_setup(window,button1,button2,button3,label,box,item,menu);

button2.signal_clicked().connect([&](){
	state = 5;
	live_feed(label,button3,state);

	clicked_button3(button3,button1,button2,window,label,box,menu);
});

button1.signal_clicked().connect([&](){
	state =1;
	live_feed(label,button3,state);	
	

	clicked_button3(button3,button1,button2,window,label,box,menu);
	item.show();
	
	menu.show();
	men(menu);
});

	initial_display(window,button1,button2,button3,label,box);
	Gtk::Main::run(window);

	return 0;
}
