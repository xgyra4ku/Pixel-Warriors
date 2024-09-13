#include "LibKyraText.h"
namespace ktx {
    cInputText::cInputText() {
        setColorRect(sf::Color(255, 255, 255));
        setColorText(sf::Color(0, 0, 0));
        setString("");
        m_fTime = 0;
        m_bInput = false;
    }

    cInputText::~cInputText() = default;

    void cInputText::readKey(const float l_fTime) {
        if (m_bInput) {
            std::string inputText = m_txText.getString();

            std::map<sf::Keyboard::Key, char> keyMap = {
                {sf::Keyboard::A, 'a'}, {sf::Keyboard::B, 'b'}, {sf::Keyboard::C, 'c'},
                {sf::Keyboard::D, 'd'}, {sf::Keyboard::E, 'e'}, {sf::Keyboard::F, 'f'},
                {sf::Keyboard::G, 'g'}, {sf::Keyboard::H, 'h'}, {sf::Keyboard::I, 'i'},
                {sf::Keyboard::J, 'j'}, {sf::Keyboard::K, 'k'}, {sf::Keyboard::L, 'l'},
                {sf::Keyboard::M, 'm'}, {sf::Keyboard::N, 'n'}, {sf::Keyboard::O, 'o'},
                {sf::Keyboard::P, 'p'}, {sf::Keyboard::Q, 'q'}, {sf::Keyboard::R, 'r'},
                {sf::Keyboard::S, 's'}, {sf::Keyboard::T, 't'}, {sf::Keyboard::U, 'u'},
                {sf::Keyboard::V, 'v'}, {sf::Keyboard::W, 'w'}, {sf::Keyboard::X, 'x'},
                {sf::Keyboard::Y, 'y'}, {sf::Keyboard::Z, 'z'}, {sf::Keyboard::Space, ' '},
                {sf::Keyboard::Num0, '0'}, {sf::Keyboard::Num1, '1'}, {sf::Keyboard::Num2, '2'},
                {sf::Keyboard::Num3, '3'}, {sf::Keyboard::Num4, '4'}, {sf::Keyboard::Num5, '5'},
                {sf::Keyboard::Num6, '6'}, {sf::Keyboard::Num7, '7'}, {sf::Keyboard::Num8, '8'},
                {sf::Keyboard::Num9, '9'},
            };

            m_fTime += 0.01f * l_fTime;
            m_fTime2 += 0.01f * l_fTime;
            if (m_fTime > 2.0f) {
                const bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                                          sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

                for (const auto& [key, character] : keyMap) {
                    if (sf::Keyboard::isKeyPressed(key)) {
                        if (shiftPressed && std::isalpha(character)) {
                            inputText += std::toupper(character);
                        } else {
                            inputText += character;
                        }
                        m_fTime = 0;
                        break;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !inputText.empty()) {
                    inputText.pop_back();
                    m_fTime = 0;
                }
            }

            m_txText.setString(inputText);
        }
    }
    void cInputText::setInput(const bool l_bool) {
        m_bInput = l_bool;
    }

    void cInputText::draw(sf::RenderWindow& window) const {
        window.draw(m_rsRect);
        window.draw(m_txText);
    }

    void cInputText::setPositionText(const sf::Vector2f l_pos) {
        m_txText.setPosition(l_pos);
    }

    void cInputText::setPositionRectangleShape(const sf::Vector2f l_pos) {
        m_rsRect.setPosition(l_pos);
    }

    void cInputText::setRectSize(const sf::Vector2f l_RectSize) {
        m_rsRect.setSize(l_RectSize);
    }

    void cInputText::setColorText(const sf::Color l_ColorText) {
        m_txText.setFillColor(l_ColorText);
    }

    void cInputText::setFont(const sf::Font& l_font) {
        m_txText.setFont(l_font);
    }

    void cInputText::setColorRect(const sf::Color l_ColorRect) {
        m_rsRect.setFillColor(l_ColorRect);
    }
    void cInputText::setString(const std::string& l_String) {
        m_txText.setString(l_String);

    }
    void cInputText::setCharacterSize(const unsigned int l_size) {
        m_txText.setCharacterSize(l_size);
    }



    bool cInputText::getInput() const {
        return m_bInput;
    }

    sf::Vector2f cInputText::getRectSize() const {
        return m_rsRect.getSize();
    }

    sf::Color cInputText::getColorText() const {
        return m_txText.getFillColor();
    }

    sf::Color cInputText::getColorRect() const {
        return m_rsRect.getFillColor();
    }

    std::string cInputText::getString() const {
        return m_txText.getString();
    }

    sf::Vector2f cInputText::getPositionText() const {
        return m_txText.getPosition();
    }

    sf::Vector2f cInputText::getPositionRectangleShape() const {
        return m_rsRect.getPosition();
    }

    unsigned int cInputText::getCharacterSize() const {
        return m_txText.getCharacterSize();
    }
}