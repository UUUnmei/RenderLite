#include "OrbitCamera.h"

#include <sstream>
#include "ChiliWin.h"


OrbitCamera::OrbitCamera(int width, int height, float fovY_rad, float ratio_wh, float near_z, float far_z)
	: width(width), height(height), fovY_rad(fovY_rad), ratio_wh(ratio_wh), near_z(near_z), far_z(far_z)
{ 
	reset();
}

void OrbitCamera::set_target(const Vec3f& t)
{
	target = t;
}

Mat4f OrbitCamera::get_view()
{
	Vec3f cur = get_camera_position();
	return Transform3::view(cur, target - cur, UP);
}

Mat4f OrbitCamera::get_persp()
{
	return Transform3::persp(fovY_rad, ratio_wh, near_z, far_z);
}

void OrbitCamera::update(Mouse& m, Keyboard& k) {
	static std::pair<int, int> origin{ -1, -1 };
	static std::pair<int, int> def{ -1, -1 };
	bool f = false;

	// 左shift复位
	if (k.KeyIsPressed(VK_SHIFT)) {
		reset();
		return; // 让此时后续调整无效
	}

	//  左键
	if (m.LeftIsPressed()) {
		f = true;
		if (origin == def)
			origin = m.GetPos();
		std::pair<int, int> cur = m.GetPos();
		update_phi_theta(
			(float)(cur.first - origin.first) / height,
			(float)(cur.second - origin.second) / height
		);
	//		std::stringstream ss;
	//ss << "origin: (" << origin.first << ", " << origin.second << "), "
	//	<< "cur: (" << cur.first << ", " << cur.second << ")";
	//OutputDebugString(ss.str().c_str());
		origin = cur;
	}
	// 右键
	if (m.RightIsPressed()) {
		f = true;
		if (origin == def)
			origin = m.GetPos();
		std::pair<int, int> cur = m.GetPos();
		update_target(
			(float)(cur.first - origin.first) / height,
			(float)(cur.second - origin.second) / height
		);
		origin = cur;
	}
	if (!f) {
		origin = def;
	}
	// 滚轮
	update_zoom(m);
	
}

void OrbitCamera::update_phi_theta(float x, float y)
{
	// xy是横纵向位移与窗口高度之比，（这里只取高度，避免因为宽高比影响不同方向的速度）
	float d_theta = y * 2 * PI;  // 转为弧度
	float d_phi = x * 2 * PI;

	phi -= d_phi * rotate_speed;
	theta -= d_theta * rotate_speed;

	//phi = Math::clamp(phi, MIN_PHI, MAX_PHI);		// 不限制左右转
	theta = Math::clamp(theta, MIN_THETA + EPSILON, MAX_THETA - EPSILON);  // 上下转（俯仰）限制一下，避免更新“向上方向”
	
}

void OrbitCamera::update_target(float x, float y)
{
	Vec3f cur = get_camera_position();
	Vec3f forward = (target - cur).normalize();
	Vec3f left = UP.cross(forward);
	Vec3f up = forward.cross(left);

	float factor = distance * (float)tan(fovY_rad / 2) * 2;
	Vec3f dx = x * factor * left;
	Vec3f dy = y * factor * up;

	target += dx + dy;

}

void OrbitCamera::update_zoom(Mouse& m) {
	int i = 0;
	while (!m.IsEmpty()) {
		const auto e = m.Read();
		if (!e.has_value()) continue;
		switch (e.value().GetType()) {
		case Mouse::Event::Type::WheelUp:
			i++;
			break;
		case Mouse::Event::Type::WheelDown:
			i--;
			break;
		}
	}

	distance *= pow(0.95, i);
}


void OrbitCamera::reset(void)
{
	static const float DEFAULT_PHI = 0;
	static const float DEFAULT_THETA = PI / 2;
	static const float DEFAULT_DISTANCE = 10.0f;

	phi = DEFAULT_PHI;
	theta = DEFAULT_THETA;
	distance = DEFAULT_DISTANCE;

	set_target();
}

Vec3f OrbitCamera::get_camera_position()
{
	float x = distance * std::sin(theta) * std::cos(phi);
	float y = distance * std::sin(theta) * std::sin(phi);
	float z = distance * std::cos(theta);
	// 注意坐标系的变化
	return Vec3f(
		target.x + y,
		target.y + z,
		target.z + x
	);
}

Vec3f OrbitCamera::get_camera_dir()
{
	return target - get_camera_position();
}
