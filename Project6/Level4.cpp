#include "Level4.h"

#define LEVEL4_WIDTH 11
#define LEVEL4_HEIGHT 25
#define BULLET_MAX 100
unsigned int level4_data[] =
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


void Level4::Initialize() {



    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, level4_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(9, -23, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0.0, 0.0f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("char.png");





    state.enemy = new Entity[BULLET_MAX + 6];
    state.enemy[0].textureID = Util::LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(3, -21, 0);
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

    state.enemy[4].textureID = Util::LoadTexture("enem.png");
    state.enemy[4].position = glm::vec3(6, -8, 0);
    state.enemy[4].type = ENEMY;
    state.enemy[4].acceleration = glm::vec3(0, 1.0f, 0);
    state.enemy[4].speed = 1.0f;
    state.enemy[4].aiType = WAITANDGO;
    state.enemy[4].aiState = IDLE;

    state.enemy[5].textureID = Util::LoadTexture("enem.png");
    state.enemy[5].position = glm::vec3(5, -10, 0);
    state.enemy[5].type = ENEMY;
    state.enemy[5].acceleration = glm::vec3(0, 1.0f, 0);
    state.enemy[5].speed = 1.0f;
    state.enemy[5].aiType = WAITANDGO;
    state.enemy[5].aiState = IDLE;


    int i = 6;
    for (i; i < BULLET_MAX + 4; i++)
    {
        state.enemy[i].textureID = Util::LoadTexture("bullet.png");
        state.enemy[i].type = BULLET;
        state.enemy[i].isActive = false;
    }
    bulletIndex = 6;
    bulletCount = BULLET_MAX + bulletIndex + 1;









    // Move over all of the player and enemy code from initialization.
}
void Level4::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[0].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[1].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[2].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[3].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[4].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);
    state.enemy[5].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 6);

    if (state.enemy[bulletIndex].isActive)
        state.enemy[bulletIndex].Update(deltaTime, state.player, state.map, state.enemy, BULLET_MAX + 4);


    if (state.player->goalAchieved)
    {
        state.nextScene = 5;
        state.player->goalAchieved = false;
    }

}
void Level4::Render(ShaderProgram* program) {
    GLuint fontID = Util::LoadTexture("font.png");
    state.map->Render(program);
    state.player->Render(program);
    state.enemy[0].Render(program);
    state.enemy[1].Render(program);
    state.enemy[2].Render(program);
    state.enemy[3].Render(program);
    state.enemy[4].Render(program);
    state.enemy[5].Render(program);
    if (state.enemy[bulletIndex].isActive)
        state.enemy[bulletIndex].Render(program);
    Util::DrawText(program, fontID, "Level 4", 0.5f, -0.25f, glm::vec3(state.player->position.x + 3, state.player->position.y + 3, 0));

    if (!state.player->isAlive)
    {
        state.gameDone = true;
        Util::DrawText(program, fontID, "You Lose!", 0.5f, -0.25f, glm::vec3(state.player->position.x + 3, state.player->position.y + 2, 0));

    }

}

