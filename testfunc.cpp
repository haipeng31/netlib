#include <iostream>
#include <functional>
using namespace std;

int main()
{
	function<void ()> func = NULL;
	func();
	cout << 1 << endl;
	return 0;
}
