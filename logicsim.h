#ifndef LOGICSIM_H
#define LOGICSIM_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "gates.h"
#include "net.h"
#include "design.h"

using namespace std;

class LogicSim 
{
 public:
 	// constructor
 	LogicSim(char *s) : num_pis(0), tot_pos(0), prev(0)
 	{
 		sim.open(s);
 	}

 	~LogicSim()
 	{
 		sim.close();
 	}

 	// parse each line in vector 
 	void parse(Design* d)
 	{	
 		bool iflag = true;
 		while( getline(sim,line) )
 		{	
 			if(iflag)
 			{
 				stringstream ss(line);

 				string temp;
 				while(ss >> temp)
 				{	
 					pi_name.push_back(temp);
 					num_pis++;
 				}

 				// check 1: A specified PI on the first line does not exist in the design
 				vector<string>::iterator it;
 				for( it = pi_name.begin(); it != pi_name.end(); ++it )
				{
					if( d->findNet(*it) == NULL )
					{
						cout << "Error: primary input names do not match" << endl;
						return;
					}
				}
				
 				iflag = false;
 			}
 			
 			else {
 				stringstream ss(line);
 				int check = 0;

 				vector<char> v; // temp vector
 				char t;
 				while(ss >> t)
 				{
 					// check 3: An input value is not in the set {‘0’, ‘1’, ‘X’}
 					if( t == '1' || t == '0' || t == 'X' )
 					{
 						v.push_back(t);
 						check++;
 					}
 					else {
 						cout << "Error: incorrect input values" << endl;
 						return;
 					} 

 				}

 				// check 2: The number of input values for a vector does not match the number of PI’s specified on the first line
 				if(check != num_pis)
 				{
 					cout << "Error: number of primary inputs do not match" << endl;
 					return;
 				}

 				// insert mappy's into vector inCombos
 				map<string,char> mappy;
 				for( int i=0; i < num_pis; i++)
 				{
 					mappy.insert( make_pair(pi_name[i],v[i]) );
 				}
 					
 				inCombos.push_back(mappy); 
 			}


 		} 
 		//line.erase( remove(line.begin(),line.end(),' '), line.end() )
 	}


 	// computing output vals and delays
	void gate_network( Design *d, map<string,char> m ) 
	{ 
		//Net.getVal() & Net.getDelay()
		vector<Net*> *top = d->toposort();
		vector<Net*>::iterator it;
		for (it = top->begin(); it != top->end(); ++it)
		{
			if( d->isPI(*it) ) // initialize PI's
			{
				map<string,char>::iterator mt;
				for(mt = m.begin(); mt != m.end(); ++mt)
				{
					if( (*it)->name() == mt->first )
					{
						(*it)->setVal(mt->second);	
					}
					
				} 
			}

			else 
			{
				int i = (*it)->computeDelay();
				char c = (*it)->computeVal();
				(*it)->setDelay(i);
				(*it)->setVal(c);

				if( d->isPO(*it) ) // add to ouput
				{
					// save output val and delay to vector
					output_vals.push_back((*it)->getVal());	//gets the TOP output and puts it in output_vals
					output_delay.push_back((*it)->getDelay()); //ets the TOP delay and puts it in output_delay
					
					tot_pos++;
				}
			}
		}

		delete top;
	}
	
	// dumps to output file
	void dump(ostream &os, Design *d) 
	{
		for(unsigned int i = 0; i < pi_name.size(); i++)
		{
			os << pi_name[i] << " ";
		}
		os << "=> "; 
		
		vector<Net *> *po_name = d->getPONets();  //need to add PONets
		vector<Net*>::iterator p;
		
		for(p = po_name->begin(); p != po_name->end(); ++p)
		{
			os << setw(5) << left << (*p)->name();
		}
		os << endl;
		vector< map<string,char> >::iterator it;
		//FOR loop for each map
		for(it = inCombos.begin(); it != inCombos.end(); ++it)
		{
			map<string,char>::iterator jt; 
			//FOR loop for EACH input ie: 0
 			for(jt = it->begin(); jt != it->end(); ++jt)
			{
				os << jt->second << " ";	
			}
			os << "=> ";
			gate_network(d,*it);

			for(int k = prev; k < tot_pos; k++)
			{
				os << output_vals[k] << "@" << output_delay[k] << " ";
				prev++;
			}	
			os << endl;
		}
		delete po_name;
	
	}

 private:
 	ifstream sim; // input sim file
 	string line; // input line
 	int num_pis; // number of inputs
 	vector<string> pi_name; // vector of input net
 	vector< map<string,char> > inCombos; // vector of input combos 
	vector<char> output_vals;
	vector<int> output_delay;
	int tot_pos; // total num of primary outputs
	int prev; 


};

#endif
