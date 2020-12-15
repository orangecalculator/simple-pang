#pragma once

#include "Material.h"
#include <GL/glut.h>

Material::Material() {
	setEmission(0.1f, 0.1f, 0.1f, 1.0f);
	setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	setShininess(50.0f);
}

Material::Material(const std::array<float, 4>& emission,
	const std::array<float, 4>& ambient,
	const std::array<float, 4>& diffuse,
	const std::array<float, 4>& specular,
	const std::array<float, 1>& shininess) {
	setEmission(emission[0], emission[1], emission[2], emission[3]);
	setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	setSpecular(specular[0], specular[1], specular[2], specular[3]);
	setShininess(shininess[0]);
}

void Material::setEmission(float r, float g, float b, float a) {
	emission[0] = r;
	emission[1] = g;
	emission[2] = b;
	emission[3] = a;
}
void Material::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}

void Material::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void Material::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void Material::setShininess(float sh) {
	shininess[0] = sh;
}

Vector4f Material::getEmission() const {
	return emission;
}

Vector4f Material::getAmbient() const {
	return ambient;
}

Vector4f Material::getDiffuse() const {
	return diffuse;
}

Vector4f Material::getSpecular() const {
	return specular;
}

const float* Material::getShininess() const {
	return shininess;
}

void Material::draw() const {
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_EMISSION, &getEmission()[0]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, &getAmbient()[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &getDiffuse()[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &getSpecular()[0]);
	glMaterialfv(GL_FRONT, GL_SHININESS, getShininess());
}

Material Brass(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.329412f, 0.223529f, 0.027451f, 1.0f },
	{ 0.780392f, 0.568627f, 0.113725f, 1.0f },
	{ 0.992157f, 0.941176f, 0.807843f, 1.0f },
	{ 27.89743616f }
);

Material Bronze(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.2125f, 0.1275f, 0.054f, 1.0f },
	{ 0.714f, 0.4284f, 0.18144f, 1.0f },
	{ 0.393548f, 0.271906f, 0.166721f, 1.0f },
	{ 25.6f }
);

Material Polished_bronze(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.25f, 0.148f, 0.06475f, 1.0f },
	{ 0.4f, 0.2368f, 0.1036f, 1.0f },
	{ 0.774597f, 0.458561f, 0.200621f, 1.0f },
	{ 76.8f }
);

Material Chrome(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.25f, 0.25f, 0.25f, 1.0f },
	{ 0.4f, 0.4f, 0.4f, 1.0f },
	{ 0.774597f, 0.774597f, 0.774597f, 1.0f },
	{ 76.8f }
);

Material Copper(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.19125f, 0.0735f, 0.0225f, 1.0f },
	{ 0.7038f, 0.27048f, 0.0828f, 1.0f },
	{ 0.256777f, 0.137622f, 0.086014f, 1.0f },
	{ 12.8f }
);

Material Polished_copper(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.2295f, 0.08825f, 0.0275f, 1.0f },
	{ 0.5508f, 0.2118f, 0.066f, 1.0f },
	{ 0.580594f, 0.223257f, 0.0695701f, 1.0f },
	{ 51.2f }
);

Material Emerald(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0215f, 0.1745f, 0.0215f, 0.55f },
	{ 0.07568f, 0.61424f, 0.07568f, 0.55f },
	{ 0.633f, 0.727811f, 0.633f, 0.55f },
	{ 76.8f }
);

Material Gold(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.24725f, 0.1995f, 0.0745f, 1.0f },
	{ 0.75164f, 0.60648f, 0.22648f, 1.0f },
	{ 0.628281f, 0.555802f, 0.366065f, 1.0f },
	{ 51.2f }
);

Material Polished_gold(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.24725f, 0.2245f, 0.0645f, 1.0f },
	{ 0.34615f, 0.3143f, 0.0903f, 1.0f },
	{ 0.797357f, 0.723991f, 0.208006f, 1.0f },
	{ 83.2f }
);

Material Jade(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.135f, 0.2225f, 0.1575f, 0.95f },
	{ 0.54f, 0.89f, 0.63f, 0.95f },
	{ 0.316228f, 0.316228f, 0.316228f, 0.95f },
	{ 12.8f }
);

Material Obsidian(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.05375f, 0.05f, 0.06625f, 0.82f },
	{ 0.18275f, 0.17f, 0.22525f, 0.82f },
	{ 0.332741f, 0.328634f, 0.346435f, 0.82f },
	{ 38.4f }
);

