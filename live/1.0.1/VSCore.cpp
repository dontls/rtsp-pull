#include "VSCore.h"
int VSFrameQueue::init(int width, int height)
{
    _readPos = 0;
    _writePos = 0;
    _length = width * height * 3;
    for (int j = 0; j < MAX_TASK_QUEUE_CNT; j++) {
        if (frame[j].empty()) {
            frame[j].create(height, width, CV_8UC3);
            VSCore_printf("buffer %d malloc success.\n", j);
        }
    }
}

int VSFrameQueue::reSize(int width, int height)
{
    _length = width * height * 3;
    for (int j = 0; j < MAX_TASK_QUEUE_CNT; j++) {
        frame[j].release();
        frame[j].create(height, width, CV_8UC3);
        VSCore_printf("buffer %d malloc success.\n", j);
    }
}

void VSFrameQueue::release()
{
    for (int j = 0; j < MAX_TASK_QUEUE_CNT; j++) {
        if (!frame[j].empty()) {
            frame[j].release();
            VSCore_printf("buffer %d release success.\n", j);
        }
    }
}

void VSFrameQueue::push(BYTE* src)
{
    int next_pos = _writePos + 1;
    if (next_pos >= MAX_TASK_QUEUE_CNT)
        next_pos = 0;
    if (next_pos == _readPos) {
        // VSCore_printf(":---> frame queue is full\n");
        return;
    }
    VSCore_printf("push :---> w: %2d, r : %2d, %d\n", next_pos, _readPos, MAX_TASK_QUEUE_CNT);
    memcpy(frame[_writePos].data, src, _length);
    _writePos = next_pos;
}

cv::Mat VSFrameQueue::pop()
{
    cv::Mat oframe;
    if (_readPos == _writePos) {
        // VSCore_printf(":---> frame queue is empty\n");
        return oframe;
    }
    VSCore_printf("pop  :---> w: %2d, r : %2d, %d\n", _writePos, _readPos, MAX_TASK_QUEUE_CNT);
    oframe = frame[_readPos];
    int next_pos = _readPos + 1;
    _readPos = (next_pos >= MAX_TASK_QUEUE_CNT) ? 0 : next_pos;
    return oframe;
}
