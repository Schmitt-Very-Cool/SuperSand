#pragma once

#undef min
#undef max
#include <SFML/Graphics.hpp>

#include <string>

enum ParticleState {
	Powder = 1,
	Solid = 2,
	Liquid = 4,
	Gas = 8,
	Rigid = 16,
	Lightning = 32,
	Error = 64
};

struct ParticleTypeDetails {
	ParticleState state;
	double density;
	double thermalConductivity;
	double specificHeat;
	double startingTemp;
	bool ignitable;
	double ignitionTemp;
	int id;
};

class ParticleType {
public:
	//									Name			State					Density	Therm	Heat	Temp	Ig	IgTmp	ID
	static constexpr ParticleTypeDetails Air		{ ParticleState::Gas,		0,		0.03,	1.01,	20,		0,	-1,		1 };
	static constexpr ParticleTypeDetails Sand		{ ParticleState::Powder,	1.6,	0.2,	0.83,	20,		0,	-1,		2 };
	static constexpr ParticleTypeDetails Water		{ ParticleState::Liquid,	1,		0.6,	4.2,	20,		0,	-1,		3 };
	static constexpr ParticleTypeDetails Stone		{ ParticleState::Solid,		3,		3,		0.79,	20,		0,	-1,		4 };
	static constexpr ParticleTypeDetails Glass		{ ParticleState::Rigid,		3,		1.4,	0.84,	20,		0,	-1,		5 };
	static constexpr ParticleTypeDetails Salt		{ ParticleState::Solid,		1.2,	1,		0.88,	20,		0,	-1,		6 };
	static constexpr ParticleTypeDetails Dirt		{ ParticleState::Powder,	1.4,	0.2,	1.4,	20,		0,	-1,		7 };
	static constexpr ParticleTypeDetails Gunpowder	{ ParticleState::Powder,	0.5,	0.03,	1,		20,		1,	600,	8 };
	static constexpr ParticleTypeDetails Rainbow	{ ParticleState::Powder,	1.1,	1,		1,		20,		0,	-1,		9 };
	static constexpr ParticleTypeDetails Soap		{ ParticleState::Powder,	0.8,	1,		0.88,	20,		0,	-1,		10 };
	static constexpr ParticleTypeDetails Fireworks	{ ParticleState::Powder,	0.5,	0.03,	1,		20,		1,	600,	11 };
	static constexpr ParticleTypeDetails Ash		{ ParticleState::Powder,	0.4,	0.03,	1,		20,		0,	-1,		12 };
	static constexpr ParticleTypeDetails Snow		{ ParticleState::Powder,	0.5,	0.4,	2.09,	-10,	0,	-1,		13 };
	static constexpr ParticleTypeDetails Acid		{ ParticleState::Liquid,	1.0,	0.6,	4.2,	20,		0,	-1,		14 };
	static constexpr ParticleTypeDetails Base		{ ParticleState::Liquid,	1.0,	0.6,	4.2,	20,		0,	-1,		15 };
	static constexpr ParticleTypeDetails Oil		{ ParticleState::Liquid,	0.7,	0.15,	1.8,	20,		1,	600,	16 };
	static constexpr ParticleTypeDetails Lava		{ ParticleState::Liquid,	3,		2,		0.84,	1500,	0,	-1,		17 };
	static constexpr ParticleTypeDetails Mercury	{ ParticleState::Liquid,	13,		29,		0.14,	20,		0,	-1,		18 };
	static constexpr ParticleTypeDetails Nitro		{ ParticleState::Liquid,	1.5,	1,		0.28,	20,		1,	200,	19 };
	static constexpr ParticleTypeDetails Smoke		{ ParticleState::Gas,		0,		0.03,	1.01,	20,		1,	900,	20 };
	static constexpr ParticleTypeDetails Steam		{ ParticleState::Gas,		0,		0.03,	1.01,	20,		0,	-1,		21 };
	static constexpr ParticleTypeDetails Cloud		{ ParticleState::Gas,		0,		0.03,	1.01,	20,		0,	-1,		22 };
	static constexpr ParticleTypeDetails NatGas		{ ParticleState::Gas,		-1,		0.03,	2.19,	20,		1,	600,	23 };
	static constexpr ParticleTypeDetails Helium		{ ParticleState::Gas,		-3,		0.03,	5.19,	20,		0,	-1,		24 };
	static constexpr ParticleTypeDetails Bricks		{ ParticleState::Rigid,		3,		3,		0.79,	20,		0,	-1,		25 };
	static constexpr ParticleTypeDetails Metal		{ ParticleState::Rigid,		8,		73,		0.45,	20,		0,	-1,		26 };
	static constexpr ParticleTypeDetails Gold		{ ParticleState::Rigid,		20,		318,	0.13,	20,		0,	-1,		27 };
	static constexpr ParticleTypeDetails Wood		{ ParticleState::Rigid,		10,		0.1,	0.5,	20,		1,	300,	28 };
	static constexpr ParticleTypeDetails Ice		{ ParticleState::Rigid,		0.9,	2.5,	2.05,	-20,	0,	-1,		29 };
	static constexpr ParticleTypeDetails C4			{ ParticleState::Rigid,		1.7,	1,		1,		20,		0,	-1,		30 };
	static constexpr ParticleTypeDetails Detonator	{ ParticleState::Rigid,		0.1,	1,		1,		20,		1,	280,	31 };
	static constexpr ParticleTypeDetails Lightning	{ ParticleState::Lightning,	0,		1000,	1000,	20,		0,	-1,		32 };
	static constexpr ParticleTypeDetails Error		{ ParticleState::Error,		-1,		-1,		1000,	20,		0,	-1,		0 };

