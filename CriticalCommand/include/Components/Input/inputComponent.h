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

    if (Input::KEY.UP && !Input::KEY.LEFT_ALT) {
      object.position -= object.direction * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.DOWN && !Input::KEY.LEFT_ALT) {
      object.position += object.direction * (object.deltaTime * object.velocity);
    }
    if (Input::KEY.LEFT && !Input::KEY.LEFT_ALT) {
      object.direction = glm::rotate(object.direction, glm::radians(1.0f), object.up);
    }
    if (Input::KEY.RIGHT && !Input::KEY.LEFT_ALT) {
      object.direction = glm::rotate(object.direction, glm::radians(-1.0f), object.up);
    }
    if (Input::KEY.SPACE && !Input::KEY.LEFT_ALT) {
      object.position += object.up * (object.deltaTime * object.velocity);
    }
  }
};


/*

   TODO:: move math functions to PhysicsComponent?

*/

class BaseTurretInputComponent : public InputComponent {
public:
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

  virtual void Update(GameObject& object) {
    /*object.look.x = (float)Input::xoffset;
    object.look.y = (float)Input::yoffset;*/
    //object.deltaDirection = object.direction;
    /*object.right = glm::rotate(object.right, glm::sin(glm::radians(float(0.1f * object.look.x))), object.up);
    object.direction = glm::cross(glm::normalize(object.right), glm::normalize(object.up));*/

    if (Input::KEY.RIGHT && Input::KEY.LEFT_ALT) {
      object.right = glm::rotate(object.right, glm::sin(glm::radians(-0.50f)), object.up);
    }
    else if (Input::KEY.LEFT && Input::KEY.LEFT_ALT) {
      object.right = glm::rotate(object.right, glm::sin(glm::radians(0.50f)), object.up);
    }
    //object.right.y = 0.0f;
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
    //object.up = glm::rotate(object.up, glm::sin(glm::radians(float(0.1f * object.look.y))), object.right);
    /*object.up.y = glm::clamp(object.up.y, 0.5f, 1.0f);
    object.right.x = glm::clamp(object.up.x, 0.0f, 0.10f);*/
    //object.direction = glm::cross(glm::normalize(object.right), glm::normalize(object.up));

    //object.up = glm::rotate(object.up, glm::radians(glm::dot(object.front, object.right)), glm::vec3(0.0f, 1.0f, 0.0f));
    if (Input::KEY.RIGHT && Input::KEY.LEFT_ALT) {
      object.up = glm::rotate(object.up, glm::sin(glm::radians(-0.50f)), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (Input::KEY.LEFT && Input::KEY.LEFT_ALT) {
      object.up = glm::rotate(object.up, glm::sin(glm::radians(0.50f)), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (Input::KEY.UP && Input::KEY.LEFT_ALT) {
      object.up = glm::rotate(object.up, glm::sin(glm::radians(0.50f)), object.right);
    }
    else if (Input::KEY.DOWN && Input::KEY.LEFT_ALT) {
      object.up = glm::rotate(object.up, glm::sin(glm::radians(-0.50f)), object.right);
    }
    
    object.direction = glm::cross(object.right, object.up);
    object.direction = glm::normalize(object.direction);
    /*if (Input::KEY.SPACE) {
      printf("------------------------------\n");
      printf("barrel right: %f, %f, %f\n", object.right.x, object.right.y, object.right.z);
      printf("barrel up   : %f, %f, %f\n", object.up.x, object.up.y, object.up.z);
      printf("barrel direc: %f, %f, %f\n", object.direction.x, object.direction.y, object.direction.z);
      
    }*/
  }
};

class VelocityBasedInputComponent : public InputComponent {
public:
  virtual void Load() {}
  virtual void SetUp(GameObject& object) {}

  virtual void Update(GameObject& object) {
    object.look.x = (float)Input::xoffset;
    //object.look.y = (float)Input::yoffset;

    //object.right = glm::rotate(object.right, glm::sin(glm::radians(object.look.x * 0.1f)), object.up);
    if (Input::KEY.RIGHT) {
      object.right = glm::rotate(object.right, glm::sin(glm::radians(-0.50f)), object.up);
    }
    else if (Input::KEY.LEFT) {
      object.right = glm::rotate(object.right, glm::sin(glm::radians(0.50f)), object.up);
    }
    else if (Input::KEY.UP/* && lastState*/) {
      object.velocity += object.velocity + (object.deltaTime * object.acceleration);
      lastState = false;
    }
    else if (Input::KEY.DOWN && lastState) {
      object.velocity -= object.velocity + (object.deltaTime * object.acceleration);
      lastState = false;
    }
    /*else {
      lastState = true;
      if (object.velocity > 0.0f) {
        object.velocity -= 0.1f;
      }
      if (object.velocity < 0.0f) {
        object.velocity -= 0.1f;
      }
    }*/
  /*  if (object.velocity > 2.0f) {
      object.velocity = 2.0f;
    }
    if (object.velocity < -2.0f) {
      object.velocity = 2.0f;
    }*/
    object.direction = glm::cross(object.right, object.up);
    
  }
private:
  bool newState;
  bool lastState;
};

#endif // !INPUT_COMPONENT_H

