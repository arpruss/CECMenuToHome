#include <stdarg.h>
#include "CEClient.h" // https://github.com/arpruss/cec
#include <ctype.h>

#define memzero(p,n) memset((p),0,(n))

#define DEVICE_TYPE                  CEC_LogicalDevice::CDT_PLAYBACK_DEVICE //CEC_LogicalDevice::CDT_RECORDING_DEVICE

#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           PA0
#define CEC_OUTPUT_PIN          -1

uint16_t physicalAddress = CEC_PHYSICAL_ADDRESS;
uint8_t deviceType = DEVICE_TYPE;
uint8_t logicalDevice = 15;
bool quiet = false;

class QuietCEClient : public CEClient {
public:  
  void OnReady() {}
  QuietCEClient (int physicalAddress, int inputPin, int outputPin) : CEClient(physicalAddress,inputPin,outputPin) {}
};


void MyDbgPrint(const char* fmt, ...)
{
        if (quiet)
          return;
        char FormatBuffer[128]; 
        va_list args;
        va_start(args, fmt);
        vsprintf(FormatBuffer, fmt, args);
       
        char c;
        char* addr = FormatBuffer;
      
        while ((c = *addr++))
        {
          Serial.print(c);
        }
}



// create a CEC client
QuietCEClient ceclient(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);

void receiver(int source, int dest, unsigned char* buffer, int count) {
  uint8_t srcDest = ((source&0x0f)<<4)|(dest&0x0f);
  //MyDbgPrint("Hello %x",srcDest);
  //Serial.write('.');
  int logical = ceclient.getLogicalAddress();
  if (logical == 15)
      logical = deviceType; // TODO
  if (count == 2 && buffer[0] == 0x44 && buffer[1] == 0xB && source == 0x00 && dest == logical) { 
      buffer[1] = 0x9; // root menu
      ceclient.write(dest, buffer, count, source);
      return;
  }
}

void setup() {    
    ceclient.setPhysicalAddress(physicalAddress);
    ceclient.setPromiscuous(true);
    ceclient.setMonitorMode(true);
    ceclient.setMonitorModeWriting(true);
    ceclient.Initialize((CEC_LogicalDevice::CEC_DEVICE_TYPE)deviceType);  
    ceclient.begin((CEC_LogicalDevice::CEC_DEVICE_TYPE)deviceType);
    ceclient.onReceiveCallback(receiver);
}

  
void loop() {
    ceclient.run();
}
