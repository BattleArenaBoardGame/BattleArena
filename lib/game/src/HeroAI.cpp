/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <HeroAI.h>
#include <Map.h>
#include <MapItems/Hero.h>


HeroAI::HeroAI(Map *map) : m_map(map) {

}


void HeroAI::moveToNextPosition(Hero *c, int targetX, int targetY) {
    int bestDist = INT_MAX;
    uint8_t bestX;
    uint8_t bestY;
    // Directions for 8-way movement
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < 8; i++) {
        int newX = c->getX() + dx[i];
        int newY = c->getY() + dy[i];

        // x = 11
        // y = 2
        // targetX = 13
        // targetY = 2
        if (newX >= 0 && newX < MAP_W && newY >= 0 && newY < MAP_H && m_map->getUnitItem(newX, newY) == nullptr) {
            int dist = abs(newX - targetX) + abs(newY - targetY);
            if (dist < bestDist) {
                bestDist = dist;
                bestX = newX;
                bestY = newY;
            }
        }
    }

    if (bestDist == INT_MAX) {
        return;
    }

    Serial.printf("Creep %d team %d: moves from %d, %d to %d, %d\n", c->getAIGoal(), c->getTeam(), c->getX(), c->getY(), bestX, bestY);
    c->move(bestX, bestY);
}

void HeroAI::moveTowardsGoal(Hero *c) {
    uint8_t x;
    uint8_t y;
    switch (c->getAIGoal()) {
        case AI_NO_GOAL:
            return;
        case AI_TOP_LANE:
            x = 2;
            y = 2;
            break;
        case AI_MID_LANE:
            if (c->getTeam() == Team_1) {
                x = MAP_W - 3;
                y = 2;
            }
            else {
                x = 2;
                y = MAP_H - 3;
            }
            break;
        case AI_BOT_LANE:
            x = MAP_W - 3;
            y = MAP_H - 3;
            break;
    }
    Serial.printf("Creep %d team %d: moves towards goal %d, %d\n", c->getAIGoal(), c->getTeam(), x, y);
    Point p = m_map->findEmptyPointAround(x, y);
    if (p.x == -1) {
        return;
    }
    Serial.printf("Creep %d team %d: computed the closest point %d, %d\n", c->getAIGoal(), c->getTeam(), p.x, p.y);
    moveToNextPosition(c, p.x, p.y);
}

bool HeroAI::attackEnemyIfAround(Hero *c) {
    VisibleItems *items = m_map->getUnitsInRadius(c->getX(), c->getY(), 2);
    for (int i=0; i < MAX_VISIBLE_ITEMS; i++) {
        MapItem *item = (*items)[i];
        if (!item) {
            return false;
        }

        if (item->getTeam() == c->getTeam()) {
            continue;
        }

        Hero *enemy = item->getHero();
        Serial.printf("Creep %d team %d: going to attack enemy at %d, %d\n", c->getAIGoal(), c->getTeam(), enemy->getX(), enemy->getY());

        // Move closer to the enemy if we are not standing next to it.
        if (abs(c->getX() - enemy->getX()) > 1 || abs(c->getY() - enemy->getY()) > 1) {
            moveToNextPosition(c, enemy->getX(), enemy->getY());
        }

        // Attack enemy.
        c->attack(enemy);
        return true;
    }
    return false;
}

void HeroAI::handleGameRoundStarted(uint8_t round) {
    Serial.printf("Round %d: AI START\n", round);
    Creeps *creeps = m_map->getCreeps();
    for(int i = 0; i < MAX_CREEPS_COUNT; i++) {
        Hero *c = (*creeps)[i];
        if (!c) {
            continue;
        }

        if (attackEnemyIfAround(c)) {
            continue;
        }
        moveTowardsGoal(c);
    }
    Serial.printf("Round %d: AI END\n", round);
}
