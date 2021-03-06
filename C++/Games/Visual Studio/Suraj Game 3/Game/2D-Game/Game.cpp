#include "Game.h"

#define PBounds player->getBounds()

#define XWinSize window->getSize().x
#define YWinSize window->getSize().y
#define WinSizeVec sf::Vector2f(XWinSize, YWinSize)

void Game::InitWindow() { 
	this->window = new sf::RenderWindow(
		sf::VideoMode(800, 600),
		"Swaglords of Space - Game 3",
		sf::Style::Close | sf::Style::Titlebar
	);
	
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
}

void Game::InitTextures() {
	// Set the BULLET position to a new Texture
	this->textures["BULLET"] = new sf::Texture();

	// Set the texture
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");
}

void Game::InitGUI() { 
	if(!this->font.loadFromFile("Fonts/Metropolian-Display.ttf")) {
		std::cout << "Can't load GUI Font" << std::endl;
	}

	this->points = 0;

	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(24);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("Test");
	this->pointText.setPosition(WinSizeVec / 100.f);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("GAME OVER");
	this->gameOverText.setPosition(sf::Vector2f(
		XWinSize / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		YWinSize / 2.f - this->gameOverText.getGlobalBounds().height / 2.f
	));

	this->playerHealthBar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerHealthBar.setFillColor(sf::Color::Red);
	this->playerHealthBar.setPosition(sf::Vector2f(10.f, 40.f));
	this->playerHealthBarBack = playerHealthBar;
	this->playerHealthBarBack.setFillColor(sf::Color(25, 25, 25, 200));
}

void Game::InitWorldBackground() {
	if(!this->worldBackgroundTexture.loadFromFile("Textures/background1.jpg")) {
		std::cout << "Can't load background texture" << std::endl;
	}

	this->worldBackground.setTexture(this->worldBackgroundTexture);
}

void Game::UpdateGUI() {
	std::stringstream ss;

	ss << "Points: " << points;

	pointText.setString(ss.str());

	float hpPercent = static_cast<float>(player->getHP()) / player->getHPMax();

	playerHealthBar.setSize(sf::Vector2f(300.f * hpPercent, playerHealthBar.getSize().y));
}

void Game::UpdateBullets() {
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && player->canAttack()) {
		// Add the Bullets to the bullets vector
		bullets.push_back(new Bullet(
			textures["BULLET"],
			player->getPosition().x + 32.f, 
			player->getPosition().y - 14.f,
			0,
			-1,
			5.f
		));
	} 

	if(!bullets.empty()) {
		unsigned counter = 0;

		for(auto* bullet : bullets) {
			// Update bullets
			bullet->Update();

			// Bullet Culling (top of the screen)
			if(bullet->getBounds().top + bullet->getBounds().height < 0.f) {
				// Delete Bullet
				delete bullets[counter];
				bullets.erase(bullets.begin() + counter);
			}

			++counter;
		}
	}
}

void Game::UpdateEnemies() { 
	// Spawning
	spawnTimer += 0.8f;

	if(spawnTimer >= spawnTimerMax) {
		// Add enemy to the enemies vector (Basically spawning them) 
		enemies.push_back(new Enemy(
			static_cast<float>( rand() % window->getSize().x - 45 ),
			0.f
		));

		spawnTimer = 0.f;
	}

	// Update
	if(!enemies.empty()) {
		unsigned counter = 0;

		for(auto* enemy : enemies) {
			// Update bullets
			enemy->Update();

			// Bullet Culling (top of the screen)
			if(enemy->getBounds().top > window->getSize().y) {
				player->loseHp(enemies[counter]->getDamage());

				// Delete Enemy
				delete enemies[counter];
				enemies.erase(enemies.begin() + counter);	
			} else if(enemy->getBounds().intersects(player->getBounds())) {
				if(enemies[counter] != NULL) {
					player->loseHp(static_cast<int>( floor(enemies[counter]->getDamage() / 2) ));
				}

				// Delete Enemy
				delete enemies[counter];
				enemies.erase(enemies.begin() + counter);
			}

			++counter;
		}
	}
}

void Game::UpdateCombat() { 
	// isEnemyDeleted it's used to prevent looping a deleted enemy or bullet
	for(int i = 0; i < enemies.size(); i++) {
		bool isEnemyDeleted = false;

		for(int j = 0; j < bullets.size() && !isEnemyDeleted; j++) {
			if(enemies[i]->getBounds().intersects(bullets[j]->getBounds())) {
				points += enemies[i]->getPoints();

				delete enemies[i];
				enemies.erase(enemies.begin() + i);
				
				delete bullets[j];
				bullets.erase(bullets.begin() + j);

				isEnemyDeleted = true;
			}
		}
	}
}

void Game::UpdateCollision() {
	if(PBounds.left < 0.f) {
		player->setPosition(0.f, PBounds.top);
	}

	if(PBounds.left + player->getBounds().width > window->getSize().x) {
		player->setPosition(XWinSize - PBounds.width, PBounds.top);
	}

	if(player->getBounds().top < 0.f) {
		player->setPosition(PBounds.left, 0.f);
	}

	if(player->getBounds().top + player->getBounds().height > window->getSize().y) {
		player->setPosition(PBounds.left, YWinSize - PBounds.height);
	}
}

void Game::Update() { 
	player->Update();

	UpdateBullets();
	UpdateEnemies();
	UpdateCombat();
	UpdateGUI();
	UpdateCollision();
}


void Game::Render() { 
	window->clear();
		
		window->draw(worldBackground);

		// Render Player
		player->Render(*window);

		window->draw(pointText);

		window->draw(playerHealthBarBack);
		window->draw(playerHealthBar);
		
		if(player->getHP() <= 0) {
			window->draw(gameOverText);
		}

		// Render all Enemy
		for(auto* enemy : enemies) {
			enemy->Render(*window);
		}

		// Render all bullets
		for(auto* enemy : bullets) {
			enemy->Render(*window);
		}

	window->display();
}

void Game::Run() {
	while(window->isOpen()) {
		while(window->pollEvent(ev)) {
			switch(ev.type) {
				case sf::Event::Closed:
					this->window->close();
				break;

				case sf::Event::KeyPressed:
					if(ev.key.code == sf::Keyboard::Escape)
						this->window->close();
				break;
			}
		}

		if(player->getHP() >= 0) {
			this->Update();
		}

		this->Render();
	}
}

Game::Game() {
	this->player = new Player();

	this->spawnTimerMax = 40.f;
	this->spawnTimer = this->spawnTimerMax;

	this->InitWindow();
	this->InitTextures();
	this->InitGUI();
	this->InitWorldBackground();
}

Game::~Game() { 
	delete this->window;
	delete this->player;

	// Delete Textures
	for(auto& texture : this->textures)
		delete texture.second;

	// Delete Bullets
	for(auto* bullet : this->bullets)
		delete bullet;
	
	// Delete Enemies
	for(auto* enemy : this->enemies)
		delete enemy;
}
