#include "videostream_abstract.h"
#include "videostream_mediaplayer.h"
#include "vlc/vlc.h"

static inline VAbstractVideoStream *p_this(void* opaque) { return static_cast<VAbstractVideoStream*>(opaque); }
static inline VAbstractVideoStream *p_this(void **opaque) { return static_cast<VAbstractVideoStream *>(*opaque); }
#define P_THIS p_this(opaque)

VAbstractVideoStream::VAbstractVideoStream() 
{
}


VAbstractVideoStream::~VAbstractVideoStream() 
{
}


void VAbstractVideoStream::setCallbacks(VMediaPlayer* player) 
{
	libvlc_video_set_callbacks(player->core(), lockCallbackInternal, unlockCallbackInternal, displayCallbackInternal, this);
}

void VAbstractVideoStream::setFormat(VMediaPlayer* player, int width, int height) 
{
	libvlc_video_set_format(player->core(), "RV24", width, height, width * 3);
}

void VAbstractVideoStream::unsetCallbacks(VMediaPlayer* player) 
{
	if (player) {
		libvlc_video_set_callbacks(player->core(), 0, 0, 0, 0);
	}
}

void *VAbstractVideoStream::lockCallbackInternal(void *opaque, void **planes)
{
	return P_THIS->lockCallback(planes);
}

void VAbstractVideoStream::unlockCallbackInternal(void *opaque, void *picture, void *const *planes)
{
	P_THIS->unlockCallback(picture, planes);
}

void VAbstractVideoStream::displayCallbackInternal(void *opaque, void *picture)
{
	P_THIS->displayCallback(picture);
}


unsigned VAbstractVideoStream::formatCallbackInternal(void **opaque, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines)
{
	return P_THIS->formatCallback(chroma, width, height, pitches, lines);
}

void VAbstractVideoStream::formatCleanUpCallbackInternal(void *opaque)
{
	P_THIS->formatCleanUpCallback();
}
