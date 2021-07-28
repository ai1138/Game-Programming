#include "Level1.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

unsigned int level1_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {

    
    
    state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -3.9f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("char.png");
    state.player->jumpPower = 5.0f;

   


    state.enemy = new Entity[2];
    state.enemy[0].textureID = Util::LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(3, 1.5f, 0);
    state.enemy[0].type = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemy[0].speed = 1.0f;
    state.enemy[0].aiType = WAITANDGO;
    state.enemy[0].aiState = IDLE;

   
    state.enemy[1].textureID = Util::LoadTexture("goal.png");
    state.enemy[1].position = glm::vec3(10, 0, 0);
    state.enemy[1].type = GOAL;
    state.enemy[1].acceleration = glm::vec3(0, -9.81f, 0);
    



	// Move over all of the player and enemy code from initialization.
}
void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.map,state.enemy, 2);
    state.enemy[0].Update(deltaTime, state.player, state.map, state.enemy, 2);
    state.enemy[1].Update(deltaTime, state.player, state.map, state.enemy, 2);
    
    if (state.player->goalAchieved)
    {
        state.nextScene = 2;
        state.player->goalAchieved = false;
    }

}
void Level1::Render(ShaderProgram* program) {
    GLuint fontID = Util::LoadTexture("font.png"); 
	state.map->Render(program); 
	state.player->Render(program);
    for (int i = 0; i < 2; i++)
    {
        state.enemy[i].Render(program);
    }
    if (state.player->lives == 0)
    {
        state.gameDone = true;
        Util::DrawText(program, fontID, "You Lose!", 0.5f, -0.25f, glm::vec3(3.0f, -0.25f, 0));

    }
    
}

