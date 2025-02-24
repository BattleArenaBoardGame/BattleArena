/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_MAPITEM_H
#define BABG_MAPITEM_H

#include <Arduino.h>

enum Direction {
  UP,
  UP_LEFT,
  LEFT,
  DOWN_LEFT,
  DOWN,
  DOWN_RIGHT,
  RIGHT,
  UP_RIGHT
};

enum Team {
  Team_Unknown,
  Team_1,
  Team_2
};

enum MapItemType {
  MapItemType_None,
  MapItemType_Hero,
  MapItemType_HeroSpawnPoint,
  MapItemType_Ancient,
  MapItemType_Creep,
};

enum HeroAIGoal {
  AI_NO_GOAL,
  AI_TOP_LANE,
  AI_MID_LANE,
  AI_BOT_LANE,
};

class Hero;
class Map;

class MapItem {
  public:
    MapItem(Map *map, MapItemType type) : m_map(map), m_type(type) {}
    virtual uint32_t getColor() = 0;
    virtual Team getTeam() = 0;
    virtual MapItemType getType() { return m_type; }
    virtual void setType(MapItemType type) { m_type = type; }
    virtual Hero *getHero() { return NULL; }
    virtual ~MapItem() {}

    virtual void handleGameRoundStarted(uint8_t round) {}
    virtual void setPosition(uint8_t x, uint8_t y) {}

  protected:
    Map *m_map;
    MapItemType m_type;
};


#endif