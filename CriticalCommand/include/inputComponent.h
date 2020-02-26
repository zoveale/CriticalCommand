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
    
    if (Input::KEY.W) {
      object.position += object.front * ((object.deltaTime * object.velocity) + 0.5f * 0.25f * object.deltaTime * object.deltaTime);
    }
    else if (Input::KEY.S) {
      object.position -= object.front * ((object.deltaTime * object.velocity) + 0.5f * 0.25f * object.deltaTime * object.deltaTime);
    }
    else if (Input::KEY.A) {
      object.position -= object.right * ((object.deltaTime * object.velocity) + 0.5f * 0.25f * object.deltaTime * object.deltaTime);
    }
    else if (Input::KEY.D) {
      object.position += object.right * ((object.deltaTime * object.velocity) + 0.5f * 0.25f * object.deltaTime * object.deltaTime);
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


#endif // !INPUT_COMPONENT_H

