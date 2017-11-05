#include "network.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Network::Network(double networkStopTime, vector<Neuron*> neurons)
: networkStopTime_(networkStopTime), neurons_(neurons)
{	
	index_.resize(getNbNeurons());
	
	nbSpikesTotal_ = 0;
	writeBox_ = 15;
	readBox_ = 0;
	
	// Random engine for both poisson and uniform
	std::random_device device;
	mt19937 generator(device());
	distributionPoisson = poisson_distribution<unsigned int>(dt*Vext);
	
	//Neuron type definition
	if(getNbNeurons() >= 50) {
		defineTypeNeuron();
	}
	
	//Connection
	connect();
	
	//File opening
	spikesFile_ = new ofstream ("../res/spikes2.txt");
	if (spikesFile_->fail()) {
			cerr << "Error opening file " << endl;
	} else { *spikesFile_ << "dt,spikes" << endl; 
	}
	
	spikesIndexFile_ = new ofstream ("../res/spikes.gdf");
	if (spikesIndexFile_->fail()) {
			cerr << "Error opening file " << endl;
	}
}
//----------------------------------------------------------------------
Network::~Network()
{
	//File closing
	spikesFile_->close();
	spikesIndexFile_->close();
}
//======================================================================
//getter/setter
vector<Neuron*> Network::getNeurons() const
{
	return neurons_;
}
//----------------------------------------------------------------------
unsigned int Network::getNbNeurons() const
{
	return neurons_.size();
}
/*vector<vector< unsigned int> > Network::getIndex() const
{
	return index_;
}*/
//----------------------------------------------------------------------
// Initialisation of the connected neurons list for each neuron of the network
// We create Ce = Ne*0.1 excitatory connections and Ci=Ni*0.1 inhibitory connections
// Ni / Ne = 0.25 according to Brunel's model
unsigned int Network::getNbExcitatoryConnections()
{
	double n = getNbNeurons()*ConnectionPercent*0.8;
	unsigned int connections = static_cast<unsigned long>(n);
	return connections;
}
//----------------------------------------------------------------------
unsigned int Network::getNbInhibitoryConnections()
{
	double n = getNbNeurons()*ConnectionPercent*0.2;
	unsigned int connections = static_cast<unsigned long>(n);
	return connections;
}
//----------------------------------------------------------------------
unsigned int Network::getNbExternalConnections()
{
	return getNbExcitatoryConnections();
}
//----------------------------------------------------------------------
/*double Network::getFrequencyThr() //obsolète
{
	return Threshold / (getNbExcitatoryConnections() * Amplitude * tau);
}
//----------------------------------------------------------------------
double Network::getExternalFrequency() // obsolète
{	// cest/vtzhr * ce * vthr -> la multiplication des ce se fait dans poisson
	return getNbExcitatoryConnections()*Eta*getFrequencyThr();
}
//----------------------------------------------------------------------
void Network::setNetworkStopTime(double t)
{
	networkStopTime_ = t;
}*/
//----------------------------------------------------------------------
void Network::updateBufferIndex()
{				
	if (readBox_+1 > 15) {
		readBox_ = 0;
	
	} else { ++readBox_;
	}

	if (writeBox_+1 > 15) {
		writeBox_ = 0;
	
	} else { ++writeBox_; 
	}
}
//----------------------------------------------------------------------
unsigned int Network::getReadBox() const //necessary for gtest
{
	return readBox_;
}
//======================================================================
//determine si le neurone est excitatory or inhibitory
void Network::defineTypeNeuron()
{
	//note: Ni = 0.2 N , Ne = 0.8N
	unsigned int n = getNbNeurons()/5;

	for (unsigned int i(0); i < n; ++i) {
			neurons_[i]->setIsInhibiter(true);
	}
}
//======================================================================
//méthode connect
void Network::connect()
{	
	//First method connect for test_twoneurons and google test
	if (getNbNeurons() < 50) {
		
		for (unsigned int i(0); i < getNbNeurons(); ++i) {
			for (unsigned int j(0); j < getNbNeurons(); ++j) {
				if (i != j) {
					index_[i].push_back(j);
				}
			}
		}

	} else {

		unsigned int nbInhibitory = getNbNeurons()/5;
		unsigned int nbExcitatory = getNbNeurons() - nbInhibitory;
			
		for (unsigned int i(0); i < getNbNeurons(); ++i) {
			for (unsigned int k(0); k < getNbInhibitoryConnections(); ++k) {
				index_[uniform(nbInhibitory)].push_back(i);
			}
			for (unsigned int j(0); j < getNbExcitatoryConnections(); ++j) {
				index_[uniform(nbExcitatory) + nbInhibitory].push_back(i);
			}
		}
		
	}
}
//======================================================================
//Poisson distribution of randomly external spike
unsigned int Network::poisson()
{
	return distributionPoisson(generator);
}
//----------------------------------------------------------------------
//To generate the connection we need uniformly distributed random numbers
unsigned int Network::uniform(unsigned int size)
{
	uniform_int_distribution<unsigned int> distributionUniform(0,size-1);
	return distributionUniform(generator);
}
//======================================================================
void Network::writeSpikeToFile() // for gnuplot
{	
	*spikesFile_ << clock_*dt << " " << nbSpikesTotal_ << endl;
}
//======================================================================
//update du network
void Network::update()
{
	// Conversion of the netork stop time (ms) in time step
	unsigned int networkStopTime = static_cast<unsigned long>(floor(networkStopTime_/dt));
	
	while(clock_ < networkStopTime)	{
		
		//for each neuron of the network
		for (unsigned int i(0); i < index_.size(); ++i) {
			
			//update the potential and state of the neuron
			neurons_[i]->update(readBox_);			
			
			// Condition made to preserve the first gtests that do not take account of random spikes		
			if (getNbNeurons() >= 50) {
			
				//randomly distributed external spike from outside network
				unsigned int backgroundNoise(poisson());
				
				// If the poisson process activates the external synapses
				// Then for each external synapses activated (random) a spikes is distributed to the neuron
				neurons_[i]->fillBuffer(backgroundNoise, writeBox_);
			}		


			// If the source neuron spikes, the neuron receives a spikes
			if (neurons_[i]->hasSpike()) {	
						
				// record of spikes in the Jupyter file: If the neuron has spiked during this dt, 
				//the spike and the index of the neuron is recorded in a file
				++nbSpikesTotal_;
				*spikesIndexFile_ << clock_ << "\t" << i+1 << endl;
	
				for (unsigned int j(0); j < index_[i].size(); ++j) {
	
					// If the source neuron is inhibitatory, the neuron receives a negative spike
					if (neurons_[i]->isInhibiter()) {

						neurons_[index_[i][j]]->fillBuffer(-g, writeBox_);
					
					//if the source neuron is excitatory, the neuron receives a positive spike
					} else {
						
						neurons_[index_[i][j]]->fillBuffer(1, writeBox_);
					}
				}			
			}
			
			// Emptying of buffer index just read
			neurons_[i]->buffer_[readBox_] = 0.0;
		}
		
		// update of the buffer indexes
		updateBufferIndex();
	
	// record of total number of spikes per dt in the Gnuplot file
	writeSpikeToFile();
	nbSpikesTotal_ = 0;
		
	//step time incrementation
	clock_ += h;	
		
	}
}
//======================================================================
