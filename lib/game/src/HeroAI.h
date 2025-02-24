/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_HEROAI_H
#define BABG_HEROAI_H

#include <Arduino.h>
#include <Map.h>

class HeroAI {
  public:
    HeroAI(Map *map);

    void handleGameRoundStarted(uint8_t round);

    void moveToNextPosition(Hero *c, int targetX, int tartetY);

    bool attackEnemyIfAround(Hero *c);
    void moveTowardsGoal(Hero *c);

  private:
    Map *m_map;
};

#endif