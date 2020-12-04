
/**
 * Mirror
 * Clue Media Experience
 * Resource.h - declaration of Resource object
 */


#ifndef RESOURCE_H
#define RESOURCE_H

#include "bcm_host.h"

class Resource
{
	public:
		~Resource();
		void Create(VC_IMAGE_TYPE_T type, int width, int height, uint32_t *vc_image_ptr);
		void Close();
		void ReadData(VC_RECT_T& rect, void *image, int pitch);
		DISPMANX_RESOURCE_HANDLE_T GetResourceHandle();

	private:
		DISPMANX_RESOURCE_HANDLE_T  m_resource = DISPMANX_NO_HANDLE;
};

#endif // RESOURCE_H
