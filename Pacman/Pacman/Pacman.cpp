#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	_frameCount = 0;

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
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
		_pacmanPosition->X += 0.1f * elapsedTime; //Moves Pacman right on the X axis
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
		_pacmanPosition->X -= 0.1f * elapsedTime; //Moves Pacman left on the X axis
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
		_pacmanPosition->Y += 0.1f * elapsedTime; //Moves Pacman Down on the Y axis
	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
		_pacmanPosition->Y -= 0.1f * elapsedTime; //Moves Pacman Up on the Y axis

	// Checks if Pacman is trying to dissapear
	if (_pacmanPosition->X + _pacmanSourceRect->Width > 1024) // 1024 is game width
	{
		// Pacman hit right wall - Reset his position
		_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacmanPosition->X + _pacmanSourceRect->Width < 32) // 0 is game width
	{
		// Pacman hit left wall - Reset his position
		_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacmanPosition->Y + _pacmanSourceRect->Height > 765) // 765 is game height
	{
		// Pacman hit bottom wall - Reset his position
		_pacmanPosition->Y = 32 - _pacmanSourceRect->Width;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacmanPosition->Y + _pacmanSourceRect->Height < 32) // 0 is game height
	{
		// Pacman hit up wall - Reset his position
		_pacmanPosition->Y = 765 - _pacmanSourceRect->Width;
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}