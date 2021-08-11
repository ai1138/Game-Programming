#include "Level1.h"

#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 25
#define BULLET_MAX 100
unsigned int level1_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
};


void Level1::Initialize() {

    
    
    state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(9, -23, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0.0, 0.0f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("char.png");
    

   


    state.enemy = new Entity[BULLET_MAX+4];
    state.enemy[0].textureID = Util::LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(3, -10, 0);
    state.enemy[0].type = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, 1.0f, 0);
    state.enemy[0].speed = 1.0f;
    state.enemy[0].aiType = WAITANDGO;
    state.enemy[0].aiState = IDLE;

    state.enemy[1].textureID = Util::LoadTexture("enem.png");
    state.enemy[1].position = glm::vec3(4, -8, 0);
    state.enemy[1].type = ENEMY;
    state.enemy[1].acceleration = glm::vec3(0, 1.0f, 0);
    state.enemy[1].speed = 1.0f;
    state.enemy[1].aiType = WAITANDGO;
    state.enemy[1].aiState = IDLE;


    state.enemy[2].textureID = Util::LoadTexture("enem.png");
    state.enemy[2].position = glm::vec3(5, -6, 0);
    state.enemy[2].type = ENEMY;
    state.enemy[2].acceleration = glm::vec3(0, 1.0f, 0);
    state.enemy[2].speed = 1.0f;
    state.enemy[2].aiType = WAITANDGO;
    state.enemy[2].aiState = IDLE;

    state.enemy[3].textureID = Util::LoadTexture("goal.png");
    state.enemy[3].position = glm::vec3(3, -2, 0);
    state.enemy[3].type = GOAL;
    int i = 4;
    for (i; i < BULLET_MAX+4; i++)
    {
        state.enemy[i].textureID = Util::LoadTexture("bullet.png");
        state.enemy[i].type = BULLET;
        state.enemy[i].isActive = false;
    }
    bulletIndex = 4;
    bulletCount = BULLET_MAX + bulletIndex+1;

    


   
    



	// Move over all of the player and enemy code from initialization.
}
void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.map,state.enemy, BULLET_MAX + 4);
    state.enemy[0].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 4);
    state.enemy[1].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 4);
    state.enemy[2].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 4);
    state.enemy[3].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX+4);
    if (state.enemy[bulletIndex].isActive)
        state.enemy[bulletIndex].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 4);
   

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
    state.enemy[0].Render(program);
    state.enemy[1].Render(program);
    state.enemy[2].Render(program);
    state.enemy[3].Render(program);
    if (state.enemy[bulletIndex].isActive)
        state.enemy[bulletIndex].Render(program);
    Util::DrawText(program, fontID, "Level 1", 0.5f, -0.25f, glm::vec3(state.player->position.x + 3, state.player->position.y + 3, 0));

    if(!state.player->isAlive)
    {
        state.gameDone = true;
        Util::DrawText(program, fontID, "You Lose!", 0.5f, -0.25f, glm::vec3(state.player->position.x+3, state.player->position.y + 2, 0));

    }
    
}

