#ifndef VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
#define VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H

#include <memory> 
#include "model.h"
#include "objects.h"

class GraphicsComponent {
public:

	GraphicsComponent() {}
	virtual ~GraphicsComponent() {}
	virtual void Load(Model* modelPointers, Shader* shader) = 0;
	virtual void SetUp(GameObject& object) = 0;
	virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) = 0;
	virtual void Draw(GameObject& object) = 0;
};

class DefaultGraphicsComponent : public GraphicsComponent {
public:
	DefaultGraphicsComponent() : modelData(nullptr), shader(nullptr) {}

	virtual void Load(Model* m, Shader* s) {
		modelData.reset(m);  // takes ownership of pointer
		shader.reset(s);
	}
	virtual void SetUp(GameObject& object) {
		//check is position was set from code, otherwise use model information
		if (object.position.x >= FLT_MAX - 1.0f)
			object.position = modelData->Position();

	}
	virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
		//Component updates the object, object does not update the component 
	}

	virtual void Draw(GameObject& object) {
		shader->Use();
		shader->SetMat4("model", object.modelMatrix);
		modelData->Draw(*shader);
	}

private:
	std::unique_ptr<Model> modelData;
	std::unique_ptr <Shader> shader;
};

class IcoSphereGraphicsComponent : public GraphicsComponent {
public:
	IcoSphereGraphicsComponent() :modelPointer(nullptr), icoShader(nullptr), model(glm::mat4(1.0f))/*, lights(nullptr)*/ {}

	virtual void Load(Model* modelPointers, Shader* shader) {
		modelPointer = modelPointers;
		icoShader = shader;
		//lights = lightContainer;
	}
	virtual void SetUp(GameObject& object) {
		//Component sets up the object, object does not set up the component 
	}
	virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
		//Component updates the object, object does not update the component 
	}

	virtual void Draw(GameObject& object) {
		icoShader->Use();
		icoShader->SetMat4("model", object.modelMatrix);
		modelPointer->Draw(*icoShader);
	}

private:
	Model* modelPointer;
	Shader* icoShader;
	//TODO::below not need right now
	//LightFactory* lights;
	glm::mat4 model;
};


class PlayerGraphicsComponent : public GraphicsComponent {
public:
	PlayerGraphicsComponent() : modelData(nullptr), shader(nullptr),
		lights(nullptr), model(glm::mat4(1.0f)) {}

	virtual void Load(Model* m, Shader* s) {
		modelData = m;
		shader = s;
		model = glm::mat4(1.0f);
	}
	virtual void SetUp(GameObject& object) {
		object.position += modelData->Position();
		object.modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
	}
	virtual void Update(GameObject& object, const glm::mat4 P, const glm::mat4 V) {
		model = object.modelMatrix;
	}

	virtual void Draw(GameObject& object) {
		shader->Use();
		shader->SetMat4("model", model);
		shader->SetMat4("inverseModel", glm::inverse(model));
		modelData->Draw(*shader);
	}

private:
	Model* modelData;
	Shader* shader;
	LightFactory* lights;
	glm::mat4 model;
};



//class BombGraphicsComponent : public GraphicsComponent {
//public:
//
////TODO::make less awful
//  virtual void Load(std::vector<Model*> modelPointers, Shader* shader) {
//    this->modelPointers = modelPointers;
//    this->bombShader = shader;
//    timer = 0.0f;
//    bombModel = this->modelPointers[0]; 
//  }
//  virtual void SetUp(GameObject& object) {
//    object.position = modelPointers[0]->Position();
//  }
//
//  virtual void Update(GameObject& object, const glm::mat4 PV = glm::mat4(1.0f)) {
//    timer += 0.1f;
//    if (timer > 25.0f) {
//      //bombModel = modelPointers[1];
//    }
//    else {
//      //object.modelMatrix = glm::translate(object.modelMatrix, -object.position);
//    }
//    bombShader->Use();
//    bombShader->SetMat4("PVM", PV * object.modelMatrix);
//    bombShader->SetFloat("iTime", (timer - 30.0f) * 0.25f);
//    bombShader->SetFloat("timer", timer);
//    //bombShader->SetVec2("iResolution", 1280, 720);
//    //bombShader->SetVec2("iMouse", 0, 0);
//  }
//
//  virtual void Draw() {
//    bombModel->Draw(*bombShader);
//  }
//
//private:
//  Model* bombModel;
//  Shader* bombShader;
//  std::vector<Model*> modelPointers;
//  float timer;
//};

#endif // !VEALE_1FE65B4A_908C_414D_A4E1_D59A282CFB13_H
