/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <Map.h>
#include <Game.h>
#include <HeroAI.h>
#include <MapItems/MapItemNone.h>
#include <MapItems/HeroSpawnPoint.h>
#include <MapItems/Hero.h>
#include <MapItems/Ancient.h>


// Convert 16-bit RGB565 to 24-bit RGB888
void rgb565_to_rgb888(uint16_t rgb565, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = ((rgb565 >> 11) & 0x1F) * 255 / 31;  // Extract 5-bit Red and scale to 8-bit
    *g = ((rgb565 >> 5) & 0x3F) * 255 / 63;   // Extract 6-bit Green and scale to 8-bit
    *b = (rgb565 & 0x1F) * 255 / 31;         // Extract 5-bit Blue and scale to 8-bit
}

// Convert 24-bit RGB888 to 16-bit RGB565
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r * 31 / 255) << 11) | ((g * 63 / 255) << 5) | (b * 31 / 255);
}

// Adjust brightness (factor > 1.0 = brighter, factor < 1.0 = darker)
uint16_t adjust_brightness(uint16_t color, float factor) {
    uint8_t r, g, b;

    // Convert RGB565 to RGB888
    rgb565_to_rgb888(color, &r, &g, &b);

    // Apply brightness scaling
    r = (uint8_t)(r * factor);
    g = (uint8_t)(g * factor);
    b = (uint8_t)(b * factor);

    // Clamp values (0-255)
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    // Convert back to RGB565
    return rgb888_to_rgb565(r, g, b);
}


Map::Map(Game *game) {
  m_game = game;
  m_mapVisibleToBothTeams = false;

  // Initialize the map with empty items.
  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      m_map[y][x] = new MapItemNone(this);
    }
  }

  // Initialize the units layer with empty items.
  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      m_units[y][x] = nullptr;
    }
  }

  for(int i = 0; i < MAX_HEROES_COUNT; i++) {
    m_heroes[i] = new Hero(this, Team_Unknown, HERO_COLORS[i]);
  }

  for(int i = 0; i < MAX_CREEPS_COUNT; i++) {
    m_creeps[i] = nullptr;
  }

  m_heroAI = new HeroAI(this);
}

Map::~Map() {
  delete m_heroAI;
}

Point Map::findEmptyPointAround(uint8_t x, uint8_t y) {
  // Possible relative positions for 8-neighborhood (including diagonals).
  int dx[] = {0, -1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {0, 1, 0, 1, -1, 1, -1, 0, 1};
  for (int di = 0; di < 9; di++) {
    int newX = x + dx[di];
    int newY = y + dy[di];
    
    // Check for valid boundaries.
    if (newX >= 0 && newX < MAP_W && newY >= 0 && newY < MAP_H) {
      // Try to spawn the item. If it cannot be spawned there, we continue
      // to next location around the spawn point.
      if (getUnitItem(newX, newY) == nullptr) {
        return (Point){newX, newY};
      }
    }
  }
  return (Point){-1, -1};
}

void Map::spawnCreep(Team team, uint8_t x, uint8_t y, HeroAIGoal aiGoal) {
  for(int i = 0; i < MAX_CREEPS_COUNT; i++) {
    if (m_creeps[i]) {
      continue;
    }
    m_creeps[i] = new Hero(this, team, 0xFC00, aiGoal);
    m_creeps[i]->setType(MapItemType_Creep);

    Point p = findEmptyPointAround(x, y);
    if (p.x == -1) {
      delete m_creeps[i];
      m_creeps[i] = nullptr;
      return;
    }

    setUnitItem(p.x, p.y, m_creeps[i]);
    return;
  }
}

void Map::removeUnitItem(MapItem *item) {
  for(int i = 0; i < MAX_CREEPS_COUNT; i++) {
    if (!m_creeps[i]) {
      continue;
    }
    if (m_creeps[i] == item) {
      m_creeps[i] = nullptr;
    }
  }

  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      if (m_units[y][x] != nullptr && m_units[y][x] == item) {
        m_units[y][x] = nullptr;
      }
    }
  }
};

