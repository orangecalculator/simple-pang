#pragma once

#include <array>
#include "Vector4f.h"
using namespace std;

class Material
{
public:
	Material();
	Material(const std::array<float, 4>& emission,
		const std::array<float, 4>& ambient,
		const std::array<float, 4>& diffuse,
		const std::array<float, 4>& specular,
		const std::array<float, 1>& shininess);

	void setEmission(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setShininess(float sh);

	Vector4f getEmission() const;
	Vector4f getAmbient() const;
	Vector4f getDiffuse() const;
	Vector4f getSpecular() const;
	const float* getShininess() const;

	void draw() const;

private:
	Vector4f emission;
	Vector4f ambient;
	Vector4f diffuse;
	Vector4f specular;
	float shininess[1];
};

extern Material Brass;
extern Material Bronze;
extern Material Polished_bronze;
extern Material Chrome;
extern Material Copper;
extern Material Polished_copper;
extern Material Emerald;
extern Material Gold;
extern Material Polished_gold;
extern Material Jade;
extern Material Obsidian;
extern Material Pearl;
extern Material Pewter;
extern Material Ruby;
extern Material Silver;
extern Material Polished_silver;
extern Material Turquoise;
extern Material Plastic_black;
extern Material Plastic_cyan;
extern Material Plastic_green;
extern Material Plastic_red;
extern Material Plastic_white;
extern Material Plastic_yellow;
extern Material Rubber_black;
extern Material Rubber_cyan;
extern Material Rubber_green;
extern Material Rubber_red;
extern Material Rubber_white;
extern Material Rubber_yellow;