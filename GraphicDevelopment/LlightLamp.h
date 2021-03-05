#pragma once

class LightLamp
{
public:
	void SetAmbientStrength(float value);
	void SetColor(Common::ColorRGB& color);
	Common::ColorRGB GetColor() const;
	float GetAmbientStrength() const;

	void SetShader(Common::Resources::ShaderProgramPtr shader);
	Common::Resources::ShaderProgramPtr GetShader() const;
	bool CanDraw() const;
	void Draw();
private:
	float ambientStrength_ = 0.999f;
	Common::ColorRGB color_ = { 1,1,0 };
	glm::vec3 possition_ = { std::numeric_limits<float>::infinity(),0,0 };

	Common::Resources::ShaderProgramPtr shader_;
};

