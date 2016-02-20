#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
using namespace std;
//1:Raw_Splcing_Graph;2:Line_Graph;3:Result_L;4:Result_R;5:Packing.fa

int main(int argc, char *argv[])
{
	if (argc!=6)
	{
		cout<<"Error Parameters!"<<endl;
		return 0;
	}
	int i,j,k;
	int t=0;
	int start_exon_num=-1;
	vector<string> vec;
	vector<string> Node_seq;
	vector<int> Exon_left;
	vector<int> Exon_right;
	vector<int> Exon_middle;
	vector<int> Exon_L;
	vector<int> Exon_R;
	vector<int> Exon_single;
	vector<float> Weights_more;
	vector<float> Weights_less;
	vector<float> Weights_new;
	vector<int> Line_Graph;
	vector<vector<int> > Seqs;
	vector<int> Seqs_sub;
	vector<vector<int> > Seqs_new;
    vector<vector<int> >Isoforms_num;
	vector<int> Isoforms_num_sub;
	vector<int> Isoforms_num_less;
	vector<float> Output_weights_more;
	vector<float> Output_weights_less;
    vector<string> Output_weights_single;
	string temp;
	vec.clear();
	Node_seq.clear();
	Exon_left.clear();
	Exon_right.clear();
	Exon_middle.clear();
	Exon_L.clear();
	Exon_R.clear();
	Exon_single.clear();
	Weights_more.clear();
	Weights_less.clear();
	Line_Graph.clear();
	Seqs.clear();
	Seqs_sub.clear();
	Isoforms_num.clear();
	Isoforms_num_sub.clear();
	Isoforms_num_less.clear();
	ifstream ifs;
	ifs.open(argv[3]);
	while (ifs.peek()!=EOF)
	{
		getline(ifs,temp);
		vec.push_back(temp);
	}
	ifs.close();
	ifs.open(argv[4]);
	while (ifs.peek()!=EOF)
	{
		getline(ifs,temp);
		if (temp[0]=='3' || temp[0]=='1')
		    vec.push_back(temp);
	}
	ifs.close();
  if (vec.size()==0)
  {
	//Begin read ATCG -> Node_seq
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
							Output_weights_single.push_back(temp.substr(i,j-1));
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
	int j_argv,k_argv;
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
	t=0;
	int ii;
	ofstream file;
	file.open(argv[5],ios_base::app);
	for (i=0;i<Node_seq.size();i++)
	{
		if (Node_seq[i].size()<=500 && atof(Output_weights_single[i].c_str())<=3.0)
		{
			continue;
		}
		if (Node_seq[i].size()>=200)
		{
			k=Node_seq[i].size();
		    file<<">";
		    for (ii=j_argv;ii<k_argv;ii++)
	    	{
		    	file<<temp_argv[ii];
	    	}
		    file<<temp_argv[k_argv]<<" len="<<k<<" path: ";
		    t=t+1;
		    file<<i<<endl;
		    file<<Node_seq[i]<<endl;
		}
	}
	file.close();
  }
  else
  {
	for (i=0;i<vec.size();i++)
	{
	/*	if (vec[i][0]=='1')
		{
			for (j=2;j<vec[i].size();j++)
			{
				if (vec[i][j]==';')
				    Exon_single.push_back(atoi(vec[i].substr(2,j-2).c_str()));
			}
		}
		if (vec[i][0]=='2')
		{
			for (j=2;j<vec[i].size();j++)
			{
				if (vec[i][j]=='-')
				{
					Exon_L.push_back(atoi(vec[i].substr(2,j-2).c_str()));
					k=j+2;
				}
				if (vec[i][j]==':')
				{
					Exon_R.push_back(atoi(vec[i].substr(k,j-k).c_str()));
					k=j+1;
				}
				if (vec[i][j]==';')
				{
					Weights_less.push_back(atof(vec[i].substr(k,j-k).c_str()));
				}
			}
		}
	*/	
		k=1000000;
		if (vec[i][0]=='3')
		{
			t=0;
			for (j=3;j<vec[i].size();j++)
			{
				if (t==0 && vec[i][j]=='-')
				{
					Exon_left.push_back(atoi(vec[i].substr(2,j-2).c_str()));
					k=j+2;
					t=t+1;
				}
				if (j>k && vec[i][j]=='-')
				{
					Exon_middle.push_back(atoi(vec[i].substr(k,j-k).c_str()));
					k=j+2;
				}
				if (vec[i][j]==':')
				{
					Exon_right.push_back(atoi(vec[i].substr(k,j-k).c_str()));
					k=j+1;
				}
				if (vec[i][j]==';')
		    	{
					Weights_more.push_back(atof(vec[i].substr(k,j-k).c_str()));
				}
			}
		}
	}
//	sort(Exon_single.begin(),Exon_single.end());
//	vector<int>::iterator it = unique(Exon_single.begin(),Exon_single.end());
//	Exon_single.erase(it, Exon_single.end());
	//Begin constructing isoforms
	vec.clear();
	ifs.open(argv[2]);
	while (1)
	{
		getline(ifs,temp);
		if (temp=="end;" || temp=="")
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
	}//Get Line_Graph
	Weights_new=Weights_more;
	sort(Weights_new.begin(),Weights_new.end());
	vector<float>::iterator iter = unique(Weights_new.begin(),Weights_new.end());
	Weights_new.erase(iter, Weights_new.end());
	for (i=0;i<Weights_new.size();i++)
	{
		for (j=0;j<Weights_more.size();j++)
		{
			if (Weights_more[j]==Weights_new[i])
			{
				Seqs_sub.push_back(j);
			}
		}
		Seqs.push_back(Seqs_sub);
		Seqs_sub.clear();
	}
	Seqs_new=Seqs;
	for (i=0;i<Seqs.size();i++)
	{
		start_exon_num=-1;
	  while (Seqs_new[i].size()>0)
	  {
    	for (j=0;j<Line_Graph.size();j++)
		{
			for (k=0;k<Seqs_new[i].size();k++)
			{
				if (Line_Graph[j]==Exon_left[Seqs_new[i][k]])
				{
					start_exon_num=Seqs_new[i][k];
					break;
				}
			}
			if (start_exon_num>=0)
			{
				Isoforms_num_sub.push_back(start_exon_num);
				break;
			}
		}
		int num_diff=1;
		int num_Isoforms_num_sub=0;
		while (num_diff!=0)
		{
		    for (j=0;j<Seqs_new[i].size();j++)
		    {
			    if (Exon_left[Seqs_new[i][j]]==Exon_middle[start_exon_num] && Exon_middle[Seqs_new[i][j]]==Exon_right[start_exon_num])
			    {
				    Isoforms_num_sub.push_back(Seqs_new[i][j]);
				    start_exon_num=Seqs_new[i][j];
				    num_diff=Isoforms_num_sub.size()-num_Isoforms_num_sub;
				    num_Isoforms_num_sub=Isoforms_num_sub.size();
				    break;
		    	}
			    else
				    num_diff=0;
		    }
		}
		Isoforms_num.push_back(Isoforms_num_sub);
		Output_weights_more.push_back(Weights_new[i]);
		for (j=0;j<Isoforms_num_sub.size();j++)
		{
			Seqs_new[i].erase(remove(Seqs_new[i].begin(),Seqs_new[i].end(),Isoforms_num_sub[j]),Seqs_new[i].end());
		}
		Isoforms_num_sub.clear();
		start_exon_num=-1;
	  }	
	}
/*for (i=0;i<Isoforms_num.size();i++)
{
    for (j=0;j<Isoforms_num[i].size();j++)
    {
        cout<<"Isoforms_num_"<<i<<"_"<<j<<": "<<Isoforms_num[i][j]<<endl;
    }
}*/
	//Begin start->end exon isoforms
/*	for (i=0;i<Weights_less.size();i++)
	{
		k=0;
		for (j=0;j<Isoforms_num.size();j++)
		{
			if (Exon_L[i]!=Exon_left[Isoforms_num[j][0]] || Exon_R[i]!=Exon_middle[Isoforms_num[j][0]])
			{
				k=k+1;
			}
		}
		if (k==Isoforms_num.size())
		{
			k=0;
			for (t=0;t<Isoforms_num_less.size();t++)
			{
				if (Exon_L[Isoforms_num_less[t]]==Exon_L[i] && Exon_R[Isoforms_num_less[t]]==Exon_R[i])
				{
					break;
				}
				else
				{
					k=k+1;
				}
			}
			if (k==Isoforms_num_less.size())
			{
				Isoforms_num_less.push_back(i);
				Output_weights_less.push_back(Weights_less[i]);
			}
		}
	}
*/
	//Begin read ATCG -> Node_seq
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
							Output_weights_single.push_back(temp.substr(i,j-1));
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
	//Begin write ATCG -> Packing.fa
	char * temp_argv;
	int j_argv,k_argv;
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
	ofstream file;
	file.open(argv[5],ios_base::app);
	t=0;
	int ii,jj;
	//1: write long isoforms
	vector<int> vec_same;
	vector<vector<int> > Isoforms_num_none;
	vector<float> Output_weights_more_none;
	float w;
	vec_same.clear();

  while (Isoforms_num.size()>0)
  {
		for (j=0;j<Isoforms_num.size();j++)
		{
			i=0;
			if (0<j && Isoforms_num[0].size()==Isoforms_num[j].size())
			{
				for (k=0;k<Isoforms_num[0].size();k++)
				{
					if (Exon_left[Isoforms_num[0][k]]==Exon_left[Isoforms_num[j][k]] && Exon_middle[Isoforms_num[0][k]]==Exon_middle[Isoforms_num[j][k]] && Exon_right[Isoforms_num[0][k]]==Exon_right[Isoforms_num[j][k]])
					{
						i=i+1;
					}
				}
				if (i==Isoforms_num[0].size())
				{
					vec_same.push_back(j);
				}
			}
		}
		w=Output_weights_more[0];
		Isoforms_num_none.push_back(Isoforms_num[0]);
		Isoforms_num.erase(remove(Isoforms_num.begin(),Isoforms_num.end(),Isoforms_num[0]),Isoforms_num.end());
		Output_weights_more.erase(remove(Output_weights_more.begin(),Output_weights_more.end(),Output_weights_more[0]),Output_weights_more.end());
		for (j=0;j<vec_same.size();j++)
		{
			w=w+Output_weights_more[j];
			Isoforms_num.erase(remove(Isoforms_num.begin(),Isoforms_num.end(),Isoforms_num[j]),Isoforms_num.end());
			Output_weights_more.erase(remove(Output_weights_more.begin(),Output_weights_more.end(),Output_weights_more[j]),Output_weights_more.end());
		}
		Output_weights_more_none.push_back(w);
		vec_same.clear();
  }
    Isoforms_num=Isoforms_num_none;
	Output_weights_more=Output_weights_more_none;
	for (i=0;i<Isoforms_num.size();i++)
	{
		if (Exon_right[Isoforms_num[i][0]]==1000)
		{
			k=Node_seq[Exon_middle[Isoforms_num[i][0]]].size();
			if (k<=500 && atof(Output_weights_single[Exon_middle[Isoforms_num[i][0]]].c_str())<=3.0)
				continue;
			if (k>=200)
			{
			file<<">";
			for (ii=j_argv;ii<k_argv;ii++)
			{
				file<<temp_argv[ii];
			}
			file<<temp_argv[k_argv]<<" len="<<k<<" path: ";
			t++;
			file<<Exon_middle[Isoforms_num[i][0]]<<endl;
			file<<Node_seq[Exon_middle[Isoforms_num[i][0]]]<<endl;
			}
		}
		else
	      {
		k=Node_seq[Exon_middle[Isoforms_num[i][0]]].size()+Node_seq[Exon_right[Isoforms_num[i][0]]].size();
		for (j=1;j<Isoforms_num[i].size()-1;j++)
		{
			k=k+Node_seq[Exon_right[Isoforms_num[i][j]]].size();
		}
		if (k<=500 && Output_weights_more[i]<=3.0)
		{
			continue;
		}
		file<<">";
	        for (ii=j_argv;ii<k_argv;ii++)
	        {
		    file<<temp_argv[ii];
	        }
	        file<<temp_argv[k_argv]<<" len="<<k<<" path: ";
		t=t+1;
		file<<Exon_middle[Isoforms_num[i][0]]<<"->"<<Exon_right[Isoforms_num[i][0]];
		for (j=1;j<Isoforms_num[i].size()-1;j++)
		{
		    file<<"->"<<Exon_right[Isoforms_num[i][j]];
		}
		file<<endl;
		file<<Node_seq[Exon_middle[Isoforms_num[i][0]]]<<Node_seq[Exon_right[Isoforms_num[i][0]]];
		for (j=1;j<Isoforms_num[i].size()-1;j++)
		{
			file<<Node_seq[Exon_right[Isoforms_num[i][j]]];
		}
		file<<endl;
	      }
	}
	//2: write start->end isoforms
/*	for (i=0;i<Isoforms_num_less.size();i++)
	{
		k=Node_seq[Exon_L[Isoforms_num_less[i]]].size()+Node_seq[Exon_R[Isoforms_num_less[i]]].size();
		if (k<=500 && Output_weights_less[i]<=3.0)
		{
			continue;
		}
		if (k>=200)
		{
		    file<<">";
	        for (ii=j_argv;ii<k_argv;ii++)
	        {
		        file<<temp_argv[ii];
	        }
	        file<<temp_argv[k_argv]<<"_seq"<<t<<" len="<<k<<" level="<<Output_weights_less[i]<<" path: ";
		    t=t+1;
		    file<<Exon_L[Isoforms_num_less[i]]<<"->"<<Exon_R[Isoforms_num_less[i]]<<endl;
		    file<<Node_seq[Exon_L[Isoforms_num_less[i]]]<<Node_seq[Exon_R[Isoforms_num_less[i]]]<<endl;
	    }
	}
*/
	//3: write single exon isoforms
/*	for (i=0;i<Exon_single.size();i++)
	{
		if (Node_seq[Exon_single[i]].size()<=500 && atof(Output_weights_single[Exon_single[i]].c_str())<=3.0)
		{
			continue;
		}
		if (Node_seq[Exon_single[i]].size()>=200)
		{
			k=Node_seq[Exon_single[i]].size();
		    file<<">";
		    for (ii=j_argv;ii<k_argv;ii++)
		    {
			    file<<temp_argv[ii];
		    }
		    file<<temp_argv[k_argv]<<"_seq"<<t<<" len="<<k<<" level="<<Output_weights_single[Exon_single[i]]<<" path: ";
		    t=t+1;
		    file<<Exon_single[i]<<endl;
		    file<<Node_seq[Exon_single[i]]<<endl;
		}
	}
*/
	//4: write isolated exon isoforms
	vector<int>::iterator pos_max;
	vector<int> Line_Graph_New;
	for (i=1;i<Line_Graph.size()-1;i++)
	{
		Line_Graph_New.push_back(Line_Graph[i]);
	}
	pos_max=max_element(Line_Graph_New.begin(),Line_Graph_New.end());
	if (*pos_max<Node_seq.size()-1)
	{
		for (i=(*pos_max)+1;i<Node_seq.size();i++)
		{
		    if (Node_seq[i].size()<=500 && atof(Output_weights_single[i].c_str())<=3.0)
	    	{
		    	continue;
	    	}
			if (Node_seq[i].size()>=200)
			{
				k=Node_seq[i].size();
			    file<<">";
			    for (ii=j_argv;ii<k_argv;ii++)
		    	{
			    	file<<temp_argv[ii];
		    	}
			    file<<temp_argv[k_argv]<<" len="<<k<<" path: ";
			    t=t+1;
			    file<<i<<endl;
			    file<<Node_seq[i]<<endl;
			}
		}
	}
	file.close();
  }
	return 0;
}








