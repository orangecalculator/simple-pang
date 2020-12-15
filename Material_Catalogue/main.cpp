#include <vector>
#include <string>

#include "Light.h"
#include "Material.h"
#include "Text.h"
#include <GL/glut.h>

Light light(0, 0, -3, GL_LIGHT0);

vector<std::string> cataloguename = {
	"Brass",
	"Bronze",
	"Polished_bronze",
	"Chrome",
	"Copper",
	"Polished_copper",
	"Emerald",
	"Gold",
	"Polished_gold",
	"Jade",
	"Obsidian",
	"Pearl",
	"Pewter",
	"Ruby",
	"Silver",
	"Polished_silver",
	"Turquoise",
	"Plastic_black",
	"Plastic_cyan",
	"Plastic_green",
	"Plastic_red",
	"Plastic_white",
	"Plastic_yellow",
	"Rubber_black",
	"Rubber_cyan",
	"Rubber_green",
	"Rubber_red",
	"Rubber_white",
	"Rubber_yellow",
};

vector<Material*> catalogue = {
	& Brass,
	& Bronze,
	& Polished_bronze,
	& Chrome,
	& Copper,
	& Polished_copper,
	& Emerald,
	& Gold,
	& Polished_gold,
	& Jade,
	& Obsidian,
	& Pearl,
	& Pewter,
	& Ruby,
	& Silver,
	& Polished_silver,
	& Turquoise,
	& Plastic_black,
	& Plastic_cyan,
	& Plastic_green,
	& Plastic_red,
	& Plastic_white,
	& Plastic_yellow,
	& Rubber_black,
	& Rubber_cyan,
	& Rubber_green,
	& Rubber_red,
	& Rubber_white,
	& Rubber_yellow,
};

void DisplayCatalogue() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);


	constexpr int nCol = 5;
	const int nRow = ((int)catalogue.size() - 1) / nCol + 1;

	const double rowstep = 2.0 / nRow;
	const double colstep = 2.0 / nCol;

	for (int k = 0; k < (int)catalogue.size(); ++k) {
		const Material& M = *catalogue[k];
		
		const int rowno = k / nCol;
		const int colno = k % nCol;

		const double centerX = (-1.0 + colstep / 2.0) + colno * colstep;
		const double centerY = (1.0 - rowstep / 2.0 + rowstep / 8.0) + rowno * -rowstep;
		const double radius = std::min(rowstep, colstep) / 4.0;

		glPushMatrix();

		glEnable(GL_LIGHTING);
		glEnable(light.getID());
		light.draw();

		M.draw();

		glTranslated(centerX, centerY, 0.0);
		glutSolidSphere(radius, 50, 50);

		glDisable(light.getID());
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glPopMatrix();

		glColor3d(1.0, 1.0, 1.0);
		const std::string& name = cataloguename[k];
		Text T({ centerX - radius, centerY - rowstep / 2.0 });
		T.draw(name);
	}

	glutSwapBuffers();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Material Catalogue View");

	glutDisplayFunc(DisplayCatalogue);

	glutMainLoop();
}