#ifndef ASSEMBLE_H
#define ASSEMBLE_H
/*
 * assemble.h
 *
 */
#include <boost/serialization/set.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include "common.h"
#include "kmerhash.h"
#include "splicing_graph.h"

#ifdef WIN32
#define __STDC__ 1
#include <Windows.h>
#include "windows/getopt.h"
#else
#include <getopt.h>
#endif


static const char *short_options = "k:i:o:p:h";

static struct option long_options[] = {
  // general options
  {"kmer_length",                 required_argument,      0,      'k'},
  {"help",                        no_argument,            0,      'h'},
  {"CPU",			  required_argument,      0,      'p'},
  // assemble option
  {"min_kmer_coverage",           required_argument,      0,      OPT_MIN_KMER_COVERAGE},
  {"min_kmer_entropy",            required_argument,      0,      OPT_MIN_KMER_ENTROPY},
  {"min_seed_coverage",           required_argument,      0,      OPT_MIN_SEED_COVERAGE},
  {"min_seed_entropy",            required_argument,      0,      OPT_MIN_SEED_ENTROY },
  {"min_junction_coverage",       required_argument,      0,      OPT_MIN_JUNCTION_COVERAGE},
  {"min_ratio_non_error",         required_argument,      0,      OPT_MIN_RATIO_NON_ERROR},
  {"min_exon_length",             required_argument,      0,      OPT_MIN_EXON_LENGTH},
  {"min_kmers_per_graph",         required_argument,      0,      OPT_MIN_KMERS_PER_GRAPH},
  {"reads",                       required_argument,      0,      'i'},
  {"kmers",			  required_argument,	  0,	  OPT_KMERS},
  {"double_stranded_mode",	  no_argument,		  0,	  OPT_DOUBLE_STRANDED_MODE},
  {"pair_gap_length",             required_argument,      0,      OPT_PAIR_GAP_LENGTH},
  {"fr_strand",			  required_argument,      0,      OPT_FR_STRAND},
  {"out_dir",                     required_argument,      0,      'o'},
  {"pair_end",			  no_argument,            0,	  OPT_IS_PAIR_END},
  {"debug",			  no_argument,            0,      OPT_DEBUG},
//...
  {0,0,0,0} // terminator

};

int parse_options(int argc, char* argv[]);
std::string usage();

static size_t rg_index = 0;

std::string base_name() {

    std::stringstream idx ;
    idx << "comp" << rg_index ;
    rg_index++;

    return idx.str();
}


