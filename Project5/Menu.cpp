#include "Menu.h"

#define MENU_WIDTH 14
#define MENU_HEIGHT 8

unsigned int menu_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};
void Menu::Initialize()
{
    
	state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1);
	state.player = new Entity();
	state.player->type = PLAYER;
	state.player->position = glm::vec3(5, -5, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0);
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


}
void Menu::Update(float deltaTime) 
{
	state.player->Update(deltaTime, state.player, state.map, state.enemy, 3);
	
}
void Menu::Render(ShaderProgram* program) 
{
	state.player->Render(program);
	state.map->Render(program);
	GLuint fontID = Util::LoadTexture("font.png");
	Util::DrawText(program, fontID, "Welcome to The Zone", 0.5f, -0.25f, glm::vec3(3.0, -0.75f, 0));
    Util::DrawText(program, fontID, "Get the Coin to win the game!", 0.5f, -0.25f, glm::vec3(3.0, -1.5f, 0));
	Util::DrawText(program, fontID, "Press Enter To Play", 0.5f, -0.25f, glm::vec3(3.0, -3.25f, 0));
}
