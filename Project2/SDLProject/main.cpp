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
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, player2Matrix, ballMatrix, projectionMatrix;
glm::vec3 playerPosition = glm::vec3(0, 0, 0);
glm::vec3 player2Position = glm::vec3(0, 0, 0);
glm::vec3 playerMovement = glm::vec3(0, 0, 0);
glm::vec3 player2Movement = glm::vec3(0, 0, 0);
glm::vec3 playerScale = glm::vec3(1.25f, 1.0f, 0.0f);
glm::vec3 ballPosition = glm::vec3(0.0f, 0.0f, 0.0f);     
glm::vec3 ballMovement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ballScale = glm::vec3(0.5f, 0.5f, 0.0f);
float playerWidth = playerScale.x;
float playerHeight = playerScale.y;
float ballWidth = ballScale.x;
float ballHeight = ballScale.y;
float playerSpeed = 1.0f;
float ballSpeed = 1.75f;
GLuint playerTextureID;
GLuint player2TextureID;
GLuint ballID;



//----------- DO NOT TOUCH
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


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);     // the box of the world you see

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    glUseProgram(program.programID);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);   // background color, when clear the window use this color

    // enables blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load Textures
    playerTextureID = LoadTexture("playerShipOrange.png");
    player2TextureID = LoadTexture("playerShipOrange.png");
    ballID = LoadTexture("meteorBrown_small1.png");
}



void ProcessInput()
{

    playerMovement = glm::vec3(0);
    player2Movement = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:

                ballMovement = glm::vec3(-.5f, -.5f, 0.0f);
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP])
    {
        if (playerPosition.y < 3.0f)
        {
            playerMovement.y = 2.0f;
        }

    }
    else if (keys[SDL_SCANCODE_DOWN])
    {
        if (playerPosition.y > -3.0f)
        {
            playerMovement.y = -2.0f;
        }
    }
    if (keys[SDL_SCANCODE_W])
    {

        if (player2Position.y < 3.0f) 
        {
            player2Movement.y = 2.0f;
        }

    }
    else if (keys[SDL_SCANCODE_S])
    {
        if (player2Position.y > -3.0f)
        {
            player2Movement.y = -2.0f;
        }
    }


}



float lastTicks = 0.0f;  
void Update()
{
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    modelMatrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(4.5f, 0.0f, 0.0f));
    player2Matrix = glm::translate(player2Matrix, glm::vec3(-4.5f, 0.0f, 0.0f));
    
    playerPosition += playerMovement * playerSpeed * deltaTime;
    player2Position += player2Movement * playerSpeed * deltaTime;
    ballPosition += ballMovement * ballSpeed * deltaTime;
    
    modelMatrix = glm::scale(modelMatrix, playerScale);
    player2Matrix = glm::scale(player2Matrix, playerScale);
    ballMatrix = glm::scale(ballMatrix, ballScale);

    modelMatrix = glm::translate(modelMatrix, playerPosition);
    player2Matrix = glm::translate(player2Matrix, player2Position);
    ballMatrix = glm::translate(ballMatrix, ballPosition);

    if ((ballPosition.x > 10.0f || ballPosition.x < -10.0f))
    {
        ballSpeed = 0.0f;
    }
    else
    {
        if (ballPosition.y > 7|| ballPosition.y < -7)
        {
            ballMovement.y *= -1.0f;
            ballPosition.y += ballMovement.y * 0.2f;
        }
        //attempted to use bounding box collision
        if (ballPosition.x >= 0.0f)
        {
            if (ballPosition.x < player2Position.x + playerWidth &&
                ballPosition.x + ballWidth > player2Position.x &&
                ballPosition.y < player2Position.y + playerHeight &&
                ballPosition.y + ballHeight > player2Position.y)
            {
                ballMovement.x *= -1.0f;
                ballPosition.x += 0.2f;
            }


        }
        else
        {
            if (ballPosition.x < playerPosition.x + playerWidth &&
                ballPosition.x + ballWidth > playerPosition.x &&
                ballPosition.y < playerPosition.y + playerHeight &&
                ballPosition.y + ballHeight > playerPosition.y)
            {
                ballMovement.x *= -1.0f;
                ballPosition.x += 0.2f;
            }

        }
    }
}




void Render() {
    glClear(GL_COLOR_BUFFER_BIT);   // clear background

    // verticies & texture coordinates for the 2 triangles
    float verticies[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    // tell to use this list of coordinates
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, verticies);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(modelMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(player2Matrix);
    glBindTexture(GL_TEXTURE_2D, player2TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);   // always do last
}

void Shutdown() {
    SDL_Quit();
}


// main loop
int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();       // happens as fast as computer can do it
    }

    Shutdown();
    return 0;
}