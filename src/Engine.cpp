#include "../include/Engine.hpp"
#include "../include/globals.hpp"

//
// Конструктор класса
//
Engine::Engine() : m_iMenu(0), m_bOffsetRUN(true), m_bCollisionRUN(true), m_bPlayerPosRUN(false),m_bInfo(false), m_bCheats(false) {
    m_mpSettings = save_and_load_.loadSettings();
    sf::ContextSettings settingsDisplay;
    // установление сглаживания
    settingsDisplay.antialiasingLevel = 8;
    if (m_mpSettings["fullscreen"] == 1) {
        m_oWindow.create(sf::VideoMode::getDesktopMode(), "Pixel Warriors", sf::Style::Fullscreen, settingsDisplay);
    } else {
        m_oWindow.create(sf::VideoMode(m_mpSettings["windowWidth"], m_mpSettings["windowHeight"]), "Pixel Warriors", sf::Style::Close, settingsDisplay);
    }
    // получени размера окна
    g_iWindowWidth = static_cast<int>(m_oWindow.getSize().x);
    g_iWindowHeight = static_cast<int>(m_oWindow.getSize().y);

    sf::Texture textureInventory;

    // проверка открытия окна
    if (!m_oWindow.isOpen()) {
        oCmdInfo.error("Failed creating window");
        m_oWindow.close();
        return;
    }
    oCmdInfo.info("Window created successfully");

    // загрузка текстур и проверка
    if (!m_oTexturePlayerList[0].loadFromFile("Assets/tex1.png")) {
        oCmdInfo.error("Failed loading Assets/tex1.png");
        m_oWindow.close();
        return;
    }
    if (!textureInventory.loadFromFile("UI/2/Inventory_style_02b.png")) {
        oCmdInfo.error("Failed loading UI/2/Inventory_style_02b.png");
        m_oWindow.close();
        return;
    }
    oCmdInfo.info("Texture load is successfully");

    // загрузка шрифта и проверка
    if (!m_ftFont.loadFromFile("Dependency/font.otf")) {
        oCmdInfo.error("Failed loading Dependency/font.otf");
        m_oWindow.close();
        return;
    }
    oCmdInfo.info("Font load is successfully");

    // определения консоли и инвентаря
    oConsole = new Console(m_ftFont, m_oWindow);
    oInventory = new cInventory(textureInventory,m_oWindow);

    // загрузка дополнеий
    _loadDependency("Dependency/");


    // проверка и загрузка модов
    //modslist = loadMods("Mods");

    m_sftTextInfo.setFillColor(sf::Color::Black);
    m_sftTextInfo.setFont(m_ftFont);
    m_sftTextInfo.setCharacterSize(15);
    m_sftTextInfo.setPosition(50,50);
    //m_oWindow.close();
}

Engine::~Engine() = default;

//
// Функция загрузки дополнений
//
void Engine::_loadDependency(const std::string& directory) {
    // определение структуры функций
    DependencyFunctions functions{};
    const std::string filePath = directory + "libmod-menu.dll";
    const HMODULE hModule = LoadLibrary(filePath.c_str());

    // проверна на наличие дополнений
    if (hModule == nullptr) {
        oCmdInfo.error("Could not load " + filePath);
        return;
    }

    // определение функций дополнеий
    functions.initLib = reinterpret_cast<void (*)(sf::RenderWindow &)>(GetProcAddress(hModule, "initLib"));
    functions.menuLib = reinterpret_cast<void (*)(sf::RenderWindow &, int &, std::map<std::string, int> &, int &, float&, std::map<std::string, std::string>&)>(GetProcAddress(hModule, "menuLib"));

    //проверна на наличие функций
    if (!functions.initLib || !functions.menuLib) {
        oCmdInfo.error("Could not locate functions in  " + filePath);
        FreeLibrary(hModule);
    }
    // запуск иницидизации
    functions.initLib(m_oWindow);
    // помещегия функций дополнеий в мап
    m_mpDependencyList["menuLib"] = functions;
}
//
// Функция ждя определения модов
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

