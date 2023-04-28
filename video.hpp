#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <gtkmm.h>
#include <iostream>
#include <opencv2/opencv.hpp>

class VideoWidget : public Gtk::DrawingArea {
public:
    VideoWidget() {
		cap_.open("song.mp4");
		
        if (!cap_.isOpened()) {
            std::cerr << "Failed to open camera" << std::endl;
            return;
        }

        cap_ >> frame_;
        cv::cvtColor(frame_, frame_, cv::COLOR_BGR2RGB);

        pixbuf_ = Gdk::Pixbuf::create_from_data(
            frame_.data, Gdk::COLORSPACE_RGB, false, 8, frame_.cols, frame_.rows, frame_.step);

        set_size_request(frame_.cols, frame_.rows);

        Glib::signal_timeout().connect([&]() {
            if (cap_.read(frame_)) {

                cv::cvtColor(frame_, frame_, cv::COLOR_BGR2RGB);
                guint8* pixels = pixbuf_->get_pixels();
                memcpy(pixels, frame_.data, pixbuf_->get_byte_length());
                queue_draw();
            }
            return true;
        }, 16);
    }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        Gdk::Cairo::set_source_pixbuf(cr, pixbuf_, 0, 0);
        cr->paint();
        return true;
    }

private:
    cv::VideoCapture cap_;
    cv::Mat frame_;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf_;
   //Gtk::Main& app_ = Gtk::Main::instance();
};

#endif
