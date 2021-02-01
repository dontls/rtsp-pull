#pragma once

#include <functional>
#include <string.h>
#include <vector>

typedef void (*TyCallBackFuncFrame)(unsigned char* sPropBytes, int sPropLength, uint8_t* frameBuffer, int frameLength,
                                    void* arg);

class UsageEnvironment;
class ourRTSPClient;

class VSRtsp {
public:
    VSRtsp();
    virtual ~VSRtsp();
    int  open(char const* rtspURL, char const* sUser, char const* sPasswd, TyCallBackFuncFrame func, void* arg);
    void gotoNextframe();
    void close();

private:
    ourRTSPClient*    _rtspClient;
    UsageEnvironment* _env;
};
