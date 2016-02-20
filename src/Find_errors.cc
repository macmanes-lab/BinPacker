#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
using namespace std;
int main(int argc, char *argv[])
{
if (argc!=3)
{
	cout<<"Error Parameters!"<<endl;
	return 0;
}
int i,j,k;
vector<string> vec;
ifstream ifs;
ifs.open(argv[1]);
ofstream file;
file.open(argv[2]);
string temp,temp_1,temp_2;
getline(ifs,temp_1);
while (ifs.peek()!=EOF)
{
getline(ifs,temp_2);
if (temp_1[0]=='>' && temp_2[0]=='>')
 {
    vec.push_back(temp_1);
 }
temp_1=temp_2;
}
ifs.close();
ifs.open(argv[1]);
while (ifs.peek()!=EOF)
{
  getline(ifs,temp);
  k=0;
  for (i=0;i<vec.size();i++)
  {
    if (temp!=vec[i])
      k=k+1;
    else
      break;
  }
 // cout<<vec.size()<<"->"<<k<<endl;
  if (k==vec.size())
  {
    file<<temp<<endl;
  }
}
ifs.close();
file.close();
return 0;
}





