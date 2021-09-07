#include <iostream>
#include <cstdlib>
#include "MMNotificationClientImpl.h"
using namespace std;


int main(void)
{
    HRESULT ret;
    IMMDeviceEnumerator *pEnumerator = NULL;

    ret = CoInitialize(NULL);
    if (S_OK != ret){return -1;}
    ret = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                           NULL, CLSCTX_ALL,
                           __uuidof(IMMDeviceEnumerator),
                           (void**)&pEnumerator);
    if (ret != S_OK)
    {
        CoUninitialize();
        return -1;
    }
    MMNotificationClientImpl::setPrintSwitch(false);
    MMNotificationClientImpl client;
    ret = pEnumerator->RegisterEndpointNotificationCallback(&client);
    if (ret == S_OK)
    {
      while (true)
      {
	if(client.isHeadphoneActivated()){system("start /B runApp.bat > nul");}
        Sleep(1000);
      };
    }
    CoUninitialize();
    return 0;
}
//Telephone (Polycom CX300)
//{0.0.0.00000000}.{bd1cd574-b8a8-4f1a-b9a1-23effdaa327c}
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#24 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#27 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#22 => 0
//{8943b373-388c-4395-b557-bc6dbaffafdb}#2  => 255
//{fb1de864-e06d-47f4-82a6-8a0aef44493c}#2  => 0


//Telephone (Polycom CX300)
//{0.0.1.00000000}.{dfa1f46a-9bf8-4c71-b37d-6323a37da33c}
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#24 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#27 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#22 => 0
//{8943b373-388c-4395-b557-bc6dbaffafdb}#2  => 255
//{fb1de864-e06d-47f4-82a6-8a0aef44493c}#2  => 0

//Speaker/HP (Realtek High Definition Audio)
//{0.0.0.00000000}.{f5890348-3fa5-4581-a4e3-9865b59d9a57}
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#24 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#27 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#22 => 0
//{8943b373-388c-4395-b557-bc6dbaffafdb}#2  => 255
//{fb1de864-e06d-47f4-82a6-8a0aef44493c}#2  => 0

//Microphone Array (Realtek High Definition Audio)
//{0.0.1.00000000}.{ad45ae4a-686b-4422-9b86-ce38d194db1f}
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#24 => 0
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#27 => 0
//{540b947e-8b40-45bc-a8a2-6a0b894cbda2}#9  => 20
//{b3f8fa53-0004-438e-9003-51a46e139bfc}#22 => 0
//{8943b373-388c-4395-b557-bc6dbaffafdb}#2  => 255
//{fb1de864-e06d-47f4-82a6-8a0aef44493c}#2  => 255
