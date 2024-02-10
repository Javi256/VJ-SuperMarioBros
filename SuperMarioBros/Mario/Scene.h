#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Bloque.h" //Tocado
#include "Text.h" //tocado
#include "Banner.h"
#include "Moneda.h"
#include "PantallaMundo.h"
#include "PantallaTimeUp.h"
#include "PowerUp.h"
#include "Audio.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void restart();
	void update(int deltaTime);
	void render();

	void changeLevel(string level);

	bool collisionPlayerEnemy(glm::vec2 pos1, glm::vec2 tam1, glm::vec2 pos2, glm::vec2 tam2);
	bool isGameOver();
	bool hasWon();

	int getPoints();
	int getMonedas();
	int getWorld();
	int getLevel();
	int getRecord();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Goomba *goomba;
	Koopa *koopa;
	ShaderProgram texProgram;
	float currentTime;
	float pos_camara;
	glm::mat4 projection;
	
	Bloque* bloque; //tocado
	Text *text; //tocado
	Banner* banner;
	Moneda* moneda;
	PowerUp* powerUp;
	PantallaMundo* pantallaMundo;
	PantallaTimeUp* pantallaTimeUp;
	int numKoopa, numGoomba, numBloque, numMoneda, numPowerUp;
	int numVidasMario, numMonedasMario, puntosMario, record;
	int world, level;
	float gameTime;
	bool finTiempo, win, inmunidadMario, first;
	string estadoMario;

};


#endif // _SCENE_INCLUDE

