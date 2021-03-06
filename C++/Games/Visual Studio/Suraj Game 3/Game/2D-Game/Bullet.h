#ifndef BULLET_H
	#define BULLET_H

	#include <iostream>

	#include "SFML/Graphics.hpp"

class Bullet { 
	private:
		sf::Sprite sprite;

		sf::Vector2f direction;

		float moveSpeed;

	public:
		void Update();
		void Render(sf::RenderTarget& target);

	public:
		Bullet(
			sf::Texture* texture,
			float posX,
			float posY,
			int dirX,
			int dirY,
			float moveSpeed
		);

		virtual ~Bullet();

		const sf::FloatRect getBounds() const;
};
#endif // ! BULLET_H