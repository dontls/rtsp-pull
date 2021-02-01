#include "VS.h"
#include "VSFFmpeg.h"
#include "VSRtsp.h"
#include <thread>

VSStream::VSStream() {}

VSStream::~VSStream() {}

void Recvframe(unsigned char* sPropBytes, int sPropLength, uint8_t* frameBuffer, int frameLength, void* arg)
{
    if (frameLength <= 20)
        return;
    unsigned char  nalu_header[4] = { 0x00, 0x00, 0x00, 0x01 };
    int            totalSize = 4 + sPropLength + 4 + frameLength;
    unsigned char* tmp = new unsigned char[totalSize];
    memcpy(tmp, nalu_header, 4);
    memcpy(tmp + 4, sPropBytes, sPropLength);
    memcpy(tmp + 4 + sPropLength, nalu_header, 4);
    memcpy(tmp + 4 + sPropLength + 4, frameBuffer, frameLength);

    VSStream* vs = ( VSStream* )arg;
    vs->_ffmpeg->decoderOnepacket(tmp, totalSize);
    // 读取下一帧
    vs->nextFrame();
}

int VSStream::openCamera(char const* rtspURL, char const* sUser, char const* sPasswd)
{
    _rtspClient = new VSRtsp();
    if (!_rtspClient) {

        return -1;
    }
    _rtspClient->open(rtspURL, sUser, sPasswd, Recvframe, this);
    _ffmpeg = new VSFFmpeg();
    if (!_ffmpeg) {
        return -1;
    }
    _ffmpeg->openDecoder(AV_CODEC_ID_H264);
    return 0;
}

int VSStream::nextFrame()
{
    _rtspClient->gotoNextframe();
}

int VSStream::close()
{
    if (_rtspClient) {
        _rtspClient->close();
        delete _rtspClient;
        _rtspClient = nullptr;
    }
    if (_ffmpeg) {
        delete _ffmpeg;
        _ffmpeg = nullptr;
    }
}

int VSStream::getFrame(unsigned char* rgbBuf, int& len)
{
    _ffmpeg->getframe(rgbBuf, len);
}