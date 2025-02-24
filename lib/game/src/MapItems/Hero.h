/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_HERO_H
#define BABG_HERO_H

#include <Arduino.h>
#include <MapItems/MapItem.h>
#include <HeroAI.h>

enum AttackInfo{
  ATTACK_OK,
  ATTACK_TARGET_TOO_FAR,
  ATTACK_TARGET_NOT_VISIBLE,
};

class Hero : public MapItem {
  public:
    Hero(Map *map, Team team, uint32_t color, HeroAIGoal aiGoal = AI_NO_GOAL);

    uint32_t getColor() { return m_color; }
    void setColor(uint32_t color) { m_color = color; }
    Team getTeam() { return m_team; }
    Hero *getHero() { return this; }
    HeroAIGoal getAIGoal() { return m_aiGoal; }

    bool isDead() { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }

    void move(uint8_t x, uint8_t y);
    void move(Direction direction);

    AttackInfo canAttack(Hero *target);
    AttackInfo attack(Hero *target);
    void takeDamage(Hero *source, uint16_t dmg);

    void setPosition(uint8_t x, uint8_t y) { m_x = x; m_y = y; }
    uint8_t getX() { return m_x; }
    uint8_t getY() { return m_y; }

    uint8_t getMaxHealth() { return m_maxHealth; }
    uint8_t getHealth() { return m_health; }
    uint8_t getMaxMana() { return m_maxMana; }
    uint8_t getMana() { return m_mana; }
    uint8_t getXP() { return m_xp; }
    uint8_t getLevel() { return m_level; }
    uint8_t getViewRadius() { return m_viewRadius; }
    uint8_t getBaseDmg() { return m_baseDmg; }
    uint8_t getBonusDmg() { return m_bonusDmg; }
    uint8_t getArmor() { return m_armor; }
    uint8_t getMagicRes() { return m_magicRes; }
    bool isDmgMagic() { return m_isDmgMagic; }

  private:

    uint32_t m_color;     // Color.
    Team m_team;
    uint8_t m_x;          // X coordinate to backtrack the hero to Map.
    uint8_t m_y;          // Y coordinate to backtrack the hero to Map.
    uint8_t m_maxHealth;  // Max health.
    uint8_t m_health;     // Curent health.
    uint8_t m_maxMana;    // Max Mana.
    uint8_t m_mana;       // Current mana.
    uint8_t m_xp;         // Current XP.
    uint8_t m_level;      // Current level.
    uint8_t m_viewRadius; // View radius of hero.
    uint8_t m_baseDmg;    // Base damage.
    uint8_t m_bonusDmg;   // Bonus damage added by items and buffs.
    bool m_isDmgMagic;    // True if the basic damage is magic.
    uint8_t m_armor;      // Armor.
    uint8_t m_magicRes;   // Magic Resistance - similar to armor, but or magic attacks.
    bool m_isDead;        // True if dead and not visible on map;
    HeroAIGoal m_aiGoal;  // AI Goal.
};

#endif