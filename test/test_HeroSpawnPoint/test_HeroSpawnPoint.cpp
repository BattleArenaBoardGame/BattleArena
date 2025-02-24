#include <Arduino.h>
#include <unity.h>
#include <Game.h>
#include <Map.h>
#include <MapItems/Hero.h>
#include <MapItems/HeroSpawnPoint.h>

Game game;
Map *game_map = nullptr;

void setUp(void) {
    game_map = game.getMap();
    game_map->initializeMap(1, 1);
}

void tearDown(void) {
    // clean stuff up here
}

void test_handleGameRoundStarted() {
    // No hero at first.
    MapItem *hero = game_map->getUnitItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(nullptr, hero);

    // Let's start the first round, it should spawn the hero.
    MapItem *spawn = game_map->getMapItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(MapItemType_HeroSpawnPoint, spawn->getType());
    spawn->handleGameRoundStarted(1);

    // The Hero is spawned at the spawn point location.
    hero = game_map->getUnitItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(MapItemType_Hero, hero->getType());
}

void test_handleGameRoundStartedNotDead() {
    // No hero at first.
    MapItem *hero = game_map->getUnitItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(nullptr, hero);

    // Hero is not dead, so it should not be spawned.
    MapItem *spawn = game_map->getMapItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(MapItemType_HeroSpawnPoint, spawn->getType());
    spawn->getHero()->setIsDead(false);
    spawn->handleGameRoundStarted(1);

    // The Hero is not spawned, since it is dead.
    hero = game_map->getUnitItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(nullptr, hero);
}

void test_handleGameRoundStartedTimerNotReady() {
    // No hero at first.
    MapItem *hero = game_map->getUnitItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(nullptr, hero);

    // Timer is not ready, so no hero should be spawned.
    MapItem *spawn = game_map->getMapItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(MapItemType_HeroSpawnPoint, spawn->getType());
    static_cast<HeroSpawnPoint *>(spawn)->setSpawnTimer(1);

    // The Hero is not spawned, because the timer is 1.
    spawn->handleGameRoundStarted(1);
    Hero *h = static_cast<Hero*>(game_map->getUnitItem(0, MAP_H - 1));
    TEST_ASSERT_EQUAL(nullptr, hero);

    // The Hero is spawned now.
    spawn->handleGameRoundStarted(2);
    h = static_cast<Hero*>(game_map->getUnitItem(0, MAP_H - 1));
    TEST_ASSERT_EQUAL(MapItemType_Hero, h->getType());
    TEST_ASSERT_EQUAL(0, h->getX());
    TEST_ASSERT_EQUAL(MAP_H - 1, h->getY());
}

void run_tests() {
    UNITY_BEGIN();
    RUN_TEST(test_handleGameRoundStarted);
    RUN_TEST(test_handleGameRoundStartedNotDead);
    RUN_TEST(test_handleGameRoundStartedTimerNotReady);
    UNITY_END();
}

void setup() {
    delay(2000); // service delay
    run_tests();
}

void loop() {
}

int main(int argc, char **argv) {
    run_tests();
}
