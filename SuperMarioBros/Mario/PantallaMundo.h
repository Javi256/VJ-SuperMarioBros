#ifndef _PANTALLA_MUNDO
#define _PANTALLA_MUNDO

#include "Sprite.h"
#include "Text.h"


class PantallaMundo
{

public:
	~PantallaMundo();
	void init(glm::ivec2 pos, ShaderProgram& shaderProgram);
	void restart();

	void update(int deltaTime, int pos_camara);
	void render();
	void setLevel(int world, int level);
	void setVides(int vides);

private:
	glm::ivec2 tileMapDispl;
	ShaderProgram shader;
	Texture spritesheetPantallaMundo;
	Sprite* pantallaMundo;
	Text* text;
};


#endif // _PANTALLA_MUNDO_INCLUDE
