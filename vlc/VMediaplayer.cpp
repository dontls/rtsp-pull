#include "videostream_mediaplayer.h"

/*
VMediaPlayer

*/
VMediaPlayer::VMediaPlayer(std::string location, bool localFile) 
{
	VInstance* instance = VInstance::getInstance();
	if(localFile) {
		_vlcMedia = libvlc_media_new_path(instance->core(), location.c_str());
	}
	else {
		_vlcMedia = libvlc_media_new_location(instance->core(), location.c_str());
	}
	_vlcMediaPlayer = libvlc_media_player_new_from_media(_vlcMedia);
}


VMediaPlayer::~VMediaPlayer() 
{

}

static int wait_playing(libvlc_media_player_t *mp)
{
	libvlc_state_t state;
	do {
		state = libvlc_media_player_get_state(mp);
	} while (state != libvlc_Playing && state != libvlc_Error && state != libvlc_Ended);
	return libvlc_media_player_get_state(mp);
}

int VMediaPlayer::play() 
{
	if (!_vlcMediaPlayer) {
		return -1;
	}
	libvlc_media_player_play(_vlcMediaPlayer);
	return wait_playing(_vlcMediaPlayer);
}

int VMediaPlayer::close() 
{

	if (!_vlcMediaPlayer) {
		return -1;
	}

	if(libvlc_media_player_is_playing(_vlcMediaPlayer)) {
		libvlc_media_player_stop(_vlcMediaPlayer);
	}
	this->release();
	return 0;
}

int VMediaPlayer::release() 
{
	if(_vlcMedia) {
		libvlc_media_release(_vlcMedia);
	}

	if(_vlcMediaPlayer) {
		libvlc_media_player_release(_vlcMediaPlayer);
	}
}

int VMediaPlayer::state()
{
	return libvlc_media_player_get_state(_vlcMediaPlayer);
}


int VMediaPlayer::replay() 
{
	libvlc_state_t state;
	do {
		state = libvlc_media_player_get_state(_vlcMediaPlayer);
	} while (state == libvlc_Ended);
	libvlc_media_player_play(_vlcMediaPlayer);
	return libvlc_media_player_get_state(_vlcMediaPlayer);
}

int VMediaPlayer::getDisplayWidth() 
{
	return libvlc_video_get_width(_vlcMediaPlayer);
}

int VMediaPlayer::getDisplayHeight() 
{
	return libvlc_video_get_height(_vlcMediaPlayer);
}

libvlc_media_player_t *VMediaPlayer::core() const 
{
	return _vlcMediaPlayer;
}
