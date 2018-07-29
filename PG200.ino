/*############################################
#
#  PG200 simulator
#
#  (c) Patrick Mans 2018
#
#
#  Translates Midi CC messages into the 
#  codes as sent out by the Roland PG200
#
############################################*/


//     Includes 
//#######################################################
#include <MIDI.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 4); // RX, TX
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
MIDI_CREATE_DEFAULT_INSTANCE();

//     Definitions 
//#######################################################

//Addresses list:
//...no idea if this is usefull....
byte Fine_tune = 16;
byte Tune = 17;
byte Env_amount = 18;
byte LFO_amount = 19;
byte Source_mix = 20;
byte HPF = 21;
byte Resonance = 22;
byte Cut_off_freq = 23;
byte Env_mod = 24;
byte LFO_mod = 25;
byte Pitch_follow = 26;
byte VCA_level = 27;
byte LFO_rate = 28;
byte LFO_delay_time = 29;
byte Attack = 30;
byte Decay = 31;
byte Sustain = 32;
byte Release = 33; 

//fader arrays
int NumOfFaders = 19;
byte AddrArray[] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};
String NameArray[] = {"Fine_tune","Tune","Env_amount","LFO_amount","Source_mix","HPF","Resonance","Cut_off_freq","Env_mod","LFO_mod","Pitch_follow","VCA_level","LFO_rage","LFO_delay_time","Attack","Decay","Sustain","Release"};

//Switch groups and Masks
int DCO1_range_group = 0;
byte DCO1_range = B00000011;
int DCO1_range_val = 3;
String DCO1_range_vals[] = {"16","8","4"};
int DCO1_waveform_group = 0;
byte DCO1_waveform = B00001100;
int DCO1_waveform_val = 3;
String DCO1_waveform_vals[] = {"Saw","Pulse","Square"};
int DCO2_range_group = 0;
byte DCO2_range = B00110000;
int DCO2_range_val = 3;
String DCO2_range_vals[] = {"16","8","4"};
int DCO2_waveform_group = 0;
byte DCO2_waveform = B11000000;
int DCO2_waveform_val = 4;
String DCO2_waveform_vals[] = {"Saw","Pulse","Square"};
int Crossmod_group = 1;
byte Crossmod = B00000011;
int Crossmod_val = 3;
String Crossmod_vals[] = {"Off","Sync","Metal"};
int VCF_Env_polarity_group = 1;
byte VCF_Env_polarity = B00000100;
int VCF_Env_polarity_val = 2;
String VCF_Env_polarity_vals[] = {"Inverted","Normal"};
int VCA_mode_group = 1;
byte VCA_mode = B00001000;
int VCA_mode_val = 2;
String VCA_mode_vals[] = {"Gate","Envelope"};
int DCO2_Env_switch_group = 1;
byte DCO2_Env_switch = B00010000;
int DCO2_Env_switch_val = 2;
String DCO2_Env_switch_vals[] = {"Off","On"};
int DCO2_LFO_switch_group = 1;
byte DCO2_LFO_switch = B00100000;
int DCO2_LFO_switch_val = 2;
String DCO2_LFO_switch_vals[] = {"Off","On"};
int DCO1_Env_switch_group = 1;
byte DC01_Env_switch = B01000000;
int DCO1_Env_switch_val = 2;
String DCO1_Env_switch_vals[] = {"Off","On"};
int DCO1_LFO_switch_group = 1;
byte DCO1_LFO_switch = B10000000;
int DCO1_LFO_switch_val = 2;
String DCO1_LFO_switch_vals[] = {"Off","On"};
int LFO_waveform_group = 2;
byte LFO_waveform = B00000011;
int LFO_waveform_val = 3;
String LFO_waveform_vals[] = {"Sine","Square","Random"};
int Env_polarity_group = 2;
byte Env_polarity = B00000100;
int Env_polarity_val = 2;
String Env_polarity_vals[] = {"Inverted","Normal"};
int Chorus_group = 2;
byte Chorus = B00001000;
int Chorus_val = 2;
String Chorus_vals[] = {"Off","On"};

int NumberOfSwitches = 14;
int SwitchNrArray[] = {100,101,102,103,104,105,106,107,108,109,110,111,112,113};
String SwitchArray[] = {"DCO1_range","DCO1_waveform","DCO2_range","DCO2_waveform","Crossmod","VCF_Env_polarity","VCA_mode","DCO2_Env_switch","DCO2_LFO_switch","DC01_Env_switch","DCO1_LFO_switch","LFO_waveform","Env_Env_polarity","Chorus"};

//     WriteLine routine
//#######################################################
void WriteLine(String TEXT) {
   lcd.setCursor(0,0);
   lcd.print(TEXT);  
}

/*
//     doDelay routine
//#######################################################
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  if ( t1 > t2 ) {
    t3 = (0xFFFFFFFF - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}

*/

//     SendCntrlCode routine
//#######################################################

void SendCntrlCode(byte Address, byte Value) {
      int AddrCode = 0x01 + Address;
      int ValCode = 0x00 + Value;
      mySerial.write(AddrCode);
      mySerial.write(ValCode);
      int wantedpos;
      for (int i=0; i<NumOfFaders; i++) {
         if (Address = AddrArray[i]) {
             wantedpos = i;
             break;
            }
       }
      String NameString = NameArray[wantedpos];
      String ValString = String(Value);
      WriteLine(NameString + ": " + ValString);
}
//     SendSwitchCode routine
//#######################################################

void SendSwitchCode(byte Address, byte Value) {
      int wantedpos;
      for (int i=0; i<NumberOfSwitches; i++) {
         if (Address = SwitchNrArray[i]) {
             wantedpos = i;
             break;
            }
       }

       String SwitchName = SwitchArray[wantedpos];
       int Group = 'String(SwitchName + "_group")';
       int NrOfVals = 'String(SwitchName + "_val")';
       byte Mask = 'SwitchName';
       int SwitchVal = Value; 
       byte MaxVal = NrOfVals - 1;
       if (SwitchVal > NrOfVals) {
          SwitchVal = MaxVal;
       }
       
       String ValsNames[NrOfVals];
       int Swval = 'String(SwitchName + "_vals[SwitchVal]")';
       String ValString = String(SwitchVal);

       WriteLine(SwitchName + ": " + Swval);
}
//     SendPGCode routine
//#######################################################

void SendPGCode(byte NR, byte VAL, byte CHAN) {
     byte Address = NR;
     byte Value = VAL;
     if (Address <= 40) {
         SendCntrlCode(Address, Value);
     }
     if (Address >= 100) {
        SendSwitchCode(Address, Value);
     }
}



//     SendPChange routine
//#######################################################
void SendPChange() {

}

//     Setup Routine
//#######################################################
void setup() {
//    Serial2.begin (115200, true);  // 9 bit mode
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);
    lcd.begin(8, 2);
    MIDI.setHandleControlChange(SendPGCode);
    MIDI.setHandleProgramChange(SendPChange);
    MIDI.begin();
    mySerial.begin(31250);
    WriteLine("PG200 SIMULATOR");
}

void loop() {
    MIDI.read(MIDI_CHANNEL_OMNI);
}
