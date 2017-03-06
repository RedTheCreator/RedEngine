#include "RedFrustum.h"

namespace Red
{
	RFrustum::RFrustum(CAMERA_TYPE mode, const RVector4& pos, const RVector4& dir,
		const RVector4& target, RFloat nearZ, RFloat farZ, RFloat ffov,
		RFloat viewportWidth, RFloat viewportHeight)
		:camMode(mode), position(pos), direction(dir), camTarget(target), clip_z_near(nearZ), clip_z_far(nearZ), fov(ffov), viewport_width(viewportWidth), viewport_height(viewportHeight),
		camUp(RVector4::UNIT_Y), camRight(RVector4::UNIT_X), camLook(RVector4::UNIT_Z), mWorldToCamera(RMatrix4x4::IDENTITY), mCameraToPerspective(RMatrix4x4::IDENTITY),
		mPerspectiveToScreen(RMatrix4x4::IDENTITY)
	{
		viewport_center_X = (viewport_width - 1) * 0.5f;
		viewport_center_Y = (viewport_height - 1) * 0.5f;

		aspect_ratio = viewport_width / viewport_height;
		viewplane_width = 2.0f;
		viewplane_height = 2.0f / aspect_ratio;

		RFloat tan_fov_div2 = tan(DEG_TO_RAD(fov*0.5f));
		view_dist = 0.5f * viewplane_width / tan_fov_div2;

		RVector4 point = RVector4::ZERO;

		RVector4 normalR = RVector4(-view_dist, 0, viewplane_width * 0.5f);
		clip_plane_R = RPlane3(point, normalR);
		RVector4 normalL = RVector4(view_dist, 0, viewplane_width * 0.5f);
		clip_plane_L = RPlane3(point, normalL);
		RVector4 normalT = RVector4(0, -view_dist, viewport_height * 0.5f);
		clip_plane_T = RPlane3(point, normalT);
		RVector4 normalB = RVector4(0, view_dist, viewport_height * 0.5f);
		clip_plane_B = RPlane3(point, normalB);

		mPerspectiveToScreen = RMatrix4x4(
			view_dist, 0, 0, 0,
			0, view_dist*aspect_ratio, 0, 0,
			0, 0, 1, 1,
			0, 0, 0, 0);
	}
}