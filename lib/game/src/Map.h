/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_MAP_H
#define BABG_MAP_H

#include <Arduino.h>
#include <MapItems/MapItem.h>

#define MAP_W 16
#define MAP_H 16
#define MAX_HEROES_COUNT 6
static uint32_t HERO_COLORS[MAX_HEROES_COUNT] = {
  0xF800, // Red.
  0x07E0, // Green.
  0x001F, // Blue.
  0xFFE0, // Yellow.
  0x07FF, // Cyan.
  0xF81F, // Magenta.
};
#define MAX_VISIBLE_ITEMS (MAP_H * MAP_W + 1)
#define MAX_CREEPS_COUNT 60

class Game;
class HeroAI;

typedef uint32_t ColorMatrix[MAP_H][MAP_W];
typedef MapItem *VisibleItems[MAX_VISIBLE_ITEMS];
typedef Hero *Creeps[MAX_CREEPS_COUNT];

typedef struct {
  int x, y;
} Point;

class Map {
  public:
    Map(Game *game);
    ~Map();

    void initializeMap(uint8_t team1Heroes, u_int8_t team2Heroes);
    MapItem *getMapItem(uint8_t x, uint8_t y);
    MapItem *getUnitItem(uint8_t x, uint8_t y);
    ColorMatrix *getColorMatrix();
    Hero *getHero(uint8_t id) { return m_heroes[id]; }

    VisibleItems *getCurrentlyVisibleItems() { return &m_visibleItems; }

    // Returns the index in VisibleItems returned by getCurrentlyVisibleItems() defining
    // the end of the list of items visible just to the current Hero on turn.
    uint16_t getVisibleItemsToCurrentHeroId() { return m_visibleItemsToCurrentHeroId; }

    bool isItemVisible(MapItem *item);
    bool isMapVisibleToBothTeams() { return m_mapVisibleToBothTeams; }

    void handleGameStarted();
    void handleGameRoundStarted(uint8_t round);
    void handleGameTurnStarted();

    Point findEmptyPointAround(uint8_t x, uint8_t y);
    VisibleItems *getUnitsInRadius(int x, int y, int radius);

    void spawnCreep(Team team, uint8_t x, uint8_t y, HeroAIGoal aiGoal);
    void removeUnitItem(MapItem *item);
    Creeps *getCreeps() { return &m_creeps; }

    void setMapItem(uint8_t x, uint8_t y, MapItem *item);
    bool setUnitItem(uint8_t x, uint8_t y, MapItem *item);
  private:
    void refreshColorMatrix();
    int refreshColorMatrixForHero(Hero *h, int visibleId);

    MapItem *m_map[MAP_H][MAP_W];
    MapItem *m_units[MAP_H][MAP_W];
    VisibleItems m_visibleItems;
    uint16_t m_visibleItemsToCurrentHeroId;
    Hero *m_heroes[MAX_HEROES_COUNT];
    Hero *m_creeps[MAX_CREEPS_COUNT];
    Game *m_game;
    ColorMatrix m_colorMatrix;
    bool m_mapVisibleToBothTeams;
    HeroAI *m_heroAI;
};

#endif