bool Map::setUnitItem(uint8_t x, uint8_t y, MapItem *item) {
  // Cannot move in case there is something already here.
  if (item && m_units[y][x] != nullptr) {
    return false;
  }

  m_units[y][x] = item;
  if (item != nullptr) {
    item->setPosition(x, y);
  }
  refreshColorMatrix();
  return true;
}


void Map::setMapItem(uint8_t x, uint8_t y, MapItem *item) {
  delete m_map[y][x];
  m_map[y][x] = item;
}

MapItem *Map::getMapItem(uint8_t x, uint8_t y) {
  return m_map[y][x];
}

MapItem *Map::getUnitItem(uint8_t x, uint8_t y) {
    return m_units[y][x];
}

bool Map::isItemVisible(MapItem *target) {
  for (int i = 0; i < MAX_VISIBLE_ITEMS && m_visibleItems[i] != nullptr; i++) {
    MapItem *item = m_visibleItems[i];
    if (item == target) {
      return true;
    }
  }
  return false;
}

VisibleItems *Map::getUnitsInRadius(int cx, int cy, int viewRadius) {
  static VisibleItems items;
  int visibleId = 0;

  for (int y = -viewRadius; y <= viewRadius; y++) {
    for (int x = -viewRadius; x <= viewRadius; x++) {
      if (cx + x >= 0 && cx + x < MAP_W && cy + y >= 0 && cy + y < MAP_H) {
        if (x * x + y * y <= viewRadius * viewRadius) {
          // Detect other units visible to this user and show them on map.
          MapItem *item = m_units[cy + y][cx + x];
          if (item) {
            items[visibleId] = item;
           visibleId++;
          }
        }
      }
    }
  }
  items[visibleId] = nullptr;
  return &items;
}

int Map::refreshColorMatrixForHero(Hero *h, int visibleId) {
  Hero *hero = m_heroes[m_game->getCurrentHeroId()];
  uint8_t viewRadius = hero->getViewRadius();

  // Skip the heroes from opposite team.
  if (h->getTeam() != hero->getTeam()) {
    return visibleId;
  }
  // Use the mid-point circle algorithm to define the visibility of hero.
  int cx = h->getX();
  int cy = h->getY();
  for (int y = -viewRadius; y <= viewRadius; y++) {
    for (int x = -viewRadius; x <= viewRadius; x++) {
      if (cx + x >= 0 && cx + x < MAP_W && cy + y >= 0 && cy + y < MAP_H) {
        if (x * x + y * y <= viewRadius * viewRadius) {
          // Store saturated color so it is clear what is visible.
          m_colorMatrix[cy + y][cx + x] = m_map[cy + y][cx + x]->getColor();
          m_visibleItems[visibleId] = m_map[cy + y][cx + x];

          // Detect other units visible to this user and show them on map.
          MapItem *item = m_units[cy + y][cx + x];
          if (item) {
            m_colorMatrix[cy + y][cx + x] = item->getColor();
            m_visibleItems[visibleId] = item;

            // In case the current hero sees a hero from opposite team, show the map to both teams.
            if (hero == h && item->getType() == MapItemType_Hero && item->getHero()->getTeam() != hero->getTeam()) {
              m_mapVisibleToBothTeams = true;
            }
          }
          visibleId++;
        }
      }
    }
  }
  return visibleId;
}

