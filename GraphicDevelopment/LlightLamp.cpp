#include "stdafx.h"
#include "LlightLamp.h"

using namespace Common::Graphic;
void LightLamp::SetAmbientStrength(float value)
{
	ambientStrength_ = value;
}

void LightLamp::SetColor(Common::ColorRGB& color)
{
	color_ = color;
}

Common::ColorRGB LightLamp::GetColor() const
{
	return color_;
}

float LightLamp::GetAmbientStrength() const
{
	return ambientStrength_;
}

void LightLamp::SetShader(Common::Resources::ShaderProgramPtr shader)
{
	shader_ = shader;
}

Common::Resources::ShaderProgramPtr LightLamp::GetShader() const
{
	return shader_;
}

bool LightLamp::CanDraw() const
{
	return Common::detial::IsValid(possition_) && shader_ != nullptr;
}

void LightLamp::Draw()
{
	shader_->Use();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GVertexBuffer posBuf;
	posBuf.Atach(glm::value_ptr(possition_), sizeof(possition_));
	posBuf.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	posBuf.Unbind();
	shader_->Unuse();
}
