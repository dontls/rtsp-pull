#include "VSFFmpeg.h"
#include "VSRtsp.h"
// #include <opencv2/opencv.hpp>
#include <unistd.h>
int main()
{
    RtspThread clientThread("rtsp://192.168.1.64:554", "admin", "admin");
    clientThread.Run();
    // cv::Mat m_frameMat;
    while (1) {
        int width, heigth;
        clientThread.ffmpegH264->GetDecodedFrameInfo(width, heigth);
        if (width > 0 && heigth > 0) {
            // if(m_frameMat.empty()) {
            // 	m_frameMat.create(cv::Size(width, heigth), CV_8UC3);
            // }
            // int length;
            // clientThread.ffmpegH264->GetDecodedFrameData(m_frameMat.data, length);
            // for(int i = 0; i < length; i += 3) {
            // 	uchar temp = m_frameMat.data[i];
            // 	m_frameMat.data[i] = m_frameMat.data[i+2];
            // 	m_frameMat.data[i+2] = temp;
            // }
            // cv::imshow("Camera", m_frameMat);
            // cv::waitKey(1);
        }
    }
}
