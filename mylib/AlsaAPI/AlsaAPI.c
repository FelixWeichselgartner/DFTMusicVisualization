/*
this code is based on
https://gist.github.com/albanpeignier/104902
*/

#include "AlsaAPI.h"

// global variables
int i;
int err;
int buffer_frames; //128
unsigned int rate; //44100
snd_pcm_t *capture_handle;
snd_pcm_hw_params_t *hw_params;
snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

int init(int deviceNumber, int amountChannels, int bufferFrames, int samplingRate) {
    buffer_frames = bufferFrames;
    rate = samplingRate;

    if ((err = snd_pcm_open(&capture_handle, deviceNumber, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "audio interface opened\n");

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params allocated\n");

    if ((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params initialized\n");

    if ((err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params access setted\n");

    if ((err = snd_pcm_hw_params_set_format(capture_handle, hw_params, format)) < 0) {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params format setted\n");

    if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params rate setted\n");

    if ((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, amountChannels)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params channels setted\n");

    if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
        exit(1);
    }

    fprintf(stdout, "hw_params setted\n");
}

void read(unsigned short *buffer, int *length) {
    //snd_pcm_format_width == the bit-width of the format
    *length = buffer_frames * snd_pcm_format_width(format) / 8 * amountChannels;
    if ((err = snd_pcm_readi(capture_handle, buffer, buffer_frames)) != buffer_frames) {
        fprintf(stderr, "read from audio interface failed (%s)\n", err, snd_strerror(err));
        exit(1);
    }
}

void close() {
    snd_pcm_close(capture_handle);
    fprintf(stdout, "audio interface closed\n");
}