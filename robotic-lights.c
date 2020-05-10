#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

HMIDIOUT outputDevices[4];

typedef struct
{
    DWORD milliseconds;
    LPCSTR filename;
} WAVE_RECORDER_PARAMS;
WAVE_RECORDER_PARAMS recordParams;

DWORD WINAPI recordWaveFile(LPVOID param)
{
    WAVE_RECORDER_PARAMS *params = (WAVE_RECORDER_PARAMS *)param;
    LPCSTR filename = params->filename;
    DWORD milliseconds = params->milliseconds;

    UINT wDeviceID;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_RECORD_PARMS mciRecordParms;
    MCI_SAVE_PARMS mciSaveParms;

    mciOpenParms.lpstrDeviceType = "waveaudio";
    mciOpenParms.lpstrElementName = "";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParms);

    wDeviceID = mciOpenParms.wDeviceID;
    mciRecordParms.dwTo = milliseconds;
    mciSendCommand(wDeviceID, MCI_RECORD, MCI_TO | MCI_WAIT, (DWORD)(LPVOID)&mciRecordParms);

    mciSaveParms.lpfilename = filename;
    mciSendCommand(wDeviceID, MCI_SAVE, MCI_SAVE_FILE | MCI_WAIT, (DWORD)(LPVOID)&mciSaveParms);
    mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);

    printf("Finished recording.\n");
    return 0;
}

int main(int argc, char **args)
{
    printf("Robotic Lights by NR4/Team210. (c) 2020 Alexander Kraus <nr4@z10.info>\n");

    MIDIOUTCAPS capabilities;
    int nDevices = midiOutGetNumDevs();

    for (int i = 0; i < nDevices; ++i)
    {
        midiOutGetDevCaps(i, &capabilities, sizeof(MIDIOUTCAPS));
        printf("%s\n", capabilities.szPname);

        if (!strcmp(capabilities.szPname, "MIDISPORT 4x4 Anniv"))
            midiOutOpen(&(outputDevices[0]), i, 0, 0, CALLBACK_NULL);
        if (!strcmp(capabilities.szPname, "MIDIOUT2 (MIDISPORT 4x4 Anniv)"))
            midiOutOpen(&(outputDevices[1]), i, 0, 0, CALLBACK_NULL);
        if (!strcmp(capabilities.szPname, "MIDIOUT3 (MIDISPORT 4x4 Anniv)"))
            midiOutOpen(&(outputDevices[2]), i, 0, 0, CALLBACK_NULL);
        if (!strcmp(capabilities.szPname, "MIDIOUT4 (MIDISPORT 4x4 Anniv)"))
            midiOutOpen(&(outputDevices[3]), i, 0, 0, CALLBACK_NULL);
    }

    const float duration = 3.;
    recordParams.filename = "drumtrack.wav";
    recordParams.milliseconds = (int)(duration * 1000.);
    HANDLE recordThread;
    CreateThread(NULL, 0, recordWaveFile, &recordParams, 0, &recordThread);
    printf("Started recording.\n");

    while (1)
        ;

    return 0;
}