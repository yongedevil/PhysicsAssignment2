#ifndef HELPERS_H
#define HELPERS_H

namespace Helpers
{
	typedef struct EVector3f 
	{
		EVector3f()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		EVector3f(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		float x;
		float y;
		float z;
	};
}

#endif