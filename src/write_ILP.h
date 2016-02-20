#ifndef WRITE_ILP_H
#define WRITE_ILP_H

//write_ILP.h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
using namespace std;
void Write_ILP_L(vector<int> Edges_left,vector<int> Edges_right,vector<float> Weights,vector<int> Line_Graph,int max_ind_set_pos,vector<int> Inhibit_left,vector<int> Inhibit_middle,vector<int> Inhibit_right,char * path,char * path2)
{
vector<int> arcs_exon_out;
vector<int> arcs_exon_in;
vector<float> weights_exon_out;
vector<float> weights_exon_in;
vector<int> Inhibit_exon_out;
vector<int> Inhibit_exon_in;
int i,j,k,num_i,num_j,sum_num=0;
arcs_exon_out.clear();
arcs_exon_in.clear();
weights_exon_in.clear();
weights_exon_out.clear();
if (Weights.size()>0)
{

	for (i=0;i<Edges_left.size();i++)
	{
		if (Edges_left[i]==Line_Graph[max_ind_set_pos])
		{
			arcs_exon_out.push_back(Edges_right[i]);
			weights_exon_out.push_back(Weights[i]);
		}
		if (Edges_right[i]==Line_Graph[max_ind_set_pos])
		{
			arcs_exon_in.push_back(Edges_left[i]);
			weights_exon_in.push_back(Weights[i]);
		}
	}
	num_i=arcs_exon_in.size();
	num_j=arcs_exon_out.size();
	if (num_i==0)
	{
	    ofstream outfile(path);
	    streambuf *streams=cout.rdbuf();
	    cout.rdbuf(outfile.rdbuf());
	    cout<<"";
	    cout.rdbuf(streams);
	    //Begin writing to Result_L.txt
	    ofstream file;
	    file.open(path2,ios_base::app);
	    for (i=0;i<=max_ind_set_pos;i++)
    	{
			arcs_exon_out.clear();
			weights_exon_out.clear();
			for (j=0;j<Edges_left.size();j++)
			{
			    if (Edges_left[j]==Line_Graph[i])
    			{
	    			arcs_exon_out.push_back(Edges_right[j]);
		    		weights_exon_out.push_back(Weights[j]);
			    }
			}
			for (j=0;j<arcs_exon_out.size();j++)
			{
				file<<2<<":"<<Line_Graph[i]<<"->"<<arcs_exon_out[j]<<":"<<weights_exon_out[j]<<";"<<endl;
			}
	    }
		file.close();
	}
	else
	{
    	for (i=0;i<Inhibit_middle.size();i++)
	    {
	    	if (Inhibit_middle[i]==Line_Graph[max_ind_set_pos])
	    	{
		    	for (j=0;j<arcs_exon_in.size();j++)
		    	{
		    		for (k=0;k<arcs_exon_out.size();k++)
		    		{
			    		if (arcs_exon_in[j]==Inhibit_left[i] && arcs_exon_out[k]==Inhibit_right[i])
				    	{
					    	Inhibit_exon_in.push_back(j);
					    	Inhibit_exon_out.push_back(k);
					    }
			    	}
		    	}
	    	}
	    }
	ofstream outfile(path);
	streambuf *streams=cout.rdbuf();
	cout.rdbuf(outfile.rdbuf());
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			cout<<"var x_"<<i<<"_"<<j<<", integer;"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<=k)
				cout<<"var x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<", integer;"<<endl;
			}
		}
	}

	cout<<"minimize z: ";
	sum_num=sum_num+1;
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			cout<<weights_exon_out[j]*weights_exon_out[j]<<"*x_"<<i<<"_"<<j<<"_"<<i<<"_"<<j<<"+";
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<k)
					cout<<2*weights_exon_out[j]*weights_exon_out[k]<<"*x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<"+";
			}
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			if (i<num_i-1 || j<num_j-1)
		    	cout<<"("<<-2*weights_exon_in[i]*weights_exon_out[j]<<"*x_"<<i<<"_"<<j<<")+";
			else
				cout<<"("<<-2*weights_exon_in[i]*weights_exon_out[j]<<"*x_"<<i<<"_"<<j<<");"<<endl;;
		}
	}
	for (j=0;j<num_j;j++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		for (i=0;i<num_i;i++)
		{
			if (i<num_i-1)
		    	cout<<"x_"<<i<<"_"<<j<<"+";
			else
				cout<<"x_"<<i<<"_"<<j<<"="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		for (j=0;j<num_j;j++)
		{
			if (j<num_j-1)
				cout<<"x_"<<i<<"_"<<j<<"+";
			else
				cout<<"x_"<<i<<"_"<<j<<">="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<"<="<<"x_"<<i<<"_"<<j<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<"<="<<"x_"<<i<<"_"<<k<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"+"<<"x_"<<i<<"_"<<k<<"-"<<1<<"<="<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<";"<<endl;
				}
				else if (j==k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<"<="<<"x_"<<i<<"_"<<j<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"+"<<"x_"<<i<<"_"<<k<<"-"<<1<<"<="<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<";"<<endl;
				}
			}
		}
	}

	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			cout<<"s.t. con"<<sum_num<<": ";
			sum_num=sum_num+1;
			cout<<"x_"<<i<<"_"<<j<<">="<<0<<";"<<endl;
			cout<<"s.t. con"<<sum_num<<": ";
			sum_num=sum_num+1;
			cout<<"x_"<<i<<"_"<<j<<"<="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<=k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
				    cout<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<">="<<0<<";"<<endl;
				    cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<i<<"_"<<j<<"_"<<i<<"_"<<k<<"<="<<1<<";"<<endl;
				}
			}
		}
	}
	for (i=0;i<Inhibit_exon_in.size();i++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		cout<<"x_"<<Inhibit_exon_in[i]<<"_"<<Inhibit_exon_out[i]<<"="<<0<<";"<<endl;
	}
	cout<<"end;"<<endl;
	cout.rdbuf(streams);
		}
}//exon number > 1

}//end Write_ILP_L

