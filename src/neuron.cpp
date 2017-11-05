#include "neuron.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Neuron::Neuron(double stopTime, double iext, double potential)
: stopTime_(stopTime), iext_(iext), potential_(potential)
{
	//on initialise pour le moment le start time à 0 par défaut (à voir pour la suite)
//	startTime_ = 0.0;
//	isRefractory_ = false;
	hasSpike_ = false;
	isInhibiter_ = false;
	refractoryTime_ = 0;
//	spikesReceived_ = 0;
	nbSpikes_ = 0;

	//la taille du buffer initial est 15 (soit 16 cases).
	//de ce fait, il y aura un délai de 15 cases pour qu'un spike s'y inscrive, soit 1.5 ms
	buffer_.resize(DelayStep+1);
}	
Neuron::~Neuron()
{}
//======================================================================
//Getters
double Neuron::getPotential() const
{
	return potential_;
}
//----------------------------------------------------------------------
unsigned int Neuron::getNbSpikes() const
{
	return nbSpikes_;
}
//----------------------------------------------------------------------
int Neuron::getRefractoryTime() const
{
	return refractoryTime_;
}
//----------------------------------------------------------------------
bool Neuron::hasSpike() const
{
	return hasSpike_;
}
//----------------------------------------------------------------------
bool Neuron::isInhibiter() const
{
	return isInhibiter_;
}
//======================================================================
//Setters
void Neuron::setPotential(double potential)
{
	potential_ = potential;
}
//----------------------------------------------------------------------
void Neuron::setIext(double Iext)
{
	iext_ = Iext;
}
//----------------------------------------------------------------------
void Neuron::setIsInhibiter(bool type)
{
	isInhibiter_ = type;
}
//======================================================================
//Membrane equation : temporal evolution of the membrane potential
double Neuron::membraneEq(size_t readBox) //
{
	assert(!buffer_.empty());
	return e*potential_ + iext_*Resistance*OneMinus_e + buffer_[readBox]*Amplitude;
}
//======================================================================
// Gestion du buffer
void Neuron::fillBuffer(double spike, size_t writeBox)
{
	buffer_[writeBox] += spike;
}
//======================================================================
//update du potentiel
void Neuron::update(size_t readBox)
{	
	hasSpike_ = false;
	
	// if the refractory time is greater than the refractory period	
	if (refractoryTime_ > 0) { 
		// We reset the potential to 0.0 because the neuron is in a refractory state and cannot receive  any spikes.
		potential_ = PotentialReset;
		
		// refractoryTime decrementation
		--refractoryTime_;
	
	//if the potential is greater than threshold
	} else if (potential_ > Threshold) {
		
		//The neuron spikes
		hasSpike_ = true;
		++nbSpikes_;
		
//		potential_ = PotentialReset;
		
		// = tauRp/dt,the refractory time is expressed in steptime
		refractoryTime_ = RefractoryStep; 
	
	//if the potential is smaller thant  the Threshold
	} else {
		
		//we calculate the new accurate value of the potential 
		potential_ = membraneEq(readBox);
	}
}
//======================================================================
