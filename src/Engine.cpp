#include "../include/Engine.hpp"
#include "../include/globals.hpp"

Engine::Engine() {
    m_mpSettings = save_and_load_.loadSettings();
    sf::ContextSettings settingsDisplay;
    settingsDisplay.antialiasingLevel = 0;
    if (m_mpSettings["fullscreen"] == 1) {
        m_oWindow.create(sf::VideoMode::getDesktopMode(), "3D Engine", sf::Style::Fullscreen, settingsDisplay);
    } else {
        m_oWindow.create(sf::VideoMode(m_mpSettings["windowWidth"], m_mpSettings["windowHeight"]), "3D Engine", sf::Style::Close, settingsDisplay);
    }
    g_iWindowWidth = static_cast<int>(m_oWindow.getSize().x);
    g_iWindowHeight = static_cast<int>(m_oWindow.getSize().y);


    if (!m_oWindow.isOpen()) {
        std::cerr << "ERROR: Failed creating window" << std::endl;
        m_oWindow.close();
        return;
    }

    std::cout << "INFO: Window created successfully" << std::endl;

    if (!m_oTexturePlayerList[0].loadFromFile("Assets/tex1.png")) {
        std::cerr << "ERROR: Failed loading Assets/tex1.png" << std::endl;
        m_oWindow.close();
        return;
    }

    std::cout << "INFO: Texture loaded successfully" << std::endl;

    _loadDependency("Dependency/");
    m_iMenu = 0;

    //modslist = loadMods("Mods");
    m_bOffsetRUN = true;
    m_bCollisionRUN = true;
    m_bPlayerPosRUN = false;

    //map.init(TODO, TODO);
    //map.initChunks(12345, 16, sf::Vector2f(100, 100), sf::Vector2f(16, 16));


    //map.load();
    // std::srand(std::time(NULL));

    //m_initPlayer(1);
    //const unsigned int seed = std::rand();
    //std::cout << "INFO: Seed: " << seed << std::endl;
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

void Engine::_loadDependency(const std::string& directory) {
    DependencyFunctions functions{};
    const std::string filePath = directory + "libmod-menu.dll";
    const HMODULE hModule = LoadLibrary(filePath.c_str());

    if (hModule == nullptr) {
        std::cerr << "[ERROR]: Could not load " << filePath << std::endl;
        return;
    }

    functions.initLib = reinterpret_cast<void (*)(sf::RenderWindow &)>(GetProcAddress(hModule, "initLib"));
    functions.menuLib = reinterpret_cast<void (*)(sf::RenderWindow &, int &, std::map<std::string, int> &, int &, float&, std::map<std::string, std::string>&)>(GetProcAddress(hModule, "menuLib"));

    if (!functions.initLib || !functions.menuLib) {
        std::cerr << "[ERROR]: Could not locate functions in " << filePath << std::endl;
        FreeLibrary(hModule);
    }
    functions.initLib(m_oWindow);
    m_mpDependencyList["menuLib"] = functions;
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

void Engine::vRun() {
    if (m_mpSettings["V-sync"] == 1) {
        m_oWindow.setVerticalSyncEnabled(true);
        std::cout << "INFO: V-sync enabled" << std::endl;
    } else {
        m_oWindow.setVerticalSyncEnabled(false);
        std::cout << "INFO: V-sync disabled" << std::endl;
        if (m_mpSettings["fps"] == -1) {
            std::cout << "INFO: FPS not set" << std::endl;
        } else {
            std::cout << "INFO: FPS set to " << m_mpSettings["fps"] << std::endl;
            m_oWindow.setFramerateLimit(m_mpSettings["fps"]);
        }
    }

	while (m_oWindow.isOpen()) {
        m_oWindow.clear();
        _events();
        _timer();
        if (m_iMenu == -1) {
            _logic();
            _updateDisplay();
        } else if (m_iMenu == -2) {
            save_and_load_.saveSettings(m_mpSettings);
	        m_iMenu = 3;
            m_mpDependencyList["menuLib"].menuLib(m_oWindow, m_iMenu, m_mpSettings, m_iWheelEventMouse, m_fTime, m_mpFileWorld);
        } else if (m_iMenu == -3) {
            std::cout << "INFO: Load world >" << m_mpFileWorld["name"] << "<" << std::endl;
            map.init(4, m_mpFileWorld["name"], m_oWindow);
            _initPlayer(1);
            player1.setPosition(map.getPosPlayer());
            g_fOffsetX = (player1.getPosition().x - static_cast<float>(m_oWindow.getSize().x) / 2);
            g_fOffsetY = (player1.getPosition().y - static_cast<float>(m_oWindow.getSize().y) / 2);
            m_iMenu = -1;
        } else if (m_iMenu == -4) {
            std::cout << "INFO: Create world >" << m_mpFileWorld["name"] << "<" << std::endl;
            map.init(4, m_mpFileWorld["name"], m_oWindow, std::stoul(m_mpFileWorld["seed"]));
            _initPlayer(1);
            player1.setPosition(sf::Vector2f(0,0));
            g_fOffsetX = (player1.getPosition().x - static_cast<float>(m_oWindow.getSize().x) / 2);
            g_fOffsetY = (player1.getPosition().y - static_cast<float>(m_oWindow.getSize().y) / 2);
            m_iMenu = -1;
        } else {
	        m_mpDependencyList["menuLib"].menuLib(m_oWindow, m_iMenu, m_mpSettings, m_iWheelEventMouse, m_fTime, m_mpFileWorld);
        }
        m_oWindow.display();
	}
}
void Engine::_timer() {

    m_fTime = static_cast<float>(m_oClock.getElapsedTime().asMicroseconds());
    m_oClock.restart();
    m_fTime = m_fTime / 700.0f;

    if (m_fTime > 60.0f) m_fTime = 60.0f;
    m_fFrameCount++;
    if (m_oFpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        m_fFps = static_cast<float>(m_fFrameCount) / m_oFpsClock.getElapsedTime().asSeconds();
        m_fFrameCount = 0;
        m_oFpsClock.restart();
        std::cout << "FPS: " << static_cast<int>(m_fFps) << std::endl;
    }
}
void Engine::_logic() {
    if (m_bCollisionRUN)
        _collision();
    _controlKeyboard();
    if (m_bOffsetRUN)
        _offset();
    if (m_bPlayerPosRUN)
        std::cout << "Player position: (" << m_oPlayerPos.x - g_fOffsetX << ", " << m_oPlayerPos.y - g_fOffsetY << ")" << std::endl;

}
void Engine::_updateDisplay() {
    //map.draw(window, mapGenerated, player1.getPosition(), sf::Vector2f((WindowWidth / 2.0f + 30), (WindowHeight / 2.0f + 30)));

   map.draw(m_oWindow, m_oPlayerPos, sf::Vector2f((static_cast<float>(g_iWindowWidth) / 2.0f + 30), (static_cast<float>(g_iWindowHeight) / 2.0f + 30)), 16);



    player1.draw(m_oWindow);
}


void Engine::_events() {
    m_iWheelEventMouse = 0;
    sf::Event event{};
    while (m_oWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_oWindow.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape && m_iMenu == 0) {
                m_oWindow.close();
            } else if (event.key.code == sf::Keyboard::Escape && m_iMenu == -1) {
                m_iMenu = 0;
                map.deleting();
            } else if (event.key.code == sf::Keyboard::Escape && m_iMenu == 2) {
                m_iMenu = 0;
                map.deleting();
            }
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            m_iWheelEventMouse = event.mouseWheel.delta;
        }
    }

}


