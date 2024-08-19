#include "../include/Engine.hpp"
#include "../include/globals.hpp"

Engine::Engine() {
    settings = save_and_load_.loadSettings();
    sf::ContextSettings settingsDisplay;
    settingsDisplay.antialiasingLevel = 8;
    if (settings["fullscreen"] == 1) {
        window.create(sf::VideoMode::getDesktopMode(), "3D Engine", sf::Style::Fullscreen, settingsDisplay);

    } else {
        window.create(sf::VideoMode(settings["windowWidth"], settings["windowHeight"]), "3D Engine", sf::Style::Close, settingsDisplay);
    }
    WindowWidth = static_cast<int>(window.getSize().x);
    WindowHeight = static_cast<int>(window.getSize().y);


    if (!window.isOpen()) {
        std::cerr << "ERROR: Failed creating window" << std::endl;
        window.close();
        return;
    }

    std::cout << "INFO: Window created successfully" << std::endl;

    if (!texturePlayerList[0].loadFromFile("Assets/tex1.png")) {
        std::cerr << "ERROR: Failed loading Assets/tex1.png" << std::endl;
        window.close();
        return;
    }

    std::cout << "INFO: Texture loaded successfully" << std::endl;

    loadDependency("Dependency/");
    menu = 0;

    //modslist = loadMods("Mods");
    offsetRUN = true;
    collisionRUN = true;
    playerPosRUN = false;
    map.init(4);
   // map.initChunks(12345, 16, sf::Vector2f(100, 100), sf::Vector2f(16, 16));


    //map.load();
    // std::srand(std::time(NULL));

    initPlayer(1);
    const unsigned int seed = std::rand();
    std::cout << "INFO: Seed: " << seed << std::endl;
    // std::time(NULL);
    // map.generateMap(seed, 128);
    //generateMap(111111111, 1900, 1900);
    // map.generateChunk(100, 100, chunkMap);
    // chunckPos.x = playerPos.x;
    // chunckPos.y = playerPos.y;
    //map.generateMap(seed, 32);
    //noise(12345);  // Seed
    //world(noise, 12345, 3);  // Seed and render distance
    //map.initializeMap();

}

Engine::~Engine() = default;

void Engine::loadDependency(const std::string& directory) {
    DependencyFunctions functions{};
    const std::string filePath = directory + "libmod-menu.dll";
    const HMODULE hModule = LoadLibrary(filePath.c_str());

    if (hModule == nullptr) {
        std::cerr << "[ERROR]: Could not load " << filePath << std::endl;
        return;
    }

    functions.initLib = reinterpret_cast<void (*)(sf::RenderWindow &)>(GetProcAddress(hModule, "initLib"));
    functions.menuLib = reinterpret_cast<void (*)(sf::RenderWindow &, int &, std::map<std::string, int> &)>(GetProcAddress(hModule, "menuLib"));

    if (!functions.initLib || !functions.menuLib) {
        std::cerr << "[ERROR]: Could not locate functions in " << filePath << std::endl;
        FreeLibrary(hModule);
    }
    functions.initLib(window);
    dependencyList["menuLib"] = functions;
}
//
// std::vector<Mod> Engine::loadMods(const std::string& directory) {
//     std::vector<Mod> mods;
//     WIN32_FIND_DATA findFileData;
//     HANDLE hFind = FindFirstFile((directory + "\\*.dll").c_str(), &findFileData);
//
//     if (hFind == INVALID_HANDLE_VALUE) {
//         std::cerr << "No mods found in " << directory << std::endl;
//         return mods;
//     }
//
//     do {
//         std::string filePath = directory + "\\" + findFileData.cFileName;
//         HMODULE hModule = LoadLibrary(filePath.c_str());
//         if (hModule == NULL) {
//             std::cerr << "Could not load " << filePath << std::endl;
//             continue;
//         }
//
//         DependencyFunctions functions;
//         functions.initLib = (void (*)(sf::RenderWindow&)) GetProcAddress(hModule, "initLib");
//         functions.menuLib = (void (*)(sf::RenderWindow&, int&)) GetProcAddress(hModule, "menuLib");
//
//         if (!functions.initLib || !functions.menuLib) {
//             std::cerr << "Could not locate functions in " << filePath << std::endl;
//             FreeLibrary(hModule);
//             continue;
//         }
//
//         functions.initLib(window);
//         mods.push_back({ hModule, functions });
//     } while (FindNextFile(hFind, &findFileData) != 0);
//
//     FindClose(hFind);
//     return mods;
// }

