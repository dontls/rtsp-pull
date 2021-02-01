APP-build := onvif-test
# 头文件路径
INCLUDES += 

CXXFLAGS += $(INCLUDES)

LDFLAGS += -L./ -lOnvif
LDFLAGS += -ldl -Wl,-rpath=./

SRCS += Main.cpp

-include ../scripts/compile.mk
