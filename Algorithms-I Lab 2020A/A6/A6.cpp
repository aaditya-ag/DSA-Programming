//Aaditya Agrawal
//19CS10003
#include<iostream>
#include<fstream>
using namespace std;

typedef struct{
	int id;	//Person number
	int a;	//start time
	int b;	//end time
}Order;

int max(int a,int b){
	return (a>b)?a:b;
}

bool isFeasible(int L,Order order[],int n){		//Checks if a possible value of L is valid or not
	bool flag=1;
	int prevVisit = order[0].a*60;
	for(int i=1;i<n;i++){
		if((order[i].b)*60 < prevVisit + L){
			return false;
		}
		else{
			prevVisit = max((order[i].a)*60,prevVisit+L);
		}
	}
	return true;
}

int binarysearch(Order order[], int n){		//Binary search searches for the max possible L(time difference)
	int lo = 1,hi = 1440*60;
	int mid;
	while(lo<hi){
		mid = (lo+hi)/2;
		if(isFeasible(mid,order,n)){
			lo = mid+1;
		}
		else{
			hi = mid;
		}
	} 
	return lo-1;
}

void swap(Order *a,Order *b){
	Order temp = *a;
	*a = *b;
	*b = temp;
}


void schedule_visits(Order order[],int i,int n,Order ans[],int *val){		//Generates all possible permutations and finds the optimal case among them
	if(i==n-1){
		int temp = binarysearch(order,n);	
		if(temp>*val){
			*val = temp;
			for(int k=0;k<n;k++){
				ans[k] = order[k];
			}
		}
		return;
	}
	for(int j=i;j<n;j++){
		swap(&order[i],&order[j]);
		schedule_visits(order,i+1,n,ans,val);
		swap(&order[i],&order[j]);
	}	

}

int main(){
	ifstream fin;
	fin.open("input1.txt");
	ofstream fout;
	fout.open("output1.txt");
	int n;
	fin>>n;
	Order *order = new Order[n];
	Order *ans = new Order[n];	//Mantains the optimal ordering
	int val;	//Mantains the best possible ans
	val = 0;
	for(int i=0;i<n;i++){
		order[i].id = i;
		fin>>order[i].a>>order[i].b;
		ans[i].id = order[i].id;
		ans[i].a = order[i].a;
		ans[i].b = order[i].b;
	}
	schedule_visits(order,0,n,ans,&val);
	fout<<val/60<<":";
	if(val%60<10){
		fout<<"0"<<val%60<<endl;
	}
	else{
		fout<<val%60<<endl;
	}
	for(int i=0;i<n;i++){
		fout<<ans[i].id<<" ";
	}
	fout<<endl;

}