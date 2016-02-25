#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<cstring>
#include"get_junction_graph.h"
#include"get_junction_graph_compulsory.h"
#include"junction_paths.h"
#include"recover_paths.h"
using namespace std;

int main(int argc, char * argv[])
{
	if (argc!=5)
	{
		cout<<"Error Parameters!"<<endl;
		return 0;
	}
	float Filter=atof(argv[3]);
	int Duplicate=atoi(argv[4]);
	int i,j,k;
	vector<string> Output_weights;
	vector<string> Node_seq;
	string temp,temp_1;
        int temp_2,temp_3;
	int num_id;
	ifstream ifs;
	ifs.open(argv[1]);
	while (ifs.peek()!=EOF)
	{
		getline(ifs,temp);
		if (temp.substr(0,10)=="node id = ")
		{
			for (i=10;i<temp.size();i++)
			{
				if (temp.substr(i-6,6)=="cov = ")
				{
					for (j=0;j<temp.size();j++)
					{
						if (temp.substr(i+j,2)=="se")
						{
							Output_weights.push_back(temp.substr(i,j-1));
							break;
						}
					}
				}
			}
			getline(ifs,temp);
			Node_seq.push_back(temp);
		}
	}
	ifs.close();
        char * temp_argv;
        int ii,j_argv,k_argv;
        int t=0;
        temp_argv=argv[1];
        for (i=0;i<strlen(temp_argv);i++)
        {
                if (temp_argv[i]=='c' && temp_argv[i+1]=='o' && temp_argv[i+2]=='m' && temp_argv[i+3]=='p')
                {
                        j_argv=i;
                        break;
                }
        }
        for (i=j_argv;i<strlen(temp_argv)-4;i++)
        {
                if (temp_argv[i]=='.')
                {
                        k_argv=i-1;
                        break;
                }
        }
	string comp_num;
	for (ii=j_argv+4;ii<=k_argv;ii++)
	{
		comp_num.push_back(temp_argv[ii]);
	}

	ofstream file;
	file.open(argv[2],ios_base::app);
    if (Duplicate==0)
    {
	Get_Junction_Graph junction_graph(argv[1]);
	junction_graph.Construct_Junction_Graph();
        Get_Junction_Graph_ junction_graph_(argv[1]);
        junction_graph_.Construct_Junction_Graph();
	Get_Junction_Paths get_junction_paths(junction_graph.Edges_left,junction_graph.Edges_right,junction_graph.Weights,junction_graph.DAG_weights,junction_graph.Cons_left,junction_graph.Cons_right,junction_graph.S,junction_graph.T,junction_graph.Single_nodes,junction_graph.max_node_num,Filter);
	get_junction_paths.Search_Junction_Paths();
        Get_Junction_Paths get_junction_paths_(junction_graph_.Edges_left,junction_graph_.Edges_right,junction_graph_.Weights,junction_graph_.DAG_weights,junction_graph_.Cons_left,junction_graph_.Cons_right,junction_graph_.S,junction_graph_.T,junction_graph_.Single_nodes,junction_graph_.max_node_num,Filter);
        get_junction_paths_.Search_Junction_Paths();
	Recover_Junction_Paths recover_junction_paths(junction_graph.DAG_left,junction_graph.DAG_right,get_junction_paths.Junction_Path_cover,junction_graph.max_node_num);
	recover_junction_paths.Get_Path_cover();
        Recover_Junction_Paths recover_junction_paths_(junction_graph_.DAG_left,junction_graph_.DAG_right,get_junction_paths_.Junction_Path_cover,junction_graph_.max_node_num);
        recover_junction_paths_.Get_Path_cover();
	vector<vector<int> > Path_cover;
        vector<vector<int> >::iterator iter;
	Path_cover=recover_junction_paths.Path_Cover;
        for (i=0;i<recover_junction_paths_.Path_Cover.size();i++)
        {
                Path_cover.push_back(recover_junction_paths_.Path_Cover[i]);
        }
        sort(Path_cover.begin(),Path_cover.end());
        iter=unique(Path_cover.begin(),Path_cover.end());
        if(iter != Path_cover.end())
        {
                Path_cover.erase(iter,Path_cover.end());
        }
	num_id=1;
	for (i=0;i<Path_cover.size();i++)
        {
                k=0;
                for (j=0;j<Path_cover[i].size();j++)
                {
                        k=k+Node_seq[Path_cover[i][j]].size();
                }
                if (k>=200) 
		{
			file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
			num_id++;
			for (j=0;j<Path_cover[i].size();j++)
			{
				file<<Node_seq[Path_cover[i][j]];
			}
			file<<endl;
		}
	}
        //}
        for (i=0;i<junction_graph.Single_nodes.size();i++)
        {
                if (Node_seq[junction_graph.Single_nodes[i]].size()<500 && atof(Output_weights[junction_graph.Single_nodes[i]].c_str())<3.0)
                {
                        continue;
                }
                if (Node_seq[junction_graph.Single_nodes[i]].size()>=200)
		{
			file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
			num_id++;
			file<<Node_seq[junction_graph.Single_nodes[i]]<<endl;
		}
        }
        int t_size=Node_seq.size()-1;
        if (junction_graph.max_node_num<t_size)
        {
                for (i=junction_graph.max_node_num+1;i<Node_seq.size();i++)
                {
                        if (Node_seq[i].size()<500 && atof(Output_weights[i].c_str())<3.0)
                        {
                                continue;
                        }
                        if (Node_seq[i].size()>=200)
			{
				file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
				num_id++;
				file<<Node_seq[i]<<endl;
			}
                }
        }
    }
    if (Duplicate==1)
    {
	Get_Junction_Graph junction_graph(argv[1]);
        junction_graph.Construct_Junction_Graph();
	Get_Junction_Paths get_junction_paths(junction_graph.Edges_left,junction_graph.Edges_right,junction_graph.Weights,junction_graph.DAG_weights,junction_graph.Cons_left,junction_graph.Cons_right,junction_graph.S,junction_graph.T,junction_graph.Single_nodes,junction_graph.max_node_num,Filter);
        get_junction_paths.Search_Junction_Paths();
	Recover_Junction_Paths recover_junction_paths(junction_graph.DAG_left,junction_graph.DAG_right,get_junction_paths.Junction_Path_cover,junction_graph.max_node_num);
        recover_junction_paths.Get_Path_cover();
	vector<vector<int> > Path_cover;
	Path_cover=recover_junction_paths.Path_Cover;
	num_id=1;
	for (i=0;i<Path_cover.size();i++)
        {
                k=0;
                for (j=0;j<Path_cover[i].size();j++)
                {
                        k=k+Node_seq[Path_cover[i][j]].size();
                }
                if (k>=200) 
		{
			file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
			num_id++;
			for (j=0;j<Path_cover[i].size();j++)
			{
				file<<Node_seq[Path_cover[i][j]];
			}
			file<<endl;
		}
	}
        //}
        for (i=0;i<junction_graph.Single_nodes.size();i++)
        {
                if (Node_seq[junction_graph.Single_nodes[i]].size()<500 && atof(Output_weights[junction_graph.Single_nodes[i]].c_str())<3.0)
                {
                        continue;
                }
                if (Node_seq[junction_graph.Single_nodes[i]].size()>=200)
		{
			file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
			num_id++;
			file<<Node_seq[junction_graph.Single_nodes[i]]<<endl;
		}
        }
        int t_size=Node_seq.size()-1;
        if (junction_graph.max_node_num<t_size)
        {
                for (i=junction_graph.max_node_num+1;i<Node_seq.size();i++)
                {
                        if (Node_seq[i].size()<500 && atof(Output_weights[i].c_str())<3.0)
                        {
                                continue;
                        }
                        if (Node_seq[i].size()>=200)
			{
				file<<">BINPACKER."<<comp_num<<"."<<num_id<<endl;
				num_id++;
				file<<Node_seq[i]<<endl;
			}
                }
        }
    }
	file.close();

    return 0;
}



