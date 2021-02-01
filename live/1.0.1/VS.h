#pragma once

#include <mutex>

class VSRtsp;
class VSFFmpeg;

class VSStream {
public:
    VSStream();
    virtual ~VSStream();
    int openCamera(const char* rtspURL, const char* sUser, const char* sPasswd);
    int close();
    int getFrame(unsigned char* rgbBuf, int& len);
    int nextFrame();

public:
    VSFFmpeg* _ffmpeg;

private:
    VSRtsp* _rtspClient;
};
