#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <gtkmm.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class VideoWidget : public Gtk::DrawingArea {
public:
    VideoWidget() : playing_(false) {}

void play(){
    if(numsong ==1){if (cap_.isOpened())cap_.release();  cap_.open("song.mp4");}
    if(numsong ==2){if (cap_.isOpened())cap_.release();  cap_.open("song2.mp4");}
	if(numsong ==3){if (cap_.isOpened())cap_.release();  cap_.open("song3.mp4");}
    if (!cap_.isOpened()) {
        std::cerr << "Failed to open video" << std::endl;
        return;
    }

    playing_ = true;

    cap_ >> frame_;
    cv::cvtColor(frame_, frame_, cv::COLOR_BGR2RGB);

    pixbuf_ = Gdk::Pixbuf::create_from_data(
	frame_.data, Gdk::COLORSPACE_RGB, false, 8, frame_.cols, frame_.rows, frame_.step);

    set_size_request(frame_.cols, frame_.rows);

    Glib::signal_timeout().connect([&]() {
        if (playing_) {
            if (cap_.read(frame_)) {
                cv::cvtColor(frame_, frame_, cv::COLOR_BGR2RGB);
                guint8* pixels = pixbuf_->get_pixels();
                memcpy(pixels, frame_.data, pixbuf_->get_byte_length());
                queue_draw();
}
            else { // Reached the end of the video
				return false;
            }
        }
        return true;
    }, 60);
}
  void reset() {
      playing_ = false;

    // Wait for the current frame to be processed
    while (Glib::MainContext::get_default()->iteration(false)) {}

    if (cap_.isOpened()) cap_.release(); 
    play(); // Open a new stream and start playing

    playing_ = true;
}

    VideoWidget& operator=(const VideoWidget& other) {
	if (this != &other) {
			cap_ = other.cap_;
			frame_ = other.frame_;
			pixbuf_ = other.pixbuf_;
			playing_ = false;
			}
			return *this;
    }
void set_song(int song){
	numsong= song;
}

protected:
bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
    Gdk::Cairo::set_source_pixbuf(cr, pixbuf_, 0, 0);
    cr->paint();
    return true;
}

private:
    int numsong;
    cv::VideoCapture cap_;
    cv::Mat frame_;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf_;
    bool playing_;
};

#endif
