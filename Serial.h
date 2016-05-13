//Based on https://msdn.microsoft.com/en-us/library/ms810467.aspx
//Created by Tjwk
#ifndef Serial_h
#define Serial_h

#include <windows.h>
#include <cstring>
#include <string>

class comPort
{
   private:
      //A handle to the COM port.
      HANDLE hComPort;
   public:
      //Doesn't contain defined constructor because
      //we want to check if opening was successful.
      //
      //DWORD is a WINAPI 32-bit unsigned integer.
      //
      //Function returns code of error. You can check
      //its meaning by following the link bellow:
      //https://msdn.microsoft.com/en-us/library/windows/desktop/ms681382(v=vs.85).aspx
      //
      //You can check all the valid values for parameters
      //by following the link bellow:
      //https://msdn.microsoft.com/en-us/library/windows/desktop/aa363214(v=vs.85).aspx
      DWORD open(std::string portName,
         DWORD baudRate = CBR_9600,    //CBR_X, X is a baud rate, you can also skip CBR_
         BYTE byteSize = 8,
         BYTE parity = NOPARITY,       //NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
         BYTE stopBits = ONESTOPBIT);  //ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS

      ~comPort() { close(); }
      //Closes the handle to COM port. Returns true if successful.
      bool close();

      //Returns false if there is no data to read or an error
      //occured while reading. Otherwise returns true.
      //
      //It reads from serial buffer byte by byte until it reads 0 characters
      bool read(std::string &readBuff);

      //Returns true if writing was successful.
      bool write(const std::string &writeBuff);
};

#endif
