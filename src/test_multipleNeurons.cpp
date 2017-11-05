#include "network.hpp"
#include "neuron.hpp"
#include <iostream>

using namespace std;

int main()
{
	double stopTime;
	unsigned int NbNeurons;

	
	cout << "Entrez le nombre de neurones: "; 
	cin >> NbNeurons;
	assert(NbNeurons>0);
	
	cout << "Entrez la durée de la simulation: ";
	cin >> stopTime;
	assert(stopTime>0);
	
	vector<Neuron*> neurons;
	
	for (unsigned int i(0); i < NbNeurons; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}

	Network network(stopTime, neurons);

	network.update();		
			
	return 0;
}


