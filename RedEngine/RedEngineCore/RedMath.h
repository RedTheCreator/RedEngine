#pragma once

#include "RedCommon.h"

namespace Red
{
	template<class T>
	const T& Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	const T& Max(const T& a, const T& b)
	{
		return (b < a) ? a : b;
	}

	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV)
	{
		return Min(Max(val, maxV), minV);
	}

	void GetTranslateMatrix4x4(RMatrix4x4& mat, RFloat x, RFloat y, RFloat z);

	void GetScaleMatrix4x4(RMatrix4x4& mat, RFloat x, RFloat y, RFloat z);

	//Pitch,X
	void GetRotateMatrix4x4Pitch(RMatrix4x4& mat, RFloat degree);
	//Yaw,Y
	void GetRotateMatrix4x4Yaw(RMatrix4x4& mat, RFloat degree);
	//Roll,Z
	void GetRotateMatrix4x4Roll(RMatrix4x4& mat, RFloat degree);

	void GetRotateMatrix4x4(RMatrix4x4& mat, const RVector4& n, RFloat degree);
	void GetVertex4AddVector4(const RVertex4& v, const RVector4& p, RVertex4& result);

	void GetVector3MulMatrix4x4(const RVector3& vec, const RMatrix4x4& mat, RVector3& result);
	void GetVector4MulMatrix4x4(const RVector4& vec, const RMatrix4x4& mat, RVector4& result);
	void GetVertex4MulMatrix4x4(const RVertex4& vec, const RMatrix4x4& mat, RVertex4& result);

}