#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieTimeFrame(500)
{
	_munchieFrameCount = 0;
	//Sets pause menu to false
	_paused = false;
	_pKeyDown = false;
	_startGame = false;
	_pacmanDirection = 0; //Set the direction of the facing pacman
	_pacmanCurrentFrameTime = 0; //Set animation frame
	_pacmanFrame = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieCombinedTexture;
	delete _munchieRect;
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman right on the X axis
		_pacmanDirection = 0;

	}
	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman left on the X axis
		_pacmanDirection = 2;
	}
	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman Down on the Y axis
		_pacmanDirection = 1;
	}
	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman Up on the Y axis
		_pacmanDirection = 3;
	}


	//Hold key down will stop the munchie going through wall
	if (state->IsKeyDown(Input::Keys::E) && _isPressed == false)
	{
		_isPressed = true;
		_wrap = !_wrap;
	}

	if (state->IsKeyUp(Input::Keys::E))
		_isPressed = false;

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
		if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth() + 30) // 1064 is game width
		{
			// Pacman hit right wall - Move to other screen
			_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacmanPosition->X + _pacmanSourceRect->Width < 0) // 0 is game width
		{
			// Pacman hit left wall - Move to other screen
			_pacmanPosition->X = Graphics::GetViewportWidth() + 30 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight()) // 765 is game height
		{
			// Pacman hit bottom wall - Move to other screen
			_pacmanPosition->Y = 0 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to wrap through screen
		if (_pacmanPosition->Y + _pacmanSourceRect->Height < 0) // 0 is game height
		{
			// Pacman hit up wall - Move to other screen
			_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Width;
		}
	}
	else
	{
		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth()) // 1024 is game width
		{
			// Pacman hit right wall - Reset his position
			_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->X + _pacmanSourceRect->Width < 32) // 0 is game width
		{
			// Pacman hit left wall - Reset his position
			_pacmanPosition->X = 32 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight()) // 765 is game height
		{
			// Pacman hit bottom wall - Reset his position
			_pacmanPosition->Y = 765 - _pacmanSourceRect->Width;
		}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height < 32) // 0 is game height
		{
			// Pacman hit up wall - Reset his position
			_pacmanPosition->Y = 32 - _pacmanSourceRect->Width;
		}
	}
}


void Pacman::UpdatePacman(int elapsedTime)
{
	//Set the direction facing of Pacman
	_pacmanCurrentFrameTime += elapsedTime;
	//In this code, we are updating the _pacman frame variable by one
	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;

		//If pacmanFrame is greater then 2, reset pacman to frame to 0
		if (_pacmanFrame >= 2)
			_pacmanFrame = 0;

		_pacmanCurrentFrameTime = 0;
	}
	_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
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


void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieCombinedTexture = new Texture2D();
	_munchieCombinedTexture->Load("Textures/MunchieCombined.png", true);
	_munchiePosition = new Vector2(100.0f, 100.0f);
	_munchieRect = new Rect(0.0f, 0.0f, 12, 12);
	_munchieCurrentFrameTime = 0;

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
			Input(elapsedTime, keyboardState);
			CheckViewpointCollision();
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;
	stream << " Is wall wrap true: " << _wrap;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman
	SpriteBatch::Draw(_munchieCombinedTexture, _munchiePosition, _munchieRect);//Draws munchie

	if (_munchieFrameCount == 0)
	{
		_munchieRect->X = 12;
	}
	else
	{
		_munchieRect->X = 24;
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