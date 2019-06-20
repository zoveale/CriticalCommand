#ifndef PLAYER_H
#define PLAYER_H
#include "input.h"


class PlayerState;
class IdelState;
class ForwardState;

class Player {
private:
  PlayerState* state_;

public:
  Player();
  void HandleInput(Input input);
  virtual void Update();
  void MoveForward();
};



class PlayerState {

public:
  
  PlayerState();


  virtual ~PlayerState();
  virtual PlayerState* HandleInput(Player& player,
    Input input);
  virtual void Update(Player& player);

  static IdelState idel;
  static ForwardState forward;

};

class IdelState : public PlayerState {

public:
  IdelState() : idelTime(0.0f) {}
  PlayerState* HandleInput(Player& player, Input input);
  virtual void Update(Player& player);
  //virtual ~StandingState() {}
private:
  float idelTime;
};

class ForwardState : public PlayerState {
public:

  PlayerState* HandleInput(Player& player, Input input);
  virtual void Update(Player& player);
};



#endif // !PLAYER_H
