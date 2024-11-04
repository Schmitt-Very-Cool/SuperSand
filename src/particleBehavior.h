#pragma once

#include <string>

#include "field.h"


class ParticleBehavior {
public:
	static void update(Field& f);
	//static void updateColors();				//Update Particle Color for heat
	static void doInteractions();			//Chemical Reactions
	static void applyGravity();				//Downward Velocity
	static void doProjectileMovement();		//Falling or Flying things
	static void doPowderMovement();			//Falling into Piles
	static void doLiquidMovement();			//Falling and sliding into settled mass
	static void doGasMovement();			//Random motion within Gas
	//static void doHeatTransfer();			//Heat Conductivity
	//static void doPhaseChanges();			//Freezing, Melting, Boiling, Condensing
	//static void doIgnition();				//Igniting ignitables at Ignition Temperatures
	//static void doCombustion();				//Turning ignited objects into their combusted form
	//static void doLightningCharge();		//Charges potential difference in clouds
	//static void doLightning();				//Strikes lightning, if able
	//static void doElectricity();			//Moves electricity through conductors
	static std::unique_ptr<Field> field;
};