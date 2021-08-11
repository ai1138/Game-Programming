#include "Entity.h"
#include "Map.h"

Entity::Entity()
{
    position = glm::vec3(0);
    speed = 0;
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);


    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CollisionCheck(Entity* other)
{
    if (other == this) return false;
    if (isActive == false || other->isActive == false ) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f) ;
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CollisionCheck(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
            if ((type == PLAYER && object->type == ENEMY) && collidedTop)
            {
                isAlive = false;
                isActive = false;
                
            }
            else if ((type == PLAYER && object->type == ENEMY) && collidedBottom)
            {
                isAlive = false;
                isActive = false;
                
            }
            if ((type == BULLET && object->type == ENEMY) && collidedTop)
            {
                object->isActive = false;
                isActive = false;

            }
            else if ((type == BULLET && object->type == ENEMY) && collidedBottom)
            {
                object->isActive = false;
                isActive = false;

            }
            if ((type == BULLET && object->type == GOAL) && collidedTop)
            {
                isActive = false;

            }
            else if ((type == BULLET && object->type == GOAL) && collidedBottom)
            {
                isActive = false;

            }
            if ((type == PLAYER && object->type == GOAL) && collidedTop)
            {
                goalAchieved = true;

            }
            else if ((type == PLAYER && object->type == GOAL) && collidedBottom)
            {
                goalAchieved = true;

            }

        }
        
       
        
      
    }
    
}
void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CollisionCheck(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
            if ((type == PLAYER && object->type == ENEMY) && collidedLeft )
            {
                isAlive = false;
                isActive = false;
            }
            else if ((type == PLAYER && object->type == ENEMY) && collidedRight)
            {
                isAlive = false;
                isActive = false;
                
            }
            if ((type == BULLET && object->type == ENEMY) && collidedLeft)
            {
                object->isActive = false;
                isActive = false;
              
                
            }
            else if ((type == BULLET && object->type == ENEMY) && collidedRight)
            {
                object->isActive = false;
                isActive = false;
            }
            if ((type == PLAYER && object->type == GOAL) && collidedLeft)
            {
                goalAchieved = true;

            }
            else if ((type == PLAYER && object->type == GOAL) && collidedRight)
            {
                goalAchieved = true;

            }

        }
        
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0)
    {
        
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        if (type == BULLET)
        {
            isActive = false;
        }

    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if (type == BULLET)
        {
            isActive = false;
        }
    }

}


void Entity::Update(float deltaTime, Entity* player ,Map* platform , Entity* enemy ,int enemyCount)
{
    if (isActive == false) return;
   
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (type == ENEMY)
    {
        Ai(player);
    }

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;
        }
    }
  
   
    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    velocity += acceleration * deltaTime;
    

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platform);
    
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platform);
    if(type == PLAYER || type == BULLET)
        CheckCollisionsX(enemy, enemyCount);
        CheckCollisionsY(enemy, enemyCount);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Ai(Entity* player)
{
    switch (aiType) 
    {
    case WALKER:
        AiWalker();
        break;

    case WAITANDGO:
        AiWaitAndGo(player);
        break;
    }
}

void Entity::AiWalker()
{
    movement = glm::vec3(-1.0f, -1.0f, 0.0f);

}

void Entity::AiWaitAndGo(Entity* player)
{
    switch (aiState) {
    case IDLE:
        // if player close enough, switch to walking
        if (glm::distance(this->position, player->position) < 4.0f) {
            aiState = WALKING;
        }
        else {
            if (position.x < -5)
            {
                movement = glm::vec3(1.0f, 0.0f, 0.0f);
            }
            if(position.x > 5)
                movement = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        break;

    case WALKING:
        
        if (glm::distance(position, player->position) >= 4.0f) {
            aiState = IDLE;
        }

        
        else if (player->position.x < position.x) {
            movement = glm::vec3(-1.0f, -1.0f, 0.0f);
        }
        else {
            movement = glm::vec3(1.0f, 1.0f, 0.0f);
        }

        break;

    case ATTACKING:
        break;
    }
}

void Entity::Render(ShaderProgram* program) 
{
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) 
    {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}