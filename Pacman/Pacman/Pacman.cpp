#include "Pacman.h"

#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), speedMultiplier(0.1f),  _cPacmanFrameTime(250), _cMunchieTimeFrame(500)
{
	enum class S2D_API ButtonState
	{
		RELEASED = 0,
		PRESSED
	};

	srand(time(NULL));

	//Initilise Member variable
	_pacman = new Player();
	_pacman->dead = false;
	//Initialise array fpr munchie count
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i] = new Enemy();
	}

	//Initialise array for Moving Ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
	}
	_cherry = new Cherry();

	_munchieFrameCount = 0;
	_munchieCurrentFrameTime = 0;
	//Sets pause menu to false
	_paused = false;
	_pKeyDown = false;
	_startGame = false;
	_pacman->direction = 0; //Set the direction of the facing pacman
	_pacman->speedBoost = 1.0f;
	currentFrameTime = 0; //Set animation frame
	_pacman->frame = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;
	
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie[i];
		delete _munchie[i]->_munchieCombinedTexture;
		delete _munchie[i]->_munchiePosition;
		delete _munchie[i]->_munchieRect;
	}
	delete _cherry;
	delete _cherry->_cherryCombinedTexture;
	delete _cherry->_cherryRect;
	delete _cherry->_cherryPosition;
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	//Create a variable to unify speed  with added speed boost
	float pacmanSpeed = speedMultiplier * elapsedTime * _pacman->speedBoost;

	//Changing position value using the new speed float variable
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		//Moves Pacman across +X axis (right)
		_pacman->position->X += pacmanSpeed;
		_pacman->direction = 0;

	}
	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		//Moves Pacman across +X axis (Left)
		_pacman->position->X -= pacmanSpeed;
		_pacman->direction = 2;
	}
	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		//Moves Pacman across +X axis (Up)
		_pacman->position->Y += pacmanSpeed;
		_pacman->direction = 1;
	}
	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		//Moves Pacman across +X axis (Down)
		_pacman->position->Y -= pacmanSpeed;
		_pacman->direction = 3;
	}

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Apply mulitiplier
		_pacman->speedBoost = 2.0f;
	}
	else
	{
		//Reset multiplier
		_pacman->speedBoost = 1.0f;
	}

	//Hold key down will stop the munchie going through wall
	if (state->IsKeyDown(Input::Keys::E) && _isPressed == false)
	{
		_isPressed = true;
		_wrap = !_wrap;
	}

	if (state->IsKeyUp(Input::Keys::E))
		_isPressed = false;

	//Handle mouse input - Reposition Cherry
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_cherry->_cherryPosition->X = mouseState->X;
		_cherry->_cherryPosition->Y = mouseState->Y;
	}

}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	//Checks if pause menu is activated
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
		
	}
}

void Pacman::CheckViewpointCollision()
{
	if (_wrap == true)
	{
		// Checks if Pacman is trying to wrap through screen
		if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth() + 30) // 1064 is game width
		{
			// Pacman hit right wall - Move to other screen
			_pacman->position->X = 0 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacman->position->X + _pacman->sourceRect->Width < 0) // 0 is game width
		{
			// Pacman hit left wall - Move to other screen
			_pacman->position->X = Graphics::GetViewportWidth() + 30 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight()) // 765 is game height
		{
			// Pacman hit bottom wall - Move to other screen
			_pacman->position->Y = 0 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacman->position->Y + _pacman->sourceRect->Height < 0) // 0 is game height
		{
			// Pacman hit up wall - Move to other screen
			_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Width;
		}
	}
	else
	{
		// Checks if Pacman is trying to dissapear
		if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth()) // 1024 is game width
		{
			// Pacman hit right wall - Reset his position
			_pacman->position->X = 1024 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacman->position->X + _pacman->sourceRect->Width < 32) // 0 is game width
		{
			// Pacman hit left wall - Reset his position
			_pacman->position->X = 32 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight()) // 765 is game height
		{
			// Pacman hit bottom wall - Reset his position
			_pacman->position->Y = 765 - _pacman->sourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacman->position->Y + _pacman->sourceRect->Height < 32) // 0 is game height
		{
			// Pacman hit up wall - Reset his position
			_pacman->position->Y = 32 - _pacman->sourceRect->Width;
		}
	}
}

void Pacman::CheckGhostCollisions()
{
}

void Pacman::UpdateGhost(MovingEnemy*, int elapsedTime)
{
}


void Pacman::UpdatePacman(int elapsedTime)
{
	//Set the direction facing of Pacman
	currentFrameTime += elapsedTime;
	//In this code, we are updating the _pacman frame variable by one
	if (currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		//If pacmanFrame is greater then 2, reset pacman to frame to 0
		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
}

void Pacman::UpdateMunchie(int elapsedTime)
{

		_munchieCurrentFrameTime += elapsedTime;

		if (_munchieCurrentFrameTime > _cMunchieTimeFrame)
		{
			_munchieFrameCount++;

			if (_munchieFrameCount >= 2)
				_munchieFrameCount = 0;

			_munchieCurrentFrameTime = 0;
		}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherryCurrentFrameTime += elapsedTime;
	if (_cherryCurrentFrameTime > _cMunchieTimeFrame)
	{
		_cherryFrameCount++;

		if (_cherryFrameCount >= 2)
			_cherryFrameCount = 0;

		_cherryCurrentFrameTime = 0;
	}
}


void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->_munchieCombinedTexture = new Texture2D();
		_munchie[i]->_munchieCombinedTexture->Load("Textures/MunchieCombined.png", true);
		_munchie[i]->_munchiePosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchie[i]->_munchieRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchieCurrentFrameTime = 0;
	}

	//Load Cherry
	_cherry->_cherryCombinedTexture = new Texture2D();
	_cherry->_cherryCombinedTexture->Load("Textures/CherryMerged.png", true);
	_cherry->_cherryPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_cherry->_cherryRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherryCurrentFrameTime = 0;

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set Menu Parameters
	_menuBackGround = new Texture2D();
	_menuBackGround->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_startGame)
	{
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			_startGame = true;
		}
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_paused)
		{
			Input(elapsedTime, keyboardState, mouseState);
			CheckViewpointCollision();
			UpdatePacman(elapsedTime);
			
			UpdateMunchie(elapsedTime);
			
			UpdateCherry(elapsedTime);
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
	stream << " Is wall wrap true: " << _wrap;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchie[i]->_munchieCombinedTexture, _munchie[i]->_munchiePosition, _munchie[i]->_munchieRect);//Draws munchie
	}
	SpriteBatch::Draw(_cherry->_cherryCombinedTexture, _cherry->_cherryPosition, _cherry->_cherryRect);//Draws Cherry

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		/*_munchie[i]->_munchieRect->X = _munchie[i]->_munchieRect->Width * _munchieFrameCount;*/
		if (_munchieFrameCount == 0)
		{
		
			_munchie[i]->_munchieRect->X = 0;
		
		}
		else
		{
		
			_munchie[i]->_munchieRect->X = 12;
		
		}
	}

	if (_cherryFrameCount == 0)
	{
		_cherry->_cherryRect->X = 0;
	}
	else
	{
		_cherry->_cherryRect->X = 32;
	}

	//Start Game Menu
	if (! _startGame)
	{
		std::stringstream menuStream;
		//Draw string to screen.
		menuStream << "Press Space to Start Game";

		
		SpriteBatch::Draw(_menuBackGround, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Blue);
	}

	//If game is paused then Pause is printed to screen
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackGround, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing

	
}