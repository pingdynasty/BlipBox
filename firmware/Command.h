#ifndef _COMMAND_H_
#define _COMMAND_H_

#define PRESET_REQUEST_COMMAND (1<<4)
#define PRESET_RECEIVE_COMMAND (2<<4)

enum Command {
  TOGGLE          = 3,  // XOR all LED values
  FADE            = 4,  // Decrease LED brightness
  BRIGHTEN        = 5,  // Increase LED brightness
  CFG_REQUEST     = 6,  // Request configuration parameters
  CFG_RESET       = 7,  // Reset configuration
  CFG_READ        = 8,  // Read configuration from EEPROM
  CFG_WRITE       = 9,  // Write configuration to EEPROM
  START_LED_BLOCK = 10, // Start LED update block
  END_LED_BLOCK   = 11, // End LED update block
  MIDI_PRESET     = 12, // Send MIDI zone preset
/*   ENABLE_MENU     = 13, // Enable tap-tap menu */
/*   DISABLE_MENU    = 14, // Disable tap-tap menu */
  RESET           = 15  // Soft reset the BlipBox
};

#endif /* _COMMAND_H_ */
