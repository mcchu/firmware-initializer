#include <ostream>
#include <vector>
#include <map>
#include <string>
#include "gates.h"
#include "net.h"
#include "design.h"

using namespace std;


   // Constructor @param[in] n module name of the design
  Design::Design(string n) : desname(n) { }

  //Destructor
  Design::~Design()
  {
  	// dealloc memory from maps
	map<string, Net*>::iterator it;
	for (it = designNets.begin(); it != designNets.end(); ++it)
	{
		delete it->second;
		designNets.erase(it);
	}
	map<string, Gate*>::iterator it1;
	for (it1 = designGates.begin(); it1 != designGates.end(); ++it1)
	{
		delete it1->second;
		designGates.erase(it1);
	}

	// dealloc pointer to vectors <<------???? 

  }

  // Returns the name of the Design 
  // @return module name of the design
  string Design::name()
  {
	return desname;
  }

  //Adds the primary input with the given name to the internal database @param[in] n name of the PI
  void Design::addPI(string n)
  {
	pis.push_back(n);
  }

  //Adds the primary output with the given name to the internal database @param[in] n name of the PI
  void Design::addPO(string n)
  {
	pos.push_back(n); // <<-----valgrind error: at 0x4C2B1C7: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)

  }

  // Returns a pointer to the net with the given name or NULL if that net doesn't exist
  // @param[in] net_name name of the Net to find 
  // @return Pointer to a Net object or NULL
  Net* Design::findNet(string net_name)
  {
	
	if (designNets.find(net_name) != designNets.end())
	{
		return (designNets.find(net_name))->second;
	}	
	return NULL;
  }

  // Returns a pointer to the gate with the given name or NULL if that gate doesn't exist
  // @param[in] inst_name name of the Gate instance to find 
  // @return Pointer to a Gate object or NULL
  Gate* Design::findGate(string inst_name)
  {
	if (designGates.find(inst_name) != designGates.end())
	{
		return (designGates.find(inst_name))->second;
	}	
	return NULL;

  }

  //Checks if a Net with the given name exists already, returning a pointer to it if it does.  
  //It not, it creates a new Net object and returns a pointer to that new object 
  // @param[in] n Name of the gate to find/create
  // @return Pointer to the Net object
  Net* Design::addFindNet(string n)
  {
	//map<string, Net*>::iterator it;
	if (designNets.find(n) == designNets.end())
	{
		Net* net = new Net(n);        // <<--------how to dealloc???
		designNets.insert(make_pair(n, net));
		return net;

	}
	return designNets.find(n)->second;

  }

  // Checks if a gate with the given instance name exists already, returning a pointer to it if it does.  
  // It not, it creates a new Gate object of the given type and name and returns a pointer to that new object
  // @param[in] gtype Integer/Enumeration value of the type of gate to create 
  // @param[in] g Instance name of the gate to find/create
  // @return Pointer to the Gate object
  Gate* Design::addFindGate(int gtype, string g)
 {
	if (designGates.find(g) == designGates.end())	
	{	
		/*type gt;
		string val = gt(gtype);
		gt(gtype)* gate = new (gt(gtype)(g));
		designGates.insert(make_pair(g, gate));
		return gate;*/
		//type gtype = static_cast<type>(gtype);
		if (gtype == AND)
		{				
			And* a = new And(g,1);
			designGates.insert(make_pair(g, a));
			return a;
		}
		if (gtype == OR)
		{
			Or* b = new Or(g,1);
			designGates.insert(make_pair(g, b));
			return b;
		}
		if (gtype == NAND)
		{
			Nand* c = new Nand(g,1);
			designGates.insert(make_pair(g, c));
			return c;
		}
		if (gtype == NOR)
		{
			Nor* d = new Nor(g,1);
			designGates.insert(make_pair(g, d));
			return d;
		}
		if (gtype == XOR)
		{
			Xor* e = new Xor(g,1);
			designGates.insert(make_pair(g, e));
			return e;
		}
		if (gtype == NOT)
		{
			Not* f = new Not(g,1);
			designGates.insert(make_pair(g, f));
			return f;
		}
	}

	return designGates.find(g)->second;

  }

  // Same as addFindGate above but if a gate is created the delay value provided will be used
  // @param[in] gtype Integer/Enumeration value of the type of gate to create @param[in] g Instance name of the gate to find/create
  // @param[in] d Delay value @return Pointer to a Gate object
  Gate* Design::addFindGate(int gtype, string g, int d)
  {
	if (designGates.find(g) == designGates.end())
	{
		/*type gt;
		gtype* gate = new Gate(g, d);
		designGates.insert(make_pair(g, gate));
		return gate;*/
		if (gtype == AND)
		{				
			And* a = new And(g,d);
			designGates.insert(make_pair(g, a));
			return a;
		}
		if (gtype == OR)
		{
			Or* b = new Or(g,d);
			designGates.insert(make_pair(g, b));
			return b;
		}
		if (gtype == NAND)
		{
			Nand* c = new Nand(g,d);
			designGates.insert(make_pair(g, c));
			return c;
		}
		if (gtype == NOR)
		{
			Nor* x = new Nor(g,d);
			designGates.insert(make_pair(g, x));
			return x;
		}
		if (gtype == XOR)
		{
			Xor* e = new Xor(g,d);
			designGates.insert(make_pair(g, e));
			return e;
		}
		if (gtype == NOT)
		{
			Not* f = new Not(g,d);
			designGates.insert(make_pair(g, f));
			return f;
		}


	}
	return designGates.find(g)->second;
  }


  // Dynamically allocates a new vector and adds pointers to all Nets that correspond to primary inputs into the vector and then 
  // returns a pointer to the vector 
  // @return Pointer to a new vector storing Net pointers
  vector<Net *>* Design::getPINets()
  {
	vector<Net *>* PINet = new vector<Net *>;
	for (unsigned int i = 0; i < pis.size(); i++)
	{
		PINet->push_back(addFindNet(pis[i]));	//addFindNet or findNet? not sure about implementation
	}	
	return PINet;
  }
  // Dynamically allocates a new vector and adds pointers to all Nets that correspond to primary
  // outputs into the vector and then returns a pointer to the vector @return Pointer to a new vector storing Net pointers
  vector<Net *> * Design::getPONets()
  {
	vector <Net * >* PONet = new vector < Net*> ;
	for (unsigned int i = 0; i < pos.size(); i++)
	{
		PONet->push_back(addFindNet(pos[i]));
	}
	return PONet;
  }

  // Dynamically allocates a new vector and adds pointers to all Nets that correspond to internal wires into the vector 
  // and then returns a pointer to the vector @return Pointer to a new vector storing Net pointers
  vector<Net *> * Design::getWireNets()
  {
	vector <Net* > * WIRENet = new vector <Net*>;
	map<string, Net *>::iterator it;

 	for(it = designNets.begin(); it != designNets.end(); ++it)
	{
		bool flag = true;
		bool flag1 = true;

		for(unsigned int i = 0; i < pis.size(); i++)
		{
			//cout << "not PIS" <<endl;
			//if(findNet(it->first) == NULL)
			if(it->first == pis[i] && flag && flag1)	//if Net* IS an input...		
			{
				flag = false;
			}	
			//cout << pis[i] << endl;		
		}	

		for(unsigned int i = 0; i < pos.size(); i++)
		{
			//cout << "not POS " << endl;
			//if(findNet(pos[i]) == NULL)
			if(it->first == pos[i] && flag1 && flag)			
			{
				flag1 = false;
			}
			//cout << pos[i] << endl;
		}

		if(flag && flag1)
		{
			//Net* wire_net = new Net(it->first); // <<----
			WIRENet->push_back(it->second);
		}
				
	}

	return WIRENet;
  }

  // Dynamically allocates a new vector and adds pointers to all Nets (PI, PO, internal wire) into the vector and then 
  // returns a pointer to the vector @return Pointer to a new vector storing Net pointers
  vector<Net *> * Design::allNets()
  {
	vector <Net* >* allNet = new vector <Net* >;
	vector<Net* >::iterator it0, it1, it2;	//vector<Net*>*?

	vector<Net *> *a = getPINets();    // <<--------need to delete?!? 
	vector<Net *> *b = getPONets();
	vector<Net *> *w = getWireNets();

	for(it0 = a->begin() ; it0 != a->end(); ++it0)
	{
		allNet->push_back(*it0);
	}
	
	for(it1 = b->begin(); it1 != b->end(); ++it1)
	{
		allNet->push_back(*it1);
	}

	for(it2 = w->begin(); it2 != w->end(); ++it2)
	{
		allNet->push_back(*it2);
	}

	delete a;
	delete b;
	delete w;

	return allNet;
  }

  // Dynamically allocates a new vector and adds pointers to all Gates into the vector and then returns a pointer to the vector 
  //@return Pointer to a new vector storing Gate pointers
  vector<Gate *> *Design::allGates()
  {
	vector<Gate *>* allgates = new vector<Gate *>;
	map<string, Gate*>::iterator it;
	for (it = designGates.begin(); it != designGates.end(); ++it)
	{
		allgates->push_back(it->second);	
	}	

	return allgates;
  }

  /**
   * To be implemented in Part 2.  For Part 1 just return NULL
   * 
   * Performs a topological sorting of the nets in the design
   *  and returns a dynamically allocated vector with pointers
   *  to Net objects in a satisfactory topological order
   *
   * @return Pointer to a new vector storing Net pointers
   */
  vector<Net *> * Design::toposort()
  {
	map<string, int> colormap;
	vector<Net*>* temp = allNets();		//pointer to a net*
	vector<Net*>::iterator it;
	for (it = temp->begin(); it != temp->end(); ++it)
	{
		colormap.insert(make_pair(((*it)->name()), 0));	//sets all to white 
	}

	vector<Net*>* finish = new vector<Net*>;
	
	for(it = temp->begin(); it != temp->end(); ++it)
	{
		if(colormap[((*it)->name())] == 0)	
		{
			dfs_visit(*it, finish, colormap);	
		
		}
	}

	delete temp;
	return finish;
  }

  //Recreates the Verilog description of the design dumping it to the provided ostream
  // @param[in] os Ostream representing a file, stdout, or stringstream to which the Verilog description should be output
  void Design::dump(ostream &os)
  {
	//module name (inputs, outputs)
	os << "module " << name() <<"(";
	for (unsigned int i = 0; i < pis.size(); i++)
	{
		os << pis[i] << ",";
	}
	
	for (unsigned int i = 0; i < pos.size(); i++)
	{
		if( i < pos.size()-1 ) os << pos[i] << ","; 
		else os << pos[i];
	}
	os << ");" << endl;

	//input NAME
	for(unsigned int i = 0; i < pis.size(); i++)
	{
		os << "\tinput " << pis[i] << ";" << endl;
	}
	//OUTPUT NAME
	for(unsigned int i = 0; i < pos.size(); i++)
	{
		os << "\toutput " << pos[i]<< ";" << endl;
	}
	os << endl;

	//wire NAME
	vector <Net* > *w = getWireNets();
	for(unsigned int i = 0; i < w->size(); i++)
	{
		os << "\twire  " << (w->at(i))->name() << ";" << endl;
	}

	os << endl;

	// dealloc getWireNets()
	/*vector <Net* >*::iterator v;
	for (v = w->begin(); v != w->end(); ++v)
	{
		delete *v;
	}*/
	delete w;

	//vector<Net*>::iterator it0;
	//vector<Gate*>::iterator it1;

	/*
	for(it0 = allNets()->begin(); it0 != allNets()->end(); ++it0)  //<---??
	{
		os << (*it0)->name();
		os << endl;
	}*/

	// GATES DUMP	
	map<string, Gate*>::iterator it;
	for (it = designGates.begin(); it != designGates.end(); ++it)
	{
		os << "\t";
		(it->second)->dump(os);	
	}	
	//os << endl;
	os << "endmodule";

	/*for(it1 = allGates()->begin(); it1 != allGates()->end(); ++it1)
	{
		//os << (*it1)->name();
		//os << endl;
		it1->dump();
	}*/

  }

  //Checks if the given Net object represents a primary input 
  //@param[in] n Pointer to the Net 
  //@return true if the Net is a primary input, false otherwise
  bool Design::isPI(Net *n)
  {
	vector<Net*>::iterator it;	//vector<Net*> *??
	vector<Net *> *a = getPINets();
	for(it = a->begin() ; it != a->end(); ++it)
	{
		if (n == *it)
		{
			return true;
		}
	}
	delete a;
	return false;

  }

  // Checks if the given Net object represents a primary output @param[in] n Pointer to the Net 
  // @return true if the Net is a primary output, false otherwise
  bool Design::isPO(Net *n)
  {
	vector<Net* >::iterator it;
	vector<Net *> *b = getPONets();
	for(it = b->begin() ; it != b->end(); ++it)
	{
		if (n == *it)
		{
			return true;
		}
	}
	delete b;
	return false;

  }

  /**
   * To be implemented in Part 2.  For Part 1 just return 
   *
   * Recursive helper function to perform the depth-first search
   * of nets
   * 
   * @param[in] n Pointer to the current Net on which the DFS is visiting 
   * @param[out] flist Stores the Nets as they are finished...this is the
   *                   ordering after the topological sort finishes
   * @param[inout] colormap Maps the net name to its visited color 
   */
  void Design::dfs_visit(Net *n, vector<Net *> *flist, map<string, int> &colormap)
  {
    
    colormap[n->name()] = 1; 		//initialize to gray
	vector<Gate*>::iterator it;
	vector<Gate*>* temp = n->getDrivers();	// returns drivers of this net (pointer to a vector)
	//for(it = (n->temp).begin(); it != (n->temp).end(); ++it)
	for(it = temp->begin(); it != temp->end(); ++it)
	{
		//cout << (*it)->name() << endl; // debug gate name

        /*if (colormap[((*it)->getOutput())->name()] == 0)  //if adj is white	   
        {
            cout << colormap[((*it)->getOutput())->name()] << endl; // debug
			dfs_visit(((*it)->getOutput()), flist, colormap);	//call recursively 
		}*/
	
        vector<Net *> *get_ins = (*it)->getInputs();
        vector<Net*>::iterator jt;
        for(jt = get_ins->begin(); jt != get_ins->end(); ++jt)
        {
            if (colormap[(*jt)->name()] == 0)  //if adj is white    
            {
                dfs_visit(*jt, flist, colormap);   //call recursively 
            }        
        }

        delete get_ins;


    }
	colormap[n->name()] = 2;		//changes color to black
	flist->push_back(n);			//adds to our finished list    
	delete temp;
  }