template <typename T>
void check_incompatible_edges(SplicingGraph<T> & splicing_graph,
                              KmerMap<T>& kmer_map,
                              std::vector<std::set<size_t> >& reads_mapped_to_node,
                              std::map<pair_t,int>& edge_index,
                              int node_id,
			      std::set<pair_t>& inhibit_edges) {

  assert(splicing_graph.node_set_[node_id].parents.size() >= 2);
  assert(splicing_graph.node_set_[node_id].children.size() >= 2);
  int p1 = splicing_graph.node_set_[node_id].parents[0];
  int p2 = splicing_graph.node_set_[node_id].parents[1];
  int c1 = splicing_graph.node_set_[node_id].children[0];
  int c2 = splicing_graph.node_set_[node_id].children[1];
  int p1_length = splicing_graph.node_set_[p1].sequence.length();
  int p2_length = splicing_graph.node_set_[p2].sequence.length();
  int c1_length = splicing_graph.node_set_[c1].sequence.length();
  int c2_length = splicing_graph.node_set_[c2].sequence.length();
  int length = splicing_graph.node_set_[node_id].sequence.length();
  int e1 = edge_index[pair_t(p1,node_id)];
  int e2 = edge_index[pair_t(p2,node_id)];
  int e3 = edge_index[pair_t(node_id,c1)];
  int e4 = edge_index[pair_t(node_id,c2)];

  /* draft 

    p1              c1
        \1       3/
          node_id 
        /2       4\
    p2              c2

  */

  bool p1_to_c1 = true;
  bool p1_to_c2 = true;
  bool p2_to_c1 = true;
  bool p2_to_c2 = true;
  const std::string& edge1 = splicing_graph.get_edge_sequence(p1, node_id);
  std::set<size_t> reads1;
  splicing_graph.set_reads(kmer_map, edge1, reads1);
  std::map<int,size_t> d1;
  //splicing_graph.get_mate_distribution(reads1,d1,reads_mapped_to_node, 1);
  splicing_graph.get_mate_distribution(reads1,d1,reads_mapped_to_node, 2);

  const std::string& edge2 = splicing_graph.get_edge_sequence(p2, node_id);
  std::set<size_t> reads2;
   splicing_graph.set_reads(kmer_map, edge2, reads2);
  std::map<int,size_t> d2;
  //splicing_graph.get_mate_distribution(reads2,d2,reads_mapped_to_node, 1);
  splicing_graph.get_mate_distribution(reads2,d2,reads_mapped_to_node, 2);

  const std::string& edge3 = splicing_graph.get_edge_sequence(node_id, c1);
  std::set<size_t> reads3;
  splicing_graph.set_reads(kmer_map, edge3, reads3);
  std::map<int,size_t> d3;
  //splicing_graph.get_mate_distribution(reads3,d3,reads_mapped_to_node, 0);
  splicing_graph.get_mate_distribution(reads3,d3,reads_mapped_to_node, 2);

  const std::string& edge4 = splicing_graph.get_edge_sequence(node_id, c2);
  std::set<size_t> reads4;
   splicing_graph.set_reads(kmer_map, edge4, reads4);
  std::map<int,size_t> d4;
  //splicing_graph.get_mate_distribution(reads4,d4,reads_mapped_to_node, 0);
  splicing_graph.get_mate_distribution(reads4,d4,reads_mapped_to_node, 2);


  int tolerance = 80;
  if ( ((!d1.empty() && d1.find(c1) == d1.end() && (length + c1_length >= g_pair_gap_length + tolerance)) &&
        (!d3.empty() && d3.find(p1) == d3.end() && (length + p1_length >= g_pair_gap_length + tolerance))) &&   // p1 -| c1
      // (d1.find(c2) != d1.end() || d4.find(p1) != d4.end()) &&     // p1 -> c2
       (d2.find(c1) != d2.end() || d3.find(p2) != d3.end()) ) {     // p2 -> c1
    if (d1.find(c2) != d1.end() || d4.find(p1) != d4.end()) {  // p1 -> c2
      p1_to_c1 = false;
    } else if (length + p1_length < g_pair_gap_length + tolerance && 
      length + c2_length < g_pair_gap_length + tolerance) {
       int parent = -1;
       int child = -1;
       if (splicing_graph.node_set_[p1].parents.size() == 1) 
	 parent = splicing_graph.node_set_[p1].parents[0];
       if (splicing_graph.node_set_[c2].children.size() == 1)
	 child = splicing_graph.node_set_[c2].children[0];
       if ( (parent > 0 && d4.find(parent) != d4.end()) ||
	    (child > 0 && d1.find(child) != d1.end()) )
	 p1_to_c1 = false; 
    }
  }
  if ( ((!d1.empty() && d1.find(c2) == d1.end() && (length + c2_length >= g_pair_gap_length + tolerance)) &&
        (!d4.empty() && d4.find(p1) == d4.end() && (length + p1_length >= g_pair_gap_length + tolerance))) &&   // p1 -| c2
       //(d1.find(c1) != d1.end() || d3.find(p1) != d3.end()) &&     // p1 -> c1
       (d2.find(c2) != d2.end() || d4.find(p2) != d4.end()) ) {      // p2 -> c2
    if (d1.find(c1) != d1.end() || d3.find(p1) != d3.end()) {        // p1 -> c1
      p1_to_c2 = false;
    } else if (length + c1_length <  g_pair_gap_length + tolerance &&
      length + p1_length < g_pair_gap_length + tolerance) {
      int parent = -1;
      int child = -1;
      if (splicing_graph.node_set_[p1].parents.size() == 1)
        parent = splicing_graph.node_set_[p1].parents[0];
      if (splicing_graph.node_set_[c1].children.size() == 1)
        child = splicing_graph.node_set_[c1].children[0];
      if ( (parent > 0 && d3.find(parent) != d3.end()) ||
           (child > 0 && d1.find(child) != d1.end()) )
        p1_to_c2 = false;
    }
  }


  if ( ((!d2.empty() && d2.find(c1) == d2.end() && (length + c1_length >= g_pair_gap_length + tolerance)) &&
        (!d3.empty() && d3.find(p2) == d3.end() && (length + p2_length >= g_pair_gap_length + tolerance))) &&   // p2 -| c1
       //(d2.find(c2) != d2.end() || d4.find(p2) != d4.end()) &&      // p2 -> c2
       (d1.find(c1) != d1.end() || d3.find(p1) != d3.end()) ) {      // p1 -> c1
    if (d2.find(c2) != d2.end() || d4.find(p2) != d4.end()) {        // p2 -> c2
      p2_to_c1 = false;
    } else if (length + c2_length <  g_pair_gap_length + tolerance &&
      length + p2_length < g_pair_gap_length + tolerance) {
      int parent = -1;
      int child = -1;
      if (splicing_graph.node_set_[p2].parents.size() == 1)
        parent = splicing_graph.node_set_[p2].parents[0];
      if (splicing_graph.node_set_[c2].children.size() == 1)
        child = splicing_graph.node_set_[c2].children[0];
      if ( (parent > 0 && d4.find(parent) != d4.end()) ||
           (child > 0 && d2.find(child) != d2.end()) )
         p2_to_c1 = false;
    }
  }

  if ( ((!d2.empty() && d2.find(c2) == d2.end() && (length + c2_length >= g_pair_gap_length + tolerance)) &&
        (!d4.empty() && d4.find(p2) == d4.end() && (length + p2_length >= g_pair_gap_length + tolerance))) &&   // p2 -| c2
      // (d2.find(c1) != d2.end() || d3.find(p2) != d3.end()) &&      // p2 -> c1
       (d1.find(c2) != d1.end() || d4.find(p1) != d4.end()) ) {      // p1 -> c2
    if (d2.find(c1) != d2.end() || d3.find(p2) != d3.end()) {        // p2 -> c1
      p2_to_c2 = false;
    } else if (length + c1_length <  g_pair_gap_length + tolerance &&
      length + p2_length < g_pair_gap_length + tolerance) {
      int parent = -1;
      int child = -1;
      if (splicing_graph.node_set_[p2].parents.size() == 1)
        parent = splicing_graph.node_set_[p2].parents[0];
      if (splicing_graph.node_set_[c1].children.size() == 1)
        child = splicing_graph.node_set_[c1].children[0];
      if ( (parent > 0 && d3.find(parent) != d3.end()) ||
           (child > 0 && d2.find(child) != d2.end()) )
         p2_to_c2 = false;
    }
  }

  if (!p1_to_c1) inhibit_edges.insert(pair_t(e1,e3));
  if (!p1_to_c2) inhibit_edges.insert(pair_t(e1,e4));
  if (!p2_to_c1) inhibit_edges.insert(pair_t(e2,e3));
  if (!p2_to_c2) inhibit_edges.insert(pair_t(e2,e4));

}


