#include <iostream>
#include "Serial.h"
#include <windows.h>
#include <cstdio>

//Notes:
#define C_1     65
#define Cis_1   69
#define D_1     73
#define Dis_1   77
#define E_1     82
#define F_1     87
#define Fis_1   92
#define G_1     97
#define Gis_1   103
#define A_1     110
#define Ais_1   116
#define B_1     123
#define C       130
#define Cis     138
#define D       146
#define Dis     155
#define E       164
#define F       174
#define Fis     184
#define G       195
#define Gis     207
#define A       220
#define Ais     233
#define B       246
#define C1      261
#define Cis1    277
#define D1      293
#define Dis1    311
#define E1      329
#define F1      349
#define Fis1    369
#define G1      391
#define Gis1    415
#define A1      440
#define Ais1    466
#define B1      493

using namespace std;

void GetKeyState(int *Keys);
string GetNoteName(int Freq);
//QWERTY...
const int PianoKeys[12] = {0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, 0xDB, 0xDD};
//const int PianoFreq[12] = {262, 277, 294, 311, 329, 349, 369, 391, 415, 440, 466, 493};
//const int PianoFreq[12] = {G_1, A_1, B_1, C, D, E, Fis, G, A, B, C1, D1};//, Fis1, G1, A1, B1};
const int PianoFreq[12] = {E_1, Fis_1, G_1, A_1, B_1, C, D, E, Fis, G, A, B};
//const string PianoNotes[12] = {"c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b"};

int main()
{
    comPort MyPort;

    if (MyPort.open("COM6", CBR_57600))
    {
        cout << "Wystapil problem z otwarciem portu!" << endl;
        return -1;
    }
    else
        cout << "Udalo sie otworzyc port!" << endl;

    bool WasReleased = false;
    bool KeyPushed[12] = {0};

    int KeyState[256];

    while (true)
    {
        GetKeyState(KeyState);

        for (int i = 0; i < 12; ++i)
        {
            if (KeyState[PianoKeys[i]] < 0 && !KeyPushed[i])
            {
                WasReleased = false;
                KeyPushed[i] = true;

                char buff[20];
                sprintf(buff, "%d", PianoFreq[i]);
                string str2send = "f";
                str2send += buff;
                str2send += "\n";
                cout << GetNoteName(PianoFreq[i]) << endl;

                MyPort.write(str2send);
                MyPort.write("c\n");
            }
            else if (KeyState[PianoKeys[i]] >= 0 && KeyPushed[i])
            {
                KeyPushed[i] = false;
            }
        }

        bool AllReleased = true;

        for (int i = 0; i < 12; ++i)
            if (KeyPushed[i])
                AllReleased = false;

        if (AllReleased && !WasReleased)
        {
            WasReleased = true;
            cout << "AllReleased" << endl;

            MyPort.write("s\n");
        }
        else if (!AllReleased && WasReleased)
            WasReleased = false;
    }

    return 0;
}

void GetKeyState(int *Keys)
{
    for (int i = 0; i < 256; ++i)
        Keys[i] = GetKeyState(i);
}

string GetNoteName(int Freq)
{
    switch (Freq)
    {
        case C_1:
            return "C_1";
        case Cis_1:
            return "C#_1";
        case D_1:
            return "D_1";
        case Dis_1:
            return "D#_1";
        case E_1:
            return "E_1";
        case F_1:
            return "F_1";
        case Fis_1:
            return "F#_1";
        case G_1:
            return "G_1";
        case Gis_1:
            return "G#_1";
        case A_1:
            return "A_1";
        case Ais_1:
            return "A#_1";
        case B_1:
            return "B_1";
        case C:
            return "C";
        case Cis:
            return "C#";
        case D:
            return "D";
        case Dis:
            return "D#";
        case E:
            return "E";
        case F:
            return "F";
        case Fis:
            return "F#";
        case G:
            return "G";
        case Gis:
            return "G#";
        case A:
            return "A";
        case Ais:
            return "A#";
        case B:
            return "B";
        case C1:
            return "C1";
        case Cis1:
            return "C#1";
        case D1:
            return "D1";
        case Dis1:
            return "D#1";
        case E1:
            return "E1";
        case F1:
            return "F1";
        case Fis1:
            return "F#1";
        case G1:
            return "G1";
        case Gis1:
            return "G#1";
        case A1:
            return "A1";
        case Ais1:
            return "A#1";
        case B1:
            return "B1";
    }

    return "Other";
}
