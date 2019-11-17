#ifndef STATES_H
#define STATES_H



#include <cstdio>


//#include "player.h"
//#define PRINT_STATES

#include "input.h"
class Input;

#include "player.h"
class Player;

class IdelState;
class ForwardState;
class BackwardState;
class StrafeLeftState;
class StrafeRightState;


//States to add
//class ForwardLeftState
//class ForwardRightState
//class BackwardLeftState
//class BackwardRightState
///

//FIXME:: change class identifier
class InputState {
///

public:
  InputState();
  static IdelState idel;
  static ForwardState forward;
  static BackwardState backward;
  static StrafeLeftState left;
  static StrafeRightState right;

  virtual void HandleInput(Player& player, Input input) {}
  virtual void Update(Player& player) {}
};

class IdelState : public InputState {
private:
  float idelTime;

public:
  IdelState() : idelTime(1.0f) {}
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};

class ForwardState : public InputState {
public:
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};

class BackwardState : public InputState {
public:
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};
class StrafeLeftState : public InputState {
public:
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};
class StrafeRightState : public InputState {
public:
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};



#endif // !STATES_H