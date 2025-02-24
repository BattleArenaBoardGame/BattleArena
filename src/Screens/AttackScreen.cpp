/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include "Screens/AttackScreen.h"
#include "Screens/MovementScreen.h"
#include "Screens/PopUpScreen.h"
#include "Widgets/HeroStats.h"
#include "Map.h"
#include "Game.h"
#include "MapItems/Hero.h"

enum ButtonAction{
    CANCEL_ATTACK=50,
    ATTACK_NORMAL
};

AttackScreen::AttackScreen(TFT_eSPI *tft, Game *game): Screen(tft, game) {
    m_tft->fillScreen(TFT_BLACK);

    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        m_heroStats[i] = nullptr;
    }

    int heroStatsId = 0;
    Map *m = m_game->getMap();
    VisibleItems *items = m->getCurrentlyVisibleItems();
    for (int i = 0; i<m->getVisibleItemsToCurrentHeroId(); i++) {
        MapItem *item = (*items)[i];
        if (item->getType() != MapItemType_Hero) {
            continue;
        }

        Hero *h = item->getHero();
        if (h->getTeam() == m_game->getCurrentHero()->getTeam()) {
            continue;
        }

        m_heroStats[heroStatsId] = new HeroStats(m_game, h, 0, heroStatsId*45, 180, 45);
        heroStatsId++;
    }

    m_buttons = new Buttons(tft);
    // Cancel.
    m_buttons->addButton(new Button(CANCEL_ATTACK, "Cancel", 350, 10, 130, 60, TFT_RED));
    // Normal Attack.
    m_buttons->addButton(new Button(ATTACK_NORMAL, "Attack", 210, 10, 130, 60, TFT_YELLOW));
}

AttackScreen::~AttackScreen() {
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        if (m_heroStats[i]) {
            delete m_heroStats[i];
        }
    }

    delete m_buttons;
}



Screen *AttackScreen::refresh() {
    bool someHeroVisible = false;
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        if (m_heroStats[i]) {
            m_heroStats[i]->refresh(m_tft);
            someHeroVisible = true;
        }
    }

    if (!someHeroVisible) {
        m_tft->setTextSize(2);
        m_tft->setTextFont(4);
        m_tft->setTextColor(TFT_WHITE, TFT_BLACK);
        m_tft->setTextDatum(MC_DATUM);
        m_tft->drawString("No target visible.", 240, 160);
        return nullptr;
    }

    m_buttons->refresh();

    return nullptr;
}

Hero *AttackScreen::getSelectedHero() {
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        if (!m_heroStats[i]) {
            return nullptr;
        }
        if (m_heroStats[i]->getSelected()) {
            return m_heroStats[i]->getHero();
        }
    }
    return nullptr;
}

Screen *AttackScreen::touch(uint16_t x, uint16_t y) {
    int selectedHeroId = -1;
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        if (!m_heroStats[i]) {
            break;
        }
        if (m_heroStats[i]->touch(x, y)) {
            m_heroStats[i]->setSelected(true);
            selectedHeroId = i;
        }
    }
    if (selectedHeroId != -1) {
        for (int i = 0; i < MAX_HEROES_COUNT; i++) {
            if (!m_heroStats[i]) {
                break;
            }
            if (i != selectedHeroId) {
                m_heroStats[i]->setSelected(false);
            }
        }
        return nullptr;
    }

    Button *b = m_buttons->touch(x, y);
    Hero *h = nullptr;
    Screen *scr;
    AttackInfo ai;
    if (b) {
        switch(b->getId()) {
            case CANCEL_ATTACK:
                m_tft->fillScreen(TFT_BLACK);
                return new MovementScreen(m_tft, m_game);
            case ATTACK_NORMAL:
                h = getSelectedHero();
                if (!h) {
                    scr = new AttackScreen(m_tft, m_game);
                    return new PopUpScreen(m_tft, m_game, "No target selected.", scr);
                }
                ai = m_game->getCurrentHero()->attack(h);
                switch(ai) {
                    case ATTACK_OK:
                        m_tft->fillScreen(TFT_BLACK);
                        return new MovementScreen(m_tft, m_game);
                    case ATTACK_TARGET_NOT_VISIBLE:
                        scr = new AttackScreen(m_tft, m_game);
                        return new PopUpScreen(m_tft, m_game, "Target not visible.", scr);
                    case ATTACK_TARGET_TOO_FAR:
                        scr = new AttackScreen(m_tft, m_game);
                        return new PopUpScreen(m_tft, m_game, "Target too far.", scr);
                };
                m_tft->fillScreen(TFT_BLACK);
                return new MovementScreen(m_tft, m_game);
        }
    }

    return nullptr;
}

