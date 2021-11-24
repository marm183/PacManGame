#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif
#define MUNCHIECOUNT 50 //Defining how a macro for how many Munchies on screen
#define GHOSTCOUNT 1 //Defining a macro for how many ghosts on screen

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Structure Definition
struct Player
{
	bool dead;
	int direction;
	int frame;
	float speedBoost;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct Enemy
{
	
	Rect* _munchieRect;
	Texture2D* _munchieCombinedTexture;
	Vector2* _munchiePosition;
};

struct Cherry
{
	Rect* _cherryRect;
	Texture2D* _cherryCombinedTexture;
	Vector2* _cherryPosition;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewpointCollision();
	void CheckGhostCollisions();
	void UpdateGhost(MovingEnemy*, int elapsedTime);

	//Update method
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);
	void UpdateCherry(int elapsedTime);


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
	Enemy* _munchie[MUNCHIECOUNT];
	int _munchieFrameCount;
	int _munchieCurrentFrameTime;

	//Data to represent moving enemy
	MovingEnemy* _ghosts[GHOSTCOUNT];

	//Data to represented Cherry
	Cherry* _cherry;
	int _cherryFrameCount;
	int _cherryCurrentFrameTime;

	//Changing the animation to Elapsed time
	const int _cMunchieTimeFrame;
	int _munchieFrame;
	

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
