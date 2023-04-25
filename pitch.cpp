#include <stdio.h>
#include "portaudio.h"
#include <aubio/aubio.h>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define PITCH_THRESHOLD 0.2

static aubio_pitch_t *pitch_detector = NULL;
static int paRecordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    const fvec_t* input = (const fvec_t*) inputBuffer;
  

      fvec_t pitch;
		fvec_zeros(&pitch);
    
    // Initialize pitch detector
    if (pitch_detector == NULL) {
        pitch_detector = new_aubio_pitch("default", FRAMES_PER_BUFFER, FRAMES_PER_BUFFER, SAMPLE_RATE);
        aubio_pitch_set_unit(pitch_detector, "Hz");
        aubio_pitch_set_tolerance(pitch_detector, PITCH_THRESHOLD);
    }
    
    // Process audio data
    aubio_pitch_do(pitch_detector, input, &pitch);
    
    // Do something with pitch
    printf("Detected pitch: %f Hz\n", pitch.data[0]);
	return paContinue;
}

int main(int argc, char *argv[]) {
    PaError err = Pa_Initialize();
    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paRecordCallback, NULL);
    err = Pa_StartStream(stream);
    getchar();
    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);
    err = Pa_Terminate();
    return err;
}
