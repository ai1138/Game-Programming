#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define platCount 20
#define enemCount 3
#include "Entity.h"
#include <vector>


struct GameState {
    Entity* player;
    Entity* platform;
    Entity* enemy;

};

GameState state;
SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

int enemKilled = 0;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
   

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void DrawText(ShaderProgram* program, std::string text, float size, float spacing, glm::vec3 position) 
{
    GLuint fontTextureID = LoadTexture("font.png");
    // calculate U,V coordinates: 16 rows and columns
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    // for each character: get index & get U,V coordinates 
    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        //-----  Set up the vertices and texCoords  -----
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });

    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    // vertices.data() makes the vector into a normal array so it can be used
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

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


    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->type = PLAYER; 
    state.player->position = glm::vec3(0,4,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.9f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("char.png");
    state.player->jumpPower = 0.5f;

    state.enemy = new Entity[enemCount];
    state.platform = new Entity[platCount];
    GLuint platID = LoadTexture("tile.png");
    
   
    

    state.enemy[0].textureID = LoadTexture("enem.png");
    state.enemy[0].position = glm::vec3(3, 1.5f, 0);
    state.enemy[0].type = ENEMY;
    state.enemy[0].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[0].aiType = WAITANDGO;
    state.enemy[0].aiState = IDLE;

    state.enemy[1].textureID = LoadTexture("enem.png");
    state.enemy[1].position = glm::vec3(-2, 2.5f, 0);
    state.enemy[1].type = ENEMY;
    state.enemy[1].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[1].aiType = WAITANDGO;
    state.enemy[1].aiState = IDLE;

    state.enemy[2].textureID = LoadTexture("enem.png");
    state.enemy[2].position = glm::vec3(-4, -1.5f, 0);
    state.enemy[2].type = ENEMY;
    state.enemy[2].acceleration = glm::vec3(0, -0.9f, 0);
    state.enemy[2].aiType = WAITANDGO;
    state.enemy[2].aiState = IDLE;

    

    state.platform[0].textureID = platID;
    state.platform[0].position = glm::vec3(-1, -3.5f, 0);

    state.platform[1].textureID = platID;
    state.platform[1].position = glm::vec3(0, -3.5f, 0);
    
    state.platform[2].textureID = platID;
    state.platform[2].position = glm::vec3(-4.5, 0.5f, 0);

    state.platform[3].textureID = platID;
    state.platform[3].position = glm::vec3(1, -3.5f, 0);
    
    state.platform[4].textureID = platID;
    state.platform[4].position = glm::vec3(2, -3.5f, 0);

    state.platform[5].textureID = platID;
    state.platform[5].position = glm::vec3(3, -3.5f, 0);

    state.platform[6].textureID = platID;
    state.platform[6].position = glm::vec3(-1, -3.5f, 0);

    state.platform[7].textureID = platID;
    state.platform[7].position = glm::vec3(-2, -3.5f, 0);

    state.platform[8].textureID = platID;
    state.platform[8].position = glm::vec3(-3, -3.5f, 0);
    
    state.platform[9].textureID = platID;
    state.platform[9].position = glm::vec3(-4, -3.5f, 0);

    state.platform[10].textureID = platID;
    state.platform[10].position = glm::vec3(-5, -3.5f, 0);

    state.platform[11].textureID = platID;
    state.platform[11].position = glm::vec3(4, -3.5f, 0);

    state.platform[12].textureID = platID;
    state.platform[12].position = glm::vec3(5, -3.5f, 0);

    state.platform[13].textureID = platID;
    state.platform[13].position = glm::vec3(3, -0.5f, 0);

    state.platform[14].textureID = platID;
    state.platform[14].position = glm::vec3(2, -0.5f, 0);

    state.platform[15].textureID = platID;
    state.platform[15].position = glm::vec3(5, -0.5f, 0);

    state.platform[16].textureID = platID;
    state.platform[16].position = glm::vec3(4, -0.5f, 0);
    
    state.platform[17].textureID = platID;
    state.platform[17].position = glm::vec3(-3.5, 0.5f, 0);

    state.platform[18].textureID = platID;
    state.platform[18].position = glm::vec3(-2.5, 0.5f, 0);

    state.platform[19].textureID = platID;
    state.platform[19].position = glm::vec3(-1.5, 0.5f, 0);

    

    for (int i = 0; i < platCount; i++)
    {
        state.platform[i].type = PLATFORM;
        state.platform[i].Update(0,NULL,NULL,NULL,0,0);
    }


}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

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
                state.player->jump = true;
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
  
        if (keys[SDL_SCANCODE_LEFT]) {

            state.player->movement.x = -1.0f;

        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;;
        }


        if (glm::length(state.player->movement) > 1.0f) {
            state.player->movement = glm::normalize(state.player->movement);
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
        state.player->Update(FIXED_TIMESTEP,state.player,state.platform,state.enemy,platCount, enemCount);
        for (int i = 0; i < enemCount; i++)
        {
            state.enemy[i].Update(FIXED_TIMESTEP, state.player,state.platform, state.enemy, platCount, enemCount);
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < platCount; i++)
    {
        state.platform[i].Render(&program);
    }
    for (int i = 0; i < enemCount; i++)
    {
        state.enemy[i].Render(&program);
    }
    state.player->Render(&program);
    SDL_GL_SwapWindow(displayWindow);

    /*if (state.player->isAlive == false) 
    {
        DrawText(&program, "You Lose!", 0.5f, -0.25f, glm::vec3(-1.20f, -0.25f, 0));
        
    }
    else if (state.player->isAlive == true && enemKilled == 3) 
    {
        DrawText(&program,  "You Win!", 0.5f, -0.25f, glm::vec3(-1.18f, -0.25f, 0));
        
    }*/
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}