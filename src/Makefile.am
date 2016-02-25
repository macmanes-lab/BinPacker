AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I ../m4
# Libtool dependencies
LIBTOOL_DEPS = @LIBTOOL_DEPS@

bin_PROGRAMS = Assemble change_fasta Find_errors packing packing_num read_result BinPacker_denovo

Assemble_SOURCES = utility.cpp kmerhash.cpp common.cpp loadreads.cpp splicing_graph.cpp assemble.cpp utility.h kmerhash.h common.h loadreads.h splicing_graph.h assemble.h
Assemble_CPPFLAGS =  -g -Wno-deprecated -static $(CPPFLAGS) 
#Assemble_LDFLAGS = $(BOOST_THREAD_LIBS) $(BOOST_SERIALIZATION_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_LDFLAGS)
Assemble_LDFLAGS =  -lboost_serialization -lboost_system -lboost_filesystem $(BOOST_LDFLAGS)
Assemble_LIBS = $(BOOST_THREAD_LIBS) $(BOOST_SERIALIZATION_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS)
change_fasta_SOURCES= change_fasta.cc
Find_errors_SOURCES= Find_errors.cc
packing_SOURCES= line_graph.h write_ILP.h ILP_process.h packing.cc
packing_num_SOURCES= packing_num.cc
read_result_SOURCES= read_result.cc
BinPacker_denovo_SOURCES= BinPacker_denovo.cc get_junction_graph.h get_junction_graph_compulsory.h junction_paths.h recover_paths.h
