#ifndef LINE_GRAPH_H
#define LINE_GRAPH_H

//line_graph.h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
using namespace std;
class LineGraph
{
private:
char * path;
float low_cov;
float out_in_ratio;//1.2
float high_ratio;//2.0
public:
LineGraph(char * p,float low,float out,float high);
~LineGraph();
void Build_line_graph();
vector<int> Edges_left;
vector<int> Edges_right;
vector<int> Line_Graph;
vector<float> Weights;
vector<int> Inhibit_left;
vector<int> Inhibit_middle;
vector<int> Inhibit_right;
int max_ind_set_num;
int max_ind_set_pos;//position in Line_Graph, not exon number
vector<int> max_ind_set_arcs;//numbers in this vector are row numbers of Edges_left and Edges_right
};
LineGraph::LineGraph(char * p,float low,float out,float high)
{
path=p;
low_cov=low;
out_in_ratio=out;
high_ratio=high;
return;
}
LineGraph::~LineGraph()
{
return;
}
void LineGraph::Build_line_graph()
{
vector<string> vec;
vector<string> vec_pair_1;
vector<string> vec_pair_2;
vector<string> weight;
vector<string> edge_left;
vector<string> edge_right;
vector<int> Line_graph_head;
vector<int> Line_graph_end;
vector<int> del_row;
vector<int> Edges_middle_left;
vector<int> Edges_middle_right;
vector<int> Edges_middle_row;
vector<int> Edges_left_new;
vector<int> Edges_right_new;
vector<int> max_ind_set_arcs_middle;
vector<float> weights_exon_out;
vector<float> weights_exon_in;
vector<int> Line_Graph_New;
vector<int>::iterator pos_left;
vector<int>::iterator pos_right;
vector<string> inhibit_left;
vector<string> inhibit_right;
vec.clear();
weight.clear();
Weights.clear();
edge_left.clear();
edge_right.clear();
Edges_left.clear();
Edges_right.clear();
Line_Graph.clear();
Line_graph_head.clear();
Line_graph_end.clear();
del_row.clear();
Edges_middle_left.clear();
Edges_middle_right.clear();
Edges_middle_row.clear();
max_ind_set_arcs.clear();
max_ind_set_arcs_middle.clear();
ifstream ifs(path);
string temp;
string terminal="** Inhibit_edges **";
int i,j,k,t,sum_j;
int size_vec;
int len_vec;
int max_node;
float cov_out;
float cov_in;
float ratio=low_cov*(high_ratio-out_in_ratio);//ratio is coefficient k
while (1)
{
    getline(ifs,temp);
    if (temp==terminal)
        break;
    else
        vec.push_back(temp);
}
while (1)
{
	getline(ifs,temp);
	if (temp=="** Pair_edges **")//|| temp=="end;")
	{
		vec_pair_2.clear();
		break;
	}
	else if (temp=="end;")
	{
		vec_pair_2.clear();
		vec_pair_2=vec_pair_1;
		vec_pair_1.clear();
		break;
	}
	else
		vec_pair_1.push_back(temp);
}
ifs.close();
size_vec=vec.size();
if (size_vec>=2)
{
for (i=0;i<size_vec;i++)
{
     if (vec[i]!="** Edges **")
       {
        len_vec=vec[i].size();
        for (j=0;j<len_vec;j++)
        { //display edge_left
           if (j<len_vec-1 && vec[i][j+1]=='-')
            {
             if (j==0)
               edge_left.push_back(vec[i].substr(j,1));
             else
              {sum_j=1;
               while (1)
                  {
                   if ((j-sum_j==0) || (j-sum_j!=0 && vec[i][j-sum_j]==' '))
                      break;
                   else
                      sum_j=sum_j+1;
                  }
               if (j-sum_j==0)
                  edge_left.push_back(vec[i].substr(j-sum_j,sum_j+1));
               else
                  edge_left.push_back(vec[i].substr(j-sum_j+1,sum_j));
              }
             }
         //display edge_right
           if (j>1 && vec[i][j-1]=='>')
              {sum_j=1;
               while (1)
                  {
                   if (vec[i][j+sum_j]==':')
                      break;
                   else
                      sum_j=sum_j+1;
                  }
               edge_right.push_back(vec[i].substr(j,sum_j));
               }
            if (j>1 && vec[i][j-1]==':')
               {sum_j=1;
               while (1)
                   {
                    if (vec[i][j+sum_j]==',' || vec[i][j+sum_j]==';')
                       break;
                    else
                       sum_j=sum_j+1;
                   }
                weight.push_back(vec[i].substr(j,sum_j));
               }
       }
       }
}
for (i=0;i<weight.size();i++)
{
Edges_left.push_back(atoi(edge_left[i].c_str()));
Edges_right.push_back(atoi(edge_right[i].c_str()));
Weights.push_back(atof(weight[i].c_str()));
}
if (vec_pair_1.size()>0)
{
  for (i=0;i<vec_pair_1.size();i++)
  {
	for (j=0;j<vec_pair_1[i].size();j++)
	{
		if (vec_pair_1[i][j]=='-')
		{
			inhibit_left.push_back(vec_pair_1[i].substr(0,j));
			for (k=j+2;k<vec_pair_1[i].size();k++)
			{
				if (vec_pair_1[i][k]==';')
				{
					inhibit_right.push_back(vec_pair_1[i].substr(j+2,k-j-2));
				}
			}
			break;
		}
	}
  }
  for (i=0;i<inhibit_left.size();i++)
  {
	Inhibit_left.push_back(Edges_left[atoi(inhibit_left[i].c_str())]);
	Inhibit_middle.push_back(Edges_right[atoi(inhibit_left[i].c_str())]);
	Inhibit_right.push_back(Edges_right[atoi(inhibit_right[i].c_str())]);
  }
}
if (vec_pair_2.size()>0)
{
	for (i=0;i<vec_pair_2.size();i++)
	{
		for (j=0;j<vec_pair_2[i].size();j++)
		{
			if (vec_pair_2[i][j]=='-')
			{
				Inhibit_left.push_back(atoi(vec_pair_2[i].substr(0,j).c_str()));
				for (k=j+2;k<vec_pair_2[i].size();k++)
				{
					if (vec_pair_2[i][k]=='-')
					{
						Inhibit_middle.push_back(atoi(vec_pair_2[i].substr(j+2,k-j-2).c_str()));
						for (t=k+2;t<vec_pair_2[i].size();t++)
						{
							if (vec_pair_2[i][t]==';')
							{
								Inhibit_right.push_back(atoi(vec_pair_2[i].substr(k+2,t-k-2).c_str()));
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
}
//Start constructig ling_graph
pos_left=max_element(Edges_left.begin(),Edges_left.end());
pos_right=max_element(Edges_right.begin(),Edges_right.end());
if (*pos_left>*pos_right)
max_node=*pos_left;
else
max_node=*pos_right;
for (i=0;i<=max_node;i++)
{
    pos_left=find(Edges_left.begin(),Edges_left.end(),i);
    pos_right=find(Edges_right.begin(),Edges_right.end(),i);
    if(pos_left!=Edges_left.end() && pos_right!=Edges_right.end())   
    continue;
    else if (pos_left!=Edges_left.end() && pos_right==Edges_right.end())
    Line_graph_head.push_back(i);
    else
     Line_graph_end.push_back(i);
}//choose head and end nodes
for (i=0;i<Line_graph_head.size();i++)
{
    for (j=0;j<Edges_left.size();j++)
    {   
        if (Edges_left[j]==Line_graph_head[i])
           del_row.push_back(j);
    }   
}//delete rows from head nodes
for (i=0;i<Weights.size();i++)
{
    sum_j=0;
    for (j=0;j<del_row.size();j++)
    {
        if (i!=del_row[j])
           sum_j=sum_j+1;
    }
    if (sum_j==del_row.size())
    {
        Edges_middle_row.push_back(i);
        Edges_middle_left.push_back(Edges_left[i]);
        Edges_middle_right.push_back(Edges_right[i]);
    }
}//get rows still left
//begin while following:
while (Edges_middle_row.size())
{
for (i=0;i<=max_node;i++)
{
    pos_left=find(Edges_middle_left.begin(),Edges_middle_left.end(),i);
    pos_right=find(Edges_middle_right.begin(),Edges_middle_right.end(),i);
    if(pos_left!=Edges_middle_left.end() && pos_right!=Edges_middle_right.end())
    continue;
    else if (pos_left!=Edges_middle_left.end() && pos_right==Edges_middle_right.end())
    Line_graph_head.push_back(i);
}//choose head nodes
for (i=0;i<Line_graph_head.size();i++)
{
    for (j=0;j<Edges_middle_left.size();j++)
    { 
        if (Edges_middle_left[j]==Line_graph_head[i])
           del_row.push_back(Edges_middle_row[j]);
    } 
}//delete rows from head nodes
Edges_middle_row.clear();
Edges_middle_left.clear();
Edges_middle_right.clear();
for (i=0;i<Weights.size();i++)
{
    sum_j=0;
    for (j=0;j<del_row.size();j++)
    {
        if (i!=del_row[j])
           sum_j=sum_j+1;
    }
    if (sum_j==del_row.size())
    {    
       Edges_middle_row.push_back(i);
       Edges_middle_left.push_back(Edges_left[i]);
       Edges_middle_right.push_back(Edges_right[i]);
    }
}//get rows still left
}//end while here
for (i=0;i<Line_graph_end.size();i++)
{
    Line_graph_head.push_back(Line_graph_end[i]);
}//finish constructing Line_graph which is Line_graph_head
Line_Graph=Line_graph_head;
Edges_left_new=Edges_left;
Edges_right_new=Edges_right;
for (i=0;i<Line_Graph.size();i++)
{
	k=0;//have edges out
	t=0;//have edges in
	for (j=0;j<Edges_left.size();j++)
	{
	    if (Edges_left[j]==Line_Graph[i])
		{
			k=k+1;
			weights_exon_out.push_back(Weights[j]);
		}
		else if (Edges_right[j]==Line_Graph[i])
		{
			t=t+1;
			weights_exon_in.push_back(Weights[j]);
		}
	}
	if (k>=1 && t==0)
	{
		Edges_left_new.push_back(-1);
		Edges_right_new.push_back(Line_Graph[i]);
		cov_out=0;
		for (j=0;j<weights_exon_out.size();j++)
		{
			cov_out=cov_out+weights_exon_out[j];
		}
		Weights.push_back(cov_out);
	}
	else if (k==0 && t>=1)
	{
		Edges_left_new.push_back(Line_Graph[i]);
		Edges_right_new.push_back(1000);
		cov_in=0;
		for (j=0;j<weights_exon_in.size();j++)
		{
			cov_in=cov_in+weights_exon_in[j];
		}
		Weights.push_back(cov_in);
	}
	else if (k==0 && t==0)
	{
		Edges_left_new.push_back(-1);
		Edges_right_new.push_back(Line_Graph[i]);
		Weights.push_back(0.0);
		Edges_left_new.push_back(Line_Graph[i]);
		Edges_right_new.push_back(1000);
		Weights.push_back(0.0);
	}
	else if (k>=1 && t>=1)
	{
		cov_out=0;
		cov_in=0;
		for (j=0;j<weights_exon_out.size();j++)
		{
			cov_out=cov_out+weights_exon_out[j];
		}
		for (j=0;j<weights_exon_in.size();j++)
		{
			cov_in=cov_in+weights_exon_in[j];
		}
		if (cov_out>=low_cov && cov_in>=low_cov && cov_out/cov_in>=(out_in_ratio+ratio/cov_in))
		{
			Edges_left_new.push_back(-1);
			Edges_right_new.push_back(Line_Graph[i]);
			Weights.push_back(cov_out-cov_in);
		}
		else if (cov_out>=low_cov && cov_in>=low_cov && cov_in/cov_out>=(out_in_ratio+ratio/cov_out))
		{
			Edges_left_new.push_back(Line_Graph[i]);
			Edges_right_new.push_back(1000);
			Weights.push_back(cov_in-cov_out);
		}
	}
	weights_exon_out.clear();
	weights_exon_in.clear();
}
Edges_left=Edges_left_new;
Edges_right=Edges_right_new;
Line_Graph_New=Line_Graph;
Line_Graph.clear();
Line_Graph.push_back(-1);
for (i=0;i<Line_Graph_New.size();i++)
{
	Line_Graph.push_back(Line_Graph_New[i]);
}
Line_Graph.push_back(1000);
//begin searching max independent set and the set capacity
max_ind_set_num=0;
max_ind_set_pos=0;
for (i=0;i<Line_Graph.size()-1;i++)
{
    Edges_middle_left.clear();
    Edges_middle_right.clear();
    max_ind_set_arcs_middle.clear();
    for (j=0;j<=i;j++)
    {
        Edges_middle_left.push_back(Line_Graph[j]);
    }
    for (j=i+1;j<Line_Graph.size();j++)
    {
        Edges_middle_right.push_back(Line_Graph[j]);
    }
    sum_j=0;
    for (j=0;j<Weights.size();j++)
    {
        pos_left=find(Edges_middle_left.begin(),Edges_middle_left.end(),Edges_left[j]);
        pos_right=find(Edges_middle_right.begin(),Edges_middle_right.end(),Edges_right[j]);
        if (pos_left!=Edges_middle_left.end() && pos_right!=Edges_middle_right.end())
        {
           sum_j=sum_j+1;
           max_ind_set_arcs_middle.push_back(j);
        }
    }
    if (sum_j>max_ind_set_num)
    {
       max_ind_set_num=sum_j;
       max_ind_set_pos=i;
       max_ind_set_arcs.clear();
       max_ind_set_arcs=max_ind_set_arcs_middle;
    }
}

}//end of else, which number of exons > 1
}//end of Build

#endif








