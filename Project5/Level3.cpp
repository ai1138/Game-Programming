#include "Level3.h"

#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0,
 3, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 2, 1, 2, 1, 1, 1, 1, 1, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {
   

    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -3.9f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("char.png");
    state.player->jumpPower = 5.0f;

    state.enemy = new Entity[4];





    state.enemy[0].textureID = Util::LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(6, 1.5f, 0);
    state.enemy[0].type = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -0.9, 0);
    state.enemy[0].speed = 1.0f;
    state.enemy[0].aiType = WALKER;
    state.enemy[0].aiState = IDLE;

    state.enemy[1].textureID = Util::LoadTexture("enem.png");
    state.enemy[1].position = glm::vec3(0, 2.5f, 0);
    state.enemy[1].type = ENEMY;
    state.enemy[1].speed = 1.0f;
    state.enemy[1].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[1].aiType = WALKER;
    state.enemy[1].aiState = IDLE;

    state.enemy[2].textureID = Util::LoadTexture("enem.png");
    state.enemy[2].position = glm::vec3(5, 3.5f, 0);
    state.enemy[2].type = ENEMY;
    state.enemy[2].speed = 1.0f;
    state.enemy[2].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[2].aiType = WALKER;
    state.enemy[2].aiState = IDLE;
    
    state.enemy[3].textureID = Util::LoadTexture("goal.png");
    state.enemy[3].position = glm::vec3(10, 0, 0);
    state.enemy[3].type = GOAL;
    state.enemy[3].acceleration = glm::vec3(0, -9.81f, 0);




    // Move over all of the player and enemy code from initialization.
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemy, 4);
    state.enemy[0].Update(deltaTime, state.player, state.map, state.enemy, 4);
    state.enemy[1].Update(deltaTime, state.player, state.map, state.enemy, 4);
    state.enemy[2].Update(deltaTime, state.player, state.map, state.enemy, 4);
    state.enemy[3].Update(deltaTime, state.player, state.map, state.enemy, 4);

    
}
void Level3::Render(ShaderProgram* program) {
    GLuint fontID = Util::LoadTexture("font.png");
    state.map->Render(program);
    state.player->Render(program);
    state.enemy[0].Render(program);
    state.enemy[1].Render(program);
    state.enemy[2].Render(program);
    state.enemy[3].Render(program);
   
    if (state.player->isAlive == false)
    {
        Util::DrawText(program, fontID, "You Lose!", 0.5f, -0.25f, glm::vec3(state.player->position.x, -0.25f, 0));
        state.gameDone = true;
    }
    if (state.player->goalAchieved)
    {
        Util::DrawText(program, fontID, "You Win!", 0.5f, -0.25f, glm::vec3(state.player->position.x, -0.25f, 0));
        state.gameDone = true;
    }
}

