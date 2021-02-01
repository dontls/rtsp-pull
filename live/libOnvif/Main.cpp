#include "lib/Onvif.h"

int main(int argc, char** argv)
{
    struct IPCameraInfo ipcInfo;
    if (ONVIF_getInformation("10.0.63.200", "admin", "wavecamera1", 0, &ipcInfo) < 0) {
        return -1;
    }
    printf("w:%d\th:%d\turi:%s\n", ipcInfo.width, ipcInfo.height, ipcInfo.rtsp_uri);
    return 0;
}
