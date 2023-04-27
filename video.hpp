#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <gtkmm.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

class video : public Gtk::Widget {
public:
    video()
        : Gtk::Widget()
    {
        GstElement *pipeline;
        GstBus *bus;
        GstMessage *msg;
        GError *error = nullptr;
        GstStateChangeReturn ret;

        /* Initialize GStreamer */
        gst_init(NULL, NULL);

        /* Create the pipeline */
        pipeline = gst_parse_launch("playbin uri=file:///home/mrescoba/Junior_Design/song.mp4", &error);
        if (error != nullptr) {
            g_print("Failed to create pipeline: %s\n", error->message);
            g_clear_error(&error);
            return;
        }

        /* Start playing */
        ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            g_print("Failed to start playing\n");
            return;
        }

        /* Wait until error or EOS */
        bus = gst_element_get_bus(pipeline);
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

        /* Free resources */
        if (msg != nullptr)
            gst_message_unref(msg);
        gst_object_unref(bus);
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }

    virtual ~video() {};

    operator video*() {
        return this;
    }
};

#endif
