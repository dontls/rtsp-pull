#pragma once
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#include "VSCore.h"
#include <mutex>

typedef enum {
    KError_None,
    KError_FindDecoder,
    KError_OpenDecoder,
    KError_InitDecoder,
    KError_FFmpegDecodec,
    KError_AllocAVFrame,
    kError_InvalidParam,
    kError_InvalidData,
    kError_InvalidFormat,
    KError_CovertYUV420P,
} DecoderError_e;

class VSFFmpeg {
private:
    unsigned char* _rgbBuffer;
    int            _videoSize;
    std::mutex     _mutex;

public:
    AVFrame*        _vframe;
    AVFrame*        _vframeBGR;
    AVCodecContext* _videoContext;
    SwsContext*     _swsContext;
    AVPicture*      _avPicture;

public:
    VSFFmpeg();
    ~VSFFmpeg();

    DecoderError_e openDecoder(enum AVCodecID codecId);
    DecoderError_e decoderOnepacket(unsigned char* frameBuf, int frameLen);

    void getframe(unsigned char* data, int& length);

private:
    // 处理一帧video
    DecoderError_e processDecodeVideoFrame();
};
