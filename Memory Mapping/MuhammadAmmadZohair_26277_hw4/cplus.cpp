#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	string filename = "loremipsum.txt";
	ifstream input;
	char ch;
	int counter = 0;
	input.open(filename.c_str());

	
	if (!input.fail())
	{	
		while(input.get(ch))
		{
			if ('a' == ch)
			{
				counter++;
			}
		}
	}

	else
	{
		cout<<"Cannot open file!"<<endl;
	}

	cout<<"Number of times 'a' occurred = "<<counter<<endl;

	input.close();
	return 0;
}