Material Pearl(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.25f, 0.20725f, 0.20725f, 0.922f },
	{ 1.0f, 0.829f, 0.829f, 0.922f },
	{ 0.296648f, 0.296648f, 0.296648f, 0.922f },
	{ 11.264f }
);

Material Pewter(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.105882f, 0.058824f, 0.113725f, 1.0f },
	{ 0.427451f, 0.470588f, 0.541176f, 1.0f },
	{ 0.333333f, 0.333333f, 0.521569f, 1.0f },
	{ 9.84615f }
);

Material Ruby(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.1745f, 0.01175f, 0.01175f, 0.55f },
	{ 0.61424f, 0.04136f, 0.04136f, 0.55f },
	{ 0.727811f, 0.626959f, 0.626959f, 0.55f },
	{ 76.8f }
);

Material Silver(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.19225f, 0.19225f, 0.19225f, 1.0f },
	{ 0.50754f, 0.50754f, 0.50754f, 1.0f },
	{ 0.508273f, 0.508273f, 0.508273f, 1.0f },
	{ 51.2f }
);

Material Polished_silver(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.23125f, 0.23125f, 0.23125f, 1.0f },
	{ 0.2775f, 0.2775f, 0.2775f, 1.0f },
	{ 0.773911f, 0.773911f, 0.773911f, 1.0f },
	{ 89.6f }
);

Material Turquoise(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.1f, 0.18725f, 0.01745f, 0.8f },
	{ 0.396f, 0.74151f, 0.69102f, 0.8f },
	{ 0.297254f, 0.30829f, 0.306678f, 0.8f },
	{ 12.8f }
);

Material Plastic_black(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.01f, 0.01f, 0.01f, 1.0f },
	{ 0.5f, 0.5f, 0.5f, 1.0f },
	{ 32.0f }
);

Material Plastic_cyan(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.1f, 0.06f, 1.0f },
	{ 0.0f, 0.50980392f, 0.50980392f, 1.0f },
	{ 0.50196078f, 0.50196078f, 0.50196078f, 1.0f },
	{ 32.0f }
);

Material Plastic_green(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.1f, 0.35f, 0.1f, 1.0f },
	{ 0.45f, 0.55f, 0.45f, 1.0f },
	{ 32.0f }
);

Material Plastic_red(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.0f, 0.0f, 1.0f },
	{ 0.7f, 0.6f, 0.6f, 1.0f },
	{ 32.0f }
);

Material Plastic_white(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.55f, 0.55f, 0.55f, 1.0f },
	{ 0.7f, 0.7f, 0.7f, 1.0f },
	{ 32.0f }
);

Material Plastic_yellow(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.5f, 0.0f, 1.0f },
	{ 0.6f, 0.6f, 0.5f, 1.0f },
	{ 32.0f }
);

Material Rubber_black(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.02f, 0.02f, 0.02f, 1.0f },
	{ 0.01f, 0.01f, 0.01f, 1.0f },
	{ 0.4f, 0.4f, 0.4f, 1.0f },
	{ 10.0f }
);

Material Rubber_cyan(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.05f, 0.05f, 1.0f },
	{ 0.4f, 0.5f, 0.5f, 1.0f },
	{ 0.04f, 0.7f, 0.7f, 1.0f },
	{ 10.0f }
);

Material Rubber_green(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.05f, 0.0f, 1.0f },
	{ 0.4f, 0.5f, 0.4f, 1.0f },
	{ 0.04f, 0.7f, 0.04f, 1.0f },
	{ 10.0f }
);

Material Rubber_red(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.05f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.4f, 0.4f, 1.0f },
	{ 0.7f, 0.04f, 0.04f, 1.0f },
	{ 10.0f }
);

Material Rubber_white(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.05f, 0.05f, 0.05f, 1.0f },
	{ 0.5f, 0.5f, 0.5f, 1.0f },
	{ 0.7f, 0.7f, 0.7f, 1.0f },
	{ 10.0f }
);

Material Rubber_yellow(
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.05f, 0.05f, 0.0f, 1.0f },
	{ 0.5f, 0.5f, 0.4f, 1.0f },
	{ 0.7f, 0.7f, 0.04f, 1.0f },
	{ 10.0f }
);