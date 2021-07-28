
#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

unsigned int level2_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -3.9f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("char.png");
    state.player->jumpPower = 5.0f;

    state.enemy = new Entity[3];





    state.enemy[0].textureID = Util::LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(3, 1.5f, 0);
    state.enemy[0].type = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[0].velocity = glm::vec3(0.9f, 0.0f, 0);
    state.enemy[0].aiType = WALKER;
    state.enemy[0].aiState = IDLE;

    state.enemy[1].textureID = Util::LoadTexture("enem.png");
    state.enemy[1].position = glm::vec3(-2, 2.5f, 0);
    state.enemy[1].type = ENEMY;
    state.enemy[1].velocity = glm::vec3(0.9f, 0.0f, 0);
    state.enemy[1].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[1].aiType = WALKER;
    state.enemy[1].aiState = IDLE;

    state.enemy[2].textureID = Util::LoadTexture("enem.png");
    state.enemy[2].position = glm::vec3(-4, -1.5f, 0);
    state.enemy[2].type = ENEMY;
    state.enemy[2].velocity = glm::vec3(0.9f, 0.0f, 0);
    state.enemy[2].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[2].aiType = WALKER;
    state.enemy[2].aiState = IDLE;




    // Move over all of the player and enemy code from initialization.
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemy, 3);
    if (state.player->position.x >= 12)
    {
        state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram* program) {
    GLuint fontID = Util::LoadTexture("font.png");
    state.map->Render(program);
    state.player->Render(program);
    if (state.player->isAlive == false)
    {
        Util::DrawText(program, fontID, "You Lose!", 0.5f, -0.25f, glm::vec3(-1.20f, -0.25f, 0));

    }
    
}

