#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"12

#include "Text.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 128
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, 
	BEND_LEFT, BEND_RIGHT, TURN_LEFT, TURN_RIGHT, NONE, DEAD, POLE_LEFT, POLE_RIGHT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	jump_angle_step = 4;
	jump_height = 128;
	fall_step = 4;
	bJumping = false;
	dying = false;
	vel = 2;
	time = 0;
	active = true;
	rebooted = false;
	inmunidad = false;
	renderizar = true;
	mastil = false;
	win = false;
	starMario = false;
	aux = 0;
	finivel = false;
	change = true;
	change2 = true;
	change3 = true;

	//MARIO
	double tamM = 0.1;
	spritesheetMario.loadFromFile("images/Mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mario = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(tamM, 0.5), &spritesheetMario, &shaderProgram);
	mario->setNumberAnimations(14);

	mario->setAnimationSpeed(STAND_LEFT, 8);
	mario->addKeyframe(STAND_LEFT, glm::vec2(tamM*0, 0.5f));

	mario->setAnimationSpeed(STAND_RIGHT, 8);
	mario->addKeyframe(STAND_RIGHT, glm::vec2(tamM*0, 0.f));

	mario->setAnimationSpeed(MOVE_LEFT, 8);
	mario->addKeyframe(MOVE_LEFT, glm::vec2(tamM*1, 0.5f));
	mario->addKeyframe(MOVE_LEFT, glm::vec2(tamM*2, 0.5f));
	mario->addKeyframe(MOVE_LEFT, glm::vec2(tamM*3, 0.5f));

	mario->setAnimationSpeed(MOVE_RIGHT, 8);
	mario->addKeyframe(MOVE_RIGHT, glm::vec2(tamM*1, 0.f));
	mario->addKeyframe(MOVE_RIGHT, glm::vec2(tamM*2, 0.f));
	mario->addKeyframe(MOVE_RIGHT, glm::vec2(tamM*3, 0.f));

	mario->setAnimationSpeed(JUMP_LEFT, 8);
	mario->addKeyframe(JUMP_LEFT, glm::vec2(tamM*5, 0.5f));

	mario->setAnimationSpeed(JUMP_RIGHT, 8);
	mario->addKeyframe(JUMP_RIGHT, glm::vec2(tamM*5, 0.f));

	mario->setAnimationSpeed(BEND_LEFT, 8);
	mario->addKeyframe(BEND_LEFT, glm::vec2(tamM*0, 0.5f));

	mario->setAnimationSpeed(BEND_RIGHT, 8);
	mario->addKeyframe(BEND_RIGHT, glm::vec2(tamM*0, 0.f));

	mario->setAnimationSpeed(TURN_LEFT, 8);
	mario->addKeyframe(TURN_LEFT, glm::vec2(tamM * 4, 0.5f));

	mario->setAnimationSpeed(TURN_RIGHT, 8);
	mario->addKeyframe(TURN_RIGHT, glm::vec2(tamM * 4, 0.f));

	mario->setAnimationSpeed(POLE_LEFT, 8);
	mario->addKeyframe(POLE_LEFT, glm::vec2(tamM*7, 0.5f));

	mario->setAnimationSpeed(POLE_RIGHT, 8);
	mario->addKeyframe(POLE_RIGHT, glm::vec2(tamM * 7, 0.f));

	mario->setAnimationSpeed(NONE, 8);
	mario->addKeyframe(NONE, glm::vec2(tamM * 8, 0.5f));

	mario->setAnimationSpeed(DEAD, 8);
	mario->addKeyframe(DEAD, glm::vec2(tamM*6, 0.f));


	//SUPER MARIO
	double tamSM = 0.1;
	spritesheetSuperMario.loadFromFile("images/SuperMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	supermario = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(tamSM, 0.5), &spritesheetSuperMario, &shaderProgram);
	supermario->setNumberAnimations(13);

	supermario->setAnimationSpeed(STAND_LEFT, 8);
	supermario->addKeyframe(STAND_LEFT, glm::vec2(tamSM*0, 0.5f));

	supermario->setAnimationSpeed(STAND_RIGHT, 8);
	supermario->addKeyframe(STAND_RIGHT, glm::vec2(tamSM*0, 0.f));

	supermario->setAnimationSpeed(MOVE_LEFT, 8);
	supermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM*1, 0.5f));
	supermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM*2, 0.5f));
	supermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM*3, 0.5f));

	supermario->setAnimationSpeed(MOVE_RIGHT, 8);
	supermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM*1, 0.f));
	supermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM*2, 0.f));
	supermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM*3, 0.f));

	supermario->setAnimationSpeed(JUMP_LEFT, 8);
	supermario->addKeyframe(JUMP_LEFT, glm::vec2(tamSM*5, 0.5f));

	supermario->setAnimationSpeed(JUMP_RIGHT, 8);
	supermario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSM*5, 0.f));

	supermario->setAnimationSpeed(BEND_LEFT, 8);
	supermario->addKeyframe(BEND_LEFT, glm::vec2(tamSM*6, 0.5f));

	supermario->setAnimationSpeed(BEND_RIGHT, 8);
	supermario->addKeyframe(BEND_RIGHT, glm::vec2(tamSM*6, 0.f));

	//TURN LEFT AHORA ES POLE RIGHT PARA SUPER MARIO

	supermario->setAnimationSpeed(TURN_LEFT, 8);
	supermario->addKeyframe(TURN_LEFT, glm::vec2(0.7f, 0.f));

	supermario->setAnimationSpeed(TURN_RIGHT, 8);
	supermario->addKeyframe(TURN_RIGHT, glm::vec2(tamSM * 4, 0.f));

	supermario->setAnimationSpeed(POLE_LEFT, 8);
	supermario->addKeyframe(POLE_LEFT, glm::vec2(tamSM * 7, 0.5f));

	supermario->setAnimationSpeed(NONE, 8);
	supermario->addKeyframe(NONE, glm::vec2(tamSM*8, 0.f));

	//STARMARIO
	double tamSm = 0.1;
	double tamMY = 0.125f;
	spritesheetStarMario.loadFromFile("images/StarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starmario = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(tamSm, tamMY), &spritesheetStarMario, &shaderProgram);
	starmario->setNumberAnimations(14);

	starmario->setAnimationSpeed(STAND_LEFT, 8);
	starmario->addKeyframe(STAND_LEFT, glm::vec2(tamSm * 0, tamMY * 1));
	starmario->addKeyframe(STAND_LEFT, glm::vec2(tamSm * 0, tamMY * 3));
	starmario->addKeyframe(STAND_LEFT, glm::vec2(tamSm * 0, tamMY * 5));

	starmario->setAnimationSpeed(STAND_RIGHT, 8);
	starmario->addKeyframe(STAND_RIGHT, glm::vec2(tamSm * 0, tamMY * 0));
	starmario->addKeyframe(STAND_RIGHT, glm::vec2(tamSm * 0, tamMY * 2));
	starmario->addKeyframe(STAND_RIGHT, glm::vec2(tamSm * 0, tamMY * 4));

	starmario->setAnimationSpeed(MOVE_LEFT, 24);
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 1, tamMY * 1));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 1, tamMY * 3));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 1, tamMY * 5));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 2, tamMY * 1));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 2, tamMY * 3));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 2, tamMY * 5));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 3, tamMY * 1));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 3, tamMY * 3));
	starmario->addKeyframe(MOVE_LEFT, glm::vec2(tamSm * 3, tamMY * 5));

	starmario->setAnimationSpeed(MOVE_RIGHT, 24);
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 1, tamMY * 0));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 1, tamMY * 2));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 1, tamMY * 4));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 2, tamMY * 0));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 2, tamMY * 2));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 2, tamMY * 4));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 3, tamMY * 0));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 3, tamMY * 2));
	starmario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSm * 3, tamMY * 4));

	starmario->setAnimationSpeed(JUMP_LEFT, 8);
	starmario->addKeyframe(JUMP_LEFT, glm::vec2(tamSm * 5, tamMY * 1));
	starmario->addKeyframe(JUMP_LEFT, glm::vec2(tamSm * 5, tamMY * 3));
	starmario->addKeyframe(JUMP_LEFT, glm::vec2(tamSm * 5, tamMY * 5));

	starmario->setAnimationSpeed(JUMP_RIGHT, 8);
	starmario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSm * 5, tamMY * 0));
	starmario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSm * 5, tamMY * 2));
	starmario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSm * 5, tamMY * 4));

	starmario->setAnimationSpeed(BEND_LEFT, 8);
	starmario->addKeyframe(BEND_LEFT, glm::vec2(tamSm * 0, tamMY * 1));
	starmario->addKeyframe(BEND_LEFT, glm::vec2(tamSm * 0, tamMY * 3));
	starmario->addKeyframe(BEND_LEFT, glm::vec2(tamSm * 0, tamMY * 5));

	starmario->setAnimationSpeed(BEND_RIGHT, 8);
	starmario->addKeyframe(BEND_RIGHT, glm::vec2(tamSm * 0, tamMY * 0));
	starmario->addKeyframe(BEND_RIGHT, glm::vec2(tamSm * 0, tamMY * 2));
	starmario->addKeyframe(BEND_RIGHT, glm::vec2(tamSm * 0, tamMY * 4));

	starmario->setAnimationSpeed(TURN_LEFT, 8);
	starmario->addKeyframe(TURN_LEFT, glm::vec2(tamSm * 4, tamMY * 1));
	starmario->addKeyframe(TURN_LEFT, glm::vec2(tamSm * 4, tamMY * 3));
	starmario->addKeyframe(TURN_LEFT, glm::vec2(tamSm * 4, tamMY * 5));

	starmario->setAnimationSpeed(TURN_RIGHT, 8);
	starmario->addKeyframe(TURN_RIGHT, glm::vec2(tamSm * 4, tamMY * 0));
	starmario->addKeyframe(TURN_RIGHT, glm::vec2(tamSm * 4, tamMY * 2));
	starmario->addKeyframe(TURN_RIGHT, glm::vec2(tamSm * 4, tamMY * 4));

	starmario->setAnimationSpeed(POLE_LEFT, 8);
	starmario->addKeyframe(POLE_LEFT, glm::vec2(tamSm * 7, tamMY * 1));
	starmario->addKeyframe(POLE_LEFT, glm::vec2(tamSm * 7, tamMY * 3));
	starmario->addKeyframe(POLE_LEFT, glm::vec2(tamSm * 7, tamMY * 5));

	starmario->setAnimationSpeed(POLE_RIGHT, 8);
	starmario->addKeyframe(POLE_RIGHT, glm::vec2(tamSm * 7, tamMY * 0));
	starmario->addKeyframe(POLE_RIGHT, glm::vec2(tamSm * 7, tamMY * 2));
	starmario->addKeyframe(POLE_RIGHT, glm::vec2(tamSm * 7, tamMY * 4));

	starmario->setAnimationSpeed(NONE, 8);
	starmario->addKeyframe(NONE, glm::vec2(tamSm * 8, tamMY));

	starmario->setAnimationSpeed(DEAD, 8);
	starmario->addKeyframe(DEAD, glm::vec2(tamSm * 6, tamMY));

	//STAR SUPER MARIO
	tamSM = 0.1;
	spritesheetStarSuperMario.loadFromFile("images/StarSuperMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starsupermario = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(tamSM, tamMY), &spritesheetStarSuperMario, &shaderProgram);
	starsupermario->setNumberAnimations(13);

	starsupermario->setAnimationSpeed(STAND_LEFT, 8);
	starsupermario->addKeyframe(STAND_LEFT, glm::vec2(tamSM * 0, tamMY * 1));
	starsupermario->addKeyframe(STAND_LEFT, glm::vec2(tamSM * 0, tamMY * 3));
	starsupermario->addKeyframe(STAND_LEFT, glm::vec2(tamSM * 0, tamMY * 5));

	starsupermario->setAnimationSpeed(STAND_RIGHT, 8);
	starsupermario->addKeyframe(STAND_RIGHT, glm::vec2(tamSM * 0, tamMY * 0));
	starsupermario->addKeyframe(STAND_RIGHT, glm::vec2(tamSM * 0, tamMY * 2));
	starsupermario->addKeyframe(STAND_RIGHT, glm::vec2(tamSM * 0, tamMY * 4));

	starsupermario->setAnimationSpeed(MOVE_LEFT, 24);
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 1, tamMY * 1));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 1, tamMY * 3));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 1, tamMY * 5));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 2, tamMY * 1));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 2, tamMY * 3));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 2, tamMY * 5));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 3, tamMY * 1));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 3, tamMY * 3));
	starsupermario->addKeyframe(MOVE_LEFT, glm::vec2(tamSM * 3, tamMY * 5));

	starsupermario->setAnimationSpeed(MOVE_RIGHT, 24);
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 1, tamMY * 0));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 1, tamMY * 2));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 1, tamMY * 4));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 2, tamMY * 0));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 2, tamMY * 2));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 2, tamMY * 4));

	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 3, tamMY * 0));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 3, tamMY * 2));
	starsupermario->addKeyframe(MOVE_RIGHT, glm::vec2(tamSM * 3, tamMY * 4));

	starsupermario->setAnimationSpeed(JUMP_LEFT, 8);
	starsupermario->addKeyframe(JUMP_LEFT, glm::vec2(tamSM * 5, tamMY * 1));
	starsupermario->addKeyframe(JUMP_LEFT, glm::vec2(tamSM * 5, tamMY * 3));
	starsupermario->addKeyframe(JUMP_LEFT, glm::vec2(tamSM * 5, tamMY * 5));

	starsupermario->setAnimationSpeed(JUMP_RIGHT, 8);
	starsupermario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSM * 5, tamMY * 0));
	starsupermario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSM * 5, tamMY * 2));
	starsupermario->addKeyframe(JUMP_RIGHT, glm::vec2(tamSM * 5, tamMY * 4));

	starsupermario->setAnimationSpeed(BEND_LEFT, 8);
	starsupermario->addKeyframe(BEND_LEFT, glm::vec2(tamSM * 6, tamMY * 1));
	starsupermario->addKeyframe(BEND_LEFT, glm::vec2(tamSM * 6, tamMY * 3));
	starsupermario->addKeyframe(BEND_LEFT, glm::vec2(tamSM * 6, tamMY * 5));

	starsupermario->setAnimationSpeed(BEND_RIGHT, 8);
	starsupermario->addKeyframe(BEND_RIGHT, glm::vec2(tamSM * 6, tamMY * 0));
	starsupermario->addKeyframe(BEND_RIGHT, glm::vec2(tamSM * 6, tamMY * 2));
	starsupermario->addKeyframe(BEND_RIGHT, glm::vec2(tamSM * 6, tamMY * 4));

	starsupermario->setAnimationSpeed(TURN_LEFT, 8);
	starsupermario->addKeyframe(TURN_LEFT, glm::vec2(tamSM * 4, tamMY * 1));
	starsupermario->addKeyframe(TURN_LEFT, glm::vec2(tamSM * 4, tamMY * 3));
	starsupermario->addKeyframe(TURN_LEFT, glm::vec2(tamSM * 4, tamMY * 5));

	starsupermario->setAnimationSpeed(TURN_RIGHT, 8);
	starsupermario->addKeyframe(TURN_RIGHT, glm::vec2(tamSM * 4, tamMY * 0));
	starsupermario->addKeyframe(TURN_RIGHT, glm::vec2(tamSM * 4, tamMY * 2));
	starsupermario->addKeyframe(TURN_RIGHT, glm::vec2(tamSM * 4, tamMY * 4));

	starsupermario->setAnimationSpeed(POLE_RIGHT, 8);
	starsupermario->addKeyframe(POLE_RIGHT, glm::vec2(tamSM * 7, tamMY * 0));
	starsupermario->addKeyframe(POLE_RIGHT, glm::vec2(tamSM * 7, tamMY * 2));
	starsupermario->addKeyframe(POLE_RIGHT, glm::vec2(tamSM * 7, tamMY * 4));

	starsupermario->setAnimationSpeed(POLE_LEFT, 8);
	starsupermario->addKeyframe(POLE_LEFT, glm::vec2(tamSM * 7, tamMY * 1));
	starsupermario->addKeyframe(POLE_LEFT, glm::vec2(tamSM * 7, tamMY * 3));
	starsupermario->addKeyframe(POLE_LEFT, glm::vec2(tamSM * 7, tamMY * 4));

	starsupermario->setAnimationSpeed(NONE, 8);
	starsupermario->addKeyframe(NONE, glm::vec2(tamSM * 8, tamMY * 0));

	mario->changeAnimation(STAND_RIGHT);
	supermario->changeAnimation(NONE);
	starmario->changeAnimation(NONE);
	starsupermario->changeAnimation(NONE);
	sprite = mario;
	tamPlayer = glm::ivec2(32, 32); //Tama�o Mario
	first = true;


}

