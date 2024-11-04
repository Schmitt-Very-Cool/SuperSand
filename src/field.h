#pragma once

#include <iostream>
#include <vector>

#include "particle.h"

class Field {
public:
    const static int _numRows = 160;
    const static int _numCols = 160;

    Field() : field(_numCols, std::vector<particle>(_numRows)) {
        for (int i = 0; i < _numCols; i++) {
            for (int j = 0; j < _numRows; j++) {
                field[i][j] = newParticle();
            }
        }
    }

    static particle newParticle() {
        return particle{ ParticleType::Air, sf::Vector2f(0,0), sf::Color::Black, sf::Color::Black };
    }

    static const int numRows() {
        return _numRows;
    }

    static const int numCols() {
        return _numCols;
    }

    const particle get(int x, int y) const {
        if (x >= 0 && x < _numCols && y >= 0 && y < _numRows) {
            return field[x][y];
        }
        return particle{ParticleType::Error, sf::Vector2f(0,0), sf::Color::Yellow, sf::Color::Yellow, false, 20.0f};
    }

    void set(int x, int y, particle p) {
        if (x >= 0 && x < _numCols && y >= 0 && y < _numRows) {
            field[x][y] = p;
        }
    }

    void setAll(const Field& other) {
        for (int i = 0; i < _numCols; i++) {
            for (int j = 0; j < _numRows; j++) {
                field[i][j] = other.get(i, j);
            }
        }
    }

    void swap(int x1, int y1, int x2, int y2);

    void modV(int x, int y, const sf::Vector2f& v);

    void modT(int x, int y, double t);

    void reverse(int x, int y);

    void setColor(int x, int y, sf::Color c);

    bool isInBounds(int x, int y);

    Field clone() const {
        Field copy;
        copy.setAll(*this);
        return copy;
    }

private:
    std::vector<std::vector<particle>> field;
};