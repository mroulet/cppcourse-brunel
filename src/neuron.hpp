#ifndef neuron_H
#define neuron_H
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <fstream>
#include <list>
#include <math.h>

///Constants

const double g = 5.0;							//!< Relative strength of inhibitory synapses
const double Eta = 2.0;							//!< Eta function

const double Threshold = 20.0; 					//!< Maximum potential limit [mV]
const double PotentialReset = 0.0; 				//!< Potential reset  [mV]
const double dt = 0.1; 							//!< Time variation [ms]
const unsigned int h = 1; 						//!< Steptime (will be converted en ms)
const double Resistance = 20.0; 				//!< Membrane Resistance [Ohm]
const double Capacity = 1.0; 					//!< Capacity [Farrad]
const double tau = Resistance * Capacity; 		//!< Tau excitatory
const double e = exp(-dt/tau);					//!< needed for the membrane equation
const double OneMinus_e = 1 - e;				//!< needed for the membrane equation 
const double tauRp = 2.0; 						//!< Refractory time period [ms]
const double Amplitude = 0.1; 					//!< Spike Amplitude received from excitatory neurons
const double Delay = 1.5; 						//!< Transmission delay [ms]
const double ConnectionPercent = 0.1;			//!< Connections pourcentage in the whole network
const double Vext = Threshold * Eta /(Amplitude*tau); //!< External frequency
const unsigned int DelayStep = static_cast<unsigned long>(floor(Delay/dt)); //!< Delay in steps
const unsigned int RefractoryStep = static_cast<unsigned long>(floor(tauRp/dt)); //!< Refractory period in steps

/*! 
 * @class Neuron
 * 
 * @brief Managing the neuron simulation.
 * 
 * Temporal evolution of the neuron: its potential, state and number of spikes.
 * Each steptime the membrane potential is updated and the spike condition checked.
 * Each neuron receives activity (spikes) from its local network (excitatory and inhibitory)
 * and from the rest of the brain (only excitatory).
 */
class Neuron {
	
public:
	/**
     * @brief Constructor
     *
     * @param stopTime of the simuation of the neuron
     * @param iext is the external current. Default value = 0.0
     * @param potential is the membran potential. Its default value = 0.0
     * 
     * @note Initialisation of the time buffer necessary for the right implemtation of the transmission delay
     */
	Neuron(double stopTime, double iext = 0.0, double potential = 0.0);
	
	/**
	 * @brief Destructor
	 */
	~Neuron();
	
	/**
	 * @brief Get the potential.
	 * 
	 * @return current value of membrane potential.
	 */
	double getPotential() const;
	
	/**
	 * @brief Get the number of spikes.
	 * 
	 * @return current value of number of spike that occured.
	 */
	unsigned int getNbSpikes() const;
	
	/**
	 * @brief Get the refractory time.
	 * 
	 * @return current value of refractory time.
	 */
	int getRefractoryTime() const;
	
	/**
	 * @brief Get the potential state.
	 * 
	 * @return one neuron has a spike if its membrane potential is greater than threshold.
	 */	
	bool hasSpike() const;
	
	/**
	 * @brief Get the type of the neuron
	 * 
	 * @return is inhibiter or is excitatory
	 */
	bool isInhibiter() const;
	
	/**
	 * @brief Set potential.
	 * 
	 * Set current value of membrane potential.
	 */
	void setPotential(double potential);
	
	/**
	 * @brief Set the neuron type.
	 * 
	 * Whether the neuron is Excitatory or Inhibitory.
	 */
	void setIsInhibiter(bool type);
	
	/**
	 * @brief Set the external current.
	 * 
	 * Set current value of the external current.
	 */
	void setIext(double iext);
	
	/**
	 * @brief Calculate the membrane potential of the Neuron.
	 * 
	 * General solution of the Brunel linear differential equation.
	 * Approximated for a constant current input Iext and a constant amplitude J.
	 * 
	 * @param readBox is the index of the buffer in which the spikes are read in the membrane equation
	 * 
	 * @return the updated value of potential
	 */
	double membraneEq(size_t readBox);
	
	/**
	 * @brief Run the simulation of one neuron.
	 * 
	 * This function is the main loop of the class Neuron
	 * 
	 * @param readBox is the index of the buffer in which the spikes are read in the membrane equation
	 * 
	 * For a time simulation given:
	 * if the potential is greater than the threshold we set the neuron refractory
	 * if the neuron is refractory the potential is reset
	 * otherwise solve the membrane equation to update the potential
	 */
	void update(size_t readBox);
	
	/**
	 * @brief Record the spikes received by a connected neuron into the time buffer at time t + Delay.
	 * 	
	 * If the potential is greater than the threshold, 
	 * a spike is signal at time t to the post synaptic neuron. 
	 * The post synaptic neuron responds at t + Delay
	 * 
	 * @param writeBox is the index in which the spikes are recorded It equals now+Delay
	 * @param spike is the value transmitted by the neuron varies according to the source neuron which can be inhibitory, excitatory or external.
	 * 
	 * @note The buffer size is equal to the number of steptime needed to accomplish the time transmission delay +1.
	 * 
	 */
	void fillBuffer(double spike, size_t writeBox);

	/**
	 * @brief Buffer of size delay + 1
	 * 
	 * In network there are two index readBox and WriteBox that define where to read and wrtie spikes transmitted by other neurons.
	 * Hence, if I received a spike at time t, it will be read in the buffer case t+ Delay.
	 * If a neuron transmit a spike at time t, I write it in the buffer case t + Delay.
	 */
	std::vector<int> buffer_; //!< Time buffer containing each spike received at each dt (public for optimisation purpose)
	

private:
	
	double stopTime_; //!< StopTime of the neuron simulation (needed for gtest).	
	
	int refractoryTime_; //!< Refractory timer: is set to 20 time step and will be decremented when the neuron is refractory.
	
	double iext_; //!< External Current
				
	double potential_; //!< Membrane potential

	unsigned int nbSpikes_; //!< Number of spikes
	
	bool hasSpike_; //!< Neuron spike state
	
	bool isInhibiter_; //!< Neuron type: return true if is inhibiter return false if is excitatory
	
	unsigned int time_; //!< clock_ of the neuron simulation. Only needed for the cout in the gtest
};


#endif