void Engine::run() {
    if (settings["V-sync"] == 1) {
        window.setVerticalSyncEnabled(true);
        std::cout << "INFO: V-sync enabled" << std::endl;
    } else {
        window.setVerticalSyncEnabled(false);
        std::cout << "INFO: V-sync disabled" << std::endl;
        if (settings["fps"] == -1) {
            std::cout << "INFO: FPS not set" << std::endl;
        } else {
            std::cout << "INFO: FPS set to " << settings["fps"] << std::endl;
            window.setFramerateLimit(settings["fps"]);
        }
    }

	while (window.isOpen()) {
        window.clear();
        Events();
        timer();
        if (menu == -1) {
            //game
            logic();
            updateDisplay();

        } else if (menu == -2) {
            save_and_load_.saveSettings(settings);
	        menu = 3;
            dependencyList["menuLib"].menuLib(window, menu, settings);
        } else {
	        dependencyList["menuLib"].menuLib(window, menu, settings);

	    }
        window.display();
	}
}
void Engine::timer() {

    time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
    clock.restart();

    time = time / 700.0f;

    if (time > 60.0f) time = 60.0f;
    frameCount++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        fps = static_cast<float>(frameCount) / fpsClock.getElapsedTime().asSeconds();
        frameCount = 0;
        fpsClock.restart();
        std::cout << "FPS: " << static_cast<int>(fps) << std::endl;
    }
}
void Engine::logic() {
    if (collisionRUN)
        collision();
    controlKeyboard();
    if (offsetRUN)
        offset();
    if (playerPosRUN)
        std::cout << "Player position: (" << playerPos.x - offsetX << ", " << playerPos.y - offsetY << ")" << std::endl;

}
void Engine::updateDisplay() {
    //map.draw(window, mapGenerated, player1.getPosition(), sf::Vector2f((WindowWidth / 2.0f + 30), (WindowHeight / 2.0f + 30)));

   map.draw(window, playerPos, sf::Vector2f((static_cast<float>(WindowWidth) / 2.0f + 30), (static_cast<float>(WindowHeight) / 2.0f + 30)), 16);



    player1.draw(window);
}


void Engine::Events() {

    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape && menu == 0)
                window.close();
            else if (event.key.code == sf::Keyboard::Escape && menu == -1)
                menu = 0;
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
    player1.setPosition(sf::Vector2f(800, 800));
    player1.setSize(sf::Vector2f(16, 16));
    offsetX = 40 ;
    offsetY = 80 ;
}


void Engine::collision() {
    switch (sf::Vector2f newPos = player1.getPosition(); map.collision(newPos, player1.getSize(), sf::Vector2f(0, 0))) {
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
} 
void Engine::offset() {
    // Define the dead zone boundaries
    constexpr float leftDeadZone = 630.0f;
    constexpr float rightDeadZone = 650.0f;
    constexpr float topDeadZone = 390.0f;
    constexpr float bottomDeadZone = 410.0f;

    // Player's current position
    const sf::Vector2f playerPos = player1.getPosition() - sf::Vector2f(offsetX, offsetY);

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
    constexpr float smoothFactor = 0.1f; // Adjust this factor for more or less smoothness

    // Add some inertia to the camera movement
    constexpr float inertia = 0.1f; // Adjust this for more or less camera inertia

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
    if (!offsetRUN) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            offsetY -= 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            offsetX -= 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            offsetY += 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            offsetX += 5;
        }
    }
    player1.setPosition(playerPos);
    
}

void Engine::generateMap(const unsigned int seed, const int WIDTH, const int HEIGHT) {
    mapGenerated = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH, 0)); // Инициализация карты

    std::cout << "Map initialized" << std::endl;

    map.initializeMap(mapGenerated, seed, 0.58, WIDTH, HEIGHT); // Инициализация карты случайными значениями

    constexpr int generations = 20; // Количество поколений клеточного автомата

    std::cout << "Map generated..." << std::endl;

    // Запуск клеточного автомата на заданное количество поколений
    for (int gen = 0; gen < generations; ++gen) {
        map.stepAutomaton(mapGenerated, WIDTH, HEIGHT); // Выполнение одного шага клеточного автомата
        std::cout << "INFO: Generation " << gen << " completed" << std::endl;
    }
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (mapGenerated[y][x] == 0) mapGenerated[y][x] = 159;
            if (mapGenerated[y][x] == 1) mapGenerated[y][x] = 47;
        }
    }
    std::cout << "Map generated successfully" << std::endl;
}
