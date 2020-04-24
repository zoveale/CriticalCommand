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
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

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
      object.direction = glm::rotate(object.direction, glm::radians(1.0f), object.up);
    }
    if (Input::KEY.RIGHT) {
      object.direction = glm::rotate(object.direction, glm::radians(-1.0f), object.up);
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
    //object.deltaDirection = object.direction;

    object.right = glm::rotate(object.right, glm::sin(glm::radians(float(0.1f * object.look.x))), object.up);
    object.direction = glm::cross(glm::normalize(object.right), glm::normalize(object.up));
  }
};

class BaseBarrelInputComponent : public InputComponent {
public:
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;
    //object.deltaDirection = object.direction;
    object.up = glm::rotate(object.up, glm::sin(glm::radians(float(0.1f * object.look.y))), object.right);
    /*object.up.y = glm::clamp(object.up.y, 0.5f, 1.0f);
    object.right.x = glm::clamp(object.up.x, 0.0f, 0.10f);*/
    object.direction = glm::cross(glm::normalize(object.right), glm::normalize(object.up));
  }
};


#endif // !INPUT_COMPONENT_H

