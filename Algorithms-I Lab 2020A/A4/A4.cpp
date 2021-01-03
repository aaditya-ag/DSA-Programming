#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
using namespace std;

struct node{
	char *symbol;
	int frequency;
	node *next;
	node *left;
	node *right;
};

ofstream fout;
//Utility functions
bool isdigit(char ch){
	return (ch>='0'&&ch<='9');
}
bool isalpha(char ch){
	return (ch>='a'&&ch<='z');
}

node *insert(node *root,node *temp){
	if(root == NULL){
		return temp;
	}
	root->next = insert(root->next,temp);
	return root;
}

void printList(node *root){
	while(root != NULL){
		cout<<root->symbol<<"="<<root->frequency<<endl;
		root = root->next;
	}
}
//Utility functions end

//Here we find string of Ni for given i
char *findSymbol(int num){
	char *s = (char *)malloc(100*sizeof(char));
	s[0] = 'N';
	int len = 1;
	int n = num;
	while(n){
		len++;
		n /= 10;
	}
	while(num){
		s[--len] = char(num%10 + '0');
		num = num/10;
	}
	return s;
}



//Compare nodes handles the priority(should a come before b) in sorted list
int compareNode(node *x, node *y){
	if(x->frequency < y-> frequency){
		return 1;
	}
	else if(x->frequency > y-> frequency){
		return 0;
	}
	char *a = x->symbol;
	char *b = y->symbol;
	if(strlen(a) == 1 && strlen(b) > 1){
		return 0;
	}
	else if(strlen(b) == 1 && strlen(a) > 1){
		return 1;
	}
	if(strlen(a) < strlen(b)){
		return 1;
	}
	else if(strlen(a) > strlen(b)){
		return 0;
	}
	else{
		if(strlen(a) == 1){
			if(isdigit(a[0]) && isalpha(b[0])){
				return 0;
			}
			else if(isdigit(b[0]) && isalpha(a[0])){
				return 1;
			}

		}
		for(int i=0;i<strlen(a);i++){
			if(a[i] < b[i]){
				return 1;
			}
			else if(a[i] > b[i]){
				return 0;
			}
		}
	}
}

//Here we insert supernodes in linked list
node *insertSuperNode(node *root,node *temp){
	if(root == NULL){
		return temp;
	}
	if(compareNode(root,temp)){
		root->next = insertSuperNode(root->next,temp);
	}
	else{
		temp->next = root;
		root = temp;
	}
	return root;


}

void preOrder(node *root){
	if(root == NULL){
		return;
	}
	fout<<root->symbol<<"("<<root->frequency<<"),";
	preOrder(root->left);
	preOrder(root->right);
}

//Implementation of algorithm given in question
void treeTraverse(node *root,char *ans[],char cur[]){
	if(root==NULL){
		return;
	}
	if(root->left== NULL && root->right == NULL){
		if(isalpha(root->symbol[0])){
			ans[root->symbol[0] - 'a'] = (char *)malloc((strlen(cur)+1)*sizeof(char));
			for(int i=0;i<strlen(cur);i++){
				ans[root->symbol[0] - 'a'][i] = cur[i];
			}
			ans[root->symbol[0] - 'a'][strlen(cur)]='\0';
		}
		else{
			ans[root->symbol[0] - '0' + 26] = (char *)malloc((strlen(cur)+1)*sizeof(char));
			for(int i=0;i<strlen(cur);i++){
				ans[root->symbol[0] - '0' + 26][i] = cur[i];
			}
			ans[root->symbol[0] - '0' + 26][strlen(cur)]='\0';
		}
		return;
	}
	char temp1[100],temp2[100];
	for(int i=0;i<strlen(cur);i++){
		temp1[i] = cur[i];
		temp2[i] = cur[i];
	}
	temp1[strlen(cur)] = '0';
	temp1[strlen(cur) + 1] = '\0';
	temp2[strlen(cur)] = '1';
	temp2[strlen(cur) + 1] = '\0';
	treeTraverse(root->left,ans,temp1);
	treeTraverse(root->right,ans,temp2);
}

