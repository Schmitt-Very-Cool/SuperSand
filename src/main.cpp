#include <iostream>

#include <stdlib.h>
#include <windows.h>

#include "particleBehavior.h"

#define cout std::cout
#define endl std::endl
#define numRows Field::_numRows
#define numCols Field::_numCols

// Global constants
const int particleSize = 5; //size of individual cell
const int margin = 30; //padding between the edge of the window and the field
const sf::Color gray(0x80, 0x80, 0x80);

struct menuWord {
    const char* name;
    int id;
};

const menuWord menuWords[6][6] =
{   {{"Sand", 2}, {"Water", 3}, {"Stone", 4}, {"Glass", 5}, {"Salt", 6}, {"Dirt", 7}},
    {{"Air", 1}, {"Gunpowder", 8}, {"Rainbow", 9}, {"Soap", 10}, {"Fireworks", 11}, {"Ash", 12}},
    {{"Snow", 13}, {"Acid", 14}, {"Base", 15}, {"Oil", 16}, {"Lava", 17}, {"Mercury", 18}},
    {{"Nitro", 19}, {"Smoke", 20}, {"Steam", 21}, {"Cloud", 22}, {"Gas", 23}, {"Helium", 24}},
    {{"Bricks", 25}, {"Metal", 26}, {"Gold", 27}, {"Wood", 28}, {"Ice", 29}, {"C4", 30}},
    {{"Detonator", 31}, {"Lightning", 32}, {"", 2}, {"", 2}, {"Hot", 2}, {"Cold", 2}}, };

bool menuOpen = false;
int penSize = 5;

// -- Function callbacks

// Draws entire game window
void drawGame(sf::RenderWindow& window, const Field& field, const sf::Font& font);

// Draws info on current particle
void drawInfoStrings(sf::RenderWindow& window, const Field& field, const sf::Font& font);

// Draws the selection menu for choosing more particles
void drawPowderSelect(sf::RenderWindow& window, const sf::Font& font);

// Ensures coordinate is within the field's bounds
bool isInBounds(int x, int y);

// Runs one gametick
void simulate(Field &field);


int main() {

    // Create the window
    sf::RenderWindow window(sf::VideoMode((2 * margin + numRows * particleSize), 2 * margin + numCols * particleSize), "SuperSand");
    window.setPosition(sf::Vector2i(10, 50));
    window.setVerticalSyncEnabled(true);

    // Load Font
    sf::Font font;
    if (!font.loadFromFile("cour.ttf")) {
        cout << "Error loading font" << endl;
        return -1;
    }

    // Initialize field
    Field field;

    sf::Clock clock;
    ParticleTypeDetails currentType = ParticleType::Sand;

    // Main Loop ====================================================================
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                // Window closed
            case (sf::Event::Closed):
                window.close();
                break;
            case (sf::Event::KeyPressed):
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    menuOpen = !menuOpen;
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            if (menuOpen) {
                for (int i = 0; i < 6; i++) {
                    for (int j = 0; j < 6; j++) {
                        int x = margin + particleSize * numCols / 8 / 6 + i * particleSize * numCols / 6;
                        int y = margin + particleSize * numRows / 4 / 6 + j * particleSize * numRows / 6;
                        if (pos.x >= x && pos.x < x + particleSize * numCols * 3 / 6 / 4
                            && pos.y >= y && pos.y < y + particleSize * numRows / 6 / 2) {
                            currentType = ParticleType::typeFromId(menuWords[j][i].id);
                        }
                    }
                }
            }
            else {
                int x = (pos.x - margin) / particleSize;
                int y = (pos.y - margin) / particleSize;
                for (int i = -penSize; i <= penSize; i++) {
                    for (int j = -penSize; j <= penSize; j++) { 
                        int dx = x + i;
                        int dy = y + j;
                        if (isInBounds(dx, dy)) {
                            if (currentType.id == ParticleType::Air.id || field.get(dx, dy).type.state & ParticleState::Gas) {
                                if ((i * i + j * j) <= (penSize * penSize)) {
                                    sf::Color c = ParticleType::generateColor(currentType, dx, dy);
                                    field.set(dx, dy, particle{ currentType, sf::Vector2f(0, 0),
                                        c, c});
                                }
                            }
                        }
                    }
                }
            }
        }

        // Run one Gametick
        if (clock.getElapsedTime().asMilliseconds() > 50) {
            simulate(field);
            clock.restart();
        }

        // Clear screen
        window.clear();

        // Draw the shape

        drawGame(window, field, font);

        // End the current frame
        window.display();
    }

    return 0;
}