	static std::string name(ParticleTypeDetails type) {
		switch (type.id){
		case 1: return "Air";
		case 2: return "Sand";
		case 3: return "Water";
		case 4: return "Stone";
		case 5: return "Glass";
		case 6: return "Salt";
		case 7: return "Dirt";
		case 8: return "Gunpowder";
		case 9: return "Rainbow";
		case 10: return "Soap";
		case 11: return "Fireworks";
		case 12: return "Ash";
		case 13: return "Snow";
		case 14: return "Acid";
		case 15: return "Base";
		case 16: return "Oil";
		case 17: return "Lava";
		case 18: return "Mercury";
		case 19: return "Nitro";
		case 20: return "Smoke";
		case 21: return "Steam";
		case 22: return "Cloud";
		case 23: return "Natural Gas";
		case 24: return "Helium";
		case 25: return "Bricks";
		case 26: return "Metal";
		case 27: return "Gold";
		case 28: return "Wood";
		case 29: return "Ice";
		case 30: return "C4";
		case 31: return "Detonator";
		case 32: return "Lightning";
		default: return "Error";
		}
	}

	static bool isState(ParticleTypeDetails type, int states) {
		if (states & 1)
			if (type.state & ParticleState::Powder) return true;
		if (states & 2)
			if (type.state & ParticleState::Solid) return true;
		if (states & 4)
			if (type.state & ParticleState::Liquid) return true;
		if (states & 8)
			if (type.state & ParticleState::Gas) return true;
		if (states & 16)
			if (type.state & ParticleState::Rigid) return true;
		if (states & 32)
			if (type.state & ParticleState::Lightning) return true;
		if (states & 64)
			if (type.state & ParticleState::Error) return true;
		return false;
	}

