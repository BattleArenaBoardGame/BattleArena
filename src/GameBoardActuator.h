/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_GAMEBOARDACTUATOR_H
#define BABG_GAMEBOARDACTUATOR_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <MapItems/MapItem.h>


class GameBoardActuator {
  public:
    GameBoardActuator(
      int rotationDirPin,
      int rotationStepPin,
      int tiltDirPin,
      int tiltStepPin,
      int rotationLimitSwitchPin, 
      int tiltLimitSwitchPin
    );

    void setup();
    void run();
  
    void rotateToTeam(Team team);
    void tilt(boolean down);

  private:
    AccelStepper m_rotationStepper;
    AccelStepper m_tiltStepper;
    bool m_rotationInitialized;
    bool m_tiltInitialized;
    int m_tiltLimitSwitchPin;
    int m_rotationLimitSwitchPin;
    bool m_rotationLimit;
    bool m_tiltLimit;
};


#endif