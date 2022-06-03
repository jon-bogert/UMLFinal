#include "Pch.h"
#include "Player.h"
#include "InputSystem.h"

const float MOVE_SPEED = 100.f;
const float LASER_SPEED = 500.f;

Player::Player(Application* app)
	: app(app),
	window(app->GetWindow()),
	game(app->GetGame())
{
	position = { (float)((app->GetResolution().x / 2) - (ss_res.x / 2) * scale), (float)app->GetResolution().y * 0.9f };

	tempTexture = new sf::Texture;
	tempTexture->loadFromFile("Assets/player.png");

	laserTexture = new sf::Texture;
	laserTexture->loadFromFile("Assets/player-laser.png");
}

Player::~Player()
{
	for (sf::Sprite* laser : lasers)
	{
		delete laser;
	}
	delete laserTexture;
	delete sprite;
	delete tempTexture;
}

void Player::SetupSprite()
{
	sprite = new sf::Sprite;
	//std::cout << "Player: " << game->GetTexture("player") << std::endl;
	sprite->setTexture(*tempTexture);
	sprite->setTextureRect(frame);
	sprite->setScale(scale, scale);
}

void Player::Move()
{
	if ((InputSystem::MoveAxis().x < 0 && position.x > 0) || (InputSystem::MoveAxis().x > 0 && position.x < app->GetResolution().x - (ss_res.x * scale)))
		position.x += MOVE_SPEED * InputSystem::MoveAxis().x * app->DeltaTime();
	for (sf::Sprite* laser : lasers)
	{
		laser->setPosition(laser->getPosition().x, laser->getPosition().y - (LASER_SPEED * app->DeltaTime()));
	}
}

void Player::Fire()
{
	if (InputSystem::Fire())
	{
		int index = lasers.size();
		lasers.push_back(new sf::Sprite);
		lasers[index]->setTexture(*laserTexture);
		lasers[index]->setTextureRect(sf::Rect(0, 0, laserDim.x, laserDim.y));
		lasers[index]->setScale(scale, scale);
		lasers[index]->setPosition(position.x + ((ss_res.x / 2) * scale), position.y - (laserDim.y * scale));
	}
	// Check Expiry
	int i = 0;
	while (i < lasers.size())
	{
		if (lasers[i]->getPosition().y < -50)
		{
			delete lasers[i];
			lasers.erase(lasers.begin() + i);
		}
		else
			i++;
	}
}

void Player::Draw()
{
	sprite->setPosition(position);
	window->draw(*sprite);
	for (sf::Sprite* laser : lasers)
	{
		window->draw(*laser);
	}
}

void Player::ShipChange()
{
	
	if (InputSystem::ChangeType())
	{
		frame.top = (frame.top + ss_res.y) % (ss_res.y * ss_numTypes);
		sprite->setTextureRect(frame);
	}
	if (InputSystem::ChangeColor())
	{
		frame.left = (frame.left + ss_res.x) % (ss_res.x * ss_numColors);
		sprite->setTextureRect(frame);
	}
}