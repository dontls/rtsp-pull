  由于live555 doEventLoop(&eventLoopWatchVariable);是阻塞的。我们在主线程中调用SingleStep,主动读取帧数据


  VSCore 无锁队列，用于多路复用分发