
APP-build := openRtsp
# 头文件路径
INCLUDES += -I./include -I/usr/local/include
INCLUDES += -I/usr/local/include/BasicUsageEnvironment -I/usr/local/include/groupsock -I/usr/local/include/liveMedia -I/usr/local/include/UsageEnvironment

CXXFLAGS += $(INCLUDES)

#live555 lib 链接顺序不能变
LDFLAGS += -L /usr/local/lib -lliveMedia -lgroupsock -lUsageEnvironment -lBasicUsageEnvironment
LDFLAGS += -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavutil -lswscale -lavformat

SRCS += Main.cpp VSFFmpeg.cpp VSRtsp.cpp

-include ../scripts/compile.mk





