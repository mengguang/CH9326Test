#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include "CH9326DLL.H"

#define CH9326_VID  0x1A86
#define CH9326_PID  0xE010

int main() {
    BOOL result = CH9326DllInt();
    if(result != TRUE) {
        printf("CH9326DllInt failed!\n");
        return -1;
    }

    HANDLE device = CH9326OpenDevice(CH9326_VID,CH9326_PID);
    if(device == INVALID_HANDLE_VALUE) {
        printf("CH9326OpenDevice failed!\n");
        return -2;
    }

    CH9326SetIODir(device,0x000F);

    unsigned long ret;
    //115200,8N1,3ms
    ret = CH9326SetRate(device,B115200,P_NONE,STOP_1,BIT_8,0x10);
    if(!ret) {
        printf("CH9326SetRate failed.\n");
        return -3;
    }

    ret = CH9326InitThreadData(device);
    if(!ret) {
        printf("CH9326InitThreadData failed.\n");
        return -4;
    }

    char read_buffer[32];

    char message[31];
    for (int i = 0; i < 100000; ++i) {
        snprintf(message,sizeof(message),"Hello, %d.\n",i);
        ret = CH9326WriteData(device,message,strlen(message),NULL);
        if(!ret) {
            printf("CH9326WriteData failed.\n");
        }
        CH9326WriteIOData(device,0x0004);
        Sleep(100);
        CH9326WriteIOData(device,0x0008);
        Sleep(100);
        if(CH9326GetThreadDataLen(device) > 0) {
            unsigned long n_read = sizeof(read_buffer) -1;
            memset(read_buffer,0,sizeof(read_buffer));
            CH9326ReadThreadData(device,read_buffer,&n_read);
            printf("read: %s\n",read_buffer);
        }
    }

    CH9326StopThread(device);
    CH9326CloseDevice(device);

    return 0;
}