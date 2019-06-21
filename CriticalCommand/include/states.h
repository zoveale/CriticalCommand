#ifndef STATES_H
#define STATES_H

#include "input.h"
#include <cstdio>

class Player;
class IdelState;
class ForwardState;

class PlayerState {
public:
  PlayerState();
  static IdelState idelState;
  static ForwardState forwardState;
  virtual void HandleInput(Player& player, Input input) {}
  virtual void Update(Player& player) {}
};

class IdelState : public PlayerState {
private:
  float idelTime;

public:
  IdelState() : idelTime(1.0f) {}
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};

class ForwardState : public PlayerState {
public:
  void HandleInput(Player& player, Input input);
  void Update(Player& player);
};

#endif // !STATES_H