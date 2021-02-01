#pragma once

#include <iostream>
#include "vlc/vlc.h"

class VInstance {

public:
	VInstance();
	~VInstance();

	static VInstance* getInstance() {
		if(__vlc_instance == NULL) {
			__vlc_instance = new VInstance;
		}
		return __vlc_instance;
	}
	
	libvlc_instance_t* core();
	bool status() const;
	
private:
	static VInstance* __vlc_instance;
	libvlc_instance_t*  m_pVlcInstance;
	bool                m_bStatus;
};
