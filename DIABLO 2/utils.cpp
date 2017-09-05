#include "stdafx.h"
#include "utils.h"

namespace OMEGA_UTIL
{
	//거리
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		return sqrtf(x * x + y * y);
	}

	//각도
	float getAngle(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;		//x축 길이
		float y = y2 - y1;		//y축 길이

		//거리를 알았다
		float distance = sqrtf((x * x) + (y * y));

		//cos의 역치역 -> acosf
		float angle = acosf(x / distance);

		if (y2 > y1)
		{
			angle = PI * 2 - angle;
			if (angle >= PI * 2) angle -= PI * 2;
		}

		return angle;
	}
}