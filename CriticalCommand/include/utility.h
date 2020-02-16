#ifndef UTILITY_H
#define UTILITY_H

#include <random>

class Random {

public:
  //returns random float between x and y
  static float Jitter(float x, float y) {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distriburion(x, y);
    return distriburion(generator);
  }
};

#endif // !UTILITY_H
