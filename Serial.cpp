#include "Serial.h"

DWORD comPort::open(std::string portName, DWORD baudRate, BYTE byteSize, BYTE parity, BYTE stopBits)
{
   //"\\.\" must be write for port numbers over 9.
   //However it doesn't affect on lower numbers
   std::string validName = "\\\\.\\";
   validName += portName;
   //Open port
   hComPort = CreateFile(validName.c_str(),
      GENERIC_READ | GENERIC_WRITE, //(Access)read and write
      0, //(Share) 0 means that COM port can't be shared
      0, //(Security) None
      OPEN_EXISTING, //Open existing port
      FILE_ATTRIBUTE_NORMAL, //Normal, non-overlapped operation
      0 //No template files for COM port
      );

   //Check if creating the handle was successful
   if (hComPort == INVALID_HANDLE_VALUE)
      return GetLastError();

   //Set time-outs for a COM port
   COMMTIMEOUTS timeouts;
   //Following setting means ReadFile waits 200ms for byte to arrive
   timeouts.ReadIntervalTimeout = 0;
   timeouts.ReadTotalTimeoutMultiplier = 0;
   timeouts.ReadTotalTimeoutConstant = 200;
   timeouts.WriteTotalTimeoutMultiplier = 0;
   timeouts.WriteTotalTimeoutConstant = 10;

   //Set time-outs. If failed, returns error code
   if (!SetCommTimeouts(hComPort, &timeouts))
      return GetLastError();

   //Set parameters of communication
   //
   //DCB (Device Control Block) is a struct
   //which contains the control settings for
   //serial communications device.
   DCB dcb = {0};
   dcb.DCBlength = sizeof(DCB);

   //Try to get COM port information
   if (!GetCommState(hComPort, &dcb))
      return GetLastError();

   dcb.BaudRate = baudRate;
   dcb.ByteSize = byteSize;
   dcb.Parity = parity;
   dcb.StopBits = stopBits;

   if (!SetCommState(hComPort, &dcb))
      return GetLastError();

   //Opening was successful
   return ERROR_SUCCESS;
}

bool comPort::close()
{
   return CloseHandle(hComPort);
}

bool comPort::read(std::string &readBuff)
{
   DWORD bytesRead;
   char receivedByte;

   //If an error occured, return false
   if (!ReadFile(hComPort, &receivedByte, 1, &bytesRead, 0))
      return false;
   //If nothing was read, return false
   if (!bytesRead)
      return false;

   readBuff.clear();
   while (bytesRead != 0)
   {
      readBuff += receivedByte;
      if (!ReadFile(hComPort, &receivedByte, 1, &bytesRead, 0))
         return false;
   }
   return true;
}

bool comPort::write(const std::string &writeBuff)
{
   //BytesSend can be used to read how many bytes was actually send.
   //It exist only because the Microsoft documentation says, that
   //fourth parameter of WriteFile can be null only if the last
   //parameter (lpOverlapped) is not null
   DWORD bytesSend;

   return WriteFile(hComPort, writeBuff.c_str(), writeBuff.length(), &bytesSend, 0);
}
