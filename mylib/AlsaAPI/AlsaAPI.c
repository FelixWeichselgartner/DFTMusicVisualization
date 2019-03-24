/*
this code is based on
https://gist.github.com/albanpeignier/104902
*/

#include "AlsaAPI.h"

#define ALSA_PCM_NEW_HW_PARAMS_API

// struct for variables needed.
struct alsaVariables {
    int err;
    int dir;
    int buffer_frames; //128
    unsigned int rate; //44100
    int amountChannels;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format;
    snd_pcm_uframes_t frames;
};

struct alsaVariables myVar;

/**
 * @brief  initialise the alsa session.
 * @note   
 * @param  deviceNumber:        the device number of the device to use with alsa.
 * @param  amountOfChannels:    number of audio channels to record.
 * @param  bufferFrames:        the amount of frames for a buffer to hold.
 * @param  samplingRate:        the rate to sample with.
 * @retval 
 */
void initAlsa(int deviceNumber, int amountOfChannels, int bufferFrames, int samplingRate) {
    myVar.buffer_frames = bufferFrames;
    myVar.rate = samplingRate;
    myVar.amountChannels = amountOfChannels;
    myVar.frames = bufferFrames;

    myVar.format = SND_PCM_FORMAT_S16_LE;

    // open pcm device for recording (capture).
    if ((myVar.err = snd_pcm_open(&myVar.capture_handle, "hw:1", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %i (%s)\n", deviceNumber, snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "audio interface opened\n");

    // allocate a hardware parameters object.
    snd_pcm_hw_params_alloca(&myVar.hw_params)
    /* if ((myVar.err = snd_pcm_hw_params_alloca(&myVar.hw_params)) < 0)
    {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }*/

    fprintf(stdout, "hw_params allocated\n");

    // fill hardware parameters object with default values.
    if ((myVar.err = snd_pcm_hw_params_any(myVar.capture_handle, myVar.hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params initialized\n");

    // interleaved mode.
    if ((myVar.err = snd_pcm_hw_params_set_access(myVar.capture_handle, myVar.hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params access setted\n");

    // signed 16-bit little-endian format.
    if ((myVar.err = snd_pcm_hw_params_set_format(myVar.capture_handle, myVar.hw_params, SND_PCM_FORMAT_S16_LE)) < 0) { //myVar.format
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params format setted\n");

    // set sampling rate. (44100)
    if ((myVar.err = snd_pcm_hw_params_set_rate_near(myVar.capture_handle, myVar.hw_params, &myVar.rate, &myVar.dir)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params rate setted\n");

    // set period size to myVar.buffer_frames frames.
    if ((myVar.err = snd_pcm_hw_params_set_period_size_near(myVar.capture_handle, myVar.hw_params, &myVar.frames, &myVar.dir))) {
        fprintf(stderr, "cannot set period size (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    // set channels. (2)
    if ((myVar.err = snd_pcm_hw_params_set_channels(myVar.capture_handle, myVar.hw_params, myVar.amountChannels)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params channels setted\n");

    // write the parameters to the driver.
    if ((myVar.err = snd_pcm_hw_params(myVar.capture_handle, myVar.hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(myVar.err));
        exit(1);
    }

    fprintf(stdout, "hw_params setted\n");
}

/**
 * @brief  read data from alsa.
 * @note   
 * @param  **buffer: pointer to which content the adress 
 *                   of the sampled material should be written to.
 * @retval size of the sampled material
 */
int readAlsa(short **buffer) {
    snd_pcm_hw_params_get_period_size(myVar.hw_params, &myVar.frames, &myVar.dir);
    int size = myVar.frames *snd_pcm_format_width(myVar.format) / 8 * myVar.amountChannels;
    *buffer = malloc(size);
    if ((myVar.err = snd_pcm_readi(myVar.capture_handle, *buffer, myVar.frames)) != myVar.frames) {
        fprintf(stderr, "read from audio interface failed (%s)\n", myVar.err, snd_strerror(myVar.err));
        exit(1);
    }
    return size;
}

/**
 * @brief  record raw pcm for a certain amount of seconds.
 * @note   
 * @param  seconds: the amount of seconds to record.
 * @retval None
 */
void recordForSeconds(int seconds) {
    snd_pcm_hw_params_get_period_size(myVar.hw_params, &myVar.frames, &myVar.dir);
    int size = myVar.frames * snd_pcm_format_width(myVar.format) / 8 * myVar.amountChannels;
    char *buffer = malloc(size);

    int val;
    snd_pcm_hw_params_get_period_time(myVar.hw_params, &val, &myVar.dir);
    int loops = seconds * 1000000 / val;

    int rc;
    for (int i = loops; i > 0; i--) {
        rc = snd_pcm_readi(myVar.capture_handle, buffer, myVar.frames);
        if (rc == -EPIPE) {
            fprintf(stderr, "overrun occured}n");
            snd_pcm_prepare(myVar.capture_handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from read %s\n", snd_strerror(rc));
        } else if (rc != (int)myVar.frames) {
            fprintf(stderr, "short read, read %i frames\n", rc);
        }

        if (fwrite(buffer, size, 1, stdout) != 1) {
            fflush(stdout);
            fprintf(stderr, "Error writing to standard output.\n");
        }
    }

    snd_pcm_drain(myVar.capture_handle);
    snd_pcm_close(myVar.capture_handle);
    free(buffer);
}

/**
 * @brief  close alsa session.
 * @note   
 * @retval None
 */
void closeAlsa() {
    snd_pcm_close(myVar.capture_handle);
    fprintf(stdout, "audio interface closed\n");
}
