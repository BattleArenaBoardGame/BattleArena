/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_HERO_SPAWN_POINT_H
#define BABG_HERO_SPAWN_POINT_H

#include <Arduino.h>
#include <MapItems/MapItem.h>

class Hero;

class HeroSpawnPoint : public MapItem {
  public:
    HeroSpawnPoint(Map *map, uint8_t x, uint8_t y, Team team, Hero *hero);

    uint32_t getColor() { return 0x04FF; }
    Team getTeam() { return m_team; }
    Hero *getHero() { return m_hero; }

    void handleGameRoundStarted(uint8_t round);
    void setSpawnTimer(uint8_t spawnTimer) { m_spawnTimer = spawnTimer; }

  private:
    Team m_team;
    Hero *m_hero;
    uint8_t m_spawnTimer;
    uint8_t m_x;
    uint8_t m_y;
};


#endif