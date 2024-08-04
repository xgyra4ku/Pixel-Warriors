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
    //window.setFramerateLimit(60);
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
   offset();
}

void Engine::collision() {
    sf::Vector2f newPos = player1.getPosition();
    switch (map.collision(newPos, player1.getSize(), sf::Vector2f(0, 0))) {
    case 1:
        std::cout << 1 << std::endl;
        newPos.x -= ((playerSpeed * 2) * time);
        player1.setPosition(newPos);
        break;
    case 2:
        std::cout << 2 << std::endl;
        newPos.x += ((playerSpeed * 2) * time);
        player1.setPosition(newPos);
        break;
    case 3:
        std::cout << 3 << std::endl;
        newPos.y += ((playerSpeed * 2) * time);
        player1.setPosition(newPos);
        break;
    case 4:
        std::cout << 4 << std::endl;
        newPos.y -= ((playerSpeed * 2) * time);
        player1.setPosition(newPos);
        break;
    default:
        break;
    }



  //std::cout << "Player position: (" << newPos.x - offsetX << ", " << newPos.y - offsetY << ")" << std::endl;
} 
void Engine::offset() {
    // Define the dead zone boundaries
    const float leftDeadZone = 380.0f;
    const float rightDeadZone = 420.0f;
    const float topDeadZone = 280.0f;
    const float bottomDeadZone = 320.0f;

    // Player's current position
    sf::Vector2f playerPos = player1.getPosition() - sf::Vector2f(offsetX, offsetY);

    // Calculate the desired offset based on the player's position relative to the dead zone
    float targetOffsetX = offsetX;
    float targetOffsetY = offsetY;

    if (playerPos.x < leftDeadZone) {
        targetOffsetX -= 1 * time;
    } else if (playerPos.x > rightDeadZone) {
        targetOffsetX += 1 * time;
    }

    if (playerPos.y < topDeadZone) {
        targetOffsetY -= 1 * time;
    } else if (playerPos.y > bottomDeadZone) {
        targetOffsetY += 1 * time;
    }

    // Smoothly interpolate the offset values for a more gradual camera movement
    float smoothFactor = 0.1f; // Adjust this factor for more or less smoothness

    // Add some inertia to the camera movement
    float inertia = 0.1f; // Adjust this for more or less camera inertia

    // Compute the new offset values
    offsetX += (targetOffsetX - offsetX) * smoothFactor;
    offsetY += (targetOffsetY - offsetY) * smoothFactor;

    // Apply inertia when the player stops moving
    if (abs(player1.getSize().x) < 0.1f) {
        offsetX += (playerPos.x - (leftDeadZone + rightDeadZone) / 2.0f) * inertia;
    }

    if (abs(player1.getSize().y) < 0.1f) {
        offsetY += (playerPos.y - (topDeadZone + bottomDeadZone) / 2.0f) * inertia;
    }
}


void Engine::controlKeyboard() {
    playerPos = player1.getPosition();
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
       player1.animate(time, 5);
    }
    else {
        playerPos = player1.getPosition();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            playerPos.y -= (playerSpeed / 1.5f) * time;
            playerPos.x -= (playerSpeed / 1.5f)  * time;
            player1.animate(time, 2);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            playerPos.y -= (playerSpeed /  1.5f) * time;
            playerPos.x += (playerSpeed /  1.5f)  * time;
            player1.animate(time, 4);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            playerPos.y += (playerSpeed /  1.5f) * time;
            playerPos.x -= (playerSpeed /  1.5f)  * time;
            player1.animate(time, 2);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            playerPos.y += (playerSpeed /  1.5f) * time;
            playerPos.x += (playerSpeed /  1.5f)  * time;
            player1.animate(time, 4);
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            playerPos.y -= playerSpeed * time;
            player1.animate(time, 1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                playerPos.x -= playerSpeed * time;
                player1.animate(time, 2);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                playerPos.y += playerSpeed * time;
                player1.animate(time, 3);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                playerPos.x += playerSpeed * time;
                player1.animate(time, 4);
            }
        }
        player1.setPosition(playerPos);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
           offsetY -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            offsetX -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            offsetY += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            offsetX += 1;
    }
    player1.setPosition(playerPos);
    
}
