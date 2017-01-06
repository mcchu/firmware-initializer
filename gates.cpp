#include "gates.h"
#include <vector>
#include <string>
#include "net.h"

using namespace std;

  Gate::Gate(string n) : inst_name(n)
  {
	//vector<Net*>* net_inputs = new vector<Net*>;
	//inputs = net_inputs;
    inputs = new vector<Net*>;
  }

  Gate::Gate(string n, int d) : inst_name(n), delay(d)
  {
	//vector<Net*>* net_inputs = new vector<Net*>;
	//inputs = net_inputs;
    inputs = new vector<Net*>;
  }

  Gate::~Gate()
  {
    delete inputs;
  } 

  string Gate::name() const
  {
	return inst_name;
  }
 
  // Returns the delay of this gate
  // @return Delay of this gate
  int Gate::getDelay() const
  {
	return delay;
  }


   // Adds an input net to this gate
   // @param[in] n Pointer to the input Net
  void Gate::addInput(Net *n)
  {
	inputs->push_back(n);

  }

   // Returns the inputs of this gate
   // @return Returns a new dynamically allocated list of the input nets
  vector<Net *> * Gate::getInputs()
  {
	vector<Net*> *_gates = new vector<Net*>;
	
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		_gates->push_back(inputs->at(i));	
	}	

	return _gates;

  }
  // Sets the output net of this gate
  // @param[in] n Pointer to the output Net
  void Gate::addOutput(Net *n)
  {
	output = n;
  }

  // Returns the output Net of this gate
  // @return Pointer to the output net of this Gate
  Net* Gate::getOutput() const
  {
	return output;
  }

  // Returns the number of inputs
  // @return Number of inputs for this gate
  int Gate::getNumInputs() const
  {
	//getInputs() returns a vector*
	return inputs->size();
  }

