#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Structure Definition
struct Player
{
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewpointCollision();

	//Update method
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);


	// Data to represent Pacman
	Player* _pacman;
	float speedMultiplier;
	int currentFrameTime;
	const int _cPacmanFrameTime;
	


	//Data For Menu
	Texture2D* _menuBackGround;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	// Data to represent Munchie
	int _munchieFrameCount;
	Rect* _munchieRect;
	Texture2D* _munchieCombinedTexture;
	Vector2* _munchiePosition;

	//Data to represented Cherry
		// Data to represent Munchie
	int _cherryFrameCount;
	Rect* _cherryRect;
	Texture2D* _cherryCombinedTexture;
	Vector2* _cherryPosition;

	//Changing the animation to Elapsed time
	const int _cMunchieTimeFrame;
	int _munchieFrame;
	int _munchieCurrentFrameTime;

	// Position for String
	Vector2* _stringPosition;

	//Boolean to check if true or false for moving through wall
	bool _wrap = true;
	bool _isPressed = false;

	//Check key pressed for pause menu.
	bool _pKeyDown;

	//Set variable for Start game
	bool _startGame;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};

/*
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;
	//Constant data for Game Variable
	const float _cPacmanSpeed;
	//Set the direction int for facing pacman
	int _pacmanDirection;
	//Set animation int.
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	const int _cPacmanFrameTime;
*/