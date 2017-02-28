#include "RedMath.h"
#include "RedVector.h"
#include "RedMatrix.h"
#include "RedCore.h"

namespace Red
{
	void GetTranslateMatrix4x4(RMatrix4x4& mat, RFloat x, RFloat y, RFloat z)
	{
		mat.m[0][0] = 1, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0;
		mat.m[3][0] = x, mat.m[3][1] = y, mat.m[3][2] = z, mat.m[3][3] = 1;
	}

	void GetScaleMatrix4x4(RMatrix4x4& mat, RFloat x, RFloat y, RFloat z)
	{
		mat.m[0][0] = x, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = y, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = z, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetRotateMatrix4x4Pitch(RMatrix4x4& mat, RFloat degree)
	{
		RFloat sinA = sin(DEG_TO_RAD(degree));
		RFloat cosA = cos(DEG_TO_RAD(degree));

		mat.m[0][0] = 1, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = cosA, mat.m[1][2] = sinA, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = -sinA, mat.m[2][2] = cosA, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetRotateMatrix4x4Yaw(RMatrix4x4& mat, RFloat degree)
	{
		RFloat sinA = sin(DEG_TO_RAD(degree));
		RFloat cosA = cos(DEG_TO_RAD(degree));

		mat.m[0][0] = cosA, mat.m[0][1] = 0, mat.m[0][2] = -sinA, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = sinA, mat.m[2][1] = 0, mat.m[2][2] = cosA, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetRotateMatrix4x4Roll(RMatrix4x4& mat, RFloat degree)
	{
		RFloat sinA = sin(DEG_TO_RAD(degree));
		RFloat cosA = cos(DEG_TO_RAD(degree));

		mat.m[0][0] = cosA, mat.m[0][1] = sinA, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = -sinA, mat.m[1][1] = cosA, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetRotateMatrix4x4(RMatrix4x4& mat, const RVector4& n, RFloat degree)
	{
		RFloat sinA = sin(DEG_TO_RAD(degree));
		RFloat cosA = cos(DEG_TO_RAD(degree));

		RFloat oneCosA = 1 - cosA;

		mat.m[0][0] = n.x * n.x * oneCosA + cosA, mat.m[0][1] = n.x * n.y * oneCosA + n.x * sinA, mat.m[0][2] = n.x * n.z * oneCosA - n.y * sinA, mat.m[0][3] = 0;
		mat.m[1][0] = n.x * n.y * oneCosA - n.z * sinA, mat.m[1][1] = n.y * n.y * oneCosA + cosA, mat.m[1][2] = n.y * n.z * oneCosA + n.x * sinA, mat.m[1][3] = 0;
		mat.m[2][0] = n.x * n.z * oneCosA + n.y * sinA, mat.m[2][1] = n.y * n.z * oneCosA - n.x * sinA, mat.m[2][2] = n.z * n.z * oneCosA + cosA, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetVector3MulMatrix4x4(const RVector3& vec, const RMatrix4x4& mat, RVector3& result)
	{
		RFloat x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0];
		RFloat y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1];
		RFloat z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2];

		result.x = x, result.y = y, result.z = z;
	}

	void GetVector4MulMatrix4x4(const RVector4& vec, const RMatrix4x4& mat, RVector4& result)
	{
		RFloat x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0];
		RFloat y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1];
		RFloat z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2];

		result.x = x, result.y = y, result.z = z, result.w = 1;
	}

	void GetVertex4MulMatrix4x4(const RVertex4& vec, const RMatrix4x4& mat, RVertex4& result)
	{

	}

	void GetVertex4AddVector4(const RVertex4& v, const RVector4& p, RVertex4& result)
	{

	}
}