void Write_ILP_R(vector<int> Edges_left,vector<int> Edges_right,vector<float> Weights,vector<int> Line_Graph,int max_ind_set_pos,vector<int> Inhibit_left,vector<int> Inhibit_middle,vector<int> Inhibit_right,char * path,char * path2)
{
vector<int> arcs_exon_out;
vector<int> arcs_exon_in;
vector<float> weights_exon_out;
vector<float> weights_exon_in;
vector<int> Inhibit_exon_in;
vector<int> Inhibit_exon_out;
int i,j,k,num_i,num_j,sum_num=0;
arcs_exon_out.clear();
arcs_exon_in.clear();
weights_exon_in.clear();
weights_exon_out.clear();
if (Weights.size()>0)
{
	for (i=0;i<Edges_left.size();i++)
	{
		if (Edges_left[i]==Line_Graph[max_ind_set_pos+1])
		{
			arcs_exon_out.push_back(Edges_right[i]);
			weights_exon_out.push_back(Weights[i]);
		}
		if (Edges_right[i]==Line_Graph[max_ind_set_pos+1])
		{
			arcs_exon_in.push_back(Edges_left[i]);
			weights_exon_in.push_back(Weights[i]);
		}
	}
	num_i=arcs_exon_out.size();
	num_j=arcs_exon_in.size();
    if (num_i==0)
	{
	    ofstream outfile(path);
	    streambuf *streams=cout.rdbuf();
	    cout.rdbuf(outfile.rdbuf());
	    cout<<"";
	    cout.rdbuf(streams);
	    //Begin writing to Result_R.txt
	    ofstream file;
	    file.open(path2,ios_base::app);
	    for (i=max_ind_set_pos+1;i<Line_Graph.size();i++)
    	{
			arcs_exon_in.clear();
			weights_exon_in.clear();
			for (j=0;j<Edges_left.size();j++)
			{
			    if (Edges_right[j]==Line_Graph[i])
    			{
	    			arcs_exon_in.push_back(Edges_left[j]);
		    		weights_exon_in.push_back(Weights[j]);
			    }
			}
			if (arcs_exon_in.size()>=1)
			{
			    for (j=0;j<arcs_exon_in.size();j++)
		     	{
			    	file<<2<<":"<<arcs_exon_in[j]<<"->"<<Line_Graph[i]<<":"<<weights_exon_in[j]<<";"<<endl;
			    }
			}
			if (arcs_exon_in.size()==0)
			{
				file<<1<<":"<<Line_Graph[i]<<";"<<endl;
			}
	    }
		file.close();
	}
	else
	{
    	for (i=0;i<Inhibit_middle.size();i++)
	    {
	    	if (Inhibit_middle[i]==Line_Graph[max_ind_set_pos+1])
	    	{
		    	for (j=0;j<arcs_exon_in.size();j++)
		    	{
		    		for (k=0;k<arcs_exon_out.size();k++)
		    		{
			    		if (arcs_exon_in[j]==Inhibit_left[i] && arcs_exon_out[k]==Inhibit_right[i])
				    	{
					    	Inhibit_exon_in.push_back(j);
					    	Inhibit_exon_out.push_back(k);
					    }
			    	}
		    	}
	    	}
	    }
	ofstream outfile(path);
	streambuf *streams=cout.rdbuf();
	cout.rdbuf(outfile.rdbuf());
	for (j=0;j<num_j;j++)
	{
		for (i=0;i<num_i;i++)
		{
			cout<<"var x_"<<j<<"_"<<i<<", integer;"<<endl;
		}
	}
	for (j=0;j<num_j;j++)
	{
		for (i=0;i<num_i;i++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<=k)
				cout<<"var x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<", integer;"<<endl;
			}
		}
	}

	cout<<"minimize z: ";
	sum_num=sum_num+1;
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			cout<<weights_exon_in[j]*weights_exon_in[j]<<"*x_"<<j<<"_"<<i<<"_"<<j<<"_"<<i<<"+";
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<k)
					cout<<2*weights_exon_in[j]*weights_exon_in[k]<<"*x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<"+";
			}
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			if (i<num_i-1 || j<num_j-1)
		    	cout<<"("<<-2*weights_exon_out[i]*weights_exon_in[j]<<"*x_"<<j<<"_"<<i<<")+";
			else
				cout<<"("<<-2*weights_exon_out[i]*weights_exon_in[j]<<"*x_"<<j<<"_"<<i<<");"<<endl;;
		}
	}
	for (j=0;j<num_j;j++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		for (i=0;i<num_i;i++)
		{
			if (i<num_i-1)
		    	cout<<"x_"<<j<<"_"<<i<<"+";
			else
				cout<<"x_"<<j<<"_"<<i<<"="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		for (j=0;j<num_j;j++)
		{
			if (j<num_j-1)
				cout<<"x_"<<j<<"_"<<i<<"+";
			else
				cout<<"x_"<<j<<"_"<<i<<">="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<"<="<<"x_"<<j<<"_"<<i<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<"<="<<"x_"<<k<<"_"<<i<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"+"<<"x_"<<k<<"_"<<i<<"-"<<1<<"<="<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<";"<<endl;
				}
				else if (j==k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<"<="<<"x_"<<j<<"_"<<i<<";"<<endl;
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"+"<<"x_"<<k<<"_"<<i<<"-"<<1<<"<="<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<";"<<endl;
				}
			}
		}
	}

	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			cout<<"s.t. con"<<sum_num<<": ";
			sum_num=sum_num+1;
			cout<<"x_"<<j<<"_"<<i<<">="<<0<<";"<<endl;
			cout<<"s.t. con"<<sum_num<<": ";
			sum_num=sum_num+1;
			cout<<"x_"<<j<<"_"<<i<<"<="<<1<<";"<<endl;
		}
	}
	for (i=0;i<num_i;i++)
	{
		for (j=0;j<num_j;j++)
		{
			for (k=0;k<num_j;k++)
			{
				if (j<=k)
				{
					cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
				    cout<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<">="<<0<<";"<<endl;
				    cout<<"s.t. con"<<sum_num<<": ";
					sum_num=sum_num+1;
					cout<<"x_"<<j<<"_"<<i<<"_"<<k<<"_"<<i<<"<="<<1<<";"<<endl;
				}
			}
		}
	}
	for (i=0;i<Inhibit_exon_in.size();i++)
	{
		cout<<"s.t. con"<<sum_num<<": ";
		sum_num=sum_num+1;
		cout<<"x_"<<Inhibit_exon_in[i]<<"_"<<Inhibit_exon_out[i]<<"="<<0<<";"<<endl;
	}
	cout<<"end;"<<endl;
	cout.rdbuf(streams);
	}



}//exon number > 1
}//end of write ILP_R

