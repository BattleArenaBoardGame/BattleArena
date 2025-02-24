/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_GAME_H
#define BABG_GAME_H

#include <Arduino.h>
#include <Map.h>

class Hero;

class Game {
  public:
    Game();

    Map *getMap();

    void start(uint8_t team1Heroes, u_int8_t team2Heroes);
    void endTurn();

    uint8_t getCurrentHeroId();
    Hero *getCurrentHero();
    Hero *getHero(uint8_t id);

  private:
    Map m_map;
    Team m_currentTeam;
    uint8_t m_currentRound;
    uint8_t m_currentHeroId;
    uint8_t m_team1Heroes;
    uint8_t m_heroesCount;
};

#endif