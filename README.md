# Arduino-VSMidiSynth
A synthesizer and MIDI file player using an Arduino UNO and VS1053b MP3 shield.

Currently the MP3 Shield Synthesizer can be played over USB MIDI.
This requires two additional programs running on the PC, hairless midi and loopmidi.
These are provided here for completeness but please check out the relevant websites for use and possible updates.

There are also several Arduino compatible boards available that can be programmed to appear as a USB MIDI device.
These, therefore, don't require the additional PC programs running to work.
So far these boards untested.

05/05/2022 On boot the device now plays a MIDI file located on the SD card route. After pressing C3 on a USB MIDI keyboard
the device becomes playable over USB. Program changes can be used to select instruments.
