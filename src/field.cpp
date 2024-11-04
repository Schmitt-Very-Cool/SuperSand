#include "field.h"

void Field::swap(int x1, int y1, int x2, int y2) {
	if (!(isInBounds(x1, y1) && isInBounds(x2, y2))) {
		return;
	}
	particle temp = field[x1][y1];
	field[x1][y1] = field[x2][y2];
	field[x2][y2] = temp;
}

void Field::modV(int x, int y, const sf::Vector2f& v) {
	if (!isInBounds(x, y)) {
		return;
	}
	field[x][y].velocity = v;
}

void Field::modT(int x, int y, double t) {
	if (!isInBounds(x, y)) {
		return;
	}
	field[x][y].temp = t;
}

void Field::reverse(int x, int y) {
	if (!isInBounds(x, y)) {
		return;
	}
	field[x][y].liquidMovingLeft = !field[x][y].liquidMovingLeft;
}

void Field::setColor(int x, int y, sf::Color c) {
	if (!isInBounds(x, y)) {
		return;
	}
	field[x][y].color = c;
}

bool Field::isInBounds(int x, int y) {
	return (x >= 0 && x < _numCols && y >= 0 && y < _numRows);
}