//////////////////////////////////////////// New Up /////////////////////////
template <typename T>
void assembler(KmerMap<T>& kmer_map, std::vector<std::string>& data) {
  // build the hash 
  if (!data.empty()) {
    kmer_map.get_hash(data);
  } else if (kmers_file != "") {
    //kmer_map.get_hash_from_kmers(kmers_file, g_kmer_length);
  } else { 
    errAbort(const_cast<char *>("Building kmer hash failed."));
  }
 
  // prune errors
  kmer_map.remove_erroneous_kmers(g_min_ratio_non_error);
  kmer_map.prune_hash(g_min_kmer_coverage, g_min_kmer_entropy);

  // get seed kmers
  std::vector<kmer_int_type_t> seeds_sorted;
  kmer_map.get_seed_sort_descending_counts(seeds_sorted);
  //seeds_sorted = kmer_map.get_seed_sort_descending_counts();

  // assemble graphs one by one
  if (seeds_sorted.empty()) 
    errAbort(const_cast<char *>("No seeds available!\n"));

  const std::string & raw_graph_list = out_dir + "/" + rg_list;
  std::fstream out_rg;
  out_rg.open(raw_graph_list.c_str(), std::fstream::out);
  if (!out_rg.is_open()) {
    errAbort(const_cast<char *>("File %s can't be opened.\n"), raw_graph_list.c_str());
  }
  extern std::set<pair_t> Inhibit_edges;////////////////////////////////////////New
  extern std::vector<vector<int> > Pair_edges;
  for (unsigned int i = 0; i < seeds_sorted.size(); ++i) {
	  Inhibit_edges.clear();//////////////////////////////////////////////////////////////New
	  Pair_edges.clear();/////////////////////////////////////////////////////////////////New
    if (kmer_map.exists(seeds_sorted[i])) {

      SplicingGraph<T> splicing_graph;
      if (splicing_graph.build(kmer_map, seeds_sorted[i], data)) {

         splicing_graph.compact_graph();//New
         KmerMap<T> kmer_map_new(g_kmer_length, g_double_stranded_mode);//New
		 kmer_map_new.get_hash(splicing_graph.reads_, false);
		 splicing_graph.trim_graph(kmer_map_new, splicing_graph.reads_);  //New
	     splicing_graph.refine_tips(kmer_map_new, splicing_graph.reads_);//New
		 if (splicing_graph.get_size() == 1)//New
		 {  // just one contig, trivial case//New
			splicing_graph.refine_tips(kmer_map_new, splicing_graph.reads_);//New
		 }//New
        const std::string & basename = base_name();
        // save each graph as one individual file
        const std::string name = out_dir + "/" + basename + ".rg";
	//splicing_graph.save(name);

        // record the basename of graph
        out_rg << basename << std::endl;


////////////////////////////////////////////////////////////////////////////////New
// find some inhibit edges for compatibility graph
  std::map<pair_t, double> edge_cov_map; 
  splicing_graph.get_coverage_of_edges(edge_cov_map);
  std::vector<std::set<size_t> > reads_mapped_to_node;
  splicing_graph.map_reads_to_graph(kmer_map_new, reads_mapped_to_node);
  std::vector<pair_t> splicing_graph_edges;
  if (!splicing_graph_edges.empty())
    splicing_graph_edges.clear();
  splicing_graph_edges.reserve(edge_cov_map.size());
  for (size_t ii = 0; ii < splicing_graph.node_set_.size(); ++ii) {
    if (splicing_graph.node_set_[ii].children.empty())
      continue;
    for (size_t jj = 0; jj < splicing_graph.node_set_[ii].children.size(); ++jj)
      splicing_graph_edges.push_back(pair_t(ii,splicing_graph.node_set_[ii].children[jj]));
  }
if (!reads_mapped_to_node.empty()) {
    std::map<pair_t, int> edge_index;
    for (int ii = 0; ii < (int)splicing_graph_edges.size(); ++ii) {
      edge_index[splicing_graph_edges[ii]] = ii;
    }
    std::vector<int> check_nodes;
    for (int ii = 0; ii < (int)splicing_graph.get_size(); ++ii) {
      if (splicing_graph.node_set_[ii].children.size() > 1 &&
        splicing_graph.node_set_[ii].parents.size() > 1) { 
        check_nodes.push_back(ii);
      }
    }
    if (!check_nodes.empty()) {
      for (int ii = 0; ii < (int)check_nodes.size(); ++ii) {
        int id = check_nodes[ii];
        if ((int)splicing_graph.node_set_[id].sequence.length() < g_kmer_length ||
         (int)splicing_graph.node_set_[id].sequence.length() > g_max_pair_gap_length)
          continue; 
        check_incompatible_edges(splicing_graph, kmer_map_new, reads_mapped_to_node, edge_index, id, Inhibit_edges);
      }
    }
  }
  ///////////////////////////////////////////////////////////////////////////////New
  
  
  //************************************************************************************************************************
  std::vector<int> vec_temp;
  vec_temp.clear();
  for (int ii=0;ii<splicing_graph_edges.size(); ++ii)
  {
  	std::string edge=splicing_graph.get_edge_sequence(splicing_graph_edges[ii].first,splicing_graph_edges[ii].second);
  	std::set<size_t> reads;
  	splicing_graph.set_reads(kmer_map_new, edge, reads);
  	std::map<int,size_t> d;
  	splicing_graph.get_mate_distribution(reads,d,reads_mapped_to_node, 2);	
  	std::map<int,size_t>::iterator iter;
  	for (iter=d.begin();iter!=d.end();++iter)
  	{
  		vec_temp.push_back(ii);
  		vec_temp.push_back(iter->first);
  		vec_temp.push_back(iter->second);
  		Pair_edges.push_back(vec_temp);
  		vec_temp.clear();
  	}
  }
  //************************************************************************************************************************
        // output the graph in a readable format for debug
        if (g_debug)  
          splicing_graph.save_debug(name + ".debug"); 

        if (rg_index % 2000 == 0)
          std::cerr << rg_index << " graphs have been built." << std::endl;

        // remove kmers used in current graph from hash table
        splicing_graph.clear(kmer_map);
      }
    }
  } // for

  out_rg.close();
}


#endif
