/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <MapItems/Ancient.h>
#include <MapItems/Hero.h>
#include "Map.h"
#include "HeroAI.h"

Ancient::Ancient(Map *map, uint8_t x, uint8_t y, Team team) : MapItem(map, MapItemType_Ancient) {
    m_team = team;
    m_spawnTimer = 0;
    m_x = x;
    m_y = y;
    m_hero = new Hero(m_map, team, 0x4010);
    m_hero->setPosition(m_x, m_y);
    m_hero->setType(MapItemType_Ancient);
}

void Ancient::handleGameRoundStarted(uint8_t round) {
    if (m_spawnTimer == 0) {
        m_spawnTimer = 200;
        if (m_team == Team_1) {
            m_map->spawnCreep(m_team, m_x + 1, m_y, AI_BOT_LANE);
            m_map->spawnCreep(m_team, m_x + 1, m_y - 1, AI_MID_LANE);
            m_map->spawnCreep(m_team, m_x, m_y - 1, AI_TOP_LANE);
        }
        else {
            m_map->spawnCreep(m_team, m_x - 1, m_y, AI_TOP_LANE);
            m_map->spawnCreep(m_team, m_x - 1, m_y + 1, AI_MID_LANE);
            m_map->spawnCreep(m_team, m_x, m_y + 1, AI_BOT_LANE);
        }
    }

    m_spawnTimer--;
}
