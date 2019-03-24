# -*- coding: UTF-8 -*-
""" Record voice
"""

DEVICE = "plughw:1"

import alsaaudio
import time


class RecordVoice(object):
    """ Record voice Module
    """

    RECORD_FILE = "recording.wav"
    RESPONSE_FILE = "response.mp3"

    def __init__(self):
        pass

    @staticmethod
    def record():
        """ Record voice
        """
        seconds = 10
        t_end = time.time() + 60 * seconds

        inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE,
                            alsaaudio.PCM_NORMAL, DEVICE)
        inp.setchannels(1)
        inp.setrate(44100)
        inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
        inp.setperiodsize(500)

        audio = ""
        # we keep recording while the button is pressed
        while time.time() < t_end:
            valid, data = inp.read()
            if valid:
                audio += data
        save_audio = open("sample.wav", 'w')
        save_audio.write(audio)
        save_audio.close()

if __name__ == "__main__":
    RecordVoice.record()
