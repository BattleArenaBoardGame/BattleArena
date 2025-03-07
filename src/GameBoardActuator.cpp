#include "GameBoardActuator.h"

GameBoardActuator::GameBoardActuator(
    int rotationDirPin, int rotationStepPin, int tiltDirPin, int tiltStepPin,
    int rotationLimitSwitchPin, int tiltLimitSwitchPin
) :
m_rotationStepper(AccelStepper::DRIVER, rotationStepPin, rotationDirPin),
m_tiltStepper(AccelStepper::DRIVER, tiltStepPin, tiltDirPin),
m_rotationInitialized(false),
m_tiltInitialized(false),
m_rotationLimit(false),
m_tiltLimit(false),
m_rotationLimitSwitchPin(rotationLimitSwitchPin),
m_tiltLimitSwitchPin(tiltLimitSwitchPin) {

}

void GameBoardActuator::setup() {
    // Set the limit switches pins to INPUT.
    // Pull-downs are connected externally.
	pinMode(m_rotationLimitSwitchPin, INPUT); 
	pinMode(m_tiltLimitSwitchPin, INPUT);

    // Set the speed and acceleration for tilt stepper.
	m_tiltStepper.setMaxSpeed(5000);
	m_tiltStepper.setAcceleration(1800);
	m_tiltStepper.setSpeed(500);

    // Set the speed and acceleration for rotation stepper.
	m_rotationStepper.setMaxSpeed(100);
	m_rotationStepper.setAcceleration(20);
	m_rotationStepper.setSpeed(-10);
}

void GameBoardActuator::run() {
    // Check if the limit switch is pressed and update the m_rotationLimit.
	if (digitalRead(m_rotationLimitSwitchPin) == HIGH) {
        // If the stepper was moving, stop the movement immediatelly
        // and set the position to 0 to finish the calibration.
        if (!m_rotationLimit) {
            m_rotationStepper.stop();
            m_rotationStepper.setSpeed(0);
            m_rotationStepper.setCurrentPosition(0);
            m_rotationLimit = true;
            Serial.println("Rotation stepper reached limit.");
        }
		m_rotationInitialized = true;
	}
    else {
        m_rotationLimit = false;
    }

    // Check if the limit switch is pressed and update the m_tiltLimit.
	if (digitalRead(m_tiltLimitSwitchPin) == HIGH) {
        // If the stepper was moving, stop the movement immediatelly
        // and set the position to 0 to finish the calibration.
        if (!m_tiltLimit) {
            m_tiltStepper.stop();
            m_tiltStepper.setSpeed(0); 
            m_tiltStepper.setCurrentPosition(0);
            m_tiltLimit = true;
            Serial.println("Tilt stepper reached limit.");
        }
        if (!m_tiltInitialized) {
		    m_tiltInitialized = true;
            tilt(false);
        }
	}
    else {
        m_tiltLimit = false;
    }


    // Tilt in constant speed if we are calibrating, otherwise use acceleration.
	if (!m_tiltInitialized) {
		m_tiltStepper.runSpeed();
	}
	else {
		m_tiltStepper.run();
	}

    // Rotate in constant speed if we are calibrating, otherwise use acceleration.
	if (!m_rotationInitialized) {
		m_rotationStepper.runSpeed();
	}
	else {
		m_rotationStepper.run();
	}
}

void GameBoardActuator::rotateToTeam(Team team) {
    if (!m_rotationInitialized) {
        return;
    }
    switch(team) {
        case Team_Unknown:
            break;
        case Team_1:
            m_rotationStepper.moveTo(0);
            break;
        case Team_2:
            m_rotationStepper.moveTo(105);
            break;
    }
}

void GameBoardActuator::tilt(boolean down) {
    if (!m_tiltInitialized) {
        return;
    }
    if (down) {
        m_tiltStepper.moveTo(0);
    }
    else {
        m_tiltStepper.moveTo(-1700);
    }
}
