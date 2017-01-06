// Names: Ibar Romay, Michael Chu
// 

#include <iostream>
#include <fstream>

#include "vlg_parser.h"
#ifdef AUTOPARSE
#include "vlg_parser_auto.h"
#else
#include "vlg_parser_man.h"
#endif

#include "design.h"
#include "gates.h"
#include "net.h"

#include "logicsim.h"


using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3){
      cerr << "Usage ./gsim infile.v sim_file.v outfile.v" << endl;
      return 1;
    }
#ifdef AUTOPARSE
    VlgParser *parser = new VlgParserAuto();
#else
    VlgParser *parser = new VlgParserMan();
#endif
    Design *d = parser->parse(argv[1]);

    ofstream output(argv[3]);

    LogicSim logic_sim(argv[2]);
    logic_sim.parse(d);
    //logic_sim.gate_network(d);
    logic_sim.dump(output,d);

    delete parser;  //<--- uncomment later

    return 0;
}

