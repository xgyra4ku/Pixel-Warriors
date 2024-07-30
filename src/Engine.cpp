#include "../include/Engine.hpp"

Engine::Engine() {
    window.create(sf::VideoMode(WindowWidth, WindowHeight), "3D Engine", sf::Style::Close);
    if (!window.isOpen()) {
        std::cerr << "ERROR: Failed creating window" << std::endl;
        window.close();
        return;
    }
    std::cout << "INFO: Window created successfully" << std::endl;
    map.init();
    map.load();
    if (!texturePlayerList[0].loadFromFile("Assets/texture1.png")) {
        std::cerr << "ERROR: Failed loading Assets/texture1.png" << std::endl;
        window.close();
        return;
    }
    
    if (!texturePlayerList[0].loadFromFile("Assets/texture2.png")) {
        std::cerr << "ERROR: Failed loading Assets/texture2.png" << std::endl;
        window.close();
        return;
    }
    if (!texturePlayerList[0].loadFromFile("Assets/texture3.png")) {
        std::cerr << "ERROR: Failed loading Assets/texture3.png" << std::endl;
        window.close();
        return;
    }
    std::cout << "INFO: Texture loaded successfully" << std::endl;
    initPlayer(1);
    player1.setPosition(sf::Vector2f(100, 100));
}

Engine::~Engine() {
}


void Engine::run() {
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	while (window.isOpen()) {
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
    case 2:
        player1.setTexture(texturePlayerList[1]);
        break;
    case 3:
        player1.setTexture(texturePlayerList[2]);
        break;
    default:
        break;
    }
}

void Engine::logic() {
   controlKeyboard();
}

void Engine::controlKeyboard() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x, playerPos.y - plaerSpeed));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x - plaerSpeed, playerPos.y));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x, playerPos.y + plaerSpeed));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        playerPos = player1.getPosition();
        player1.setPosition(sf::Vector2f(playerPos.x + plaerSpeed, playerPos.y));
    }
}
