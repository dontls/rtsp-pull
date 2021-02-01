#pragma once
#include <opencv2/opencv.hpp>

#include <stdio.h>

// ??????
#define VSCore_printf printf

#define MAX_TASK_QUEUE_CNT 2  // VideoStreamå
typedef unsigned char BYTE;

class VSFrameQueue {

public:
    int     init(int width, int height);
    int     reSize(int width, int height);
    void    push(BYTE* src);
    cv::Mat pop();
    void    release();

private:
    int     _length;
    size_t  _readPos = 0;
    size_t  _writePos = 0;
    cv::Mat frame[MAX_TASK_QUEUE_CNT];
};
