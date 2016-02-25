#ifndef GET_JUNCTION_GRAPH
#define GET_JUNCTION_GRAPH

//get_junction_graph.h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<cmath>
using namespace std;

class Get_Junction_Graph
{
	private:
	char * path;
	public:
	Get_Junction_Graph(char * p);
	~Get_Junction_Graph();
	void Construct_Junction_Graph();// attention: an edge in junction graph represents two conseccutive edges in Raw graph.
	vector<int> Edges_left;// left node of an edge in junction graph
	vector<int> Edges_right;// right node of an edge in junction graph
	vector<int> DAG_left;// denote the two nodes( left one) of a junction edge (a node in junction graph).
	vector<int> DAG_right;// denote the two nodes (right one) of a junction edge (a node in junction graph).
	vector<double> DAG_weights;
	vector<double> Weights;//weights of an edge in junction graph
	vector<int> Cons_left;
	vector<int> Cons_right;
	vector<vector<int> > P_in;
	vector<int> S,T;
	vector<int> Single_nodes;
	int junction_num,max_node_num;
};
Get_Junction_Graph::Get_Junction_Graph(char * p)
{
	path=p;
	return;
}
Get_Junction_Graph::~Get_Junction_Graph()
{
	return;
}
void Get_Junction_Graph::Construct_Junction_Graph()
{
	ifstream ifs(path);
	string temp;
	int i,j,k,t,sum_j,max_num,temp_max,power;
	double min_weights,sum_weights,temp_sum;
	vector<string> vec_edges;
	vector<string> vec_pairs;
	vector<string> edge_left,edge_right,weight;
	vector<double> temp_weights;
	vector<int> Pair_edges,Pair_nodes,Pair_num;
	vector<int>::iterator pos_left;
	vector<int>::iterator pos_right;
	vector<int>::iterator pos;
	vector<int> temp_in,temp_out,temp_cons;
	vector<vector<int> > Cons_edges;
	vector<vector<int> >::iterator iter;
	vector<double> TEMP_weights;
	int index_in,index_out;
	double index_weights=100000000000000000;
	junction_num=-1;
	max_node_num=-1;
	getline(ifs,temp);
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Inhibit_edges **")
			break;
		else
			vec_edges.push_back(temp);
	}
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Pair_edges **")
			break;
	}
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Nodes **")
			break;
		else
			vec_pairs.push_back(temp);
	}
	ifs.close();
	if (vec_edges.size()>=1)
	{
		for (i=0;i<vec_edges.size();i++)
		{
			for (j=0;j<vec_edges[i].size();j++)
			{
				if (j<vec_edges[i].size()-1 && vec_edges[i][j+1]=='-')
				{
					if (j==0)
					{
						edge_left.push_back(vec_edges[i].substr(j,1));
					}
					else
					{
						sum_j=1;
						while (1)
						{
							if ((j-sum_j==0) || (j-sum_j!=0 && vec_edges[i][j-sum_j]==' '))
								break;
							else
								sum_j++;
						}
						if (j-sum_j==0)
							edge_left.push_back(vec_edges[i].substr(j-sum_j,sum_j+1));
						else
							edge_left.push_back(vec_edges[i].substr(j-sum_j+1,sum_j));
					}
				}
				if (j>1 && vec_edges[i][j-1]=='>')
				{
					sum_j=1;
					while (1)
					{
						if (vec_edges[i][j+sum_j]==':')
							break;
						else
							sum_j++;
					}
					edge_right.push_back(vec_edges[i].substr(j,sum_j));
				}
				if (j>1 && vec_edges[i][j-1]==':')
				{
					sum_j=1;
					while (1)
					{
						if (vec_edges[i][j+sum_j]==',' || vec_edges[i][j+sum_j]==';')
							break;
						else
							sum_j++;
					}
					weight.push_back(vec_edges[i].substr(j,sum_j));
				}
			}
		}
	if (vec_pairs.size()>=1)
	{
		for (i=0;i<vec_pairs.size();i++)
		{
			for (j=0;j<vec_pairs[i].size();j++)
			{
				if (vec_pairs[i][j]=='-')
				{
					Pair_edges.push_back(atoi(vec_pairs[i].substr(0,j).c_str()));
					for (k=j+2;k<vec_pairs[i].size();k++)
					{
						if (vec_pairs[i][k]=='-')
						{
							Pair_nodes.push_back(atoi(vec_pairs[i].substr(j+2,k-j-2).c_str()));
							Pair_num.push_back(atoi(vec_pairs[i].substr(k+2,vec_pairs[i].size()-k-2).c_str()));
						        break;
						}
					}
					break;
				}
			}
		}
	}//end of if (vec_pairs.size()>=1)
	for (i=0;i<weight.size();i++)
	{
		DAG_left.push_back(atoi(edge_left[i].c_str()));
		DAG_right.push_back(atoi(edge_right[i].c_str()));
		DAG_weights.push_back(atof(weight[i].c_str()));
	}
	pos_left=max_element(DAG_left.begin(),DAG_left.end());
	pos_right=max_element(DAG_right.begin(),DAG_right.end());
	if (*pos_left>*pos_right)
	{
		max_num=*pos_left;
	}
	else
	{
		max_num=*pos_right;
	}
	max_node_num=max_num;
	for (i=0;i<=max_num;i++)
	{
		temp_in.clear();
		temp_out.clear();
		for (j=0;j<DAG_weights.size();j++)
		{
			if (DAG_right[j]==i)
			{
				temp_in.push_back(j);
			}
			else if (DAG_left[j]==i)
			{
				temp_out.push_back(j);
			}
		}
		if (temp_in.size()>1 && temp_out.size()>1)
		{
			if (temp_in.size()>temp_out.size())
			{
				temp_max=temp_in.size();
			}
			else
			{
				temp_max=temp_out.size();
			}
			power=1;
			for (j=0;j<temp_in.size()*temp_out.size();j++)
			{
				power=2*power;
			}
			for (j=0;j<power;j++)
			{
		                temp_weights.clear();
				int a=j;
				for (k=0;a>0;k++)
				{
					temp_weights.push_back(a%2);
					a/=2;
				}
				if (k!=temp_in.size()*temp_out.size())
				{
					for (t=0;t<temp_in.size()*temp_out.size()-k;t++)
					{
						temp_weights.push_back(0);
					}
				}
				temp_sum=0;
				for (k=0;k<temp_weights.size();k++)
				{
					temp_sum=temp_sum+temp_weights[k];
				}
				if (temp_sum==temp_max)
				{
					index_in=0;
					index_out=0;
					for (k=0;k<temp_in.size();k++)
					{
						temp_sum=0;
						for (t=0;t<temp_out.size();t++)
						{
							temp_sum=temp_sum+temp_weights[temp_out.size()*k+t];
						}
						if (temp_sum==0)
						{
							break;
						}
						else
						{
							index_in++;
						}
					}
					for (k=0;k<temp_out.size();k++)
					{
						temp_sum=0;
						for (t=0;t<temp_in.size();t++)
						{
							temp_sum=temp_sum+temp_weights[k+temp_out.size()*t];
						}
						if (temp_sum==0)
						{
							break;
						}
						else
						{
							index_out++;
						}
					}
					if (index_in==temp_in.size() && index_out==temp_out.size())
					{
						min_weights=0;
						if (temp_in.size()>=temp_out.size())
						{
							for (k=0;k<temp_out.size();k++)
							{
						                sum_weights=0;
								for (t=0;t<temp_in.size();t++)
								{
									sum_weights=sum_weights+temp_weights[k+temp_out.size()*t]*DAG_weights[temp_in[t]];
								}
								min_weights=min_weights+(DAG_weights[temp_out[k]]-sum_weights)*(DAG_weights[temp_out[k]]-sum_weights);
							}
					/*		for (k=0;k<temp_weights.size();k++)
							{
								cout<<temp_weights[k]<<"; ";
							}
							cout<<"min_weights="<<min_weights<<"; "<<endl;
							*/
					                if (min_weights<index_weights)
					                {
						                index_weights=min_weights;
						                TEMP_weights.clear();
						                TEMP_weights=temp_weights;
					                }
						}
						else
						{
							for (k=0;k<temp_in.size();k++)
							{
								sum_weights=0;
								for (t=0;t<temp_out.size();t++)
								{
									sum_weights=sum_weights+temp_weights[temp_out.size()*k+t]*DAG_weights[temp_out[t]];
								}
								min_weights=min_weights+(DAG_weights[temp_in[k]]-sum_weights)*(DAG_weights[temp_in[k]]-sum_weights);
							}
					/*		for (k=0;k<temp_weights.size();k++)
							{
								cout<<temp_weights[k]<<"; ";
							}
							cout<<"min_weights="<<min_weights<<"; "<<endl;
							*/
					                if (min_weights<index_weights)
					                {
						                index_weights=min_weights;
						                TEMP_weights.clear();
						                TEMP_weights=temp_weights;
					                }
						}
					}
				}//end of if (temp_sum==temp_max)
			}//end of for (j=0;j<pow((float)2,temp_in.size()*temp_out.size());j++)
			for (j=0;j<temp_in.size();j++)
			{
				for (k=0;k<temp_out.size();k++)
				{
					Edges_left.push_back(temp_in[j]);
					Edges_right.push_back(temp_out[k]);
				}
			}
			for (j=0;j<TEMP_weights.size();j++)
			{
				Weights.push_back(1-TEMP_weights[j]);
			}
	                index_weights=100000000000000000;
		}//end of if (temp_in.size()>1 && temp_out.size()>1)
		else if ((temp_in.size()==1 || temp_out.size()==1) && (temp_in.size()>0 && temp_out.size()>0))
		{
			for (j=0;j<temp_in.size();j++)
			{
				for (k=0;k<temp_out.size();k++)
				{
					Edges_left.push_back(temp_in[j]);
					Edges_right.push_back(temp_out[k]);
					Weights.push_back(0);
				}
			}
		}
		else if (temp_in.size()==0 && temp_out.size()>0)
		{
			for (j=0;j<temp_out.size();j++)
			{
				S.push_back(temp_out[j]);
				pos=find(DAG_left.begin(),DAG_left.end(),DAG_right[temp_out[j]]);
				if (pos==DAG_left.end() && temp_out[j]>junction_num)
				{
					junction_num=temp_out[j];
				}
			}
		}
		else if (temp_in.size()>0 && temp_out.size()==0)
		{
			for (j=0;j<temp_in.size();j++)
			{
				T.push_back(temp_in[j]);
				pos=find(DAG_right.begin(),DAG_right.end(),DAG_left[temp_in[j]]);
				if (pos==DAG_right.end() && temp_in[j]>junction_num)
				{
					junction_num=temp_in[j];
				}
			}
		}
		else
		{
			Single_nodes.push_back(i);
		}
	}
	// begin to process pairs
	for (i=0;i<Pair_num.size();i++)
	{
		for (j=0;j<DAG_left.size();j++)
		{
			if (DAG_left[j]==Pair_nodes[i] && DAG_right[j]==DAG_left[Pair_edges[i]])
			{
				Cons_left.push_back(j);
				Cons_right.push_back(Pair_edges[i]);
				break;
			}
			else if (DAG_left[j]==DAG_right[Pair_edges[i]] && DAG_right[j]==Pair_nodes[i])
			{
				Cons_left.push_back(Pair_edges[i]);
				Cons_right.push_back(j);
				break;
			}
		}
	}
	for (i=0;i<Cons_left.size();i++)
	{
		temp_cons.push_back(Cons_left[i]);
		temp_cons.push_back(Cons_right[i]);
		Cons_edges.push_back(temp_cons);
		temp_cons.clear();
	}
	Cons_left.clear();
	Cons_right.clear();
	sort(Cons_edges.begin(),Cons_edges.end());
	iter=unique(Cons_edges.begin(),Cons_edges.end());
	if (iter!=Cons_edges.end())
	{
		Cons_edges.erase(iter,Cons_edges.end());
	}
	for (i=0;i<Cons_edges.size();i++)
	{
		Cons_left.push_back(Cons_edges[i][0]);
		Cons_right.push_back(Cons_edges[i][1]);
	}
	Cons_edges.clear();
	}//end of if (vec_edges.size()>=1)

