#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Audio.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10


Scene::Scene()
{
	map = NULL;
	player = NULL;
	goomba = NULL;
	koopa = NULL;
	bloque = NULL;
	banner = NULL;
	moneda = NULL;
	powerUp = NULL;
	pantallaMundo = NULL;
	pantallaTimeUp = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if(goomba != NULL)
		delete goomba;
	if (koopa != NULL)
		delete koopa;
	if (bloque != NULL)
		delete bloque;
	if (banner != NULL)
		delete banner;
	if (moneda != NULL)
		delete moneda;
	if (powerUp != NULL)
		delete powerUp;
	if (pantallaMundo != NULL)
		delete pantallaMundo;
	if (pantallaTimeUp != NULL)
		delete pantallaTimeUp;
}


void Scene::init()
{
	initShaders();
	banner = new Banner();
	banner->init(glm::ivec2(0, 0), texProgram);

	//Preparo nivel
	world = 1;
	level = 1;
	first = true;
	estadoMario = "MARIO";
	changeLevel("level01");
	banner->setLevel(world, level);
	banner->setPoints(0);
	numMonedasMario = 0;
	numVidasMario = 3;

	gameTime = 0;
	record = 0;
	finTiempo = false;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;

}

void Scene::restart() 
{
	world = 1;
	level = 1;
	numKoopa = 0;
	numGoomba = 0;
	numBloque = 0;
	numMoneda = 0;
	numPowerUp = 0;

	numMonedasMario = 0;
	numVidasMario = 3;
	gameTime = 0;
	puntosMario = 0;
	finTiempo = false;
	win = false;

	changeLevel("mundo");
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	pos_camara = 0;
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	gameTime += deltaTime;
	if (goomba != NULL) for (int i = 0; i < numGoomba; ++i) goomba[i].update(deltaTime);
	if (koopa != NULL) for (int i = 0; i < numKoopa; ++i) koopa[i].update(deltaTime);
	if (bloque != NULL) for (int i = 0; i < numBloque; ++i) bloque[i].update(deltaTime);
	if (moneda != NULL) for (int i = 0; i < numMoneda; ++i) moneda[i].update(deltaTime);
	if (powerUp != NULL) for (int i = 0; i < numPowerUp; ++i) powerUp[i].update(deltaTime);
	if (player != NULL) player->update(deltaTime, pos_camara);
	if (pantallaMundo != NULL) pantallaMundo->update(deltaTime, pos_camara);
	if (pantallaTimeUp != NULL) pantallaTimeUp->update(deltaTime, pos_camara);
	if (banner != NULL) banner->update(deltaTime, pos_camara);

	glm::vec2 posMario;
	glm::vec2 tamM;
	if (player != NULL)
	{
		posMario = player->getPos();
		tamM = player->getTam();
		estadoMario = player->getEstado();
		inmunidadMario = player->esInmune();
		player->setLevel(level);
	}

	//MUSICA	 

	//ATAJOS
	if (player != NULL) {
		if (Game::instance().getKey('m') || Game::instance().getKey('M'))
		{
			if (estadoMario == "SUPERMARIO") player->cambiaEstado("SUPERMARIO", "MARIO");
			else if (estadoMario == "MARIO") player->cambiaEstado("MARIO", "SUPERMARIO");
			else if (estadoMario == "STARSUPERMARIO") player->cambiaEstado("STARSUPERMARIO", "STARMARIO");
			else if (estadoMario == "STARMARIO") player->cambiaEstado("STARMARIO", "STARSUPERMARIO");

			Game::instance().keyReleased('M');
			Game::instance().keyReleased('m');
		}

		if (Game::instance().getKey('g') || Game::instance().getKey('G'))
		{
			if (estadoMario == "SUPERMARIO") player->cambiaEstado("SUPERMARIO", "STARSUPERMARIO");
			else if (estadoMario == "MARIO") player->cambiaEstado("MARIO", "STARMARIO");
			else if (estadoMario == "STARSUPERMARIO") player->cambiaEstado("STARSUPERMARIO", "SUPERMARIO");
			else if (estadoMario == "STARMARIO") player->cambiaEstado("STARMARIO", "MARIO");

			Game::instance().keyReleased('G');
			Game::instance().keyReleased('g');
		}
	}
	if (Game::instance().getKey('1'))
	{
		level = 1;
		changeLevel("mundo");
		Game::instance().keyReleased('1');
	}
	if (Game::instance().getKey('2'))
	{
		level = 2;
		changeLevel("mundo");
		Game::instance().keyReleased('2');
	}
	
	//BLOQUES
	if (bloque != NULL)
	{
		glm::vec2 posBloque;
		bool not_collision = true;
		for (int i = 0; i < numBloque && not_collision; ++i)
		{
			posBloque = bloque[i].getPos();
			bool activo = bloque[i].getActivo();
			glm::vec2 colMario = posMario+glm::vec2(tamM.x/2.f, 0.0f);
			glm::vec2 colTamMario = glm::vec2(1.f, tamM.y);
			if (collisionPlayerEnemy(colMario, colTamMario, posBloque, glm::ivec2(32, 32)) && activo) {
				if (posMario.y > posBloque.y) {
					bloque[i].setHit(estadoMario);
					player->setBloque("UP", posBloque);
				}
				else if (posMario.y < posBloque.y + 32) player->setBloque("DOWN", posBloque);
				else if (posMario.x < posBloque.x) player->setBloque("RIGHT", posBloque);
				else if (posMario.x > posBloque.x) player->setBloque("LEFT", posBloque);
				not_collision = false;
			}
			else player->setBloque("NONE", posBloque);
		}
	}

	//MONEDAS
	if (moneda != NULL)
	{
		glm::vec2 posMoneda;
		bool not_collision = true;
		for (int i = 0; i < numMoneda; ++i)
		{
			posMoneda = moneda[i].getPos();
			bool activo = moneda[i].getActivo();
			glm::vec2 colMario = posMario + glm::vec2(tamM.x / 2.f, 0.0f);
			glm::vec2 colTamMario = glm::vec2(1.f, tamM.y);
			if (collisionPlayerEnemy(colMario, colTamMario, posMoneda, glm::ivec2(32, 32)) && activo) {
				//printf("colision %d ", i);
				if (posMario.y >= posMoneda.y) {
					printf("Moneda");
					Audio::instance().coinEffect();
					moneda[i].setHit();
					numMonedasMario += 1;
					puntosMario += 200;
					if (numMonedasMario > 100 && numVidasMario < 100)
					{
						numMonedasMario = 0;
						numVidasMario += 1;
					}
					else if (numMonedasMario > 100) numMonedasMario = 99;
				}
			}
		}
	}

	//POWER-UP
	if (powerUp != NULL)
	{
		glm::vec2 posPowerUp;
		for (int i = 0; i < numPowerUp; ++i)
		{
			//POWERUP - MARIO
			posPowerUp = powerUp[i].getPos();
			bool powerUpActivo = powerUp[i].getActivo();
			bool fueraBloque = powerUp[i].getFueraBloque();
			glm::vec2 colMario = posMario + glm::vec2(tamM.x / 2.f, 0.0f);
			glm::vec2 colTamMario = glm::vec2(1.f, tamM.y);
			if (collisionPlayerEnemy(colMario, colTamMario, posPowerUp, glm::ivec2(32, 32)) && powerUpActivo) {
				if (posMario.y >= posPowerUp.y && !fueraBloque) {
					powerUp[i].setHit("MARIO", posMario);
				}
				else if (fueraBloque){
					string tipo = powerUp[i].getTipo();
					string estadoMario = player->getEstado();
					powerUp[i].setDesactivar();

					if (tipo == "SETA") {
						puntosMario += 1000;
						Audio::instance().powerupEffect();
						if (estadoMario == "MARIO") player->cambiaEstado("MARIO", "SUPERMARIO");
						else if (estadoMario == "STARMARIO") player->cambiaEstado("STARMARIO", "STARSUPERMARIO");
					}
					else {
						puntosMario += 1500;
						if (estadoMario == "MARIO") player->cambiaEstado("MARIO", "STARMARIO");
						else if (estadoMario == "SUPERMARIO") player->cambiaEstado("SUPERMARIO", "STARSUPERMARIO");
					}
				}
			}

			//POWERUP - BLOQUE
			glm::vec2 posBloque;
			bool not_collision = true;

			if (fueraBloque) {
				for (int j = 0; j < numBloque && not_collision; ++j) {
					posBloque = bloque[j].getPos();
					bool bloqueActivo = bloque[j].getActivo();
					if (collisionPlayerEnemy(glm::vec2(posPowerUp.x, posPowerUp.y + 2), glm::ivec2(32, 32), posBloque, glm::ivec2(32, 32)) && powerUpActivo && bloqueActivo) {
						powerUp[i].setHit("DOWN", posBloque);
						not_collision = false;
					}
				}
			}
		}
	}

	//GOOMBA
	if (goomba != NULL)
	{
		for (int i = 0; i < numGoomba; ++i)
		{
			if (!goomba[i].isDying()) {
				glm::vec2 posGoomba = goomba[i].getPos();
				glm::vec2 tamG = goomba[i].getTam();
				if (!player->isDying() && collisionPlayerEnemy(posMario, tamM, posGoomba, tamG)) {
					//Mario mata goomba
					if (posMario.y + (tamM.y/2) + 1 < posGoomba.y) {
						goomba[i].morint();
						Audio::instance().squishEffect();
						player->jump();
						puntosMario += 100;
					}
					else if (estadoMario == "STARMARIO" || estadoMario == "STARSUPERMARIO") {
						Audio::instance().kickEffect();
						goomba[i].morintkoopa();
					}
					else if (estadoMario == "SUPERMARIO") player->cambiaEstado(estadoMario, "MARIO");
					else if (estadoMario == "MARIO" && !inmunidadMario) {
						if (numVidasMario > 1) Audio::instance().dieEffect();
						player->morirsalto();
						numVidasMario -= 1;
					}
				}
				//collision goomba con los otros goombas
				for (int j = 0; j < numGoomba; ++j) {
					if (j != i) {
						if (!goomba[j].isDying()) {
							glm::vec2 posGoomba2 = goomba[j].getPos();
							if (collisionPlayerEnemy(posGoomba, tamG, posGoomba2, tamG)) {
								goomba[j].changeDirection();
							}
						}
					}
				}
				//collision goomba con koopa
				for (int j = 0; j < numKoopa; ++j) {
					glm::vec2 posKoopa = koopa[j].getPos();
					glm::vec2 tamK = koopa[j].getTam();
					if (!koopa[j].isDying() && collisionPlayerEnemy(posGoomba, tamG, posKoopa, tamK)) {
							if (!koopa[j].isShell()) {
								goomba[i].changeDirection();
								koopa[j].changeDirection();
							}
							else if (koopa[j].isShell() && !koopa[j].ismoveShell()) {
								goomba[i].changeDirection();
							}
							else {
								Audio::instance().kickEffect();
								goomba[i].morintkoopa();
								puntosMario += 200;
							}
					}
				}
			}
		}
	}
	
	//KOOPA
	if (koopa != NULL)
	{
		for (int i = 0; i < numKoopa; ++i)
		{
			if (!koopa[i].isDying()) {
				glm::vec2 posKoopa = koopa[i].getPos();
				glm::vec2 tamK = koopa[i].getTam();
				//Koppa con Mario
				if (!player->isDying() && collisionPlayerEnemy(posMario, tamM, posKoopa, tamK)) {
					if ((posMario.y + tamM.y / 2) + 1 <= posKoopa.y) {
						player->jump();
						if (koopa[i].isShell()) {
							Audio::instance().kickEffect();
							bool left = posKoopa.x < posMario.x;
							koopa[i].moveShell(left);
						}
						else {
							Audio::instance().squishEffect();
							koopa[i].transformToShell();
							puntosMario += 100;
						}
					}
					else if (estadoMario == "STARMARIO" || estadoMario == "STARSUPERMARIO") {
						Audio::instance().kickEffect();
						koopa[i].transformToShell();
						koopa[i].morintKoopa();
					}
					else if (estadoMario == "SUPERMARIO") {
						player->cambiaEstado(estadoMario, "MARIO");
					}
					else if (estadoMario == "MARIO" && !inmunidadMario) {
						if (koopa[i].isShell() && !koopa[i].ismoveShell()) {
							Audio::instance().kickEffect();
							bool left = posKoopa.x < posMario.x;
							koopa[i].moveShell(left);
							puntosMario += 100;
						}
						//Caparazon mata Mario
						else
						{
							if (numVidasMario > 1) Audio::instance().dieEffect();
							player->morirsalto();
							numVidasMario -= 1;
						}
					}
				}
				//collision koopa con otros koopa
				for (int j = 0; j < numKoopa; ++j) {
					if (i != j) {
						glm::vec2 posKoopa2 = koopa[j].getPos();
						glm::vec2 tamK2 = koopa[j].getTam();
						if (collisionPlayerEnemy(posKoopa, tamK, posKoopa2, tamK2)) {
							if (!koopa[j].isShell() && !koopa[i].isShell()) {
								koopa[i].changeDirection();
							}
							else if (koopa[j].isShell() && !koopa[j].ismoveShell() && !koopa[i].isShell()) {
								koopa[i].changeDirection();
							}
							else if (koopa[j].ismoveShell() && !koopa[i].isShell()) {
								puntosMario += 200;
								Audio::instance().kickEffect();
								koopa[i].transformToShell();
								koopa[i].morintKoopa();
								printf("morintkoopa");
							}
							else if (koopa[j].ismoveShell() && koopa[i].isShell() && !koopa[i].ismoveShell()) {
								koopa[i].morintKoopa();
								puntosMario += 200;
								printf("morintkoopa");
							}
						}
					}
				}
			}
		}
	}

	/*
	bool falling = player->isFalling();
		bool rebooted = player->isRebooted();
		bool win = player->isActive();
		if (falling) --numVidasMario;
		if (rebooted) {
			if (finTiempo) changeLevel("timeUp");
			else changeLevel("mundo");

			banner->setTime(-1.f);
			
			gameTime = 0;
		}
		else {
			banner->setTime(gameTime);
		}
		if (win) {
			gameTime = 0;
			changeLevel("mundo");
		}
	*/

	//PLAYER
	if (player != NULL) {
		bool falling = player->isFalling();
		bool rebooted = player->isRebooted();
		bool winning = player->isWin();
		if (falling) {
			player->cambiaEstado(estadoMario, "MARIO");
			Audio::instance().dieEffect();
			--numVidasMario;
		}
		if (rebooted) {
			if (finTiempo) {
				changeLevel("timeUp");
				Audio::instance().dieEffect();
			}
			else changeLevel("mundo");

			banner->setTime(-1.f);
			
			gameTime = 0;
		}
		else {
			banner->setTime(gameTime);
		}

		//MIRAR SI GANAMOS
		if (winning) {
			estadoMario = player->getEstado();
			if (level == 1) {
				//player->setWin();
				level = 2;
				changeLevel("mundo");
				gameTime = 0;
			}
			else if (level = 2) {
				Audio::instance().playWin();
				win = true;
			}
		}
	}
	else {
		if ((2 - int(gameTime / 1000) < 0))
		{
			if (pantallaMundo != NULL) changeLevel("level0" + to_string(level));
			else if (pantallaTimeUp != NULL) changeLevel("mundo");

			gameTime = 0;
			
			banner->setTime(-1.f);
		}
	}

	if (banner != NULL) {
		banner->setLevel(world, level);
		//banner->setTime(gameTime);
		banner->setPoints(puntosMario);
		banner->setMonedas(numMonedasMario);
	}

	if (pantallaMundo != NULL) {
		pantallaMundo->setLevel(world, level);
		pantallaMundo->setVides(numVidasMario);
	}

	//FIN TIEMPO
	int time = 400 - int(gameTime / 1000);
	if (time <= 0 && !player->isDying()) {
		player->morirsalto();
		numVidasMario -= 1;
		finTiempo = true;
	}

	//SCROLL
	float cuadrant = posMario.x - pos_camara;
	if (cuadrant >= SCREEN_WIDTH / 3) {
		if (player != NULL) pos_camara += player->getVel();
		projection = glm::ortho(pos_camara, float(SCREEN_WIDTH) + pos_camara, float(SCREEN_HEIGHT), 0.f);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	
	if (goomba != NULL) for (int i = 0; i < numGoomba; ++i) goomba[i].render();
	if (koopa != NULL) for (int i = 0; i < numKoopa; ++i) koopa[i].render();
	if (moneda != NULL) for (int i = 0; i < numMoneda; ++i) moneda[i].render();
	if (powerUp != NULL) for (int i = 0; i < numPowerUp; ++i) powerUp[i].render();
	if (bloque != NULL) for (int i = 0; i < numBloque; ++i) bloque[i].render();
	
	//text->render(); 
	if (player != NULL) player->render();
	if (pantallaMundo != NULL) pantallaMundo->render();
	if (pantallaTimeUp != NULL) pantallaTimeUp->render();
	banner->render();
}

void Scene::changeLevel(string lvl)
{
	Audio::instance().stopMusic();
	//Elimino los elementos anteriores
	player = NULL;
	goomba = NULL;
	koopa = NULL;
	bloque = NULL;
	moneda = NULL;
	powerUp = NULL;
	pantallaMundo = NULL;
	pantallaTimeUp = NULL;
	numKoopa = 0;
	numGoomba = 0;
	numBloque = 0;
	numMoneda = 0;

	pos_camara = 0;
	//printf("changelevel");

	//Cambio de nivel
	if (lvl == "mundo") {
		pantallaMundo = new PantallaMundo();
		pantallaMundo->init(glm::ivec2(0, 0), texProgram);
	}
	else if (lvl == "timeUp") {
		pantallaTimeUp = new PantallaTimeUp;
		pantallaTimeUp->init(glm::ivec2(0, 0), texProgram);
	}
	else {
		if (lvl == "level01") {
			world = 1;
			level = 1;
			if (first) first = false;
			else Audio::instance().playLevel(1);
			map = TileMap::createTileMap("levels/level01.txt", "levels/objects01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		}
		else if (lvl == "level02") {
			world = 1;
			level = 2;
			Audio::instance().playLevel(2);
			map = TileMap::createTileMap("levels/level02.txt", "levels/objects02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		}

		//PLAYER
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		if (lvl == "level02") player->cambiaEstado("MARIO", estadoMario);

		//GOOMBAS
		vector<pair<int, int>> posGoombas = map->getPosObj("GOOMBAS");
		numGoomba = posGoombas.size();
		//printf(" Numero goombas: %d", numGoomba);
		if (numGoomba > 0)
		{
			goomba = new Goomba[numGoomba];
			for (int i = 0; i < numGoomba; ++i)
			{
				goomba[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				goomba[i].setPosition(glm::vec2(posGoombas[i].first * map->getTileSize(), posGoombas[i].second * map->getTileSize()));
				goomba[i].setTileMap(map);
			}
		}
		
		//KOOPAS
		vector<pair<int, int>> posKoopas = map->getPosObj("KOOPAS");
		numKoopa = posKoopas.size();
		//printf(" Numero koopas: %d", numKoopa);
		if (numKoopa > 0)
		{
			koopa = new Koopa[numKoopa];
			for (int i = 0; i < numKoopa; ++i)
			{
				koopa[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				koopa[i].setPosition(glm::vec2((posKoopas[i].first+0.5f) * map->getTileSize(), (posKoopas[i].second + 0.5f) * map->getTileSize()));
				koopa[i].setTileMap(map);
			}
		}
		
		//BLOQUES
		vector<pair<int, int>> posInterrogantes = map->getPosObj("INTERROGANTES");
		vector<pair<int, int>> posLadrillos = map->getPosObj("LADRILLOS");
		int numI = posInterrogantes.size();
		int numL = posLadrillos.size();
		numBloque = numI + numL;
		if (numBloque > 0)
		{
			bloque = new Bloque[numBloque];
			int i = 0;
			for (int j = 0; j < numI; ++j)
			{
				bloque[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "INTERROGANTE");
				bloque[i].setPosition(glm::vec2(posInterrogantes[j].first * map->getTileSize(), posInterrogantes[j].second * map->getTileSize()));
				bloque[i].setTileMap(map);
				++i;
			}
			for (int j = 0; j < numL; ++j)
			{
				bloque[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "LADRILLO");
				bloque[i].setPosition(glm::vec2(posLadrillos[j].first * map->getTileSize(), posLadrillos[j].second * map->getTileSize()));
				bloque[i].setTileMap(map);
				++i;
			}
		}

		//MONEDAS
		vector<pair<int, int>> posMonedas = map->getPosObj("MONEDAS");
		numMoneda = posMonedas.size();
		//printf(" Numero koopas: %d", numMoneda);
		if (numMoneda > 0)
		{
			moneda = new Moneda[numMoneda];
			for (int i = 0; i < numMoneda; ++i)
			{
				moneda[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				moneda[i].setPosition(glm::vec2((posMonedas[i].first + 0.25f ) * map->getTileSize(), posMonedas[i].second * map->getTileSize()));
				moneda[i].setTileMap(map);
			}
		}
		//pos_camara = 0;

		//PWERUPS
		vector<pair<int, int>> posSetas = map->getPosObj("SETAS");
		vector<pair<int, int>> posEstrellas = map->getPosObj("ESTRELLAS");
		int numS = posSetas.size();
		int numE = posEstrellas.size();
		numPowerUp = numS + numE;
		//printf(" Numero koopas: %d", numMoneda);
		if (numPowerUp > 0)
		{
			powerUp = new PowerUp[numPowerUp];
			int i = 0;
			for (int j = 0; j < numS; ++j)
			{
				powerUp[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "SETA");
				powerUp[i].setPosition(glm::vec2((posSetas[j].first)* map->getTileSize(), posSetas[j].second* map->getTileSize()));
				powerUp[i].setTileMap(map);
				++i;
			}
			for (int j = 0; j < numE; ++j)
			{
				powerUp[i].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "STAR");
				powerUp[i].setPosition(glm::vec2((posEstrellas[j].first) * map->getTileSize(), posEstrellas[j].second * map->getTileSize()));
				powerUp[i].setTileMap(map);
				++i;
			}
		}

	}
	finTiempo = false;
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::collisionPlayerEnemy(glm::vec2 pos1, glm::vec2 tam1, glm::vec2 pos2, glm::vec2 tam2)
{
	float minx1 = pos1.x;
	float miny1 = pos1.y;
	float maxx1 = pos1.x + tam1.x;
	float maxy1 = pos1.y + tam1.y;
	float minx2 = pos2.x;
	float miny2 = pos2.y;
	float maxx2 = pos2.x + tam2.x;
	float maxy2 = pos2.y + tam2.y;
	if ((minx1 < maxx2) && (minx2 < maxx1) && (miny1 < maxy2) && (miny2 < maxy1)) return true;
	return false;
}

bool Scene::isGameOver() {
	if (numVidasMario <= 0)
	{
		if (puntosMario > record) record = puntosMario;
		Audio::instance().playGameOver();
		return true;
	}
	return false;
}

bool Scene::hasWon() {
	return win;
}

int Scene::getPoints() {
	return puntosMario;
}

int Scene::getMonedas() {
	return numMonedasMario;
}

int Scene::getWorld() {
	return world;
}

int Scene::getLevel() {
	return level;
}

int Scene::getRecord() {
	return record;
}