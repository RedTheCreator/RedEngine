#include "RedVector.h"

namespace Red
{
	RVector2 RVector2::ZERO(0, 0);
	RVector2 RVector2::UNIT_X(1, 0);
	RVector2 RVector2::UNIT_Y(0, 1);

	RVector3 RVector3::ZERO(0, 0, 0);
	RVector3 RVector3::UNIT_X(1, 0, 0);
	RVector3 RVector3::UNIT_Y(0, 1, 0);
	RVector3 RVector3::UNIT_Z(0, 0, 1);

	RVector4 RVector4::ZERO(0, 0, 0);
	RVector4 RVector4::UNIT_X(1, 0, 0);
	RVector4 RVector4::UNIT_Y(0, 1, 0);
	RVector4 RVector4::UNIT_Z(0, 0, 1);
}