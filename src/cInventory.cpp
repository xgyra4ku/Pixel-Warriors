#include "../include/cInventory.hpp"


cInventory::cInventory() : m_bInventoryIsOpen(false){}
cInventory::~cInventory() = default;


void cInventory::draw(sf::RenderWindow window) const {
    if (m_bInventoryIsOpen) {
        //window.draw();
    }
}

bool cInventory::checkIfTheItemIsInInventoryByIndex(int iIndexItem) {

}

void cInventory::setInventoryStatus(const bool bValue) {
    m_bInventoryIsOpen = bValue;
}

void cInventory::setItemByIndex() {

}

bool cInventory::getInventoryStatus() const {
    return m_bInventoryIsOpen;
}

int cInventory::getIdItemByIndex(int iIndexItem) const {

}


