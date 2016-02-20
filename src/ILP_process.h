#ifndef ILP_PROCESS_H
#define ILP_PROCESS_H

//ILP_process.h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
using namespace std;
class ILP_Process
{
	private:
	char * path1;
	char * path2;
	string str;
	public:
	ILP_Process(vector<int> edges_left,vector<int> edges_right,vector<float> weights,vector<int> line_graph,int Max_ind_set_pos,string str_1,char * p1,char * p2);
	~ILP_Process();
	void Display_ILP_Process();
	vector<int> Edges_left_new;
	vector<int> Edges_right_new;
	vector<float> Weights_new;
	int max_ind_set_pos;
	vector<int> Edges_left;
	vector<int> Edges_right;
	vector<float> Weights;
	vector<int> Line_Graph;
};//end of class defination
	ILP_Process::ILP_Process(vector<int> edges_left,vector<int> edges_right,vector<float> weights,vector<int> line_graph,int Max_ind_set_pos,string str_1,char * p1,char * p2)
	{
		path1=p1;
		path2=p2;
		str=str_1;
		Edges_left=edges_left;
		Edges_right=edges_right;
		Weights=weights;
		Line_Graph=line_graph;
		max_ind_set_pos=Max_ind_set_pos;
		return;
	}
	ILP_Process::~ILP_Process()
	{
		return;
	}
	void ILP_Process::Display_ILP_Process()
	{
		int i,j,k,exon_num1,exon_num2,num_i,num_j;
		vector<int> arcs_exon_out;
	    vector<int> arcs_exon_in;
		vector<float> arcs_exon_out_weights;
		vector<float> arcs_exon_in_weights;
		vector<int> arcs_exon_out_num;
		vector<int> arcs_exon_in_num;
		vector<int> exon_number_L;
		vector<int> exon_number_R;
		vector<int>::iterator pos_left;
		vector<int>::iterator pos_right;
	    vector<string> vec;
		vec.clear();
		arcs_exon_out.clear();
		arcs_exon_in.clear();
		arcs_exon_out_weights.clear();
		arcs_exon_in_weights.clear();
		arcs_exon_out_num.clear();
		arcs_exon_in_num.clear();
		exon_number_L.clear();
		exon_number_R.clear();
		Edges_left_new=Edges_left;
		Edges_right_new=Edges_right;
		Weights_new=Weights;
		for (i=0;i<Edges_left.size();i++)
		{   
		    if (Edges_left[i]==Line_Graph[max_ind_set_pos])
		    {   
		        arcs_exon_out.push_back(Edges_right[i]);
				arcs_exon_out_weights.push_back(Weights[i]);
				arcs_exon_out_num.push_back(i);
		    }
		    if (Edges_right[i]==Line_Graph[max_ind_set_pos])
		    {   
			    arcs_exon_in.push_back(Edges_left[i]);
				arcs_exon_in_weights.push_back(Weights[i]);
				arcs_exon_in_num.push_back(i);
			}
		}
		num_i=arcs_exon_in.size();
		num_j=arcs_exon_out.size();
	    ifstream ifs(path1);
	    string temp;
		string read_string;
		getline(ifs,temp);
	if (temp!="")
	{
		while (1)
	    {
		    getline(ifs,temp);
	        if (temp!="" && temp.substr(7,5)=="x_0_0")
			{
				vec.push_back(temp);
		        break;
			}
		}
		while (1)
		{
		    getline(ifs,temp);
		    if (temp.substr(7,9)=="x_0_0_0_0")
		        break;
		    else
		       {
				   vec.push_back(temp);
			   }
		}
		ifs.close();
		//cout<<"here!"<<endl;
	    ofstream file;
	    file.open(path2,ios_base::app);
		for (i=0;i<vec.size();i++)
		{
			for (j=0;j<vec[i].size();j++)
			{
				if (vec[i][j]=='_' && vec[i][35]=='1')
				{
					for (k=2;k<vec[i].size()-10;k++)
					{
						if (vec[i][j+k]=='_')
				        {
							read_string=vec[i].substr(j+1,k-1);
							exon_num1=atoi(read_string.c_str());
							exon_number_L.push_back(exon_num1);
						    file<<3<<":"<<arcs_exon_in[exon_num1]<<"->"<<Line_Graph[max_ind_set_pos]<<"->";
						    break;
						}
					}
					if (vec[i][j-1]!='x')
					{
					for (k=2;k<vec[i].size()-10;k++)
					{
						if (vec[i][j+k]==' ')
						{
							read_string=vec[i].substr(j+1,k-1);
							exon_num2=atoi(read_string.c_str());
							exon_number_R.push_back(exon_num2);
						    file<<arcs_exon_out[exon_num2]<<":";
							if (str=="left")
							{
							   file <<arcs_exon_out_weights[exon_num2]<<";"<<endl;
							}
							if (str=="right")
							{
								file <<arcs_exon_in_weights[exon_num1]<<";"<<endl;
							}
						    break;
						}
					}
					}
				}
			}
		}
		//cout<<"here"<<endl;
		file.close();
		pos_left=max_element(exon_number_L.begin(),exon_number_L.end());
		pos_right=max_element(exon_number_R.begin(),exon_number_R.end());
		int max_node_L=*pos_left;
		int max_node_R=*pos_right;
		int exon_freq=0;
	if (str=="left")
	{
		for (i=0;i<=max_node_L;i++)
		{
			for (j=0;j<exon_number_L.size();j++)
			{
				if (exon_number_L[j]==i)
				{
					exon_freq=exon_freq+1;
		  	        if (exon_freq==1)
			        {
				        Weights_new[arcs_exon_in_num[i]]=arcs_exon_out_weights[exon_number_R[j]];
			        }
					if (exon_freq>=2)
					{
						Edges_left_new.push_back(arcs_exon_in[i]);
						Edges_right_new.push_back(Line_Graph[max_ind_set_pos]);
						Weights_new.push_back(arcs_exon_out_weights[exon_number_R[j]]);
					}
				}
			}
			exon_freq=0;
		}
	}
	if (str=="right")
	{
		for (i=0;i<=max_node_R;i++)
		{
			for (j=0;j<exon_number_R.size();j++)
			{
				if (exon_number_R[j]==i)
				{
					exon_freq=exon_freq+1;
					if (exon_freq==1)
					{
						Weights_new[arcs_exon_out_num[i]]=arcs_exon_in_weights[exon_number_L[j]];
					}
					if (exon_freq>=2)
					{
						Edges_right_new.push_back(arcs_exon_out[i]);
						Edges_left_new.push_back(Line_Graph[max_ind_set_pos]);
						Weights_new.push_back(arcs_exon_in_weights[exon_number_L[j]]);
					}
				}
			}
			exon_freq=0;
	    }
	}
	}
	}//end of Display_ILP_Process()


#endif