//
// Основная функция
//
void Engine::vRun() {
    //определение настроек и применение их
    if (m_mpSettings["fps"] == 0) {
        m_oWindow.setVerticalSyncEnabled(true);
        oCmdInfo.info("V-sync enabled");
    } else {
        m_oWindow.setVerticalSyncEnabled(false);
        oCmdInfo.info("V-sync disabled");
        if (m_mpSettings["fps"] == 250) {
            oCmdInfo.info("FPS set max");
        } else {
            m_oWindow.setFramerateLimit(m_mpSettings["fps"]);
            oCmdInfo.info("FPS set to " + m_mpSettings["fps"]);
        }
    }
    // если окно открыто
    while (m_oWindow.isOpen()) {
        m_oWindow.clear();
        _events();
        _timer();
	    if (!m_oWindow.isOpen()) {
	        oCmdInfo.info("Window closed during event processing");
	        break; // Выйти из цикла, если окно закрыто
	    }
        if (m_iMenu == -1) { // если запушен мир
            _logic();
            _updateDisplay();
            oConsole->logic(m_fTime);
        } else if (m_iMenu == -2) { // применение настроек
            save_and_load_.saveSettings(m_mpSettings);
	        m_iMenu = 3;
            m_mpDependencyList["menuLib"].menuLib(m_oWindow, m_iMenu, m_mpSettings, m_iWheelEventMouse, m_fTime, m_mpFileWorld);
        } else if (m_iMenu == -3) { // загрузка мира
            oCmdInfo.info("Load world >" + m_mpFileWorld["name"] + "<");
            map.init(iDISTANCE_VIEW, m_mpFileWorld["name"], m_oWindow);
            _initPlayer(iTextureNumPlayer1);
            player1.setPosition(map.getPosPlayer());
            g_fOffsetX = (player1.getPosition().x - static_cast<float>(m_oWindow.getSize().x) / 2);
            g_fOffsetY = (player1.getPosition().y - static_cast<float>(m_oWindow.getSize().y) / 2);
            m_iMenu = -1;
        } else if (m_iMenu == -4) { // создание мира
            oCmdInfo.info("Create world >" + m_mpFileWorld["name"] + "<");
            map.init(iDISTANCE_VIEW, m_mpFileWorld["name"], m_oWindow, std::stoul(m_mpFileWorld["seed"]));
            _initPlayer(iTextureNumPlayer1);
            player1.setPosition(sf::Vector2f(0,0));
            g_fOffsetX = (player1.getPosition().x - static_cast<float>(m_oWindow.getSize().x) / 2);
            g_fOffsetY = (player1.getPosition().y - static_cast<float>(m_oWindow.getSize().y) / 2);
            m_iMenu = -1;
        } else {// меню
	        m_mpDependencyList["menuLib"].menuLib(m_oWindow, m_iMenu, m_mpSettings, m_iWheelEventMouse, m_fTime, m_mpFileWorld);
        }
	    //обновление дисплея
        m_oWindow.display();
	}
}

//
// таймер и изменения фпс
//
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
    }
    m_sftTextInfo.setString(
        "Fps: " + std::to_string(static_cast<int>(m_fFps))+ "\n"+"posPL:"+
        " X: " +std::to_string(static_cast<int>(m_oPlayerPos.x/16)) +
        " Y: " + std::to_string(static_cast<int>(m_oPlayerPos.y/16)));
}

//
// Логика обработки флагов
//
void Engine::_logic() {
    if (m_bCollisionRUN)
        _collision();
    if (!oConsole->getReflections())
        _controlKeyboard();
    if (m_bOffsetRUN)
        _offset();
}

//
// Обновление дисплея
//
void Engine::_updateDisplay() {
    map.draw(m_oWindow,
        m_oPlayerPos,
        sf::Vector2f((static_cast<float>(g_iWindowWidth) / 2.0f + 30),
            (static_cast<float>(g_iWindowHeight) / 2.0f + 30)), iCHUNK_SIZE);
    player1.draw(m_oWindow);
    oInventory->draw(m_oWindow);
    if (m_bInfo)
        m_oWindow.draw(m_sftTextInfo);
    oConsole->draw(m_oWindow);
}

//
// Евенты
//
void Engine::_events() {
    m_iWheelEventMouse = 0;
    sf::Event event{};
    while (m_oWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { // при закрытии окна
            m_oWindow.close();
        }
        if (event.type == sf::Event::KeyPressed) { // при нажатии на клавиши
            if (event.key.code == sf::Keyboard::Escape && m_iMenu == 0) {
                m_oWindow.close();
            } else if (event.key.code == sf::Keyboard::Escape && m_iMenu == -1 && oConsole->getReflections()) {
                oConsole->setReflections(false);
            } else if (event.key.code == sf::Keyboard::Escape && m_iMenu == -1) {
                m_iMenu = 0;
                map.deleting();
            } else if (event.key.code == sf::Keyboard::Escape && m_iMenu == 2) {
                m_iMenu = 0;
            } else if (event.key.code == sf::Keyboard::Tilde) {
                oConsole->setReflections(!oConsole->getReflections());
            } else if (event.key.code == sf::Keyboard::Enter) {
                oConsole->read(m_mpCommandData);
                _commandExecution();
            }
            else if (event.key.code == sf::Keyboard::E) {
                oInventory->setInventoryStatus(!oInventory->getInventoryStatus());
            }
        }
        if (event.type == sf::Event::MouseWheelMoved) { // при прокручивание колесика мыши
            m_iWheelEventMouse = event.mouseWheel.delta;
        }
    }

}

