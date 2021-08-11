#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include <vector>
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Menu.h"




SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;


Mix_Music* music;
Mix_Chunk* fire;
GLuint fontID = Util::LoadTexture("font.png");


Scene* currentScene;
Scene* sceneList[6];
Menu* menu;
Level1* level1;
Level2* level2;
Level3* level3;
Level4* level4;
Level5* level5;
int index;
void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Coin Chaser!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("lofi.mp3");
    Mix_PlayMusic(music, -1);   // -1 = loop forever
    Mix_VolumeMusic(MIX_MAX_VOLUME /3 );
    fire = Mix_LoadWAV("laser.wav");
    Mix_VolumeChunk(fire,MIX_MAX_VOLUME / 16);
    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    menu = new Menu();

    level1 = new Level1();
    level2 = new Level2();
    level3 = new Level3();
    level4 = new Level4();
    level5 = new Level5();
    sceneList[0] = menu;
    
    sceneList[1] = level1;
    
    sceneList[2] = level2;
    
    sceneList[3] = level3;
    
    sceneList[4] = level4;

    sceneList[5] = level5;

    SwitchToScene(sceneList[0]);
    index = currentScene->bulletIndex;

  
}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                if (currentScene->bulletCount == currentScene->bulletIndex + 1)
                {
                    currentScene->bulletIndex = index;
                }
                for (int i = currentScene->bulletIndex; i < currentScene->bulletCount; i++)
                {

                    if (currentScene->state.enemy[i].isActive == false)
                    {
                        currentScene->state.enemy[i].isActive = true;
                        currentScene->state.enemy[i].position = glm::vec3(currentScene->state.player->position.x,
                            currentScene->state.player->position.y + 1, 0);
                        currentScene->state.enemy[i].movement = glm::vec3(0, 5.0f, 0);
                        currentScene->state.enemy[i].acceleration = glm::vec3(0, 2.0f, 0);
                        currentScene->state.enemy[i].speed = 1;
                        currentScene->bulletIndex = i;
                        break;
                    }
                }
                Mix_PlayChannel(0, fire, 0);
                break;
            case SDLK_RETURN:
                if (currentScene == menu)
                    currentScene->state.nextScene = 1;
                break;
            }
            

            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
  
        if (keys[SDL_SCANCODE_LEFT]) {

            if (currentScene == menu)
            {
                currentScene->state.player->movement.x = 0.0f;
            }
            else
            {
                currentScene->state.player->movement.x = -1.0f;
                if (currentScene->state.gameDone)
                {
                    currentScene->state.player->movement.x = 0.0f;
                }

            }
           

        }
        else if (keys[SDL_SCANCODE_RIGHT]) 
        {
            if (currentScene == menu)
            {
                currentScene->state.player->movement.x = 0.0f;
            }
            else
            {
                currentScene->state.player->movement.x = 1.0f;
                if (currentScene->state.gameDone)
                {
                    currentScene->state.player->movement.x = 0.0f;
                }
                
            }
        }
        else if (keys[SDL_SCANCODE_UP])
        {
            if (currentScene == menu)
            {
                currentScene->state.player->movement.y = 0.0f;
            }
            else
            {
                currentScene->state.player->movement.y = 1.0f;
                if (currentScene->state.gameDone)
                {
                    currentScene->state.player->movement.y = 0.0f;
                }

            }
        }
        else if (keys[SDL_SCANCODE_DOWN])
        {
            if (currentScene == menu)
            {
                currentScene->state.player->movement.y = 0.0f;
            }
            else
            {
                currentScene->state.player->movement.y = -1.0f;
                if (currentScene->state.gameDone)
                {
                    currentScene->state.player->movement.y = 0.0f;
                }

            }
        }
        
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    
    
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    viewMatrix = glm::mat4(1.0f);
    if (currentScene == menu)
    {
        viewMatrix = glm::translate(viewMatrix, glm ::vec3(5,0.0,0.0));
    }
    else
    {
        if (currentScene->state.player->position.y < -25 ) {
            viewMatrix = glm::translate(viewMatrix,
                glm::vec3(-5, 4 , 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
        }
    }
   
    

    accumulator = deltaTime;
}


void Render() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
   
    

    
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}