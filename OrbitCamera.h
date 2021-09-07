#pragma once
#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

/*
* �������
* ��ס�����ק����ת���������ţ���ס�Ҽ��ƶ�Ŀ��㣨���ģ�
* ��shift��λ
* 
* ʵ�ֲο� https://github.com/zauonlok/renderer/blob/master/renderer/core/camera.c �Լ�test_helper.c�еĲ�������
* ����ԭ���ϱȽϼ��ԣ��������� ���λ��x ���� ���ڸ� �õ�һ�� 01 ֮������� ������� 2pi ������Ҫת���Ļ��ȡ�����Ч����ʵ������
* ���䣺�������ﴰ�ں����ʵ��ʱ���ڰ�ס������£���ʹ��겻�ڴ��ڷ�Χ��Ҳ���Խ��ܵ���Ϣ���ʳ��Ը�֮��������Դ���1
* 
* ���и����ӣ������Ͻ�Trackball���켣�򣿣�������ʵ�������Ч������
* ���ӣ� http://www.songho.ca/opengl/gl_camera.html ����trackball������
* ���������רҵ֮�����ڼ�������Ļ�ϵĵ㵽���Ƶ������ͶӰλ�ã�ʵ�ʼ���������λ��֮�����ת��ͽǶȡ�
*/

/*
ע

�������������õ�����ϵ�����������
   z
   |
   |
    ---- y
  /
 /
x
�ⲿ�ּ����ǻ�������������
get_view()ʱҪת��������
ʵ������ռ������ϵ
   y
   |
   |
	---- x
  /
 /
z

*/

#include "Math/Math.h"
#include "Mouse.h"
#include "Keyboard.h"

class OrbitCamera
{
private:
	float phi;  // ��������
	float theta;   
	float MIN_PHI = -PI;
	float MAX_PHI = PI;
	float MIN_THETA = 0;
	float MAX_THETA = PI;

	Vec3f UP{ 0, 1, 0 };  // ���Ϸ�����ʱд��
	Vec3f target; // �������λ�ã�Ҳ�൱�ڻ��Ƶ�����
	float distance;   // ��İ뾶
	float MIN_DISTANCE = 0;
	float MAX_DISTANCE = std::numeric_limits<float>::max();

	float rotate_speed = 1.0f;	// ��ת�����ŵ��ٶ�����
	float zoom_speed = 1.0f;

	// ����ʾ�������
	int width;
	int height;
	float fovY_rad;
	float ratio_wh;
	float near_z;
	float far_z;

public:
	OrbitCamera(int width, int height, float fovY_rad, float ratio_wh, float near_z, float far_z);
	void set_target(const Vec3f & = Vec3f(0, 0, 0));

	OrbitCamera(const OrbitCamera&) = delete;
	OrbitCamera& operator=(const OrbitCamera&) = delete;

	Mat4f get_view();  // ��ȡview transform����
	Mat4f get_persp();  // ��ȡ͸�ӱ任����

	
	// ��������
	// ��Ҫ���λ��
	void update(Mouse&, Keyboard&); 
	// �ָ�Ĭ��λ��
	void reset(void);  

private:
	void update_phi_theta(float x, float y);
	void update_target(float x, float y);
	void update_zoom(Mouse& m);

	Vec3f get_camera_position();
};

#endif // !ORBIT_CAMERA_H

