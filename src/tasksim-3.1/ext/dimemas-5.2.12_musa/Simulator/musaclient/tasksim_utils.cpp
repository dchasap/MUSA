
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "define.h"
#include "types.h"
#include "extern.h"
#include "list.h"
#include "subr.h"

#include <tasksimclient.h>

}

using namespace std;
#include <iostream>
#include <map>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::map< std::string, std::map<long, std::map<long,long> > > presim_results;
//std::map< std::string, long > presim_results;

////////////////////////////////////////////////////////////////////////////////////////
void show (void) {

    // Si no existe cargamos de fichero.


std::map< std::string, std::map<long, std::map<long,long> > >::iterator it_files;
std::map<long, std::map<long,long> >::iterator it_rank;
std::map<long,long>::iterator it_phase;


std::cout << "---------------" << endl << endl;

for (it_files = presim_results.begin(); it_files!=presim_results.end(); it_files++) {
  for (it_rank = it_files->second.begin(); it_rank!=it_files->second.end(); it_rank++) {
    for (it_phase = it_rank->second.begin(); it_phase!=it_rank->second.end(); it_phase++) {
      std::cout << "--> "<< it_files->first << "  _rank " << it_rank->first <<  " _phase "<<  it_phase->first << " _time " << it_phase->second << std::endl;
    }
  }
}

std::cout << "---------------" << endl << endl;


    return;
}


////////////////////////////////////////////////////////////////////////////////////////
void load_file (std::string presim_file) {

    // Si no existe cargamos de fichero.
    std::ifstream input( presim_file.c_str() );

    for( std::string line; getline( input, line ); )
    {
      // parse line to get rank, phase, and time
      std::vector<std::string> x = split(line, ':');

      
      long rank, phase, time; 
      rank  = atol(x[0].c_str());
      phase = atol(x[1].c_str());
      time  = atol(x[4].c_str());



      std::map<long, std::map<long,long> >::iterator it_rank;
      it_rank = presim_results[presim_file].find(rank); 

      if (it_rank == presim_results[presim_file].end()) {
         // rank does not exist; add map for rank and map for phase
         //std::map<long,long> token_phase;
         //std::map<long, std::map<long,long> > token_rank;
         //token_phase[phase]=time;
         //jtoken_rank[rank]=token_phase;
         presim_results[presim_file][rank][phase]=time;


         //long result = presim_results[presim_file][rank][phase];
      } else {
         // rank exists; must look for phase
         std::map<long,long>::iterator it_phase;
         it_phase = presim_results[presim_file][rank].find(phase);

         if (it_phase == presim_results[presim_file][rank].end()){
            //std::cout << rank << " " << phase << " " << time << std::endl; 
            presim_results[presim_file][rank][phase]=time;

//            std::cout << "---" << presim_results[presim_file].size() << std::endl;
//            std::cout << "XXX" <<  presim_results[presim_file][rank].size() << std::endl;
         } else {
            // this is an error....duplicated rank, phase
         
         }
      }
    }
    return;
}



double get_presimulation_cpp (char *param_presim_file, int taskid, tsim_phase *ph) {


  // TODO:<1> replace key for pair <rank,phase>
  // TODO:<2> num files is known. Equal to num_ranks. Vector instead.

  long rank=taskid+1;
  long phase=ph->master_id;

  std::string presim_file(param_presim_file);

  std::map<std::string, std::map<long, std::map<long,long> > >::iterator it_file;
  //std::map<std::string, long>::iterator it_file;

  it_file = presim_results.find(presim_file);

  if (it_file == presim_results.end()) {
    //std::cout << "Must add : " << presim_file << std::endl;

    // Si no existe cargamos de fichero.
    load_file (presim_file);
  
  }

  // buscar file, rank, phase, ---> time



  //show();



 
  double result = presim_results[presim_file][taskid+1][ph->master_id];
 
 
 // std::cout << "CPP for " << presim_file << " -- " << taskid+1 << " -- " << ph->master_id << " results is : " << result << std::endl;
  
  return result; 
}

