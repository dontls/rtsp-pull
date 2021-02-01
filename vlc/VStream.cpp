#include "videostream.h"
#include "videostream_mediaplayer.h"

extern "C" {
	#include "ipcamera/onvif_common.h"
}

VVideoStream::VVideoStream()
{

}


VVideoStream::~VVideoStream() 
{

}

int VVideoStream::GetStreamUri(const char *ip, const char* username, const char* password, int channel)
{
	ONVIF_GetIPCInformation(ip, username, password, channel, &m_pVideoStream.IPC);
	if(m_pVideoStream.IPC.state == IPC_MEDIA_OK) {
		gdbprintf("%s:---> %s\n", m_pVideoStream.IPC.ip, m_pVideoStream.IPC.rtsp_uri);
		m_pVideoStream.state = VIDEO_PLAY;
	}
	else {
		m_pVideoStream.state = VIDEO_MEDIA_ERROR;
	}
	return m_pVideoStream.state;
}


int VVideoStream::Open() 
{
	gdbprintf("Open %s\n", m_pVideoStream.IPC.rtsp_uri);
	m_pMediaPlayer = new VMediaPlayer(m_pVideoStream.IPC.rtsp_uri);
	setCallbacks(m_pMediaPlayer);
	m_nFrameLength = m_pVideoStream.IPC.width * m_pVideoStream.IPC.height * 3;
	m_uFrameData = new BYTE[m_nFrameLength];
	
	m_pFrameQueue.Init(m_pVideoStream.IPC.width, m_pVideoStream.IPC.height);
	
	setFormat(m_pMediaPlayer, m_pVideoStream.IPC.width, m_pVideoStream.IPC.height);
	if(m_pMediaPlayer->play() == libvlc_Playing) {
		gdbprintf("Set Video State VIDEO_AVTICE\n");
		m_pVideoStream.state = VIDEO_AVTICE;
	}
	else {
		m_pVideoStream.state = VIDEO_EXCEPTION;
	}
}


int VVideoStream::State() 
{
	if(m_pVideoStream.state == VIDEO_AVTICE) {
		if(m_pMediaPlayer->state() != libvlc_Playing) {
			m_pVideoStream.state = VIDEO_EXCEPTION;
		}
	}
	return m_pVideoStream.state;
}

void* VVideoStream::lockCallback(void **planes) 
{
	*planes = m_uFrameData;
	m_pFrameQueue.Push(m_uFrameData); //利用队列的实现无锁模式
	return 0;
}

void VVideoStream::unlockCallback(void *picture, void *const *planes) 
{
	return;
}

void VVideoStream::displayCallback(void *picture) 
{
	(void*)picture;
}


int VVideoStream::ReOpen() 
{
	this->Close();
	return this->Open();
}

int VVideoStream::Close() 
{
	unsetCallbacks(m_pMediaPlayer);
	if(m_uFrameData) {
		delete m_uFrameData;
		m_uFrameData = NULL;
	}
	m_pMediaPlayer->close();
	if(m_pMediaPlayer) {
		delete m_pMediaPlayer;
		m_pMediaPlayer = NULL;
	}
	m_pVideoStream.state = VIDEO_CLOSE;
	return 0;
}

cv::Mat VVideoStream::GetFrameData(int task_id) 
{
	return m_pFrameQueue.Pop(task_id);
}

void VVideoStream::SetTask(BYTE * task_id)
{
	memset(m_pFrameQueue.TaskSet, 0x00, MAX_TASK_NUM);
	memcpy(m_pFrameQueue.TaskSet, task_id, MAX_TASK_NUM);
}

