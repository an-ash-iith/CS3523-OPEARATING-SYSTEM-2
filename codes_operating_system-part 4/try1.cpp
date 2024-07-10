#include <iostream>
using namespace std;

void assign1(int &a, int &b)
{
    a=5;
    b=9;
}
int main()
{
int a,b;
assign1(a,b);

cout<<a<<" "<<b;

return 0;
}