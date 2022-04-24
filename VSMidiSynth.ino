/*************************************************** 
        Real time MIDI input via USB and SPI
        (C) Lee J Archer 2022
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the music maker shield
#define SHIELD_RESET  8      // VS1053 reset pin (unused!)
#define SHIELD_CS     6      // VS1053 chip select pin (output)
#define SHIELD_DCS    7      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 9     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 2       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer synth = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS); // realtime MIDI synth instance
byte midiByte;

// *************************************************************************

  
void setup() {
  
  Serial.begin(115200); // for debugging
  synth.begin();
  SD.begin(CARDCS);
 
  synth.setVolume(10, 10); // lower numbers higher volume
  LoadUserCode(); // load real time midi plugin to VS1053
  synth.sciWrite(VS1053_SCI_AIADDR, 0x50); // run the code

}

void loop() {
  
  // capture a midi byte and send to VS1053 via SDI
  
    if (Serial.available()) {
    
      midiByte = Serial.read();
      synth.midiWrite (midiByte);
     
    }

}

// ************ User application code loading tables for VS10xx ************

#ifndef SKIP_PLUGIN_VARNAME
#define PLUGIN_SIZE 28
const unsigned short plugin[28] = { /* Compressed plugin */
#endif
  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /*    0 */
  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /*    8 */
  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /*   10 */
  0x0200, 0x000a, 0x0001, 0x0050,
#ifndef SKIP_PLUGIN_VARNAME
};
#endif

void LoadUserCode(void) {
  int i = 0;
 
  while (i<sizeof(plugin)/sizeof(plugin[0])) {
    
    unsigned short addr, n, val;
    addr = plugin[i++];
    n = plugin[i++];
    if (n & 0x8000U) { /* RLE run, replicate n samples */
      n &= 0x7FFF;
      val = plugin[i++];
      while (n--) {
        synth.sciWrite(addr, val);
      }
    } else {           /* Copy run, copy n samples */
      while (n--) {
        val = plugin[i++];
        synth.sciWrite(addr, val);
      }
    }
  }
}
