#pragma once

class VMediaPlayer;

class VAbstractVideoStream
{
public:
	VAbstractVideoStream();
	~VAbstractVideoStream();

	void setCallbacks(VMediaPlayer* player);
	void unsetCallbacks(VMediaPlayer* player);

protected:
	/*!
	\brief Lock callback
	*/
	virtual void *lockCallback(void **planes) = 0;

	/*!
	\brief Unlock callback
	*/
	virtual void unlockCallback(void *picture, void *const *planes) = 0;

	/*!
	\brief Display callback
	*/
	virtual void displayCallback(void *picture) = 0;

	/*!
	\brief set Display Format
	*/

	void setFormat(VMediaPlayer* player, int width, int height);
	
	/*!
	\brief Format callback
	*/

	virtual unsigned formatCallback(char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines) {return 0;}

	/*!
	\brief Format cleanup callback
	*/
	virtual void formatCleanUpCallback() {}

private:
	static void *lockCallbackInternal(void *opaque, void **planes);
	static void unlockCallbackInternal(void *opaque, void *picture, void *const *planes);
	static void displayCallbackInternal(void *opaque, void *picture);
	static unsigned formatCallbackInternal(void **opaque, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines);
	static void formatCleanUpCallbackInternal(void *opaque);
};

