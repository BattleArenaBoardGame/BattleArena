/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_ANCIENT_H
#define BABG_ANCIENT_H

#include <Arduino.h>
#include <MapItems/MapItem.h>

class Hero;

class Ancient : public MapItem {
  public:
    Ancient(Map *map, uint8_t x, uint8_t y, Team team);

    uint32_t getColor() { return 0x4010; }
    Team getTeam() { return m_team; }
    Hero *getHero() { return m_hero; }

    void handleGameRoundStarted(uint8_t round);
    void setSpawnTimer(uint8_t spawnTimer) { m_spawnTimer = spawnTimer; }

  private:
    Team m_team;
    uint8_t m_spawnTimer;
    uint8_t m_x;
    uint8_t m_y;
    Hero *m_hero;
};


#endif