//////////////////////////////////////////////////////////////////////////////////////
/*        cout<<"**** Output ****"<<endl;
	cout<<"DAG Edges:"<<endl;
	for (i=0;i<DAG_weights.size();i++)
	{
		cout<<DAG_left[i]<<"->"<<DAG_right[i]<<endl;
	}
	cout<<"***************************************"<<endl;
	cout<<"Junction Edges:"<<endl;
	for (i=0;i<Edges_left.size();i++)
	{
		cout<<Edges_left[i]<<"->"<<Edges_right[i]<<": "<<Weights[i]<<endl;
	}
	cout<<"S:"<<endl;
	for (i=0;i<S.size();i++)
	{
		cout<<S[i]<<"; ";
	}
	cout<<endl;
	cout<<"T:"<<endl;
	for (i=0;i<T.size();i++)
	{
		cout<<T[i]<<"; ";
	}
	cout<<endl;
	cout<<"Single Nodes:"<<endl;
	for (i=0;i<Single_nodes.size();i++)
	{
		cout<<Single_nodes[i]<<"; ";
	}
	cout<<endl;
	cout<<"Pair information:"<<endl;
	for (i=0;i<Cons_left.size();i++)
	{
		cout<<Cons_left[i]<<"->"<<Cons_right[i]<<endl;
	}
*/
} //end of "Construct_Junction_Graph()"

#endif























