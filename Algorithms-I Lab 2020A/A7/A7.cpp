//Aaditya Agrawal
//Roll no: 19CS10003
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

typedef struct _node{
	int rank;
	struct _node *parent;
} node;

//Returns matrix of nodes of size m*n with ranks intialised to 0 and pointing to themselves
node **makeset(int m,int n){
	node **C = (node **)malloc(m*sizeof(node *));
	for(int i=0;i<m;i++){
		C[i] = (node *)malloc(n*sizeof(node));
		for(int j=0;j<n;j++){
			C[i][j].rank = 0;
			C[i][j].parent = &(C[i][j]);
		}
	}
	return C;
}

//Returns integer matrix of size m*n with all values initalised to 0
int **makegrid(int m,int n){
	int **a = (int **)malloc(m*sizeof(int *));
	for(int i=0;i<m;i++){
		a[i] = (int *)malloc(n*sizeof(int));
		for(int j=0;j<n;j++){
			a[i][j] = 0;
		}
	}
	return a;
}

void printgrid(int **H,int **V,int m,int n){
	cout<<"+   +";
	for(int i=0;i<n-1;i++){
		cout<<"---+";
	}
	cout<<endl;
	for(int i=0;i<m;i++){
		cout<<"|   ";
		for(int j=0;j<n-1;j++){
			if(V[i][j] == 0){
				cout<<"|   ";
			}
			else{
				cout<<"    ";
			}
		}
		if(i != m-1){
			cout<<"|"<<endl;
			for(int j=0;j<n;j++){
				if(H[i][j] == 0){
					cout<<"+---";
				}
				else{
					cout<<"+   ";
				}
			}
			cout<<"+"<<endl;
		}
		else{
			cout<<" "<<endl;
		}
	}
	cout<<"+";
	for(int i=0;i<n;i++){
		cout<<"---+";
	}
	cout<<endl;
}

//Finding root of tree while applying path compression
node *findset(node *a){
	if(a->parent == a){
		return a->parent;
	}
	return (a->parent = findset(a->parent));
}

//This function carries out union by rank for two disjoint sets
void mergeset(node *a,node *b){
	if(a != b){
		if(a->rank < b->rank){
			a->parent = b;
		}
		else if(a->rank > b->rank){
			b->parent = a;
		}
		else{
			b->parent = a;
			a->rank = a->rank+1;
		}
	}
}

void findtreasure(node **C,int **H,int **V,int m,int n){
	int wallRemoved = 0;	//This keeps a count of number of walls removed
	node *a,*b;

	//H[i][j] == 0 or V[i][j] == 0 represents that the wall is closed, thus eligible for removal
	while(wallRemoved < m*n - 1){
		if(rand()%2){	//If rand() gives an odd value means we remove horizontal wall
			//Finding coordinates of cells adjacent to walls
			if(m==1){
				continue;
			}
			int x1 = rand()%(m-1);
			int x2 = x1 + 1;
			int y1 = rand()%n;
			int y2 = y1;

			if(H[x1][y1] == 0){
				a = findset(&C[x1][y1]);
				b = findset(&C[x2][y2]);
				if(a != b){
					mergeset(a,b);
					wallRemoved++;
					H[x1][y1] = 1;
				}
			}
		}
		else{	//If rand() gives an even value means we remove vertical wall
			//Finding coordinates of cells adjacent to walls
			if(n==1){
				continue;
			}
			int x1 = rand()%m;
			int x2 = x1;
			int y1 = rand()%(n-1);
			int y2 = y1 + 1;

			if(V[x1][y1] == 0){
				a = findset(&C[x1][y1]);
				b = findset(&C[x2][y2]);
				if(a != b){
					mergeset(a,b);
					wallRemoved++;
					V[x1][y1] = 1;
				}
			}
		}
	}
	if(findset(&C[0][0]) == findset(&C[m-1][n-1])){
		cout<<"\nThe final chamber can be reached from the start chamber\n";
	}
	else{
		cout<<"\nThe final chamber can't be reached from the start chamber\n";
	}
}

int main(int argc, char *argv[]){
	int m,n;

	//Taking input from command line, else assigning default values to m,n
	if(argc>=3){
		m = atoi(argv[1]);
		n = atoi(argv[2]);
	}
	else{
		m = 10;n = 10;
	}

	srand((unsigned int)time(NULL));

	node **C = makeset(m,n);
	int **H = makegrid(m-1,n);
	int **V = makegrid(m,n-1);
	

	cout<<"Initial Grid\n\n";
	printgrid(H,V,m,n);

	findtreasure(C,H,V,m,n);

	cout<<"\nFinal Grid\n\n";
	printgrid(H,V,m,n);


	return 0;
}