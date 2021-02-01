#include "VSFFmpeg.h"
#include <iostream>
VSFFmpeg::VSFFmpeg()
{
    m_videondex = -1;
    m_isRecord = -1;
    m_swsContext = nullptr;
    m_codecContext = nullptr;
    av_register_all();
    m_frame = av_frame_alloc();
    m_frameBGR = av_frame_alloc();
}
VSFFmpeg::~VSFFmpeg()
{
    av_frame_free(&m_frame);
}
bool VSFFmpeg::InitH264DecodeEnv()
{
    do {
        m_codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        if (!m_codec) {
            break;
        }
        m_codecContext = avcodec_alloc_context3(m_codec);
        if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0) {
            break;
        }
        return true;
    } while (0);
    return false;
}

void VSFFmpeg::SetPlayState(bool pause)
{
    if (pause) {
        m_playMutex.lock();
    } else {
        m_playMutex.unlock();
    }
}

void VSFFmpeg::DecodeFrame(unsigned char* sPropBytes, int sPropLength, uint8_t* frameBuffer, int frameLength,
                           long second, long microSecond)
{
    if (frameLength <= 0)
        return;

    unsigned char  nalu_header[4] = { 0x00, 0x00, 0x00, 0x01 };
    int            totalSize = 4 + sPropLength + 4 + frameLength;
    unsigned char* tmp = new unsigned char[totalSize];
    memcpy(tmp, nalu_header, 4);
    memcpy(tmp + 4, sPropBytes, sPropLength);
    memcpy(tmp + 4 + sPropLength, nalu_header, 4);
    memcpy(tmp + 4 + sPropLength + 4, frameBuffer, frameLength);
    int      frameFinished = 0;
    AVPacket framePacket;
    av_init_packet(&framePacket);
    framePacket.size = totalSize;
    framePacket.data = tmp;

    int ret = avcodec_decode_video2(m_codecContext, m_frame, &frameFinished, &framePacket);
    if (ret < 0) {
        std::cout << "Decodec Error!" << std::endl;
    }
    if (frameFinished) {
        m_playMutex.lock();
        VideoWidth = m_frame->width;
        VideoHeight = m_frame->height;
        if (m_swsContext == nullptr) {
            m_swsContext = sws_getCachedContext(m_swsContext, VideoWidth, VideoHeight,
                                                AVPixelFormat::AV_PIX_FMT_YUV420P, VideoWidth, VideoHeight,
                                                AVPixelFormat::AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
            int size = avpicture_get_size(AV_PIX_FMT_RGB24, m_codecContext->width, m_codecContext->height);
            out_buffer = ( uint8_t* )av_malloc(size);
            avpicture_fill(( AVPicture* )m_frameBGR, out_buffer, AV_PIX_FMT_RGB24, m_codecContext->width,
                           m_codecContext->height);
        }
        sws_scale(m_swsContext, ( const uint8_t* const* )m_frame->data, m_frame->linesize, 0, VideoHeight,
                  m_frameBGR->data, m_frameBGR->linesize);
        m_playMutex.unlock();
    }
    av_free_packet(&framePacket);
    delete[] tmp;
    tmp = nullptr;  //防止产生悬垂指针使程序产生没必要的错误
}

void VSFFmpeg::GetDecodedFrameData(unsigned char* data, int& length)
{
    m_playMutex.lock();
    length = VideoWidth * VideoHeight * 3;
    memcpy(data, out_buffer, length);
    m_playMutex.unlock();
}

void VSFFmpeg::GetDecodedFrameInfo(int& width, int& heigth)
{
    width = VideoWidth;
    heigth = VideoHeight;
}
