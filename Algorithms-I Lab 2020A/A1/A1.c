//Name:Aaditya Agrawal
#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	int data;
	struct node *add;
}node;

void traverse_from_front_to_end(struct node *head){
	struct node *prev = NULL;
	while(head != NULL){
		printf("%d, ",head->data);
		node *temp = head;
		head = (struct node *)((unsigned long long)(head->add)^(unsigned long long)prev);
		prev = temp;
	}
}

void traverse_from_end_to_front(struct node *tail){
	struct node *next = NULL;
	while(tail != NULL){
		printf("%d, ",tail->data);
		node *temp = tail;
		tail = (struct node *)((unsigned long long)(tail->add)^(unsigned long long)next);
		next = temp;
	}
}

void reverse(struct node **head, struct node **tail){
	node *temp = *head;
	*head = *tail;
	*tail = temp;
}

void alternate(struct node **head, struct node **tail,int n){
	reverse(head,tail);
	struct node *prevhd = NULL,*prevtl = NULL;
	struct node *nexthd,*nexttl;
	struct node *prev = NULL;
	struct node *hd = *head;
	struct node *tl = *tail;

	
	*head = *tail;
	while(1){
		nexthd = (struct node *)((unsigned long long)(hd->add)^(unsigned long long)prevhd);
		nexttl = (struct node *)((unsigned long long)(tl->add)^(unsigned long long)prevtl);
		prevhd = hd;
		prevtl = tl;
		if(n==1){
			tl->add = (struct node *)((unsigned long long)prev^(unsigned long long)NULL);
			prev = tl;
			*tail = prev;
			return;
		}
		else{
			tl->add = (struct node *)((unsigned long long)prev^(unsigned long long)hd);
			prev = tl;
		}
		
		n--;
		if(n==1){
			hd->add = (struct node *)((unsigned long long)prev^(unsigned long long)NULL);
			prev = hd;
			*tail = prev;
			return;
		}
		else{
			hd->add = (struct node *)((unsigned long long)prev^(unsigned long long)nexttl);
			prev = hd;
		}
		tl = nexttl;
		hd = nexthd;
		n--;
	}

}

int main(){
	int n;
	node *head = NULL;
	node *tail = NULL;
	printf("n=");
	scanf("%d",&n);
	printf("Enter the %d integers between -100 to 100: ",n);
	struct node *add,*prev = NULL;
	struct node *next;
	struct node *cur = (struct node *)malloc(sizeof(node));
	for(int i=0;i<n;i++){
		int num;
		scanf("%d",&num);
		cur->data = num;
		tail = cur;
		if(i==n-1){
			next = NULL;
		}
		else{
			next = (struct node *)malloc(sizeof(node));
		}
		if(head == NULL){
			head = cur;
		}
		cur->add = (struct node *)((unsigned long long)prev^(unsigned long long)next);
		prev = cur;
		cur = next;
	}
	
	printf("Doubly linked list from front to end: ");
	
	traverse_from_front_to_end(head);
	
	printf("\n");	
	printf("Doubly linked list from end to front: ");
	
	traverse_from_end_to_front(tail);
	
	printf("\n");
	reverse(&head,&tail);

	printf("Reversed doubly linked list from front to end: ");	
	traverse_from_front_to_end(head);	
	printf("\n");

	alternate(&head,&tail,n);
	printf("Alternated doubly linked list from front to end: ");
	traverse_from_front_to_end(head);	
	printf("\n");

}