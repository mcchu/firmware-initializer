#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "net.h"
#include "gates.h"
using namespace std;

  //Constructor
  Net::Net(string n) : netname(n)
  {
	//vector<Gate*>* gate_vector = new vector<Gate*>;
	//drivers = gate_vector;
    drivers = new vector<Gate*>;

	//vector<Gate*>* loads_vector = new vector<Gate*>;
	//loads = loads_vector;
    loads = new vector<Gate*>;
  }

  //Destructor
  Net::~Net()
  {
	delete drivers;
	delete loads;
  }

  // Returns the name of this Net
  //return String representing the name of the net
  string Net::name() const
  {
	return netname;
  }

  //Adds the given gate to the list of drivers 
  //for this net @param[in] g Driver gate
  void Net::addDriver(Gate *g)
  {  
	drivers->push_back(g);
  }

  //Adds the given gate to the list of loads 
  //for this net @param[in] g Load gate
  void Net::addLoad(Gate *g)
  {
	loads->push_back(g);
  }

  // Returns the loads of this net
  // @return Returns a new dynamically allocated list of the load gates
  vector<Gate *>* Net::getLoads()
  {
	vector<Gate*> *load_gates = new vector<Gate*>;
	
	for (unsigned int i = 0; i < loads->size(); i++)
	{
		load_gates->push_back(loads->at(i));
	}	

	return load_gates;
  }

  //Returns the drivers of this net
  // @return Returns a new dynamically allocated list of the driver gates
  vector<Gate *>* Net::getDrivers()
  {
	vector<Gate*>* _drive = new vector<Gate*>;
	for (unsigned int i = 0; i < drivers->size(); i++)
	{
		_drive->push_back(drivers->at(i));
	}

	return _drive;
  }

  //Looks at the driver gates and input nets to those gates to compute and return the value on this net 
  //@return Value produced on this net: '0','1','X'
  char Net::computeVal() 
  {
    stringstream ss;
    string s;
   // int c = 0; 
    vector<Gate*>::iterator it;
    for (it = drivers->begin(); it != drivers->end(); ++it)
    {
        ss << (*it)->eval();
        s += ss.str(); // add singular char each time

      //  if( c>1 && (s[c-1] != s[c]) )
       // {
      //      setVal('X');
      //      return 'X';
       // } 
       // c++;
    }
    
    unsigned int total = 0;
    for(unsigned int i = 0; i < s.size(); i++)
    {
        ss << s[i];
        int t;
        ss >> t;
	//cout << "value of s[" << i << "]" << t << " ";
	if (t == 88)	{ setVal('X'); return 'X';}
        total += t;
	
    }


    if( total == 0 )
    {
        setVal('0');
        return '0';  
    } 
    else if( total > 0 )
    {
        setVal('1');
        return '1';  
    } 
    else {
        setVal('X');
        return 'X';   
    }

	/*if (val == '0')
		return '0';
	else if (val == '1') 
		return '1';
	else
		return 'X';*/
  }

  //Looks at the driver gates and input nets to those gates to compute and return the time when this net's value will be resolved
  // @return Time when this net's value is resolved
  int Net::computeDelay() //<------------PA6
  {
    int d_max = 0;  
    vector<Gate*>::iterator it;
    for (it = drivers->begin(); it != drivers->end(); ++it)
    {   
        int G = (*it)->getDelay(); // single driver gate delay

        vector<Net *> *gi = (*it)->getInputs();
        vector<Net*>::iterator g;
        int m = 0; // temp max 

        for (g = gi->begin(); g != gi->end(); ++g)
        {
            m = max( m, (*g)->getDelay() ); // max of input delays
        }    
        
        m += G;
        if(m > d_max) d_max = m; // d_max = max(m,d_max);
        delete gi;
    }
	
	return d_max;
  }

  /**
   * Debug function you can use to verify 
   * your code. Prints the drivers and loads.
   *
   */
  void Net::printDriversLoads()
  {
	cout << "Loads: ";
	for (unsigned int i = 0; i < loads->size() ; i++)
	{
		cout << loads->at(i)->name() << " ";
	}
	cout << "Drivers: ";
	for (unsigned int i = 0; i < drivers->size() ; i++)
	{
		cout << drivers->at(i)->name() << " ";
	}
	cout << "Delay: " << getDelay();
	cout << "Value: " << getVal();
  }



