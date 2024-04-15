#include <stdio.h>
#include <math.h>

int add(int x, int y);

int main(){
	int a = 5;
	int b = 4;
	int x = 0;

	x = add(a,b);
	printf("%d\n", x);
};

int add(int x, int y){
	return x+y;
}
