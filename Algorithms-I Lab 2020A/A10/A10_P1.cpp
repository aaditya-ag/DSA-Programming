//Aaditya Agrawal
//19CS10003
#include<iostream>
#include<fstream>
using namespace std;

/*
	Algorithm
	The concept here is used kind of double chaining. First keys are chained to prevent collision.
	Then we find the particular node corresponding to key.
	We insert the value in a list of values corresponding to the particular key
*/

typedef struct _list{
	int val;
	struct _list *next;
}node;

typedef struct _hashing {
	int key;
	node *head;
	struct _hashing *next;
}Hash;

int hashval(int key,int n){
	return key%n;
}

void insert(Hash **h,int key,int val,int n){
	int i = hashval(key,n);
	Hash *cur = h[i];
	node *newNode = (node *)malloc(sizeof(node));
	newNode->val = val;
	newNode->next = NULL;
	//Finding if a HashNode corresponding to key exists, if it exists then we enter the value into the list
	while(cur != NULL){
		if(cur->key == key){
			node *temp = cur->head;
			while(temp!= NULL){
				if(temp->val == val){
					return;
				}
				temp = temp->next;
			}
			newNode->next = cur->head;
			cur->head = newNode;
			break;
		}
		cur = cur->next;
	}
	//If hash node doesnt exist, we create a new hashnode corresponding to the key
	if(cur == NULL){
		Hash *newHashNode = (Hash *)malloc(sizeof(Hash));
		newHashNode->key = key;
		newHashNode->head = newNode;
		newHashNode->next = h[i];
		h[i] = newHashNode;
	}
}

void deleteval(Hash **port,Hash **counter, int y, int n){
	int idx = hashval(y,n);
	Hash *curCounter = counter[idx];
	node *head;
	//The basic idea is to find the hashnode corresponding to the counter-party in portfolio by counterparty hashmap, then we traverse 
	//the list of portfolios, and delete the corresponding (portfolio,counter-party) pair from the 
	//counter-party by portfolio map
	if(curCounter->key == y){
		head = curCounter->head;
		counter[idx] = curCounter->next;
	} 
	else{
		while(curCounter->next != NULL){
			if(curCounter->next->key == y){
				head = curCounter->next->head;
				curCounter->next = curCounter->next->next;
				break;
			}
			curCounter =curCounter->next;
		}
	}
	while(head != NULL){
		int val =head->val;
		int idx1 = hashval(val,n);
		Hash *curPort = port[idx1];
		if(curPort->key == val){
			node *head1 = curPort->head;
			if(head1->val == y){
				curPort->head = head1->next;
			}
			else{
				while(head1->next != NULL){
					if(head1->next->val == y){
						head1->next =head1->next->next;
						break;
					}
					head1 = head1->next;
				}
			}
			if(curPort->head == NULL){
				port[idx1] = curPort->next;
			}
		}
		else{
			while(curPort->next != NULL){
				if(curPort->next->key == val){
					node *head1 = curPort->next->head;
					if(head1->val == y){
						curPort->next->head = head1->next;
					}
					else{
						while(head1->next != NULL){
							if(head1->next->val == y){
								head1->next =head1->next->next;
								break;
							}
							head1 =head1->next;
						}
					}
					if(curPort->next->head == NULL){
						curPort->next = curPort->next->next;
					}
				}
				curPort = curPort->next;
			}
		}
		head = head->next;
	}

}

void printtable(Hash **h,int n,char ch,fstream &fout){
	for(int i=0;i<n;i++){
		Hash *cur = h[i];
		if(cur == NULL){
			fout<<ch<<" "<<i<<" "<<-1<<" "<<-1<<endl;
			continue;
		}
		while(cur != NULL){
			node *head = cur->head;
			while(head !=NULL){
				fout<<ch<<" "<<i<<" "<<cur->key<<" "<<head->val<<endl;
				head = head->next;
			}
			cur = cur->next;
		}
	}
}



int main(){
	fstream fin;
	fin.open("input2.txt",ios::in);
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
		// cout<<ch<<endl;
		if(ch == '+'){
			int y,p;
			fin>>y>>p;
			// cout<<y<<" "<<p<<endl;
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