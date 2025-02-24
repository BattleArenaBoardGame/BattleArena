#include "Widgets/HeroStats.h"
#include "Game.h"
#include "Map.h"
#include "MapItems/Hero.h"
#include "Widgets/ProgressBar.h"

HeroStats::HeroStats(Game *game, Hero *hero, uint16_t x, uint16_t y, uint16_t w, uint16_t h) :
m_game(game), m_hero(hero), m_x(x), m_y(y), m_w(w), m_h(h), m_selected(false) {
    m_health = new ProgressBar("Health", m_x + 20, m_y + 20, (m_w - 20) / 2 - 5, 20, TFT_DARKGREEN, TFT_WHITE);
    m_mana = new ProgressBar("Mana", m_x + (m_w - 20) / 2 + 5 + 20, m_y + 20, (m_w - 20) / 2 - 5, 20, TFT_BLUE, TFT_WHITE);
}

HeroStats::~HeroStats() {
    delete m_health;
    delete m_mana;
}

bool HeroStats::touch(uint16_t x, uint16_t y) {
    if (x > m_x && y > m_y && x < m_x + m_w && y < m_y + m_h) {
        return true;
    }
    return false;
}

void HeroStats::refresh(TFT_eSPI *tft) {
    // Skip heroes which do not play at all.
    if (m_hero->getTeam() == Team_Unknown) {
        return;
    }

    // Show the square with color for the hero.
    tft->fillRect(m_x + 20, m_y, 20, 20, m_hero->getColor());

    // Show the Turn indicator for current hero.
    uint32_t selectColor = TFT_BLACK;
    if (m_selected) {
        selectColor = TFT_WHITE;
    }
    tft->fillRect(m_x, m_y, 10, 40, selectColor);

    // Show the Hero values only if Hero is in the same team
    // as the one currently on turn or if the Hero is visible.
    if (m_game->getCurrentHero()->getTeam() == m_hero->getTeam() || m_game->getMap()->isItemVisible(m_hero)) {
        m_health->setValue(m_hero->getHealth());
        m_mana->setValue(m_hero->getMana());
    }
    else {
        m_health->setValue(0);
        m_mana->setValue(0);
    }
    m_health->setMaxValue(m_hero->getMaxHealth());
    m_mana->setMaxValue(m_hero->getMaxMana());
    m_health->refresh(tft);
    m_mana->refresh(tft);
}
