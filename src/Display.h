
/**
 * Mirror
 * Clue Media Experience
 * Display.h - declaration of Display object
 */


#ifndef DISPLAY_H
#define DISPLAY_H

#include "bcm_host.h"
#include "Resource.h"

class Display
{
	public:
		~Display();
		void Open(int screen);
		void Close();
		bool IsOpen();
		void GetInfo(DISPMANX_MODEINFO_T& info);
		void Snapshot(Resource& resource, DISPMANX_TRANSFORM_T transform);

	private:
		DISPMANX_DISPLAY_HANDLE_T m_display = DISPMANX_NO_HANDLE;
};

#endif // DISPLAY_H
