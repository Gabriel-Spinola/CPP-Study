#include "Bullet.h"

void Bullet::Update() { 

}

void Bullet::Render(sf::RenderTarget* target) {

}

Bullet::Bullet(sf::Texture& texture, int dirX, int dirY, float moveSpeed) {
	this->sprite.setTexture(texture);

	this->direction.x = dirX;
	this->direction.y = dirY;

	this->moveSpeed = moveSpeed;
}

Bullet::~Bullet() {
	
}
