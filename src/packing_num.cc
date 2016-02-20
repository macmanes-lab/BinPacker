#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
if (argc!=4)
{
	cout<<"Error Parameters!"<<endl;
	return 0;
}
int i,j,k;
vector<int> Edges_left;
vector<int> Edges_right;
vector<float> Weights;
vector<int> Line_Graph;
int max_ind_set_pos;
Edges_left.clear();
Edges_right.clear();
Weights.clear();
Line_Graph.clear();
max_ind_set_pos=0;
string temp;
vector<string> vec;
vec.clear();
int len_vec,sum_i,sum_j;
ifstream ifs;
ifs.open(argv[1]);
	while (1)
	{
		getline(ifs,temp);
		if (temp=="** Inhibit_edges **")
			break;
		else
			vec.push_back(temp);
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
	ifs.open(argv[2]);
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
	if (strcmp(argv[3],"left")==0)
	{
		for (i=0;i<=max_ind_set_pos;i++)
		{
			k=0;
			for (j=0;j<Weights.size();j++)
			{
				if (Edges_right[j]==Line_Graph[max_ind_set_pos-i])
					k=k+1;
			}
			if (k==0)
				break;
		}
		cout<<i<<endl;
		return i;
	}
	if (strcmp(argv[3],"right")==0)
	{
		for (i=0;i+1+max_ind_set_pos<Line_Graph.size();i++)
		{
			k=0;
			for (j=0;j<Weights.size();j++)
			{
				if (Edges_left[j]==Line_Graph[max_ind_set_pos+i+1])
					k=k+1;
			}
			if (k==0)
				break;
		}
		cout<<i<<endl;;
		return i;
	}
}




