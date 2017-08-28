#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MCU_TimeDisplay : public MIDI_Input_Element_CC {
  public:
    MCU_TimeDisplay()
      : MIDI_Input_Element_CC(0x40, 1, 0xA, 1) {
      ;
    }
    void refresh() {
      char barStr[5], beatStr[3], frameStr[4];
      getBars(barStr);
      getBeats(beatStr);
      getFrames(frameStr);
      Serial.printf("Bar: %s\tBeat: %s\tFrame: %s\r\n", barStr, beatStr, frameStr);
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t targetChannel = (header & 0b1111) + 1;
      uint8_t targetAddress = data1;
      if (targetAddress < this->address || targetAddress >= this->address + nb_addresses) {
        // Serial.println("\t\t\tAddress not found");
        return false;
      }
      if (targetChannel != this->channel) {
        // Serial.println("\t\t\tChannel not found");
        return false;
      }
      if ((header & 0xF0) != CC) {
        // Serial.println("\t\t\tWrong message type");
        return false;
      }
      // Serial.println("\tMatch (Time Display)");
      uint8_t charIndex = 0x9 - (data1 - 0x40);
      bool decimalPt = data2 & 0x40;
      data2 &= 0x3F;
      char character = data2 >= 0x20 ? data2 : data2 + 0x40;
      // Serial.printf("\t%d\t%c%c\r\n", charIndex, character, decimalPt ? '.' : '\0');
      textBuffer[charIndex] = character;
      refresh();
      return true;
    }
    void getBars(char* buff) {
      if (textBuffer[5] == ' ') {
        strncpy(buff, &textBuffer[0], 3);
        buff[3] = '\0';
      } else {
        strncpy(buff, &textBuffer[0], 4);
        buff[4] = '\0';
      }
    }
    void getBeats(char* buff) {
      if (textBuffer[5] == ' ') {
        strncpy(buff, &textBuffer[3], 2);
      } else {
        strncpy(buff, &textBuffer[4], 2);
      }
      buff[2] = '\0';
    }
    void getFrames(char* buff) {
      if (textBuffer[5] == ' ') {
        strncpy(buff, &textBuffer[7], 4);
      } else {
        strncpy(buff, &textBuffer[8], 3);
      }
    }
  private:
    char textBuffer[11] = "          ";
};

class MCU_AssignementDisplay : public MIDI_Input_Element_CC {
  public:
    MCU_AssignementDisplay()
      : MIDI_Input_Element_CC(0x4A, 1, 0x2, 1) {
      ;
    }
    void refresh() {
      ;
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t targetChannel = (header & 0b1111) + 1;
      uint8_t targetAddress = data1;
      if (targetAddress < this->address || targetAddress >= this->address + nb_addresses) {
        // Serial.println("\t\t\tAddress not found");
        return false;
      }
      if (targetChannel != this->channel) {
        // Serial.println("\t\t\tChannel not found");
        return false;
      }
      if ((header & 0xF0) != CC) {
        // Serial.println("\t\t\tWrong message type");
        return false;
      }
      // Serial.println("\tMatch (Time Display)");
      uint8_t charIndex = 0x9 - (data1 - 0x40);
      bool decimalPt = data2 & 0x40;
      data2 &= 0x3F;
      char character = data2 >= 0x20 ? data2 : data2 + 0x40;
      Serial.printf("\t%d\t%c%c\r\n", charIndex, character, decimalPt ? '.' : '\0');
      textBuffer[charIndex] = character;
      refresh();
      return true;
    }
  private:
    char textBuffer[3] = "  ";
};
