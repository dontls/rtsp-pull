#pragma once

#include "videostream_instance.h"

class VMediaPlayer
{
public:
	VMediaPlayer(std::string location, bool localFile = false);
	~VMediaPlayer();
	/*!
	\brief Returns libvlc media player object.
	\return libvlc media player (libvlc_media_player_t *)
	*/

	libvlc_media_player_t* core() const;
	
	int play();

	int close();

	int release();

	int replay();

	int state();

	int getDisplayWidth();

	int getDisplayHeight();
private:
	libvlc_media_player_t*       _vlcMediaPlayer;
	libvlc_event_manager_t*      _vlcEvents;
	libvlc_media_t* 			 _vlcMedia;
};

