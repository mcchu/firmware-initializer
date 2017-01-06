#ifndef GATE_H
#define GATE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "net.h"

using namespace std;

/* forward declaration */
class Net;

/**
 * Enumeration to indicate gate type
 *  AND=0, OR=1, etc.
 */
enum {AND, OR, NAND, NOR, XOR, NOT};

/**
 * This class models a Gate object
 */
class Gate {
 public:
  /**
   * Constructor
   *
   * @param[in] n Name of the gate
   */
  Gate(string n); 

  /**
   * Constructor
   *
   * @param[in] n Name of the gate
   * @param[in] d Delay of this gate instance
   */
  Gate(string n, int d);

  /**
   * Destructor
   */
  virtual ~Gate();

  /**
   * Returns the name of this Gate
   *
   * @return String representing the name of the gate
   */
  string name() const;

  /**
   * Returns the delay of this gate
   *
   * @return Delay of this gate
   */
  int getDelay() const;

  /**
   * Pure virtual function that should be 
   * overridden for specific gate types
   *
   * Evaluates the value the gate will attempt
   *  to prdoduce based on the input net values
   *
   * @return Value the gate attempts to produce:
   *         '0','1','X'
   */
  virtual char eval() = 0;

  /**
   * Pure virtual function that should be 
   * overridden for specific gate types
   *
   * Evaluates the value the gate will attempt
   *  to prdoduce based on the input net values
   *
   * @param[in] os ostream to dump the Verilog 
   *                description of this gate
   */  
  virtual void dump(ostream &os) = 0;

  /**
   * Adds an input net to this gate
   *
   * @param[in] n Pointer to the input Net
   */
  void addInput(Net *n);

  /**
   * Returns the inputs of this gate
   *
   * @return Returns a new dynamically allocated
   *          list of the input nets
   */
  vector<Net *> *getInputs();

  /**
   * Sets the output net of this gate
   *
   * @param[in] n Pointer to the output Net
   */
  void addOutput(Net *n);

  /**
   * Returns the output Net of this gate
   *
   * @return Pointer to the output net of this Gate
   */
  Net* getOutput() const;

  /**
   * Returns the number of inputs
   *
   * @return Number of inputs for this gate
   */
  int getNumInputs() const;

 protected:
  vector<Net *> *inputs;
  Net *output;
  string inst_name;
  int delay;
};

class And : public Gate {
 public:
  And(string n, int d) : Gate(n, d) { }
  ~And() { /*delete inputs;*/ }
  char eval()
  {  
	char result;
	string s;
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		result = inputs->at(i)->getVal();
		if (result == '0')
		{
			return '0';
		}
		else
		{
			s += result;
		}
	}

	for (string::iterator it = s.begin(); it != s.end() ;++it)
	{
		if( *it == 'X')
			return 'X';
	}
	return '1';
  }
  void dump(ostream &os)
  {
	os << "and #" << delay <<" " << inst_name << "(" << output->name();
	//vector<Net*>*::iterator it;
	//for (it = inputs.begin(); it != inputs.end(); ++it)
	//{
	//	os << "," << (*it)->name)_;
	//}
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }
};

class Or : public Gate {
 public:
  Or(string n, int d) : Gate(n, d) { }
  ~Or() { /*delete inputs;*/ }
  char eval()
  {
	char result;
	string s;
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		result = inputs->at(i)->getVal();
		if (result == 'X')
		{
			return 'X';
		}
		else
		{
			s += result;
		}
	}

	for (string::iterator it = s.begin(); it != s.end() ;++it)
	{
		if( *it == '1')
			return '1';
	}
	return '0';

  }
  void dump(ostream &os)
  {
	os << "or #" << delay <<" " << inst_name << "(" << output->name();
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }
};

class Nor : public Gate {
 public:
  Nor(string n, int d) : Gate(n, d) { }
  ~Nor() { /*delete inputs;*/ }
  char eval()
  {
	string s;
	char result;
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		result = inputs->at(i)->getVal();
		if (result == '1')
		{
			return '0';
		}
		else
		{
			s += result;
		}
	}

	for (string::iterator it = s.begin(); it != s.end(); ++it)
	{
		if( *it == 'X')
			return 'X';
	}
	return '1';
  }
  void dump(ostream &os)
  {
	os << "nor #" << delay <<" " << inst_name << "(" << output->name();
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }
};

class Nand : public Gate {
 public:
  Nand(string n, int d) : Gate(n, d) { }
  ~Nand() { /*delete inputs;*/ }
  char eval()
  {
	string s;
	char result;
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		result = inputs->at(i)->getVal();
		if (result == '0')
		{
			return '1';
		}
		else
		{
			s += result;
		}
	}
	for (string::iterator it = s.begin(); it != s.end(); ++it)
	{
		if( *it == 'X')
			return 'X';
	}
	return '0';
  }
  void dump(ostream &os)
  {
	os << "nand #" << delay <<" " << inst_name << "(" << output->name();
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }
};

class Xor : public Gate {
 public:
  Xor(string n, int d) : Gate(n, d) { }
  ~Xor() { /*delete inputs;*/ }
  char eval()
  {	
	string s;
	char result;
	int compare = 0;
	for (unsigned int i = 0; i < inputs->size(); i++)
	{
		result = inputs->at(i)->getVal();
		if (result == 'X')
		{
			return 'X';
		}
		else
		{
			s += result;
		}
	}
	for (string::iterator it = s.begin(); it != s.end() ;++it)
	{
		if( *it == '1')
		{
			compare++;
		}
	}
	if (compare % 2 == 0)
		return '0';
	else
		return '1';
  }
void dump(ostream &os)
  {
	os << "xor #" << delay <<" " << inst_name << "(" << output->name();
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }
};

class Not : public Gate {
 public:
  Not(string n, int d) : Gate(n, d) { }
  ~Not(){ /*delete inputs;*/ }
  char eval()
  {
	if((inputs->front()->getVal()) == '0')
		return '1';
	else if ((inputs->front()->getVal()) == '1')
		return '0';
	else
		return 'X';
  }
void dump(ostream &os)
  {
	os << "not #" << delay <<" " << inst_name << "(" << output->name();
	for(unsigned int i = 0; i < inputs->size(); i++)
	{
		//os << ", " << (*inputs)->name();
		os << ", " << (inputs->at(i))->name();
	}
	os << ");" << endl;
  }

};

#endif