int main(){
	ifstream fin;
	fin.open("log.txt",ios::in);
	fout.open("output.txt",ios::out);

	int n;
	fin>>n;

	int alpha[26];
	int num[10];

	for(int i=0;i<26;i++){
		alpha[i] = 0;
		if(i<10){
			num[i] = 0;
		}
	}

	char s[100];
	for(int i=0;i<n;i++){
		fin>>s;
		for(int j=0;j<strlen(s);j++){
			if(isalpha(s[j])){
				alpha[s[j] - 'a']++;
			}
			else{
				num[s[j] - '0']++;
			}
		}
	}

	for(int i=0;i<26;i++){
		fout<<char(i+'a')<<"="<<alpha[i]<<",";
	}
	for(int i=0;i<10;i++){
		fout<<i<<"="<<num[i]<<",";
	}
	fout<<endl;

	node *root = NULL;

	for(int i=0;i<36;i++){
		int minfreq = 1000000;
		node *temp = (node *)(malloc(sizeof(node)));
		temp->symbol = (char *)malloc(2 * sizeof(char));
		

		for(int j=0;j<26;j++){
			if(alpha[j] != -1 && alpha[j] < minfreq){
				minfreq = alpha[j];
				temp->symbol[0] = j + 'a';
			}
		}
		for(int j=0;j<10;j++){
			if(num[j] != -1 && num[j] < minfreq){
				minfreq = num[j];
				temp->symbol[0] = j + '0';
			}
		}
		if(isdigit(temp->symbol[0])){
			num[temp->symbol[0] - '0'] = -1;
		}
		else{
			alpha[temp->symbol[0] - 'a'] = -1;
		}

		temp->symbol[1] = '\0';

		temp->frequency = minfreq;
		temp->next = NULL;
		temp->left = NULL;
		temp->right = NULL;
		root = insert(root,temp);
	}
	//Part 1 ends
	//Part 2 starts
	int numSuper = 0;
	while(root -> next != NULL){
		node *leftChild = root;
		root = root->next;
		node *rightChild = root;
		root = root->next;


		if(!compareNode(leftChild,rightChild)){
			node *temp = leftChild;
			leftChild = rightChild;
			rightChild = temp;
		}


		node *superNode = (node *)malloc(sizeof(node));
		superNode->next = NULL;
		superNode->left = leftChild;
		superNode->right = rightChild;
		superNode->frequency = superNode->left->frequency + superNode->right->frequency;
		superNode->symbol = findSymbol(++numSuper);
		root = insertSuperNode(root,superNode);
	}

	preOrder(root);
	fout<<endl;
	char *ans[36];
	//0-25 holds for a-z and then 0-9
	char cur[100] = "";
	treeTraverse(root,ans,cur);
	for(int i=0;i<36;i++){
		if(i<26){
			fout<<char(i+'a')<<" ";
		}
		else{
			fout<<i-26<<" ";
		}
		fout<<ans[i]<<endl;
	}
	//Inline encoding, didnt write a function
	ifstream fin1;
	fin1.open("encode.txt");
	int numVar;
	fin1>>numVar;
	for(int i=0;i<numVar;i++){
		char s[100];
		fin1>>s;
		for(int j=0;j<strlen(s);j++){
			if(isdigit(s[j])){
				fout<<ans[s[j]-'0'+26];
			}
			else{
				fout<<ans[s[j]-'a'];
			}
		}
		fout<<endl;
	}
	//Decoding
	// ifstream fin2;
	// fin2.open("encode.txt");
	// int numVar1;
	// fin2>>numVar1;
	// for(int i=0;i<numVar;i++){
	// 	char s[1000];
	// 	fin2>>s;
	// 	node *temp = root;
	// 	for(int j=0;j<strlen(s);j++){
	// 		if(root->left == NULL && root->right == NULL){
	// 			fout<<root->sy
	// 		}
	// 	}
	// 	fout<<endl;
	// }

}