	static sf::Color generateColor(ParticleTypeDetails type, int x = 0, int y = 0) {
		double h = 0, s = 0, v = 0;

		switch(type.id){
		case Sand.id:
			h = 33.0 / 360.0;
			s = 0.7 - (0.2 * std::rand() / RAND_MAX);
			v = 1 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Water.id:
			h = (200.0 + std::rand() % 20) / 360.0;
			s = 1;
			v = 1 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Air.id:
			h = 0;
			s = 0;
			v = 0;
			break;
		case Stone.id:
			h = 16.0 / 360.0;
			s = 0.1;
			v = 0.5 - (0.3 * std::rand() / RAND_MAX);
			break;
		case Glass.id:
			h = 160.0 / 360.0;
			s = 0.1;
			v = 0.2;
			break;
		case Salt.id:
			h = 16.0 / 360.0;
			s = 0.05;
			v = 1.0 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Dirt.id:
			h = 16.0 / 360.0;
			s = 0.6 - (0.2 * std::rand() / RAND_MAX);
			v = 0.6 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Gunpowder.id:
			h = 0.0;
			s = 0.0;
			v = 0.7 - (0.05 * std::rand() / RAND_MAX) * 8;
			break;
		case Rainbow.id:
			h = 1.0 * std::rand() / RAND_MAX;
			s = 1.0;
			v = 1.0;
			break;
		case Soap.id:
			h = 220.0 / 360.0;
			s = 0.2;
			v = 1.0 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Fireworks.id:
			h = (30.0 - (3.0 * std::rand() / RAND_MAX)) / 360.0;
			s = 0.8 - (0.1 * std::rand() / RAND_MAX);
			v = 1.0 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Ash.id:
			h = 0.0;
			s = 0.0;
			v = 0.5 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Snow.id:
			h = 200.0 / 360.0;
			s = 0.1;
			v = 1.0 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Acid.id:
			h = 115.0 / 360.0;
			s = 1.0;
			v = 1.0 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Base.id:
			h = 280.0 / 360.0;
			s = 1.0;
			v = 1.0 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Oil.id:
			h = 24.0 / 360.0;
			s = 0.5 - (0.1 * std::rand() / RAND_MAX);
			v = 0.45 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Lava.id:
			h = (30.0 - (10.0 * std::rand() / RAND_MAX)) / 360.0;
			s = 1.0;
			v = 1.0 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Mercury.id:
			h = 0.0;
			s = 0.0;
			v = 0.6 - (0.05 * std::rand() / RAND_MAX);
			break;
		case Nitro.id:
			h = 45.0 / 360.0;
			s = 0.5 - (0.1 * std::rand() / RAND_MAX);
			v = 1.0;
			break;
		case Smoke.id:
			h = 0.0;
			s = 0.0;
			v = 0.2 - (0.15 * std::rand() / RAND_MAX);
			break;
		case Steam.id:
			h = (200.0 + std::rand() % 20) / 360.0;
			s = 0.1;
			v = 0.5 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Cloud.id:
			h = h = (200.0 + std::rand() % 20) / 360.0;
			s = 0.2 - (0.1 * std::rand() / RAND_MAX);
			v = 1.0;
			break;
		case NatGas.id:
			h = 75.0 / 360.0;
			s = 0.5;
			v = 0.45 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Helium.id:
			h = 54.0 / 360.0;
			s = 0.25;
			v = 0.5 - (0.1 * std::rand() / RAND_MAX);
			break;
		case Bricks.id:
			if (y % 5 == 3 || ((y-3) % 10 < 5 && x % 10 == 3) || ((y-3) % 10 >= 5 && x % 10 == 8)) {
				h = 45.0 / 360.0;
				s = 0.3 - (0.1 * std::rand() / RAND_MAX);
				v = 0.9 - (0.1 * std::rand() / RAND_MAX);
			}
			else {
				h = 5.0 / 360.0;
				s = 0.9 - (0.5 * std::rand() / RAND_MAX);
				v = 0.5 - (0.2 * std::rand() / RAND_MAX);
			}
			break;
		case Metal.id:
			h = 0.0;
			s = 0.0;
			v = 0.7 - (0.05 * std::rand() / RAND_MAX);
			break;
		case Gold.id:
			h = 48.0 / 360.0;
			s = 1.0;
			v = 1.0 - (0.05 * std::rand() / RAND_MAX);
			break;
		case Wood.id:
			h = 30.0 / 360.0;
			s = 0.7;
			v = 0.5 - (0.2 * std::rand() / RAND_MAX);
			break;
		case Ice.id:
			h = 180.0 / 360.0;
			s = 0.4 - (0.4 * std::rand() / RAND_MAX);
			v = 1.0;
			break;
		case C4.id:
			h = 54.0 / 360.0;
			s = 0.4;
			v = 1.0 - (0.5 * std::rand() / RAND_MAX);
			break;
		case Detonator.id:
			h = 290.0 / 360.0;
			s = 0.4;
			v = 1.0;
			break;
		case Lightning.id:
			h = 60.0 / 360.0;
			s = 1.0;
			v = 1.0;
			break;
		default:
			h = 0.84;
			s = 1;
			v = 1;
			break;
		}

		double r, g, b;
		int i = (int)floor(h * 6);
		double f = h * 6 - i;
		double p = v * (1 - s);
		double q = v * (1 - f * s);
		double t = v * (1 - (1 - f) * s);
		switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		sf::Color color((int)(r * 255), (int)(g * 255), (int)(b * 255));
		return color;
	}

	static ParticleTypeDetails typeFromId(int id) {
		switch (id) {
		case 1: return Air;
		case 2: return Sand;
		case 3: return Water;
		case 4: return Stone;
		case 5: return Glass;
		case 6: return Salt;
		case 7: return Dirt;
		case 8: return Gunpowder;
		case 9: return Rainbow;
		case 10: return Soap;
		case 11: return Fireworks;
		case 12: return Ash;
		case 13: return Snow;
		case 14: return Acid;
		case 15: return Base;
		case 16: return Oil;
		case 17: return Lava;
		case 18: return Mercury;
		case 19: return Nitro;
		case 20: return Smoke;
		case 21: return Steam;
		case 22: return Cloud;
		case 23: return NatGas;
		case 24: return Helium;
		case 25: return Bricks;
		case 26: return Metal;
		case 27: return Gold;
		case 28: return Wood;
		case 29: return Ice;
		case 30: return C4;
		case 31: return Detonator;
		case 32: return Lightning;
		default: return Error;
		}
	}
};

inline bool equal(ParticleTypeDetails t1, ParticleTypeDetails t2) {
	return t1.id == t2.id;
}
