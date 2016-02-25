#ifndef JUNCTION_PATHS
#define JUNCTION_PATHS

//junction_paths.h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<cmath>
using namespace std;

class Get_Junction_Paths
{
	private:
	vector<int> Edges_left;// left node of an edge in junction graph
        vector<int> Edges_right;// right node of an edge in junction graph
        vector<double> Weights;
	vector<double> DAG_weights;
        vector<int> Cons_left;
        vector<int> Cons_right;
        vector<int> S,T;
        vector<int> Single_nodes;
        int max_node_num;
	float Filter;
	public:
	Get_Junction_Paths(vector<int> edges_left,vector<int> edges_right,vector<double> weights,vector<double> dag_weights,vector<int> cons_left,vector<int> cons_right,vector<int> s,vector<int> t,vector<int> single_nodes,int max_node,float filter);
	~Get_Junction_Paths();
	void Search_Junction_Paths();
	vector<double> Weights_Pair;
	vector<vector<int> > Junction_Path_cover;
};
Get_Junction_Paths::Get_Junction_Paths(vector<int> edges_left,vector<int> edges_right,vector<double> weights,vector<double> dag_weights,vector<int> cons_left,vector<int> cons_right,vector<int> s,vector<int> t,vector<int> single_nodes,int max_node,float filter)
{
	Edges_left=edges_left;
	Edges_right=edges_right;
	Weights=weights;
	DAG_weights=dag_weights;
	Cons_left=cons_left;
	Cons_right=cons_right;
	S=s;
        T=t;
	Single_nodes=single_nodes;
	max_node_num=max_node;
	Filter=filter;
	return;
}
Get_Junction_Paths::~Get_Junction_Paths()
{
	return;
}
void Get_Junction_Paths::Search_Junction_Paths()
{
	int i,j,k,t,seed,seed_temp,SEED;
	double max_weights;
	vector<double> Weights_Pair;
	vector<int>::iterator pos;
	vector<int> Unused_junctions;
        vector<int> Used_junctions;
	vector<int> Extension_left;
        vector<int> Extension_right;
	vector<int> Extension_num_0, Extension_num_1;
	vector<int> Extension_num_unused;
        vector<int> Extension;
	
	Weights_Pair=Weights; 
	for (i=0;i<Cons_left.size();i++)
	{
	    for (j=0;j<Weights.size();j++)
	    {
		if (Cons_left[i]==Edges_left[j] && Cons_right[i]==Edges_right[j])
		{
			Weights_Pair[j]=-1;
			break;
		}
	    }
	}
	for (i=0;i<DAG_weights.size();i++)
	{
		Unused_junctions.push_back(i);
	}
    while (Unused_junctions.size()>0)
//  for (t=0;t<3;t++)
   {
	max_weights=0.0;
	seed=Unused_junctions[0];
	Extension_left.clear();
	Extension_right.clear();
	Extension_num_0.clear();
        Extension_num_1.clear();
	for (i=0;i<Unused_junctions.size();i++)
	{
		if (DAG_weights[Unused_junctions[i]]>max_weights)
		{
			max_weights=DAG_weights[Unused_junctions[i]];
			seed=Unused_junctions[i];
		}
	}
	if (DAG_weights[seed]<Filter)
	{
		break;
	}
	Unused_junctions.erase(remove(Unused_junctions.begin(),Unused_junctions.end(),seed),Unused_junctions.end());
	SEED=seed;
	for (i=0;i<Weights_Pair.size();i++)
	{
		if (seed==Edges_right[i] && Weights_Pair[i]==0)
		{
			Extension_num_0.push_back(Edges_left[i]);
		}
		else if (seed==Edges_right[i] && Weights_Pair[i]==-1)
		{
			Extension_num_1.push_back(Edges_left[i]);
		}
	}
	for (i=0;i<Extension_num_0.size();i++)
	{
		pos=find(Used_junctions.begin(),Used_junctions.end(),Extension_num_0[i]);
		if (pos==Used_junctions.end())
		{
			Extension_num_unused.push_back(Extension_num_0[i]);
		}
	}
	if (Extension_num_unused.size()>0)
	{
		Extension_num_0=Extension_num_unused;
		Extension_num_unused.clear();
	}
	else
	{
		Extension_num_unused.clear();
	}
	while (Extension_num_0.size()>0 || Extension_num_1.size()>0)
	{
		max_weights=0.0;
		if (Extension_num_1.size()>0)
		{
			seed_temp=Extension_num_1[0];
			for (i=0;i<Extension_num_1.size();i++)
			{
				if (DAG_weights[Extension_num_1[i]]>max_weights)
				{
					max_weights=DAG_weights[Extension_num_1[i]];
					seed_temp=Extension_num_1[i];
				}
			}
		}
		else
		{
			seed_temp=Extension_num_0[0];
			for (i=0;i<Extension_num_0.size();i++)
                         {
                                 if (DAG_weights[Extension_num_0[i]]>max_weights)
                                 {
                                         max_weights=DAG_weights[Extension_num_0[i]];
                                         seed_temp=Extension_num_0[i];
                                 }
                         }
		}
		Extension_left.push_back(seed_temp);
		Unused_junctions.erase(remove(Unused_junctions.begin(),Unused_junctions.end(),seed_temp),Unused_junctions.end());
		Used_junctions.push_back(seed_temp);
		seed=seed_temp;
		Extension_num_0.clear();
		Extension_num_1.clear();
	for (i=0;i<Weights_Pair.size();i++)
	{
		if (seed==Edges_right[i] && Weights_Pair[i]==0)
		{
			Extension_num_0.push_back(Edges_left[i]);
		}
		else if (seed==Edges_right[i] && Weights_Pair[i]==-1)
		{
			Extension_num_1.push_back(Edges_left[i]);
		}
	}
	for (i=0;i<Extension_num_0.size();i++)
	{
		pos=find(Used_junctions.begin(),Used_junctions.end(),Extension_num_0[i]);
		if (pos==Used_junctions.end())
		{
			Extension_num_unused.push_back(Extension_num_0[i]);
		}
	}
	if (Extension_num_unused.size()>0)
	{
		Extension_num_0=Extension_num_unused;
		Extension_num_unused.clear();
	}
	else
	{
		Extension_num_unused.clear();
	}
	}
	seed=SEED;
	Extension_num_0.clear();
	Extension_num_1.clear();
	for (i=0;i<Weights_Pair.size();i++)
	{
		if (seed==Edges_left[i] && Weights_Pair[i]==0)
		{
			Extension_num_0.push_back(Edges_right[i]);
		}
		else if (seed==Edges_left[i] && Weights_Pair[i]==-1)
		{
			Extension_num_1.push_back(Edges_right[i]);
		}
	}
	for (i=0;i<Extension_num_0.size();i++)
	{
		pos=find(Used_junctions.begin(),Used_junctions.end(),Extension_num_0[i]);
		if (pos==Used_junctions.end())
		{
			Extension_num_unused.push_back(Extension_num_0[i]);
		}
	}
	if (Extension_num_unused.size()>0)
	{
		Extension_num_0=Extension_num_unused;
		Extension_num_unused.clear();
	}
	else
	{
		Extension_num_unused.clear();
	}
	while (Extension_num_0.size()>0 || Extension_num_1.size()>0)
	{
		max_weights=0.0;
		if (Extension_num_1.size()>0)
		{
			seed_temp=Extension_num_1[0];
			for (i=0;i<Extension_num_1.size();i++)
			{
				if (DAG_weights[Extension_num_1[i]]>max_weights)
				{
					max_weights=DAG_weights[Extension_num_1[i]];
					seed_temp=Extension_num_1[i];
				}
			}
		}
		else
		{
			seed_temp=Extension_num_0[0];
			for (i=0;i<Extension_num_0.size();i++)
                         {
                                 if (DAG_weights[Extension_num_0[i]]>max_weights)
                                 {
                                         max_weights=DAG_weights[Extension_num_0[i]];
                                         seed_temp=Extension_num_0[i];
                                 }
                         }
		}
		Extension_right.push_back(seed_temp);
		Unused_junctions.erase(remove(Unused_junctions.begin(),Unused_junctions.end(),seed_temp),Unused_junctions.end());
		Used_junctions.push_back(seed_temp);
		seed=seed_temp;
		Extension_num_0.clear();
		Extension_num_1.clear();
	for (i=0;i<Weights_Pair.size();i++)
	{
		if (seed==Edges_left[i] && Weights_Pair[i]==0)
		{
			Extension_num_0.push_back(Edges_right[i]);
		}
		else if (seed==Edges_left[i] && Weights_Pair[i]==-1)
		{
			Extension_num_1.push_back(Edges_right[i]);
		}
	}
	for (i=0;i<Extension_num_0.size();i++)
	{
		pos=find(Used_junctions.begin(),Used_junctions.end(),Extension_num_0[i]);
		if (pos==Used_junctions.end())
		{
			Extension_num_unused.push_back(Extension_num_0[i]);
		}
	}
	if (Extension_num_unused.size()>0)
	{
		Extension_num_0=Extension_num_unused;
		Extension_num_unused.clear();
	}
	else
	{
		Extension_num_unused.clear();
	}
    }
	for (i=0;i<Extension_left.size();i++)
	{
		Extension.push_back(Extension_left[Extension_left.size()-1-i]);
	}
	Extension.push_back(SEED);
	for (i=0;i<Extension_right.size();i++)
	{
		Extension.push_back(Extension_right[i]);
	}
/*
for (i=0;i<Extension.size();i++)
{
	cout<<Extension[i]<<"->";
}
cout<<endl;
*/
	Junction_Path_cover.push_back(Extension);
	Extension.clear();
	Extension_left.clear();
	Extension_right.clear();
    }//end of while (Unused_junctions.size()>0)
	

}
#endif







