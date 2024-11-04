#include "particleBehavior.h"

#include <algorithm>
#include <cstdlib>

std::unique_ptr<Field> ParticleBehavior::field = nullptr;

void ParticleBehavior::update(Field& f) {
	if (!ParticleBehavior::field) {
		ParticleBehavior::field = std::make_unique<Field>(f);
	}
	else {
		ParticleBehavior::field->setAll(f);
	}
}

void ParticleBehavior::doInteractions() {
	Field* nextField = new Field();
	*nextField = field->clone();

	for (int i = 0; i < Field::_numCols; i++) {
		for (int j = 0; j < Field::_numRows; j++) {
			//Nothing to do, currently.
		}
	}
}

void ParticleBehavior::applyGravity() {
	for (int i = 0; i < Field::_numCols; i++) {
		for (int j = 0; j < Field::_numRows - 1; j++) {
			if (!ParticleType::isState(field->get(i, j).type, 
				ParticleState::Gas | ParticleState::Lightning | ParticleState::Rigid | ParticleState::Error)) {
				if (field->get(i, j + 1).type.state & ParticleState::Gas) {
					field->modV(i,j, field->get(i, j).velocity + sf::Vector2f(0, 0.75));
				}
			}
		}
	}
}

void ParticleBehavior::doProjectileMovement() {
	if (!field) {
		return;
	}

	std::unique_ptr<Field> nextField = std::make_unique<Field>();
	*nextField = field->clone();

	for (int i = 0; i < Field::_numCols; i++) {
		for (int j = 0; j < Field::_numRows; j++) {
			double dx = field->get(i, j).velocity.x;
			double dy = field->get(i, j).velocity.y;
			int max = (int)std::round(std::max(std::abs(dx),std::abs(dy)));
			if (max == 0) {
				continue;
			}
			particle temp = field->get(i, j);
			double stepx = dx / max;
			double stepy = dy / max;
			int x, y, prevx = i, prevy = j;
			bool stopped = false;
			for (int k = 1; k <= max; k++) {
				x = (int) std::round(stepx * k + i);
				y = (int) std::round(stepy * k + j);
				prevx = (int) std::round(stepx * (k - 1) + i);
				prevy = (int) std::round(stepy * (k - 1) + j);
				if (x < 0 || x >= Field::_numCols || y < 0 || y >= Field::_numRows || nextField->get(x, y).type.state != ParticleState::Gas) {
					// The Buck Stops Here
					stopped = true;
					break;
				}
				nextField->set(prevx, prevy, field->get(x, y));
			}
			if (stopped) {
				temp.velocity = sf::Vector2f(0, 0);
			}
			nextField->set(prevx, prevy, temp);
		}
	}

	field->setAll(*nextField);
}

void ParticleBehavior::doPowderMovement() {
	if (!field) {
		return;
	}

	std::unique_ptr<Field> nextField = std::make_unique<Field>();
	*nextField = field->clone();

	for (int j = Field::_numRows - 1; j >= 0; j--) {
		for (int i = 0; i < Field::_numCols; i++) {
			particle thisParticle = field->get(i, j);
			if (!(thisParticle.type.state & ParticleState::Powder)) {
				continue;
			}
			if (j < Field::_numRows - 1 
				&& ParticleType::isState(field->get(i, j + 1).type,
					ParticleState::Powder | ParticleState::Solid | ParticleState::Rigid)){
				bool downLeft = i > 0 && j < Field::_numRows - 1 &&
					ParticleType::isState(field->get(i - 1, j + 1).type,
						ParticleState::Liquid | ParticleState::Gas);
				bool downRight = i < Field::_numCols - 1 && j < Field::_numRows - 1 &&
					ParticleType::isState(field->get(i + 1, j + 1).type,
						ParticleState::Liquid | ParticleState::Gas);
				if (downLeft || downRight) {
					bool movDir = downLeft && !downRight
						? true
						: (!downLeft && downRight
							? false
							: std::rand() % 2);

					if (movDir) {
						nextField->swap(i, j, i - 1, j + 1);
					}
					else {
						nextField->swap(i, j, i + 1, j + 1);
					}
				}
			}
		}
	}

	field->setAll(*nextField);
}

//TODO: separate out vertical movement in a separate loop so the horizontal movement stops throwing sinking powder to the right

