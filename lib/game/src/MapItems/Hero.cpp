/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <MapItems/Hero.h>
#include "Map.h"
#include "Hero.h"

Hero::Hero(Map *map, Team team, uint32_t color, HeroAIGoal aiGoal) : MapItem(map, MapItemType_Hero) {
  m_aiGoal = aiGoal;
  m_color = color;
  m_team = team;
  m_x = 0;
  m_y = 0;
  m_maxHealth = 150;
  m_health = 100;
  m_maxMana = 200;
  m_mana = 100;
  m_xp = 0;
  m_level = 1;
  m_viewRadius = 3;
  m_isDead = true;
  m_armor = 1;
  m_magicRes = 1;
  m_baseDmg = 35;
  m_bonusDmg = 0;
  m_isDmgMagic = false;
}

void Hero::takeDamage(Hero *source, uint16_t dmg) {
  int health = m_health - dmg;
  if (health < 0) {
    m_health = 0;
    m_isDead = true;
    m_map->removeUnitItem(this);
    // if (m_type == MapItemType_Creep) {
    //   delete this;
    // }
    return;
  }

  m_health = health;
}

AttackInfo Hero::canAttack(Hero *target) {
  int x = target->getX();
  int y = target->getY();

  // Check if the target is visible.
  // if (!m_map->isItemVisible(target)) {
  //   return ATTACK_TARGET_NOT_VISIBLE;
  // }

  // Target is not next to us.
  if (abs(x - m_x) > 1 || abs(y - m_y) > 1) {
    return ATTACK_TARGET_TOO_FAR;
  }

  return ATTACK_OK;
}

AttackInfo Hero::attack(Hero *target) {
  AttackInfo ai = canAttack(target);
  if (ai != ATTACK_OK) {
    Serial.printf("Cannot attack %d\n", ai);
    return ai;
  }

  uint16_t dmgTaken = (m_baseDmg + m_bonusDmg) * (1 + 0.05 * m_level);
  if (m_isDmgMagic) {
    dmgTaken = dmgTaken * (1 - (target->getMagicRes() / (100 + target->getMagicRes())));
  }
  else {
    dmgTaken = dmgTaken * (1 - (target->getArmor() / (100 + target->getArmor())));
  }

  target->takeDamage(this, dmgTaken);
  Serial.printf("%d\n", target->getHealth());
  return ATTACK_OK;
}

void Hero::move(uint8_t x, uint8_t y) {
  m_map->setUnitItem(m_x, m_y, nullptr);
  m_map->setUnitItem(x, y, this);
}

void Hero::move(Direction direction) {
  uint8_t x = m_x;
  uint8_t y = m_y;
  switch(direction) {
    case UP:
      if (y != 0)
        y--;
      break;
    case UP_LEFT:
      if (y != 0)
        y--;
      if (x != 0)
        x--;
      break;
    case LEFT:
      if (x != 0)
        x--;
      break;
    case DOWN_LEFT:
      if (y != MAP_H - 1)
        y++;
      if (x != 0)
        x--;
      break;
    case DOWN:
      if (y != MAP_H - 1)
        y++;
      break;
    case DOWN_RIGHT:
      if (y != MAP_H - 1)
        y++;
      if (x != MAP_W - 1)
        x++;
      break;
    case RIGHT:
      if (x != MAP_W - 1)
        x++;
      break;
    case UP_RIGHT:
      if (y != 0)
        y--;
      if (x != MAP_W - 1)
        x++;
      break;
  }
  uint8_t original_x = m_x;
  uint8_t original_y = m_y;
  if (m_map->setUnitItem(x, y, this)) {
    m_map->setUnitItem(original_x, original_y, nullptr);
  }
}