//
// Иницилизация игрока
//
void Engine::_initPlayer(int textureNumPlayer1) {
    switch (textureNumPlayer1)
    {
    case 1:
        player1.setTexture(m_oTexturePlayerList[0]);
        break;
    default:
        break;
    }
    player1.setPosition(sf::Vector2f(0, 0));
    player1.setSize(sf::Vector2f(16, 16));
    g_fOffsetX = 40 ;
    g_fOffsetY = 80 ;
}

//
// Коллизии
//
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

//
// Оффсет/Камера
//
void Engine::_offset() const {

    // определение квадрата свободы
    const float leftDeadZone = (((630.0f / 1280.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().x);
    const float rightDeadZone = (((650.0f / 1280.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().x);
    const float topDeadZone = (((390.0f / 800.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().y);
    const float bottomDeadZone = (((410.0f / 800.0f) * 100.0f) / 100.0f) * static_cast<float>(m_oWindow.getSize().y);

    // Полусения позии игрока
    const sf::Vector2f playerPos = player1.getPosition() - sf::Vector2f(g_fOffsetX, g_fOffsetY);
    float targetOffsetX = g_fOffsetX;
    float targetOffsetY = g_fOffsetY;

    // проверна зоны свободы по x/y
    if (playerPos.x < leftDeadZone) {
        targetOffsetX -= m_fOffSetSpeed * m_fTime;
    }
    if (playerPos.x > rightDeadZone) {
        targetOffsetX += m_fOffSetSpeed * m_fTime;
    }
    if (playerPos.y < topDeadZone) {
        targetOffsetY -= m_fOffSetSpeed * m_fTime;
    }
    if (playerPos.y > bottomDeadZone) {
        targetOffsetY += m_fOffSetSpeed * m_fTime;
    }

    constexpr float smoothFactor = 0.1f;
    constexpr float inertia = 0.1f;

    // коректировка камеры
    g_fOffsetX += (targetOffsetX - g_fOffsetX) * smoothFactor;
    g_fOffsetY += (targetOffsetY - g_fOffsetY) * smoothFactor;

    if (abs(player1.getSize().x) < 0.1f) {
        g_fOffsetX += (playerPos.x - (leftDeadZone + rightDeadZone) / 2.0f) * inertia;
    }

    if (abs(player1.getSize().y) < 0.1f) {
        g_fOffsetY += (playerPos.y - (topDeadZone + bottomDeadZone) / 2.0f) * inertia;
    }
}

//
// Управление
//
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

//
// Выполнение команд
//
void Engine::_commandExecution() {
    for (auto& [strCommand, iParameter] : m_mpCommandData) {//чтение и определение параметров
        // определение команды и выполнение ее

        // не читерские команды
        if (strCommand == "info") {
            if (iParameter == 1) {
                m_bInfo = true;
            } else if (iParameter == 0) {
                m_bInfo  = false;
            }
        } else if (strCommand == "cheats") {
            if (iParameter == 1) {
                m_bCheats = true;
            } else if (iParameter == 0) {
                m_bCheats = false;
            }
        } else if (strCommand == "fpsMax") {
            m_mpSettings["fps"] = iParameter;
            m_oWindow.setFramerateLimit(m_mpSettings["fps"]);
        } else if (strCommand == "view") {
            iDISTANCE_VIEW = iParameter;
        }

        // читерские команды
        if (m_bCheats) {
            if(strCommand == "sppl") {
                m_fPlayerSpeed = static_cast<float>(iParameter) * 0.01f;
            } else if(strCommand == "spoffset") {
                m_fOffSetSpeed = static_cast<float>(iParameter) * 0.1f;
            } else if (strCommand == "offset") {
                if (iParameter == 1) {
                    m_bOffsetRUN = true;
                } else if (iParameter == 0) {
                    m_bOffsetRUN = false;
                }
            } else if (strCommand == "collision") {
                if (iParameter == 1) {
                    m_bCollisionRUN = true;
                } else if (iParameter == 0) {
                    m_bCollisionRUN = false;
                }
            } else if (strCommand == "generate") {
                if (iParameter == 1) {
                    ComGenerateChunkRUN = true;
                } else if (iParameter == 0) {
                    ComGenerateChunkRUN = false;
                }
            }
        }
    }
}
