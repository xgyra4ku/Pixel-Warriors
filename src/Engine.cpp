#include "../include/Engine.hpp"

Engine::Engine() {
    window.create(sf::VideoMode(WindowWidth, WindowHeight), "3D Engine");//sf::Style::Close
    if (!window.isOpen()) {
        std::cerr << "ERROR: Failed creating window" << std::endl;
        window.close();
        return;
    }
    std::cout << "INFO: Window created successfully" << std::endl;
    map.init();
    map.load();
    if (!texturePlayerList[0].loadFromFile("Assets/tex1.png")) {
        std::cerr << "ERROR: Failed loading Assets/tex1.png" << std::endl;
        window.close();
        return;
    }
    std::cout << "INFO: Texture loaded successfully" << std::endl;

    initPlayer(1);
}

Engine::~Engine() {
}


void Engine::run() {
	while (window.isOpen()) {
        time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 700.0f;

		if (time > 60.0f) time = 60.0f;
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount / fpsClock.getElapsedTime().asSeconds();
            frameCount = 0;
            fpsClock.restart();
            std::cout << "FPS: " << int(fps) << std::endl;
        }
        logic();
		Events();
        window.clear();
        //window.draw(shape);
        updateDisplay();
		window.display();
	}
}
void Engine::updateDisplay() {
   map.draw(window);
   player1.draw(window);
}


void Engine::Events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                window.close();
        }
    }
}


void Engine::initPlayer(int textureNumPlayer1) {
    switch (textureNumPlayer1)
    {
    case 1:
        player1.setTexture(texturePlayerList[0]);
        break;
    default:
        break;
    }
    player1.setPosition(sf::Vector2f(150, 150));
    player1.setSize(sf::Vector2f(16, 16));
}

void Engine::logic() {
   collision();
   controlKeyboard();
}

void Engine::collision() {
    sf::Vector2f newPos = player1.getPosition();
    switch (map.collision(newPos, player1.getSize(), sf::Vector2f(0, 0))) {
    case 1:
        std::cout << 1 << std::endl;
        newPos.x -= ((playerSpeed * 2) * time) - offsetX;
        player1.setPosition(newPos);
        break;
    case 2:
        std::cout << 2 << std::endl;
        newPos.x += ((playerSpeed * 2) * time) - offsetX;
        player1.setPosition(newPos);
        break;
    case 3:
        std::cout << 3 << std::endl;
        newPos.y += ((playerSpeed * 2) * time) - offsetY;
        player1.setPosition(newPos);
        break;
    case 4:
        std::cout << 4 << std::endl;
        newPos.y -= ((playerSpeed * 2) * time) - offsetY;
        player1.setPosition(newPos);
        break;
    default:
        break;
    }

    //std::cout << "Player position: (" << newPos.x << ", " << newPos.y << ")" << std::endl;
}

void Engine::controlKeyboard() {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
       player1.animate(time, 5);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x, playerPos.y - playerSpeed * time));
        player1.animate(time, 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x - playerSpeed * time, playerPos.y));
        player1.animate(time, 2);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x, playerPos.y + playerSpeed * time));
        player1.animate(time, 3);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x + playerSpeed * time, playerPos.y));
        player1.animate(time, 4);
    }
}
