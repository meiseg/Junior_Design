#include <iostream>
#include <fstream>
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include "portaudio.h"
#include <aubio/aubio.h>
#include <string>
#include <cmath>
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define hop (256)
#define PITCH_THRESHOLD (0.68)
using namespace std;

class Junior {
public:
	
	 PaError err = Pa_Initialize();
	PaStream *stream;
	// widgets create here
	static string pitches_s;
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
	static int paRecordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
  
	const float* input_samples = (const float*) inputBuffer;
	fvec_t* input_fvec = new_fvec(FRAMES_PER_BUFFER);
	fvec_t* pitch = new_fvec(1);
    const uint_t hop_size = 256;
    const uint_t samplerate = 44100;
	aubio_pitch_t* pitch_detector = new_aubio_pitch("default", FRAMES_PER_BUFFER, FRAMES_PER_BUFFER, samplerate);
	aubio_pitch_set_unit(pitch_detector, "Hz");
	aubio_pitch_set_tolerance(pitch_detector, PITCH_THRESHOLD);
       for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
        fvec_set_sample(input_fvec, input_samples[i], i);
    }
    // Process audio data
    aubio_pitch_do(pitch_detector,input_fvec,pitch);
    //printf("Detected pitch: %f Hz\n", pitch->data[0]);
	int midi = 12 * log2(pitch->data[0]/440) + 69;
	if(midi >= 0 && midi <=127){
	const char* noteNames[] = { "C", "C #", "D", "D #", "E", "F", "F #", "G", "G #", "A", "A#", "B" };
    int octave = (midi / 12) - 1;
    int noteIndex = midi % 12;
    string noteName = noteNames[noteIndex];
    noteName += std::to_string(octave);
	Junior::pitches_s = noteName;
	}
	del_aubio_pitch(pitch_detector);
    del_fvec(pitch);
	return paContinue;
}
	
    void hide() {
        auto children = box.get_children();
        for (auto &child : children)
            child->hide();
}

    void set_setup(){
		err = Pa_OpenDefaultStream(&stream,2, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paRecordCallback, NULL);
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
        label.override_font(Pango::FontDescription("Arial 100"));
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



    void live_feed() {
		
		Glib::signal_timeout().connect([&]() {
		if (button3.get_visible()) {
		if (state == 5) {label.show();
		err= Pa_StartStream(stream);
		label.set_text(pitches_s);
		}else label.hide();
		
		
		}
		else{
		err = Pa_StopStream(stream);
		err = Pa_CloseStream(stream);
		
		
		} 
		return true;
		}, 100);
}
void clicked_menubutton(){
	item1.signal_activate().connect([&](){
	WebKitSettings *settings = webkit_settings_new();
   //g_object_set(settings, "enable-mediasource", TRUE, "enable-video", TRUE, "enable-webaudio", TRUE, NULL);
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(one), settings);
	webkit_web_view_load_uri(one, "https://open.spotify.com/lyrics");
		three->show();
});
	item2.signal_activate().connect([&](){
	WebKitSettings *settings = webkit_settings_new();
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
		
		err = Pa_StopStream(stream);
		err = Pa_CloseStream(stream);
		err = Pa_Terminate();
		});
}


void set_state(int x){(*this).state = x;}

};
string Junior::pitches_s;
	int main(int x, char ** v){
	
	Gtk::Main kit (x,v);
	Junior runt;
	runt.set_setup();
runt.button2.signal_clicked().connect([&](){
	runt.set_state(5);
	
	runt.err = Pa_StartStream(runt.stream);
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
