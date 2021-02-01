TOPDIR := $(CURDIR)

APP-build ?= openRtsp

#live555 lib 链接顺序不能变
LDFLAGS += -L /usr/local/lib -lliveMedia -lgroupsock -lUsageEnvironment -lBasicUsageEnvironment 
LDFLAGS += -L /usr/local/lib -lavcodec -lavutil -lswscale -lavformat
LDFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgproc 
LDFLAGS += -lssl -lcrypto -pthread

INCLUDES += -I./include -I/usr/local/include
INCLUDES += -I/usr/local/include/BasicUsageEnvironment -I/usr/local/include/groupsock -I/usr/local/include/liveMedia -I/usr/local/include/UsageEnvironment

include ../scripts/rules.mk
