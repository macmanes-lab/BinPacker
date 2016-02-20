#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
#include"line_graph.h"
#include"write_ILP.h"
#include"ILP_process.h"
using namespace std;
//1:Iterater number;2:Raw_Splcing_Graph;3:New_Splicing_Graph;4:Line_Graph;5:raw_ILP_L;6:raw_ILP_R;7:result_ILP_L;8:result_ILP_R;9:Result_L;10:Result_R
int main(int argc, char *argv[])
{
if (argc!=15)
{
	cout<<"Error Parameters!"<<endl;
	return 0;
}
int i,j,k,t;
vector<int> Edges_left;
vector<int> Edges_right;
vector<float> Weights;
vector<int> Line_Graph;
vector<int> Inhibit_left;
vector<int> Inhibit_middle;
vector<int> Inhibit_right;
int max_ind_set_pos;
if (atoi(argv[10])==0)
{
	Edges_left.clear();
	Edges_right.clear();
	Weights.clear();
	Line_Graph.clear();
	max_ind_set_pos=0;
	vector<string> vec;
	vector<string> vec_pair;
	vec.clear();
	int len_vec,sum_i,sum_j;
	string temp;
	ifstream ifs;
    ifs.open(argv[3]);
	getline(ifs,temp);
   	if (temp=="")
	  {
        LineGraph linegraph(argv[1],atof(argv[12]),atof(argv[13]),atof(argv[14]));
        linegraph.Build_line_graph();
		if (linegraph.Weights.size()>=1)
		{
            Edges_left=linegraph.Edges_left;
            Edges_right=linegraph.Edges_right;
            Weights=linegraph.Weights;
            Line_Graph=linegraph.Line_Graph;
		    Inhibit_left=linegraph.Inhibit_left;
		    Inhibit_middle=linegraph.Inhibit_middle;
		    Inhibit_right=linegraph.Inhibit_right;
            max_ind_set_pos=linegraph.max_ind_set_pos;
        }
	  }
    else
	  {
		Line_Graph.clear();
		vec.push_back(temp);
	    while (1)
    	{
	    	getline(ifs,temp);
		    if (temp=="end;")
			    break;
		    else
			    vec.push_back(temp);
    	}
	    ifs.close();
	    for (i=0;i<vec.size();i++)
	    {
		    for (j=0;j<vec[i].size();j++)
		    {
			    if (vec[i][j]==';')
				    Line_Graph.push_back(atoi(vec[i].substr(0,j).c_str()));
		    }
	    }
	    for (i=0;i<vec[vec.size()-1].size();i++)
	    {
		    if (vec[vec.size()-1][i]=='!')
	            max_ind_set_pos=atoi(vec[vec.size()-1].substr(0,i).c_str());//end of line graph
	    }
	ifs.open(argv[2]);
	vec.clear();
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Inhibit_edges **")
			break;
		else
			vec.push_back(temp);
	}
	while (1)
	{
		getline(ifs,temp);
		if (temp=="end;")
			break;
		else
			vec_pair.push_back(temp);
	}
	ifs.close();
	for (i=0;i<vec.size();i++)
	{
		if (vec[i]!="** Edges **")
		{
			len_vec=vec[i].size();
			for (j=0;j<len_vec;j++)
			{
				if (j<len_vec-1 && vec[i][j+1]=='-')
				{
					if (j==0)
						Edges_left.push_back(atoi(vec[i].substr(j,1).c_str()));
					else
					{
						sum_j=1;
						while (1)
						{
							if ((j-sum_j==0) || (j-sum_j!=0 && vec[i][j-sum_j]==' '))
								break;
							else
								sum_j= sum_j+1;
						}
						if (j-sum_j==0)
							Edges_left.push_back(atoi(vec[i].substr(j-sum_j,sum_j+1).c_str()));
						else
							Edges_left.push_back(atoi(vec[i].substr(j-sum_j+1,sum_j).c_str()));
					}
				}
				if (j>1 && vec[i][j-1]=='>')
				{
					sum_j=1;
					while (1)
					{
						if (vec[i][j+sum_j]==':')
							break;
						else
							sum_j= sum_j+1;
					}
					Edges_right.push_back(atoi(vec[i].substr(j,sum_j).c_str()));
				}
				if (j>1 && vec[i][j-1]==':')
				{
					sum_j=1;
					while (1)
					{
						if (vec[i][j+sum_j]==',' || vec[i][j+sum_j]==';')
							break;
						else
							sum_j= sum_j+1;
					}
					Weights.push_back(atof(vec[i].substr(j,sum_j).c_str()));
				}
			}
		}
	}
	vec.clear();
	for (i=0;i<vec_pair.size();i++)
	{
		for (j=0;j<vec_pair[i].size();j++)
		{
			if (vec_pair[i][j]=='-')
			{
				Inhibit_left.push_back(atoi(vec_pair[i].substr(0,j).c_str()));
				for (k=j+2;k<vec_pair[i].size();k++)
				{
					if (vec_pair[i][k]=='-')
					{
						Inhibit_middle.push_back(atoi(vec_pair[i].substr(j+2,k-j-2).c_str()));
						for (t=k+2;t<vec_pair[i].size();t++)
						{
							if (vec_pair[i][t]==';')
							{
								Inhibit_right.push_back(atoi(vec_pair[i].substr(k+2,t-k-2).c_str()));
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
	    ofstream file;
	    file.open(argv[2]);
	    file<<"** Edges **"<<endl;
	    for (i=0;i<Weights.size();i++)
    	{
	    	file<<Edges_left[i]<<"->"<<Edges_right[i]<<":"<<Weights[i]<<";"<<endl;
        }
		file<<"** Inhibit_edges **"<<endl;
		for (i=0;i<Inhibit_left.size();i++)
		{
			file<<Inhibit_left[i]<<"->"<<Inhibit_middle[i]<<"->"<<Inhibit_right[i]<<";"<<endl;
		}
	    file<<"end;"<<endl;
	    file.close();//end of splicing graph
	    file.open(argv[3]);
	    for (i=0;i<Line_Graph.size();i++)
	    {
		    file<<Line_Graph[i]<<";"<<endl;
	    }
	    file<<max_ind_set_pos<<"!"<<endl;
	    file<<"end;"<<endl;
	    file.close();//end of line graph
}//end of if atoi(argv[1])==0
if (atoi(argv[10])>=1)
{
	Edges_left.clear();
	Edges_right.clear();
	Weights.clear();
	Line_Graph.clear();
	Inhibit_left.clear();
	Inhibit_middle.clear();
	Inhibit_right.clear();
	max_ind_set_pos=0;
	string temp;
	vector<string> vec;
	vector<string> vec_pair;
	vec.clear();
	int len_vec,sum_i,sum_j;
	ifstream ifs;
	ifs.open(argv[2]);
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Inhibit_edges **")
			break;
		else
			vec.push_back(temp);
	}
	while (1)
	{
		getline(ifs,temp);
		if (temp=="end;")
			break;
		else
			vec_pair.push_back(temp);
	}
	ifs.close();
	for (i=0;i<vec.size();i++)
	{
		if (vec[i]!="** Edges **")
		{
			len_vec=vec[i].size();
			for (j=0;j<len_vec;j++)
			{
				if (j<len_vec-1 && vec[i][j+1]=='-')
				{
					if (j==0)
						Edges_left.push_back(atoi(vec[i].substr(j,1).c_str()));
					else
					{
						sum_j=1;
						while (1)
						{
							if ((j-sum_j==0) || (j-sum_j!=0 && vec[i][j-sum_j]==' '))
								break;
							else
								sum_j= sum_j+1;
						}
						if (j-sum_j==0)
							Edges_left.push_back(atoi(vec[i].substr(j-sum_j,sum_j+1).c_str()));
						else
							Edges_left.push_back(atoi(vec[i].substr(j-sum_j+1,sum_j).c_str()));
					}
				}
				if (j>1 && vec[i][j-1]=='>')
				{
					sum_j=1;
					while (1)
					{
						if (vec[i][j+sum_j]==':')
							break;
						else
							sum_j= sum_j+1;
					}
					Edges_right.push_back(atoi(vec[i].substr(j,sum_j).c_str()));
				}
				if (j>1 && vec[i][j-1]==':')
				{
					sum_j=1;
					while (1)
					{
						if (vec[i][j+sum_j]==',' || vec[i][j+sum_j]==';')
							break;
						else
							sum_j= sum_j+1;
					}
					Weights.push_back(atof(vec[i].substr(j,sum_j).c_str()));
				}
			}
		}
	}
	vec.clear();
	for (i=0;i<vec_pair.size();i++)
	{
		for (j=0;j<vec_pair[i].size();j++)
		{
			if (vec_pair[i][j]=='-')
			{
				Inhibit_left.push_back(atoi(vec_pair[i].substr(0,j).c_str()));
				for (k=j+2;k<vec_pair[i].size();k++)
				{
					if (vec_pair[i][k]=='-')
					{
						Inhibit_middle.push_back(atoi(vec_pair[i].substr(j+2,k-j-2).c_str()));
						for (t=k+2;t<vec_pair[i].size();t++)
						{
							if (vec_pair[i][t]==';')
							{
								Inhibit_right.push_back(atoi(vec_pair[i].substr(k+2,t-k-2).c_str()));
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	ifs.open(argv[3]);
	while (1)
	{
		getline(ifs,temp);
		if (temp=="end;")
			break;
		else
			vec.push_back(temp);
	}
	ifs.close();
	for (i=0;i<vec.size();i++)
	{
		for (j=0;j<vec[i].size();j++)
		{
			if (vec[i][j]==';')
				Line_Graph.push_back(atoi(vec[i].substr(0,j).c_str()));
		}
	}
	for (i=0;i<vec[vec.size()-1].size();i++)
	{
		if (vec[vec.size()-1][i]=='!')
	        max_ind_set_pos=atoi(vec[vec.size()-1].substr(0,i).c_str());//end of line graph
	}
	ifs.open(argv[4]);
	getline(ifs,temp);
	ifs.close();
	if (temp!="" && strcmp(argv[11],"yes")==0)
	{
        ILP_Process ILP_process_L(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos-atoi(argv[10])+1,"left",argv[6],argv[8]);
        ILP_process_L.Display_ILP_Process();
        Edges_left=ILP_process_L.Edges_left_new;
        Edges_right=ILP_process_L.Edges_right_new;
        Weights=ILP_process_L.Weights_new;
	}
	if (temp!="" && strcmp(argv[11],"no")==0)
	{
		ILP_Process ILP_process_L(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos+atoi(argv[10]),"left",argv[6],argv[8]);
		ILP_process_L.Display_ILP_Process();
		Edges_left=ILP_process_L.Edges_left_new;
		Edges_right=ILP_process_L.Edges_right_new;
		Weights=ILP_process_L.Weights_new;
	    ofstream file;
	    file.open(argv[2]);
	    file<<"** Edges **"<<endl;
	    for (i=0;i<Weights.size();i++)
    	{
	    	file<<Edges_left[i]<<"->"<<Edges_right[i]<<":"<<Weights[i]<<";"<<endl;
        }
		file<<"** Inhibit_edges **"<<endl;
		for (i=0;i<Inhibit_left.size();i++)
		{
			file<<Inhibit_left[i]<<"->"<<Inhibit_middle[i]<<"->"<<Inhibit_right[i]<<";"<<endl;
		}
	    file<<"end;"<<endl;
	    file.close();//end of splicing graph
		return 0;
	}
	ifs.open(argv[5]);
	getline(ifs,temp);
	ifs.close();
	if (temp!="" && strcmp(argv[11],"yes")==0)
	{
        ILP_Process ILP_process_R(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos+atoi(argv[10]),"right",argv[7],argv[9]);
        ILP_process_R.Display_ILP_Process();
        Edges_left=ILP_process_R.Edges_left_new;
        Edges_right=ILP_process_R.Edges_right_new;
        Weights=ILP_process_R.Weights_new;
	}
	if (temp!="" && strcmp(argv[11],"no")==0)
	{
		ILP_Process ILP_process_R(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos-atoi(argv[10])+1,"right",argv[7],argv[9]);
		ILP_process_R.Display_ILP_Process();
		Edges_left=ILP_process_R.Edges_left_new;
		Edges_right=ILP_process_R.Edges_right_new;
		Weights=ILP_process_R.Weights_new;
		ofstream file;
		file.open(argv[2]);
		file<<"** Edges **"<<endl;
		for (i=0;i<Weights.size();i++)
		{
			file<<Edges_left[i]<<"->"<<Edges_right[i]<<":"<<Weights[i]<<";"<<endl;
		}
		file<<"** Inhibit_edges **"<<endl;
		for (i=0;i<Inhibit_left.size();i++)
		{
			file<<Inhibit_left[i]<<"->"<<Inhibit_middle[i]<<"->"<<Inhibit_right[i]<<";"<<endl;
		}
		file<<"end;"<<endl;
		file.close();//end of splicing graph
		return 0;
	}
	ofstream file;
	file.open(argv[2]);
	file<<"** Edges **"<<endl;
	for (i=0;i<Weights.size();i++)
	{
		file<<Edges_left[i]<<"->"<<Edges_right[i]<<":"<<Weights[i]<<";"<<endl;
    }
	file<<"** Inhibit_edges **"<<endl;
	for (i=0;i<Inhibit_left.size();i++)
	{
		file<<Inhibit_left[i]<<"->"<<Inhibit_middle[i]<<"->"<<Inhibit_right[i]<<";"<<endl;
	}
	file<<"end;"<<endl;
	file.close();//end of splicing graph
}//end of if atoi(argv[1])>=1
    //Begin comparing the number of edges left and right
	int jj;
	if (max_ind_set_pos-atoi(argv[10])>=0)
	{
		jj=0;//exon_out
		k=0;//exon_in
	    for (i=0;i<Edges_left.size();i++)
	    {
		    if (Edges_left[i]==Line_Graph[max_ind_set_pos-atoi(argv[10])])
				jj=jj+1;
			if (Edges_right[i]==Line_Graph[max_ind_set_pos-atoi(argv[10])])
				k=k+1;
	    }
		if (jj<k)
		{
			Write_ILP_R(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos-atoi(argv[10])-1,Inhibit_left,Inhibit_middle,Inhibit_right,argv[5],argv[9]);
			Write_ILP_L(Edges_left,Edges_right,Weights,Line_Graph,0,Inhibit_left,Inhibit_middle,Inhibit_right,argv[4],argv[8]);
			return 0;
		}
		jj=0;
		k=0;
	}
	if (max_ind_set_pos+atoi(argv[10])<=Line_Graph.size()-2)
	{
		jj=0;
		k=0;
		for (i=0;i<Edges_left.size();i++)
		{
			if (Edges_left[i]==Line_Graph[max_ind_set_pos+atoi(argv[10])+1])
				jj=jj+1;
			if (Edges_right[i]==Line_Graph[max_ind_set_pos+atoi(argv[10])+1])
				k=k+1;
		}
		if (k<jj)
		{
			Write_ILP_L(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos+atoi(argv[10])+1,Inhibit_left,Inhibit_middle,Inhibit_right,argv[4],argv[8]);
			Write_ILP_R(Edges_left,Edges_right,Weights,Line_Graph,Line_Graph.size()-2,Inhibit_left,Inhibit_middle,Inhibit_right,argv[5],argv[9]);
			return 0;
		}
	}
    if (max_ind_set_pos-atoi(argv[10])>=0)
	{
        Write_ILP_L(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos-atoi(argv[10]),Inhibit_left,Inhibit_middle,Inhibit_right,argv[4],argv[8]);
    }
	if (max_ind_set_pos+atoi(argv[10])<=Line_Graph.size()-2)
	{
	    Write_ILP_R(Edges_left,Edges_right,Weights,Line_Graph,max_ind_set_pos+atoi(argv[10]),Inhibit_left,Inhibit_middle,Inhibit_right,argv[5],argv[9]);
    }
   return 0;
}




