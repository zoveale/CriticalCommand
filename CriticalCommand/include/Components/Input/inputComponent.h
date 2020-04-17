#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

//class GameObject;
#include "objects.h"
#include "input.h"

class InputComponent {
public:
  InputComponent() {}
  virtual void Load() = 0;
  virtual void SetUp(GameObject& object) = 0;
  virtual void Update(GameObject& object) = 0;
};

//KEY_STATE Input::KEY;

class PlayerInputComponent : public InputComponent {
public:
  virtual void Load() {

  }
  virtual void SetUp(GameObject& object) {

  }
  //make a state machine? 

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;
    
    if (Input::KEY.UP) {
      object.position += object.front * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.DOWN) {
      object.position -= object.front * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.LEFT) {
      object.position -= object.right * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.RIGHT) {
      object.position += object.right * (object.deltaTime * object.velocity);
    }

  }
};

class DefaultInputComponent : public InputComponent {
public:
  //DefaultInputComponent() {}
  virtual void Load() {
  }
  virtual void SetUp(GameObject& object) {
  }
  virtual void Update(GameObject& object) {
  }

};

class BaseChassisInputComponent : public InputComponent {
public:
  virtual void Load() {

  }
  virtual void SetUp(GameObject& object) {

  }
  //make a state machine? 

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;

    if (Input::KEY.UP) {
      object.position += object.direction * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.DOWN) {
      object.position -= object.direction * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.LEFT) {
      object.direction = glm::normalize(glm::rotateY(object.direction, glm::radians(0.50f)));
      //object.position -= object.right * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.RIGHT) {
      object.direction = glm::normalize(glm::rotateY(object.direction, glm::radians(-0.50f)));
      //object.position += object.right * (object.deltaTime * object.velocity);
    }
  }
};

class BaseTurretInputComponent : public InputComponent {
public:
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;
    
    object.direction = glm::rotateY(object.direction, glm::radians(object.look.x));
  }
};

class BaseBarrelInputComponent : public InputComponent {
public:
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;

    object.direction = glm::rotateX(object.direction, glm::radians(object.look.y));
  }
};


#endif // !INPUT_COMPONENT_H