void drawGame(sf::RenderWindow& window, const Field& field, const sf::Font& font) {
    sf::RectangleShape rectangle(sf::Vector2f(particleSize * numCols, particleSize * numRows));
    rectangle.setPosition(margin, margin);
    rectangle.setOutlineThickness(5);
    rectangle.setOutlineColor(sf::Color::Blue);
    rectangle.setFillColor(sf::Color::Black);
    window.draw(rectangle);

    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {
            if (!equal(field.get(i,j).type, ParticleType::Air)) {
                sf::Color color(field.get(i, j).color.r,
                    field.get(i, j).color.g,
                    field.get(i, j).color.b,
                    (menuOpen ? 0x80 : 0xff));
                sf::RectangleShape square(sf::Vector2f(particleSize, particleSize));
                square.setPosition(margin + i * particleSize, margin + j * particleSize);
                square.setFillColor(color);
                window.draw(square);
            }
        }
    }

    if (!menuOpen) {
        drawInfoStrings(window, field, font);
    }
    else {
        drawPowderSelect(window, font);
    }
}

void drawInfoStrings(sf::RenderWindow& window, const Field& field, const sf::Font& font) {
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    int x = (pos.x - margin) / particleSize;
    int y = (pos.y - margin) / particleSize;
    if (isInBounds(x, y)) {
        std::string str = ParticleType::name(field.get(x,y).type);
        str += " | Velocity(";
        str += std::to_string(field.get(x, y).velocity.x);
        str += ",";
        str += std::to_string(field.get(x, y).velocity.y);
        str += ") | Color: ";
        str += std::to_string(field.get(x, y).color.r);
        str += " ";
        str += std::to_string(field.get(x, y).color.g);
        str += " ";
        str += std::to_string(field.get(x, y).color.b);

        sf::Text text;
        text.setString(str);
        text.setFont(font);
        text.setPosition(margin, 0);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }
}

void drawPowderSelect(sf::RenderWindow& window, const sf::Font& font) {
    int menuX = 6;
    int menuY = 6;
    int width = particleSize * numCols;
    int height = particleSize * numRows;
    int gapX = width / 8 / menuX;
    int gapY = height / 4 / menuY;

    sf::RectangleShape rect(sf::Vector2f(width * 3 / menuX / 4, height / menuY / 2));
    rect.setOutlineThickness(3);
    rect.setOutlineColor(sf::Color::White);
    rect.setFillColor(sf::Color::Black);

    sf::Text text;
    text.setFont(font);
    int characterSize = height / menuY / 6;
    text.setCharacterSize(characterSize);
    text.setFillColor(sf::Color::White);
    for (int i = 0; i < menuX; i++) {
        for (int j = 0; j < menuY; j++) {
            int x = margin + gapX + i * width / menuX;
            int y = margin + gapY + j * height / menuY;
            sf::Vector2i v = sf::Mouse::getPosition(window);
            if (v.x >= x && v.x < x + (width * 3 / menuX / 4) && v.y >= y && v.y < y + (height / menuY / 2)) {
                rect.setFillColor(gray);
            }
            text.setString(menuWords[j][i].name);
            rect.setPosition(sf::Vector2f(x, y));
            text.setPosition(sf::Vector2f(x, 
                y + height / menuY / 2));
            window.draw(rect);
            window.draw(text);
            if (v.x >= x && v.x < x + (width * 3 / menuX / 4) && v.y >= y && v.y < y + (height / menuY / 2)) {
                rect.setFillColor(sf::Color::Black);
            }
        }
    }

}

bool isInBounds(int x, int y) {
    return (x >= 0 && x < numCols && y >= 0 && y < numRows);
}

void simulate(Field &field) {
    ParticleBehavior::update(field);
    //ParticleBehavior::updateColors();
    ParticleBehavior::doInteractions();
    ParticleBehavior::applyGravity();
    ParticleBehavior::doProjectileMovement();
    ParticleBehavior::doPowderMovement();
    ParticleBehavior::doLiquidMovement();
    ParticleBehavior::doGasMovement();
    field.setAll(*ParticleBehavior::field);
}