void Write_ILP_Result(vector<int> Edges_left,vector<int> Edges_right,vector<int> Line_Graph,int max_ind_set_pos,char * path1,char * path2)
{
	int i,j,k,exon_num,num_i,num_j;
	vector<int> arcs_exon_out;
	vector<int> arcs_exon_in;
for (i=0;i<Edges_left.size();i++)
{
    if (Edges_left[i]==Line_Graph[max_ind_set_pos])
	{
		arcs_exon_out.push_back(Edges_right[i]);
	}
	if (Edges_right[i]==Line_Graph[max_ind_set_pos])
	{
		arcs_exon_in.push_back(Edges_left[i]);
	}
}
	num_i=arcs_exon_in.size();
	num_j=arcs_exon_out.size();
	ifstream ifs(path1);
	string temp;
	string read_string;
	vector<string> vec;
	vec.clear();
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
	ofstream file;
	file.open(path2,ios_base::app);
	for (i=0;i<vec.size();i++)
	{
		for (j=0;j<vec[i].size();j++)
		{
			if (vec[i][j]=='_')
			{
				for (k=2;k<vec[i].size()-10;k++)
				{
					if (vec[i][j+k]=='_')
					{
						read_string=vec[i].substr(j+1,k-1);
						exon_num=atoi(read_string.c_str());
						file<<arcs_exon_in[exon_num]<<"->"<<Line_Graph[max_ind_set_pos]<<"->";
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
						exon_num=atoi(read_string.c_str());
						file<<arcs_exon_out[exon_num]<<endl;
						break;
					}
				}
				}
			}
		}
	}
	file.close();

}



#endif