void Map::refreshColorMatrix() {
  // Get the view radius of current hero.
  Hero *hero = m_heroes[m_game->getCurrentHeroId()];
  uint8_t viewRadius = hero->getViewRadius();

  // Set the desaturated colors according to the map layer.
  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      m_colorMatrix[y][x] = adjust_brightness(m_map[y][x]->getColor(), 0.5);      
    }
  }

  // Visibility.
  // Current Hero can view map areas visible by all the other Heroes in the same
  // team. But if there is a Hero from opposite team visible to current Hero, then
  // the visibility must be computed only from current Hero and not from all the
  // Heroes, because the game board will be visible to all players.
  int visibleId = 0;
  m_mapVisibleToBothTeams = false;
  m_visibleItemsToCurrentHeroId = 0;
  for (int heroId=0; heroId < MAX_HEROES_COUNT && !m_mapVisibleToBothTeams; heroId++) {
    // We have to start with the current hero, because in case there is another hero
    // visible to it, we will stop iterating and set m_mapVisibleToBothTeams to show
    // the map to both teams. We must not reveal the position of other heroes.
    Hero *h = m_heroes[(heroId + m_game->getCurrentHeroId()) % MAX_HEROES_COUNT];
    visibleId = refreshColorMatrixForHero(h, visibleId);

    // We iterated over the current hero on turn. We therefore store the ID of item in
    // the m_visibleItems to m_visibleItemsToCurrentHeroId, so we can later use it in UI
    // to let the player choose the attack target which are close and visible to hero.
    if (heroId == 0) {
      m_visibleItemsToCurrentHeroId = visibleId;
    }
  }
  for (int creepId=0; creepId < MAX_CREEPS_COUNT && !m_mapVisibleToBothTeams; creepId++) {
    // We have to start with the current hero, because in case there is another hero
    // visible to it, we will stop iterating and set m_mapVisibleToBothTeams to show
    // the map to both teams. We must not reveal the position of other heroes.
    Hero *h = m_creeps[creepId];
    if (!h) {
      continue;
    }
    visibleId = refreshColorMatrixForHero(h, visibleId);
  }
  m_visibleItems[visibleId] = nullptr;
}

ColorMatrix *Map::getColorMatrix() {
    return &m_colorMatrix;
}

void Map::handleGameStarted() {
  refreshColorMatrix();
}

void Map::handleGameRoundStarted(uint8_t round) {
  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      m_map[y][x]->handleGameRoundStarted(round);
      if (m_units[y][x] != nullptr) {
        m_units[y][x]->handleGameRoundStarted(round);
      }
    }
  }

  m_heroAI->handleGameRoundStarted(round);

  refreshColorMatrix();
}

void Map::handleGameTurnStarted() {
  refreshColorMatrix();
}

void Map::initializeMap(uint8_t team1Heroes, u_int8_t team2Heroes) {
  // Initialize Heroes.
  Team team = Team_1;
  for(int i = 0; i < MAX_HEROES_COUNT; i++) {
    delete m_heroes[i];
    if (i >= team2Heroes + team1Heroes) {
      team = Team_Unknown;
    }
    else if (i >= team1Heroes) {
      team = Team_2;
    }
    m_heroes[i] = new Hero(this, team, HERO_COLORS[i]);
  }

  // Add empty field everywhere on map.
  for(int x = 0; x < MAP_W; x++) {
    for(int y = 0; y < MAP_H; y++) {
      setMapItem(x, y, new MapItemNone(this));
      setUnitItem(x, y, nullptr);
    }
  }

  // Add spawn points for team 1 to down-left corner.
  // This code iterates diagonally starting.
  int heroId = 0;
  for (int startY = MAP_H - 1; startY >= 0 && heroId < team1Heroes; startY--) {
    int y = startY, x = 0;
    while (y < MAP_H && x < MAP_W) {
      if (heroId == team1Heroes) {
        break;
      }
      setMapItem(x, y, new HeroSpawnPoint(this, x, y, Team_1, m_heroes[heroId]));
      x++;
      y++;
      heroId++;
    }
  }

  // Add spawn points for team 2 to up-right corner.
  // This code iterates diagonally starting.
  for (int startX = MAP_W - 1; startX >= 0 && heroId < team1Heroes + team2Heroes; startX--) {
      int y = 0, x = startX;
      while (x < MAP_W && y < MAP_H) {
          setMapItem(x, y, new HeroSpawnPoint(this, x, y, Team_2, m_heroes[heroId]));
          x++;
          y++;
          heroId++;
      }
  }

  // Add Ancient for Team 1 - down-left corner.
  setUnitItem(2, MAP_H - 3, new Ancient(this, 2, MAP_H - 3, Team_1));
  // Add Ancient for Team 2 - top-right corner.
  setUnitItem(MAP_W - 3, 2, new Ancient(this, MAP_W - 3, 2, Team_2));
}