void ParticleBehavior::doLiquidMovement() {

	//ensure field exists
	if (!field) {
		return;
	}

	//use buffer field to avoid accidentally moving the same particle like a thousand times
	std::unique_ptr<Field> nextField = std::make_unique<Field>();
	*nextField = field->clone();

	//vertical movement
	for (int j = Field::_numRows - 1; j >= 0; j--) {
		for (int i = 0; i < Field::_numCols; i++) {
			particle thisParticle = field->get(i, j);
			//ensure we're only moving liquid particles
			if (!(thisParticle.type.state & ParticleState::Liquid)) {
				continue;
			}

			//if powder beneath the liquid is less dense than the liquid, then swap them.
			if (j < Field::_numRows - 1 &&
				ParticleType::isState(field->get(i, j + 1).type, ParticleState::Powder)){
				if (thisParticle.type.density > field->get(i, j + 1).type.density) {
					nextField->swap(i, j, i, j + 1);
					continue;
				}
			}
			//if powder, liquid, or solid above the liquid is denser than the liquid, then swap them.
			if (j > 0 &&
				ParticleType::isState(field->get(i, j - 1).type, ParticleState::Liquid | ParticleState::Powder | ParticleState::Solid)){
				if (thisParticle.type.density < field->get(i, j - 1).type.density) {
					nextField->swap(i, j, i, j - 1);
					continue;
				}
				//randomly move same density between layers to allow for diffusion over time
				else if (thisParticle.type.density == field->get(i, j - 1).type.density && std::rand() % 10 == 0) {
					nextField->swap(i, j, i, j - 1);
					continue;
				}
			}
			//if not above gas, see if it can slide down a slope
			if (ParticleType::isState(field->get(i, j + 1).type,
				ParticleState::Powder | ParticleState::Liquid | ParticleState::Solid | ParticleState::Rigid | ParticleState::Error)) {
				//downleft and downright represent the availability of moving in that direction
				bool downLeft = i > 0 && j < Field::_numRows - 1
					&& ParticleType::isState(field->get(i - 1, j + 1).type, ParticleState::Gas);
				bool downRight = i < Field::_numCols - 1 && j < Field::_numRows - 1
					&& ParticleType::isState(field->get(i + 1, j + 1).type, ParticleState::Gas);
				if (downLeft && downRight) {
					// Randomly choose direction
					if (std::rand() % 2) {
						nextField->swap(i, j, i - 1, j + 1);
					}
					else {
						nextField->swap(i, j, i + 1, j + 1);
					}
				}
				else if (downLeft) {
					nextField->swap(i, j, i - 1, j + 1);
				}
				else if (downRight) {
					nextField->swap(i, j, i + 1, j + 1);
				}
			}
		}
	}
	field->setAll(*nextField);

	//horizontal movement
	//separated from vertical movement cause it was causing liquid to throw sinking particles sideways
	for (int j = Field::_numRows - 1; j >= 0; j--) {
		for (int i = 0; i < Field::_numCols; i++) {
			particle thisParticle = field->get(i, j);

			//ensure particle is liquid
			if (!(thisParticle.type.state & ParticleState::Liquid)) {
				continue;
			}

			//ensure particle is not above gas
			if (ParticleType::isState(field->get(i, j + 1).type, ParticleState::Gas)) {
				continue;
			}

			particle left = field->get(i - 1, j);
			particle right = field->get(i + 1, j);

			//slide sideways
			bool openLeft = false, openRight = false;

			//Allow free flowing through gas and liquid when sideways
			if (ParticleType::isState(left.type, ParticleState::Gas | ParticleState::Liquid)) {
				openLeft = true;
			}
			if (ParticleType::isState(right.type, ParticleState::Gas | ParticleState::Liquid)) {
				openRight = true;
			}

			//Prefer to flow through gas over liquid
			if (left.type.state & ParticleState::Gas && right.type.state & ParticleState::Liquid) {
				openRight = false;
			}
			if (right.type.state & ParticleState::Gas && left.type.state & ParticleState::Liquid) {
				openLeft = false;
			}

			//if within the same liquid,move randomly instead of in a particular direction,
			//since it doesn't need to worry about settling flat.
			if (left.type.id == thisParticle.type.id && right.type.id == thisParticle.type.id) {
				int random = std::rand() % 3;
				if (random == 0) {
					nextField->swap(i, j, i - 1, j);
				}
				else if (random == 1) {
					nextField->swap(i, j, i + 1, j);
				}
				else {

				}
			}
			else {
				if (openLeft) {
					if (thisParticle.liquidMovingLeft) {
						nextField->swap(i, j, i - 1, j);
					}
					else if (!openRight) {
						nextField->reverse(i, j);
					}
				}
				if (openRight) {
					if (!thisParticle.liquidMovingLeft) {
						nextField->swap(i, j, i + 1, j);
					}
					else if (!openLeft) {
						nextField->reverse(i, j);
					}
				}
			}
		}
	}

	field->setAll(*nextField);
}

void ParticleBehavior::doGasMovement() {
	if(!field) {
		return;
	}

	std::unique_ptr<Field> nextField = std::make_unique<Field>();
	*nextField = field->clone();

	for (int i = 0; i < Field::_numCols; i++) {
		for (int j = 0; j < Field::_numRows; j++) {
			particle thisParticle = field->get(i, j);
			//ensure we're only moving gas particles
			if (!(thisParticle.type.state & ParticleState::Gas)) {
				continue;
			}

			int adjacent = 0;
			int adjacentLessDense = 0;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					if (k == 0 && l == 0) continue;
					if (field->get(i + k, j + l).type.state & ParticleState::Gas) {
						adjacent++;
						if (l == 1 && field->get(i + k, j + l).type.density < thisParticle.type.density) {
							adjacentLessDense++;
						}
					}
				}
			}

			//gas sometimes just stays still
			if (std::rand() % 2) {
				continue;
			}

			if (adjacent == 0) {
				continue;
			}

			if (adjacentLessDense == 0) {
				int movDir = std::rand() % adjacent;
				int dir = -1;

				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						if (k == 0 && l == 0) continue;
						if (field->get(i + k, j + l).type.state & ParticleState::Gas && ++dir == movDir) {
							nextField->swap(i, j, i + k, j + l);
						}
					}
				}
			}
			else {
				int movDir = std::rand() % adjacentLessDense;
				int dir = -1;
				if ((field->get(i + 1, j + 1).type.state & ParticleState::Gas)
					&& field->get(i + 1, j + 1).type.density < thisParticle.type.density 
					&& ++dir == movDir) {
					nextField->swap(i, j, i + 1, j + 1);
				}
				if ((field->get(i, j + 1).type.state & ParticleState::Gas)
					&& field->get(i, j + 1).type.density < thisParticle.type.density
					&& ++dir == movDir) {
					nextField->swap(i, j, i, j + 1);
				}
				if ((field->get(i - 1, j + 1).type.state & ParticleState::Gas)
					&& field->get(i - 1, j + 1).type.density < thisParticle.type.density
					&& ++dir == movDir) {
					nextField->swap(i, j, i - 1, j + 1);
				}
			}
		}
	}

	field->setAll(*nextField);
}