void Player::update(int deltaTime, float poscam)
{
	printf("POS PLAYER TILES: %d, %d", posPlayer.x / 32, posPlayer.y / 32);
	if (active) {
		if (mastil)
		{
			if (first) {
				Audio::instance().flagpoleEffect();
				first = false;
			}
			if (posPlayer.y + tamPlayer.y <= 384) {
				printf("posPlayer.y = %d \n", posPlayer.y);
				posPlayer.y += 2;
			}
			else if (sprite->animation() != MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
			else if (posPlayer.y + tamPlayer.y >= 384) {
				if (!map->collisionMoveDown(posPlayer, tamPlayer, &posPlayer.y)) posPlayer.y += 2;
			    posPlayer.x += 2;
			}
			if (posPlayer.x >= 226 * 32){
				win = true;
				//mastil = false;
			}
			sprite->update(deltaTime);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 16), float(tileMapDispl.y + posPlayer.y)));
		}
		else {
			sprite->update(deltaTime);
			//printf("Pos Player y: %d \n", posPlayer.y);
			//se va del mapa

			//Animaci�n aterrizaje salto (JUMP_X --> STAND_X)
			if (!bJumping && sprite->animation() == JUMP_LEFT) sprite->changeAnimation(STAND_LEFT);
			if (!bJumping && sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(STAND_RIGHT);

			//Control aceleraci�n
			if (Game::instance().getKey('r') || Game::instance().getKey('R'))
			{
				//if (vel == 2) vel = 5;
				vel = 4;
			}
			//else if ((!Game::instance().getSpecialKey(GLUT_KEY_LEFT) || !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getSpecialKey(GLUT_KEY_DOWN))) vel = 2;
			else vel = 2;


			if (!dying) {
				//Caer
				if (posPlayer.y > 14 * tamPlayer.x) {
					posPlayer.y = 14 * tamPlayer.x;
					dying = true;
					falling = true;
					if (sprite == mario) sprite->changeAnimation(DEAD);
				}

				if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
				{
					if (bJumping && change) {
						change = false;
						sprite->changeAnimation(JUMP_LEFT);
					}
					else change = true;
					if (sprite->animation() != MOVE_LEFT && change3) {
						sprite->changeAnimation(MOVE_LEFT);
						change3 = false;
					}
					else change3 = true;

					if (posPlayer.x > poscam - float(vel)) posPlayer.x -= vel;
					//else posPlayer.x = poscam;

					if (map->collisionMoveLeft(posPlayer, tamPlayer) || hitBloque == "LEFT")
					{
						posPlayer.x += vel;
						sprite->changeAnimation(STAND_LEFT);
						//printf("Colision Left");
					}
				}
				//Tecla derecha
				else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || finivel)
				{
					if (bJumping && change2) {
						change2 = false;
						sprite->changeAnimation(JUMP_RIGHT);
					}
				    else if (!bJumping) change2 = true;
					if (sprite->animation() != MOVE_RIGHT && change2)
						sprite->changeAnimation(MOVE_RIGHT);

					posPlayer.x += vel;

					if (map->collisionMoveRight(posPlayer, tamPlayer) || hitBloque == "RIGHT")
					{
						posPlayer.x -= vel;
						sprite->changeAnimation(STAND_RIGHT);
					}
				}
				//Tecla abajo
				else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
				{
					if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(BEND_LEFT);
					else if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(BEND_RIGHT);
				}
				else
				{
					if (sprite->animation() == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					else if (sprite->animation() == BEND_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == BEND_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				}

			}

			//Salto con tecla arriba (KEY_UP --> bJumping; no KEY_UP --> !bJumping)
			if (bJumping)
			{
				//Animaci�n salto (STAND_X --> JUMP_X)
				sprite->update(deltaTime);
				if (!dying) {
					if (sprite->animation() == STAND_LEFT) {
						sprite->changeAnimation(JUMP_LEFT);
					}
					if (sprite->animation() == STAND_RIGHT) {
						sprite->changeAnimation(JUMP_RIGHT);
					}
				}
				else {
					if (sprite == mario) sprite->changeAnimation(DEAD);
				}

				jumpAngle += jump_angle_step;
				if (jumpAngle == 180)
				{
					bJumping = false;
					posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY - jump_height * sin(3.14159f * jumpAngle / 180.f));
					if (!dying && jumpAngle > 90) {
						bJumping = !map->collisionMoveDown(posPlayer, tamPlayer, &posPlayer.y);
						if (hitBloque == "DOWN")
						{
							bJumping = false;
						}
					}
					else {
						//jumpAngle = 180 - jumpAngle;
						bJumping = !map->collisionMoveUp(posPlayer, tamPlayer);
						if (hitBloque == "UP") {
							bJumping = false;
						}
					}
				}
			}
			else
			{
				//change = true;
				posPlayer.y += fall_step;
				if (!dying && map->collisionMoveDown(posPlayer, tamPlayer, &posPlayer.y) || hitBloque == "DOWN")
				{
					//Activa el salto
					if (Game::instance().getKey(' '))
					{
						Audio::instance().jumpEffect();
						bJumping = true;
						jumpAngle = 0;
						startY = posPlayer.y;
					}
				}
			}

			if (dying) {
				time++;
				if (time == 100) {
					rebooted = true;
					time = 0;
					active = false;
				}

			}

			if (map->collisionMastil(posPlayer, tamPlayer)) {
     			mastil = true;
				if (sprite == mario) sprite->changeAnimation(POLE_RIGHT);
				else sprite->changeAnimation(TURN_LEFT);
			}

			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
	}

	if (inmunidad) {
		timeinm++;
		if (timeinm == 150) {
			inmunidad = false;
			printf("se acabo inmunidad");
			timeinm = 0;
			renderizar = true;
		}
		else if (timeinm % 8 == 0) {
			++aux;
		}
		else if (aux%2) renderizar = false;
		else renderizar = true;
	}

	if (starmario) {
		timeStar++;
		if (timeStar == 600) {
			starMario = false;
			if (getEstado() == "STARMARIO") cambiaEstado("STARMARIO", "MARIO");
			else if (getEstado() == "STARSUPERMARIO") cambiaEstado("STARSUPERMARIO", "SUPERMARIO");
		}
	}
}

