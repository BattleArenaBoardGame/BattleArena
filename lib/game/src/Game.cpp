/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include <Game.h>

Game::Game() : m_map(this) {

}


Map *Game::getMap() {
    return &m_map;
}

void Game::start(uint8_t team1Heroes, u_int8_t team2Heroes) {
    // Set the initial game variables.
    m_currentTeam = Team_1;
    m_currentHeroId = 0;
    m_team1Heroes = team1Heroes;
    m_heroesCount = team1Heroes + team2Heroes;
    m_currentRound = 1;

    // Initialize the map and start the game.
    m_map.initializeMap(team1Heroes, team2Heroes);
    m_map.handleGameStarted();
    m_map.handleGameRoundStarted(m_currentRound);
}

void Game::endTurn() {
    m_currentHeroId++;
    if (m_currentHeroId > m_heroesCount - 1) {
        m_currentHeroId = 0;
        m_currentTeam = Team_1;
        m_currentRound++;
        m_map.handleGameRoundStarted(m_currentRound);
    }
    else if (m_currentHeroId > m_team1Heroes - 1) {
        m_currentTeam = Team_2;
    }
    m_map.handleGameTurnStarted();
}

uint8_t Game::getCurrentHeroId() {
    return m_currentHeroId;
}

Hero *Game::getCurrentHero(){
    return m_map.getHero(m_currentHeroId);
}

Hero *Game::getHero(uint8_t id) {
    return m_map.getHero(id);
}