void Engine::_initPlayer(int textureNumPlayer1) {
    switch (textureNumPlayer1)
    {
    case 1:
        player1.setTexture(m_oTexturePlayerList[0]);
        break;
    default:
        break;
    }
    player1.setPosition(sf::Vector2f(800, 800));
    player1.setSize(sf::Vector2f(16, 16));
    g_fOffsetX = 40 ;
    g_fOffsetY = 80 ;
}


void Engine::_collision() {
    switch (sf::Vector2f newPos = player1.getPosition(); map.collision(newPos, player1.getSize(), sf::Vector2f(0, 0))) {
    case 1:
        std::cout << 1 << std::endl;
        newPos.x -= ((m_fPlayerSpeed * 2) * m_fTime);
        player1.setPosition(newPos);
        break;
    case 2:
        std::cout << 2 << std::endl;
        newPos.x += ((m_fPlayerSpeed * 2) * m_fTime);
        player1.setPosition(newPos);
        break;
    case 3:
        std::cout << 3 << std::endl;
        newPos.y += ((m_fPlayerSpeed * 2) * m_fTime);
        player1.setPosition(newPos);
        break;
    case 4:
        std::cout << 4 << std::endl;
        newPos.y -= ((m_fPlayerSpeed * 2) * m_fTime);
        player1.setPosition(newPos);
        break;
    default:
        break;
    }
} 
void Engine::_offset() const {

    // Define the dead zone boundaries
    const float leftDeadZone = (((630.0f / 1280.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().x);
    const float rightDeadZone = (((650.0f / 1280.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().x);
    const float topDeadZone = (((390.0f / 800.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().y);
    const float bottomDeadZone = (((410.0f / 800.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().y);

    // Player's current position
    const sf::Vector2f playerPos = player1.getPosition() - sf::Vector2f(g_fOffsetX, g_fOffsetY);

    // Calculate the desired offset based on the player's position relative to the dead zone
    float targetOffsetX = g_fOffsetX;
    float targetOffsetY = g_fOffsetY;

    if (playerPos.x < leftDeadZone) {
        targetOffsetX -= m_fOffSetSpeed * m_fTime;
    } else if (playerPos.x > rightDeadZone) {
        targetOffsetX += m_fOffSetSpeed * m_fTime;
    }

    if (playerPos.y < topDeadZone) {
        targetOffsetY -= m_fOffSetSpeed * m_fTime;
    } else if (playerPos.y > bottomDeadZone) {
        targetOffsetY += m_fOffSetSpeed * m_fTime;
    }

    // Smoothly interpolate the offset values for a more gradual camera movement
    constexpr float smoothFactor = 0.1f; // Adjust this factor for more or less smoothness

    // Add some inertia to the camera movement
    constexpr float inertia = 0.1f; // Adjust this for more or less camera inertia

    // Compute the new offset values
    g_fOffsetX += (targetOffsetX - g_fOffsetX) * smoothFactor;
    g_fOffsetY += (targetOffsetY - g_fOffsetY) * smoothFactor;

    // Apply inertia when the player stops moving
    if (abs(player1.getSize().x) < 0.1f) {
        g_fOffsetX += (playerPos.x - (leftDeadZone + rightDeadZone) / 2.0f) * inertia;
    }

    if (abs(player1.getSize().y) < 0.1f) {
        g_fOffsetY += (playerPos.y - (topDeadZone + bottomDeadZone) / 2.0f) * inertia;
    }
}


void Engine::_controlKeyboard() {
    m_oPlayerPos = player1.getPosition();
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
       player1.animate(m_fTime, 5);
    }
    else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            m_oPlayerPos.y -= (m_fPlayerSpeed / 1.5f) * m_fTime;
            m_oPlayerPos.x -= (m_fPlayerSpeed / 1.5f)  * m_fTime;
            player1.animate(m_fTime, 2);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            m_oPlayerPos.y -= (m_fPlayerSpeed /  1.5f) * m_fTime;
            m_oPlayerPos.x += (m_fPlayerSpeed /  1.5f)  * m_fTime;
            player1.animate(m_fTime, 4);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            m_oPlayerPos.y += (m_fPlayerSpeed /  1.5f) * m_fTime;
            m_oPlayerPos.x -= (m_fPlayerSpeed /  1.5f)  * m_fTime;
            player1.animate(m_fTime, 2);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            m_oPlayerPos.y += (m_fPlayerSpeed /  1.5f) * m_fTime;
            m_oPlayerPos.x += (m_fPlayerSpeed /  1.5f)  * m_fTime;
            player1.animate(m_fTime, 4);
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            m_oPlayerPos.y -= m_fPlayerSpeed * m_fTime;
            player1.animate(m_fTime, 1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                m_oPlayerPos.x -= m_fPlayerSpeed * m_fTime;
                player1.animate(m_fTime, 2);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                m_oPlayerPos.y += m_fPlayerSpeed * m_fTime;
                player1.animate(m_fTime, 3);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                m_oPlayerPos.x += m_fPlayerSpeed * m_fTime;
                player1.animate(m_fTime, 4);
            }
        }
        player1.setPosition(m_oPlayerPos);
    }
    if (!m_bOffsetRUN) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            g_fOffsetY -= 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            g_fOffsetX -= 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            g_fOffsetY += 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            g_fOffsetX += 5;
        }
    }
    player1.setPosition(m_oPlayerPos);
    
}

void Engine::_generateMap(const unsigned int seed, const int WIDTH, const int HEIGHT) {
    m_vMapGenerated = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH, 0)); // Инициализация карты

    std::cout << "Map initialized" << std::endl;

    map.initializeMap(m_vMapGenerated, seed, 0.58, WIDTH, HEIGHT); // Инициализация карты случайными значениями

    constexpr int generations = 20; // Количество поколений клеточного автомата

    std::cout << "Map generated..." << std::endl;

    // Запуск клеточного автомата на заданное количество поколений
    for (int gen = 0; gen < generations; ++gen) {
        map.stepAutomaton(m_vMapGenerated, WIDTH, HEIGHT); // Выполнение одного шага клеточного автомата
        std::cout << "INFO: Generation " << gen << " completed" << std::endl;
    }
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (m_vMapGenerated[y][x] == 0) m_vMapGenerated[y][x] = 159;
            if (m_vMapGenerated[y][x] == 1) m_vMapGenerated[y][x] = 47;
        }
    }
    std::cout << "Map generated successfully" << std::endl;
}
