#include "videostream_instance.h"


VInstance* VInstance::__vlc_instance = NULL;

VInstance::VInstance()
	: m_pVlcInstance(0),
	  m_bStatus(false) 
{

	/*
	const char* vlc_args[] = {
		"--extraintf=logger",
		"--no-audio",
		"--ignore-config",
		"--media-library",
		"--network-caching=100",
		"--no-xlib",
		"--rtsp-tcp",
		"--rtsp-frame-buffer-size=1000000"
	};
	*/
	const char* vlc_args[] = {
		"--network-caching=300"
	};
	int argc = sizeof(vlc_args) / sizeof(vlc_args[0]);
	if (argc > 0) {
		m_pVlcInstance = libvlc_new(argc, vlc_args);
	}
	else {
		m_pVlcInstance = libvlc_new(0, NULL);
	}

	if (m_pVlcInstance) {
		m_bStatus = true;
	} 
}

VInstance::~VInstance() 
{
	if(m_pVlcInstance != NULL) {
		libvlc_release(m_pVlcInstance);
	}
}

libvlc_instance_t* VInstance::core() 
{
	return m_pVlcInstance;
}

bool VInstance::status() const 
{
	return m_bStatus;
}