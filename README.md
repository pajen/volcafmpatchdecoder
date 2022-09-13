# volcafmpatchdecoder
transcodes wav files of volca fm patches to MIDI Sysex

# Installation
Extract executables from zip file and use the one appropriate for your OS

# Usage
This is a command line application. It reads a wav file and creates an output file with a .syx file ending appended.
`./volcafm_to_sysex.exe WAVFILE.WAV`
The wav file needs to be mono 44100kHz 16bit.

```
./volcafm_to_sysex_x86linux Korg\ Volca\ FM\ Bank\ 44k.wav
Volca Decode Sysex
FILE: Korg Volca FM Bank 44k.wav, 1 channel(s), rate:44100, width:16, block:2
Classic FM
Dark Bass
MINIMOOG
Twin Pad
amuseum
ChewyBass
...
LUCRETIA
Myrid
FlyKettle
ARP 2600
```
# Compiling from source
`cc main.c`
if you long for a slimmed down 90's IDE for windows, try Pelles C: http://www.smorgasbordet.com/pellesc/
