#include "VS.h"
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <thread>
#include <unistd.h>

void threadCameraPlay(const char* rtspUrl, const char* username, const char* password)
{
    VSStream vscl;
    // 这里可以先判断Url是否真实的IPC
    // ONVIF_getInformation()
    int     ret = vscl.openCamera(rtspUrl, username, password);
    cv::Mat frameMat;
    frameMat.create(1080, 1920, CV_8UC3);
    while (1) {
        int rgbBufLen = 0;
        vscl.getFrame(frameMat.data, rgbBufLen);
        if (rgbBufLen > 0) {
            // cv::resize(frameMat, showMat, cv::Size(1280, 720), 0, 0);
            // cv::imshow("gdb", showMat);
            printf("%s :---> w: %5d, h: %5d\n", rtspUrl, frameMat.cols, frameMat.rows);
        }
        cv::waitKey(30);
        vscl.nextFrame();
    }
    vscl.close();
}

int main()
{

    std::thread threadCmaera1(threadCameraPlay, "rtsp://10.0.97.103:554", "admin", "123456");
    threadCmaera1.detach();
    std::thread threadCmaera2(threadCameraPlay, "rtsp://10.0.97.104:554", "admin", "123456");
    threadCmaera2.detach();
    std::thread threadCmaera3(threadCameraPlay, "rtsp://10.0.97.105:554", "admin", "123456");
    threadCmaera3.detach();
    for (;;)
        ;
}
