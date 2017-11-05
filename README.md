# Neuron project

Margaux Roulet
 
sciper: 274274 

BA3 SV EPFL

CS116-2017

##IMPLEMENT A NEURAL NETWORK SIMULATION FROM A PAPER

### CMAKE:
To cmake your program place yourself in the directory build and tap the next command lines:
		
		cmake ..
		make

From the build directory, you can run the main program with the command line: 

		./neuron
		
From the build directory, you can run the unit tests with the command line: 

		./neuron_unittest

### NEURON:


#### Main Simulation program: test_multipleNeurons.cpp:
	This program generates figure 8 from Brunel's Document.
	The number of neurons should be at least 50 neurons for it to be relevant. 
	To generate one of the four graphs, adapt the constant variables g and Eta in the neuron.hpp.
	
			Graph A: g = 3.0 // Eta = 2.0 
			Graph B: g = 6.0 // Eta = 4.0 
			Graph C: g = 5.0 // Eta = 2.0
			Graph D: g = 4.5 // Eta = 0.9	
	
	Run the program from the build directory.
	Choose the number of neurons of your network.
	Choose the duration of simulation.
	(To generate the graphs, See section Generate graph with matplotlib or gnuplot.)
	

### UNIT TESTS:

#### Tests with one neuron:

Test 1: Test the membrane potential value of one neuron

Test 2: Test the membrane equation with a positive input current

Test 3: Test the membrane equation with an negative input current

Test 4: Test the simulation of one neuron during 400 ms


#### Test with two neurons:

Test 1: Test the spikes Time arrival of one neuron 

Test 2: Test the transmission of spikes from an inhibitory source neuron to a neuron.

Test 3: Test the tranmission of spikes from a excitatory source neuron to a neuron.


#### Test on the network:

Test 1: Test the type definition and right number of the neurons: either inhibitory or excitatory 

Test 2: Test the right connections of the neurons within the network.


### OPEN DOXYGEN DOCUMENTATION
From the build directory, type the next command line:

		xdg-open ../doxygen/html/index.html

### GENERATE GRAPH WITH MATPLOTLIB:
We reproduce figure 8 from Brunel's document. 
Note that the python script has been implemented according to the slides. figure page 6 of week 7.

Hence, axe x = Time [ms] // axe y = Rate [Hz]

The first subplot displays the spike's time according to the neuron index.
The second subplot displays the rate of spikes.

To produce correct result, you should run the program with the next features:
	
		Number of neurons: 12500
		Duration of simulation: 300

Run the program. To generate the graph, type the next command line, from the build directory:
		
		python "../graphs/script.py"

### GENERATE GRAPH WITH GNUPLOT:
Reproduce the histogram according to figure 8 from Brunel's document.
The plot displays the total number of spikes at each dt.
You can adapt the x and y axes range according to which graph (A,B,C,D) you display.

Run the program. To generate the graph, type the next command line, from the build directory:

		gnuplot
		set style data lines
		set xlabel "Time [ms]"
		set ylabel "Number of spikes"
		set xrange [1000:1200]
		set yrange [0:200]
		plot "../res/spikes2.txt" using 1:2 with boxes

Note: adapt yrange for graph A.
		
### SOURCE:
BRUNEL Nicolas, Dynamics of Sparsely Connected Networks of Excitatory and Inhibitory Spiking Neurons, 1999

### NOTE ON GITHUB:
You'll find on github 2 repositories:

In cppcourse-brunel:
You'll find the final version of the program.

In CS116-2017-Margaux-Roulet:
You'll find two versions of network.xx neuron.xx test_oneNeuron.cpp test_twoNeurons.cpp. 
This is due to a change in my hierarchy during the project. The files within the folder src are the final one. 
But as I wanted to keep the old files to still have an history of my commits, I kept the older version.
