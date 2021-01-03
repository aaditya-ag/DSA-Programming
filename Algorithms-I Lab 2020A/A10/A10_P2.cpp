//Aaditya Agrawal
//19CS10003
#include<iostream>
#include<fstream>
using namespace std;

/*
	Algorithm
	The concept here is used kind of double chaining. First keys are chained to prevent collision.
	Then we find the particular node corresponding to key.
	We insert the value in an array of values corresponding to that node
*/

typedef struct _hashing{
	int key;
	int *arr;
	struct _hashing *next;
}Hash;

int hashval(int key,int n){
	return key%n;
}

void insert(Hash **h,int key,int val,int n){
	int idx = hashval(key,n);
	Hash *cur = h[idx];
	while(cur != NULL){
		if(cur->key == key){
			break;

		}
		cur = cur->next;
	}
	if(cur == NULL){
		Hash *newNode = (Hash *)malloc(sizeof(Hash));
		newNode->key = key;
		newNode->arr = (int *)malloc(n*sizeof(int));
		for(int i=0;i<n;i++){
			newNode->arr[i] = -1;
		}
		newNode->next = h[idx];
		h[idx] = newNode;
		cur = newNode;
	}
	int idx1 = hashval(val,n);
	int *temp = cur->arr;
	temp[idx1] = val;
}

void deleteval(Hash **port,Hash **counter,int y,int n){
	int idx = hashval(y,n);
	Hash *curCounter = counter[idx];
	int *arrTemp;
	if(curCounter->key == y){
		arrTemp = curCounter->arr;
		counter[idx] = curCounter->next;
	} 
	else{
		while(curCounter->next != NULL){
			if(curCounter->next->key == y){
				arrTemp = curCounter->next->arr;
				curCounter->next = curCounter->next->next;
				break;
			}
			curCounter =curCounter->next;
		}
	}
	for(int i=0;i<n;i++){
		if(arrTemp[i] != -1){
			int idx1 = hashval(arrTemp[i],n);
			Hash *curPort = port[idx1];
			if(curPort->key == arrTemp[i]){
				int cnt = 0;
				for(int j=0;j<n;j++){
					if(curPort->arr[j]  !=  -1){
						cnt++;
					}
					if(curPort->arr[j] == y){
						curPort->arr[j] = -1;
						cnt--;
					}
				}
				if(cnt == 0){
					port[idx1] =curPort->next;
				}
			}
			else{
				while(curPort->next != NULL){
					if(curPort->next->key == arrTemp[i]){
						int cnt = 0;
						for(int j=0;j<n;j++){
							if(curPort->next->arr[j]  !=  -1){
								cnt++;
							}
							if(curPort->next->arr[j] == y){
								curPort->next->arr[j] = -1;
								cnt--;
							}
						}
						if(cnt == 0){
							curPort->next = curPort->next->next;
						}
					}
					curPort = curPort->next;
				}
			}

		}
	}
}

void printtable(Hash **h,int n,char ch,fstream& fout){
	for(int i=0;i<n;i++){
		Hash *cur = h[i];
		if(cur == NULL){
			fout<<ch<<" "<<i<<" "<<-1<<" "<<-1<<endl;
			continue;
		}
		while(cur != NULL){
			int *tempArr = cur->arr;
			for(int j=0;j<n;j++){
				if(tempArr[j] != -1){
					fout<<ch<<" "<<i<<" "<<cur->key<<" "<<tempArr[j]<<endl;
				}
			}
			cur = cur->next;
		}
	}
}

int main(){
	fstream fin;
	fin.open("input.txt",ios::in);
	fstream fout;
	fout.open("output.txt",ios::out);
	int n,t;
	char ch;

	fin>>n>>t;
	Hash **port = (Hash **)malloc(n*sizeof(Hash *));
	Hash **counter = (Hash **)malloc(n*sizeof(Hash *));
	for(int i=0;i<n;i++){
		port[i] = NULL;
		counter[i] =NULL;
	}
	for(int i=0;i<t;i++){
		fin>>ch;
		if(ch == '+'){
			int y,p;
			fin>>y>>p;
			insert(port,p,y,n);
			insert(counter,y,p,n);
		}
		else{
			int y;
			fin>>y;
			deleteval(port,counter,y,n);
		}
	}
	printtable(port,n,'p',fout);
	printtable(counter,n,'c',fout);
	fin.close();
	fout.close();

}