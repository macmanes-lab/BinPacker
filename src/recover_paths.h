#ifndef RECOVER_PATHS
#define RECOVER_PATHS

//recover_paths.h
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<cmath>
using namespace std;

class Recover_Junction_Paths
{
	private:
	vector<int> DAG_left;
	vector<int> DAG_right;
	vector<vector<int> > Junction_Path_cover;
	int max_node_num;
	public:
	Recover_Junction_Paths(vector<int> dag_left,vector<int> dag_right,vector<vector<int> > junction_path_cover,int max_node);
	~Recover_Junction_Paths();
	void Get_Path_cover();
	vector<vector<int> > Path_Cover;
};
Recover_Junction_Paths::Recover_Junction_Paths(vector<int> dag_left,vector<int> dag_right,vector<vector<int> > junction_path_cover,int max_node)
{
	DAG_left=dag_left;
	DAG_right=dag_right;
	Junction_Path_cover=junction_path_cover;
	max_node_num=max_node;
	return;
}
Recover_Junction_Paths::~Recover_Junction_Paths()
{
	return;
}
void Recover_Junction_Paths::Get_Path_cover()
{
	int i,j,k,t;
	vector<int> Path_temp;
	for (i=0;i<Junction_Path_cover.size();i++)
	{
		Path_temp.push_back(DAG_left[Junction_Path_cover[i][0]]);
		for (j=0;j<Junction_Path_cover[i].size();j++)
		{
			Path_temp.push_back(DAG_right[Junction_Path_cover[i][j]]);
		}
		Path_Cover.push_back(Path_temp);
		Path_temp.clear();
	}
	for (i=0;i<Path_Cover.size();i++)
	{
		Path_Cover[i].erase(remove(Path_Cover[i].begin(),Path_Cover[i].end(),max_node_num+1),Path_Cover[i].end());
		Path_Cover[i].erase(remove(Path_Cover[i].begin(),Path_Cover[i].end(),max_node_num+2),Path_Cover[i].end());
	}
}

#endif


















