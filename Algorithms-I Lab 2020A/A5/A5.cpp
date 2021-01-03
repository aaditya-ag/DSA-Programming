//Name: Aaditya Agrawal
//Roll no:19CS10003
#include<iostream>
#include<fstream>
#include<stdlib.h>
using namespace std;

/*

---------- Part - 1 -----------

Approach:
Form a recurrence relation using 2D DP array such that 
dp[i][j] denotes maximum probability of success
for i reactions, given j units catalyst.

It is optimal: for N reaction using C units of catalyst, then for Nth reaction we can
use j = (1...C) units of catalyst on it, then for j units on Nth reaction, we will find best probability
for N-1 reactions and C-j units.

Base Case:
a. dp[1][j] = e[1][j]	//for j = 1 to C units
b. dp[i][0] = 0		//for i = 1 to n reactions

Recurrence Relation:
dp[i][j] = max(dp[i][j], dp[i-1][j-k] * e[i][k]) for k = 1 to j

*/

double max(double a,double b){
	return (a>b)?a:b;
}


int main(){

	fstream fin,fout;
	fin.open("input.txt",ios::in);
	fout.open("output.txt",ios::out);

	int n,c;

	//Taking input from file
	
	fin>>n;
	fin>>c;
	//e is the probability matrix
	double **dp = (double **)malloc((n+1)*sizeof(double *));
	double **e = (double **)malloc((n+1)*sizeof(double *));
	for(int i=0;i<=n;i++){
		dp[i] = (double *)malloc((c+1)*sizeof(double));
		e[i] = (double *)malloc((c+1)*sizeof(double));
	}

	for(int i=1;i<=n;i++){
		for(int j=1;j<=c;j++){
			fin>>e[i][j];
		}
	}



	//base case #1: 
	for(int i=1;i<=c;i++){
		dp[1][i] = e[1][i];
	}

	//base case #2: 
	for(int i=1;i<=n;i++){
		dp[i][0] = 0;
	}
	
	//Applying the recurrence relation
	for(int i=2;i<=n;i++){
		for(int j=1;j<=c;j++){
			dp[i][j] = 0;
			for(int k = 1;k<=j;k++){
				dp[i][j] = max(dp[i][j],dp[i-1][j-k]*e[i][k]);
			}
		}
	}
	
	fout<<dp[n][c]<<"\n";
	
	/*
		Here we backtrack and find the answer, if dp[n][c] was derived from dp[n-1][c-j],
		then j units were used in nth reaction. Then we do the same process for (n-1,c-j).
		ans[i] stores units of catalyst used in ith reaction.
	*/
	double ans[n+1];
	for(int i=n;i>=2;i--){
		for(int j=1;j<=c;j++){
			if(dp[i][c] == dp[i-1][c-j]*e[i][j]){
				ans[i] = j;
				c = c - j;
				break;
			}
		}
	}
	ans[1] = c;

	for(int i=1;i<=n;i++){
		fout<<"reaction "<<i<<" : "<<ans[i]<<endl;
	}

	fin.close();
	fout.close();

	return 0;
}