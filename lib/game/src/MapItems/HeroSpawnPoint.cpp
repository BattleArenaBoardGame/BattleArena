/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <MapItems/HeroSpawnPoint.h>
#include <MapItems/Hero.h>
#include "Map.h"

HeroSpawnPoint::HeroSpawnPoint(Map *map, uint8_t x, uint8_t y, Team team, Hero *hero) : MapItem(map, MapItemType_HeroSpawnPoint) {
    m_team = team;
    m_hero = hero;
    m_spawnTimer = 0;
    m_x = x;
    m_y = y;
}

void HeroSpawnPoint::handleGameRoundStarted(uint8_t round) {
    if (m_hero->isDead() && m_spawnTimer == 0) {
        m_map->setUnitItem(m_x, m_y, m_hero);
        m_hero->setIsDead(false);
    }

    if (m_spawnTimer != 0) {
        m_spawnTimer--;
    }
}
