#include "InputKeyboard.h"

void InputKeyboard::KeyDown(unsigned int keyIndex) {
	// If a key is pressed then save that state in the key array.
	switch (keys[keyIndex]) {
	case KeyState::IDLE:
		keys[keyIndex] = KeyState::DOWN;
		break;
	case KeyState::DOWN:
		keys[keyIndex] = KeyState::STAY;
		break;
	}
	return;
}

void InputKeyboard::KeyUp(unsigned int keyIndex) {
	
	switch (keys[keyIndex]) {
	case KeyState::DOWN:
		keys[keyIndex] = KeyState::UP;
		break;
	case KeyState::STAY:
		keys[keyIndex] = KeyState::STAY_UP;
		break;
	}
}

bool InputKeyboard::isKeyDown(unsigned int keyIndex) {
	// Return what state the key is in (pressed/not pressed).
	return keys[keyIndex] == KeyState::DOWN;
}

void InputKeyboard::readEveryFrame() {
	for (int i = 0; i < 256; i++) {
		switch (keys[i]) {
		case KeyState::UP:
		case KeyState::STAY_UP:
			keys[i] = KeyState::IDLE;
			break;

		case KeyState::DOWN:
			keys[i] = KeyState::STAY;
			break;
		}
	}
}