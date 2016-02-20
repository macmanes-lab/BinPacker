#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
  if (argc!=3)
  {
    cout<<"Error Parameters!";
    return 0;
  }
ifstream ifs;
ofstream file;
string temp;
int note_1=0;
int note_2=0;
int num=0;
int i,j;
ifs.open(argv[1]);
file.open(argv[2]);
while (ifs.peek()!=EOF)
{
  getline(ifs,temp);
  if (temp[0]=='>')
  {
    for (i=6;i<temp.size();i++)
    {
      if (temp[i]==' ')
      {
        note_2=atoi(temp.substr(5,i-5).c_str());
        break;
      }
    }
    if (note_2==note_1)
    {
      file<<temp.substr(0,5)<<note_2<<"_seq"<<num<<temp.substr(i,temp.size()-i)<<endl;
      num=num+1;
    }
    else
    {
      note_1=note_2;
      num=0;
      file<<temp.substr(0,5)<<note_2<<"_seq"<<num<<temp.substr(i,temp.size()-i)<<endl;
      num=num+1;
    }
  }
  else
  file<<temp<<endl;
}
ifs.close();
file.close();

  return 0;
}
