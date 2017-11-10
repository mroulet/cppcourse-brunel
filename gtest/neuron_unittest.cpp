#include <iostream>
#include "../src/neuron.hpp"
#include "../src/network.hpp"
#include "gtest/gtest.h"

TEST (NeuronTest1, MembranePotential) {
	
	Neuron neuron(1,1.0);

	//First update test
	neuron.update(0);
	EXPECT_EQ(20.0*(1.0-std::exp(-0.1/20.0)), neuron.getPotential());
}

TEST (NeuronTest2, PositiveInput) {
	Neuron neuron(1);
	neuron.setIext(1.0);
	
	//First update test
	neuron.update(0);
	EXPECT_EQ(20.0*(1.0-std::exp(-0.1/20.0)), neuron.getPotential());
	
	//Test after numerous update
	unsigned int i(0);
	while (i < 10000) {
		neuron.update(0);
		++i;
	}
	
	
	EXPECT_EQ(0,neuron.getNbSpikes());
	EXPECT_GT(1E-3, std::fabs(19.999 - neuron.getPotential()));
	
	neuron.setIext(0.0);
	
	unsigned int j(0);
	while (j < 2000) {
		neuron.update(0);
		++j;
	}
	
	EXPECT_NEAR(0, neuron.getPotential(), 1e-3);	
	
}

TEST (NeuronTest3, NegativeInput) {
	Neuron neuron(1);
	neuron.setIext(-1.0);
	
	//First update test
	neuron.update(0);
	EXPECT_EQ(-20.0*1.0*(1-std::exp(-0.1/20.0)), neuron.getPotential());
	
	//Test after numerous update
	unsigned int i(0);
	while (i < 1000) {
		neuron.update(0);
		++i;
	}
	
	EXPECT_EQ(0,neuron.getNbSpikes());
	EXPECT_GT(-20.0*1.0*(1-std::exp(-0.1/20.0)), neuron.getPotential());
	
	neuron.setIext(0.0);
	
	unsigned int j(0);
	while (j < 2000) {
		neuron.update(0);
		++j;
	}
	
	EXPECT_NEAR(0, neuron.getPotential(), 1e-3);	
	
}

TEST (NeuronTest5, oneNeuronSimulationSpikes) {
	Neuron neuron(1, 1.01);
	//We know that spikes times arrive at 92.4ms 186.8ms 281.2ms 376.6;
	//you can verify this with decommenting line 100 in neuron.cpp
	
	unsigned int j(0);
	while (j < 4000) {
		neuron.update(0);
		++j;
	}
	
	EXPECT_EQ(4, neuron.getNbSpikes());
}

TEST (TwoNeuronsTest1, SpikeArrivalTime) {	
					
	Neuron neuron1(1,1.01);
	Neuron neuron2(1);
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay;
	
	Network network(stopTime, neurons);
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update(network.getReadBox());
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(0.1, neuron2.getPotential());

}

TEST (TwoNeuronsTest2, InhibitatoryConnection_Delay) {
	Neuron neuron1(1,1.01);
	neuron1.setIsInhibiter(true);
	Neuron neuron2(1);
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay;
	
	Network network(stopTime, neurons);
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update(network.getReadBox());
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(-g*0.1, neuron2.getPotential());

}

TEST (TwoNeuronsTest3, ExcitatoryConnection_Delay) {
	Neuron neuron1(1,1.01);
	Neuron neuron2(1);
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay;
	
	Network network(stopTime, neurons);
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update(network.getReadBox());
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(0.1, neuron2.getPotential());

}

TEST (NetworkTest1, typeDefinition) {
	
	std::vector<Neuron*> neurons;
	
	// The network is composed of 100 neurons
	for (unsigned int i(0); i < 100; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	Network network(1, neurons);
	unsigned int nbInhibitory(0);
	unsigned int nbExcitatory(0);

	for (auto neuron : network.getNeurons()) {

		if (neuron->isInhibiter()) {
			++nbInhibitory;
		} else { ++nbExcitatory; }
	}
	
	EXPECT_EQ(20, nbInhibitory);
	EXPECT_EQ(80, nbExcitatory);
}

TEST (NetworkTest2, connections) {
	
	std::vector<Neuron*> neurons;
	
	// The network is composed of 100 neurons
	for (unsigned int i(0); i < 1000; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	
	Network network(1, neurons);
	
	EXPECT_EQ(20, network.getNbInhibitoryConnections());
	EXPECT_EQ(80, network.getNbExcitatoryConnections());
}	

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
