#include <stdio.h>
#include <math.h>

int add(int x, int y);

int main(){
	int a = 5;
	int b = 4;
	int x = 0;

	x = add(a,b);
	printf("%d and %d makes %d\n", a,b,x);
};

int add(int x, int y){
	return x+y;
}
