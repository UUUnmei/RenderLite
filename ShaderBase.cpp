#include "ShaderBase.h"

// ((v * m)^(-1))^T = (m^(-1) * v^(-1))^T = (v^(-1))^T * (m^(-1))^T
// (vm).inverse().transpose() = v.inverse().transpose() * m.inverse().transpose()

void ShadingContext::set_model(const Mat4f& m)
{
	model = m;
}

void ShadingContext::set_view(const Mat4f& m)
{
	view = m;
}

void ShadingContext::set_projection(const Mat4f& m)
{
	projection = m;
}

void ShadingContext::gen_mvp(void)
{
	mvp = projection * view * model;
	it_mv = (view * model).inverse().transpose();
}
