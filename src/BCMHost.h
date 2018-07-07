#ifndef BCMHOST_H
#define BCMHOST_H

#include "bcm_host.h"

class BCMHost
{
	public:
		BCMHost()
		{
			bcm_host_init();
		};

		~BCMHost()
		{
			bcm_host_deinit();
		}
};

#endif // BCMHOST_H
