
/**
 * Mirror - [member of Clue Media Experience]
 * Keyboard.h - declaration of Keyboard object
 */


#pragma once

#include "UFile.h"
#include <rfb/rfb.h>

class Keyboard
{
	public:
		void Open()
		{
			m_keyboard.Open();
		}

		void DoKey(rfbBool down, rfbKeySym key);

	private:
		int KeySymToScanCode(rfbKeySym key);

		bool m_downKeys[KEY_CNT]{};
		UFile m_keyboard;
};