void Player::render()
{
	if (active && renderizar) sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::setSprite(string tipoMario)
{
	if (tipoMario == "mario") {
		starMario = false;
		mario->changeAnimation(sprite->animation());
		supermario->changeAnimation(NONE);
		starmario->changeAnimation(NONE);
		starsupermario->changeAnimation(NONE);

		mario->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		sprite = mario;
		tamPlayer = glm::ivec2(32, 32); //Tama�o Mario
	}
	else if (tipoMario == "supermario")
	{
		starMario = false;
		supermario->changeAnimation(sprite->animation());
		mario->changeAnimation(NONE);
		starmario->changeAnimation(NONE);
		starsupermario->changeAnimation(NONE);

		posPlayer.y -= 32; //POSIBLE CAUSA DE ERRORES???
		supermario->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		sprite = supermario;
		tamPlayer = glm::ivec2(32, 64); //Tama�o Super Mario
	}
	else if (tipoMario == "starmario") 
	{
		starMario = true;
		timeStar = 0;
		starmario->changeAnimation(sprite->animation());
		mario->changeAnimation(NONE);
		supermario->changeAnimation(NONE);
		starsupermario->changeAnimation(NONE);

		starmario->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		sprite = starmario;
		tamPlayer = glm::ivec2(32, 32); //Tama�o Mario
	}
	else if (tipoMario == "starsupermario") 
	{
		starMario = true;
		timeStar = 0;
		starsupermario->changeAnimation(sprite->animation());
		mario->changeAnimation(NONE);
		starmario->changeAnimation(NONE);
		supermario->changeAnimation(NONE);

		posPlayer.y -= 32; //POSIBLE CAUSA DE ERRORES???
		starsupermario->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		sprite = starsupermario;
		tamPlayer = glm::ivec2(32, 64); //Tama�o Super Mario
	}
}

glm::vec2 Player::getPos() const{

	return posPlayer;
}

int Player::getVel() const {

	return vel;
}

glm::vec2 Player::getTam() const {

	return tamPlayer;
}

string Player::getEstado() const
{
	if (sprite == mario) return "MARIO";
	else if (sprite == supermario) return "SUPERMARIO";
	else if (sprite == starmario) return "STARMARIO";
	else if (sprite == starsupermario) return "STARSUPERMARIO";
}

void Player::setBloque(string d, const glm::vec2 &posB) 
{
	hitBloque = d;
	posBloque = posB;

	if (hitBloque == "DOWN")
	{
		int aux = (posPlayer.y + tamPlayer.y) - posB.y;
		posPlayer.y -= aux;
	}
}

void Player::jump() {	
	jumpAngle = 0;
	startY = posPlayer.y;
	bJumping = true;
}

void Player::morirsalto() {
	fall_step = 6;
	jump_height = 50;
	dying = true;
	jump();
	sprite->changeAnimation(DEAD);
}

bool Player::isActive() {
	return active;
}

bool Player::isDying() {
	return dying;
}

bool Player::isRebooted() {
	return rebooted;
}

bool Player::isFalling() {
	if (falling) {
		falling = false;
		return true;
	}
	else return false;
}

void Player::cambiaEstado(string ea, string ep) {
	if (ea == "SUPERMARIO" && ep == "MARIO") {
		setSprite("mario");
		Audio::instance().warpEffect();
		inmunidad = true;
	}
	else if (ea == "MARIO" && ep == "SUPERMARIO") setSprite("supermario");
	else if (ea == "MARIO" && ep == "STARMARIO") {
		starMario = true;
		Audio::instance().playStar();
		setSprite("starmario");
	}
	else if (ea == "SUPERMARIO" && ep == "STARSUPERMARIO") {
		starMario = true;
		Audio::instance().playStar();
		setSprite("starsupermario");
	}
	else if (ea == "STARMARIO" && ep == "MARIO") {
		setSprite("mario");
		Audio::instance().stopMusic();
		Audio::instance().playLevel(level);
	}
	else if (ea == "STARSUPERMARIO" && ep == "SUPERMARIO") {
		setSprite("supermario");
		Audio::instance().stopMusic();
		Audio::instance().playLevel(level);
	}
	else if (ea == "STARMARIO" && ep == "STARSUPERMARIO") setSprite("starsupermario");
	else if (ea == "STARSUPERMARIO" && ep == "STARMARIO") setSprite("starmario");
}

bool Player::esInmune() {
	return inmunidad;
}

bool Player::isWin() {
	return win;
}

void Player::setWin() {
	win = false;
}

void Player::setLevel(int l) {
	level = l;
}