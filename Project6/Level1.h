#include "Scene.h"
#pragma once
class Level1 : public Scene {

public:
	
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};
#pragma once
