#pragma once

#include "RedCommon.h"
#include "RedVector.h"
#include "RedMaterial.h"

namespace Red
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,
		CAMERA_MODEL_UVN = 0x0002,
	};

	struct RFrustum
	{
		RInt state;
		RInt attribute;
		CAMERA_TYPE camMode;

		RVector4 position;
		RVector4 direction;

		RVector4 camUp;
		RVector4 camRight;
		RVector4 camLook;
		RVector4 camTarget;

		RFloat view_dist;
		RFloat fov;
		RFloat clip_z_near;
		RFloat clip_z_far;

		RPlane3 clip_plane_R;
		RPlane3 clip_plane_L;
		RPlane3 clip_plane_T;
		RPlane3 clip_plane_B;

		RFloat viewplane_width;
		RFloat viewplane_height;
		RFloat viewport_width;
		RFloat viewport_height;
		RFloat viewport_center_X;
		RFloat viewport_center_Y;
		
		RFloat aspect_ratio;

		RMatrix4x4 mWorldToCamera;
		RMatrix4x4 mCameraToPerspective;
		RMatrix4x4 mPerspectiveToScreen;

		RFrustum(CAMERA_TYPE mode, const RVector4& pos, const RVector4& dir,
			const RVector4& target, RFloat nearZ, RFloat farZ, RFloat ffov,
			RFloat viewportWidth, RFloat viewportHeight);

	};
}