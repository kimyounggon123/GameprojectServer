#ifndef _INPUTKEYBOARD_H
#define _INPUTKEYBOARD_H


enum class KeyState {
	IDLE,
	DOWN,
	UP,
	STAY,
	STAY_UP
};
class InputKeyboard
{
	KeyState keys[256];


	static InputKeyboard* instance;
	InputKeyboard(){
		for (int i = 0; i < 256; i++)
			keys[i] = KeyState::IDLE;
	}
public:
	static InputKeyboard& getInstance() {
		if (instance == nullptr) instance = new InputKeyboard;
		return *instance;
	}

	void KeyDown(unsigned int keyIndex);
	void KeyUp(unsigned int keyIndex);

	void readEveryFrame();

	bool isKeyDown(unsigned int keyIndex);
};
#endif
