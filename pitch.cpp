#include <stdio.h>
#include "portaudio.h"
#include <aubio/aubio.h>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define hop (256)
#define PITCH_THRESHOLD 0.9


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
    printf("Detected pitch: %f Hz\n", pitch->data[0]);
	del_aubio_pitch(pitch_detector);
    del_fvec(pitch);
	return paContinue;
}
int main(int argc, char *argv[]) {
    PaError err = Pa_Initialize();
	 if (err != paNoError) goto error;

    PaStream *stream;
	
    err = Pa_OpenDefaultStream(&stream,2, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paRecordCallback, NULL);
	if (err != paNoError) goto error;
    err = Pa_StartStream(stream);
	if (err != paNoError) goto error;
    getchar();
    err = Pa_StopStream(stream);
	if (err != paNoError) goto error;
    err = Pa_CloseStream(stream);
	if (err != paNoError) goto error;
    err = Pa_Terminate();
    return 0;
error:
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return -1;


}
