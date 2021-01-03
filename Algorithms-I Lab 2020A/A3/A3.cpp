//Aaditya Agrawal
//19CS10003
#include<iostream>
using namespace std;

typedef struct{
	int x,y;
} POINT;

//Utility functions start
//Returns true if point lies inside grid
bool isvalid(int x,int y,int a,int b){
	return (x>=0&&y>=0&&x<a&&y<b);
}


void printgrid(char *grid[],int a,int b){
	cout<<"Grid of Stones is:\n";
	for(int i=0;i<a;i++){
		for(int j=0;j<b;j++){
			cout<<grid[i][j];
		}
		cout<<endl;
	}
}
//Utility functions end

//Queue implementation starts
typedef struct{
	POINT *Arr;
	int queue_size;
	int start_id,end_id;
} QUEUE;
//start_id points to front element, end_id points to rear element


void init(QUEUE *qP, int size){//allocates space for queue
	qP->Arr = (POINT *)malloc(size*sizeof(POINT));
	qP->queue_size = 0;
	qP->start_id = -1;
	qP->end_id = -1;
}

int isempty(QUEUE qP){//returns 1 if the queue is empty, 0 otherwise
	if(qP.queue_size == 0){
		return 1;
	}
	return 0;
}

void enqueue(QUEUE *qP,POINT p){
	if(isempty(*qP)){
		qP->start_id = 0;
		qP->end_id = 0;
		qP->Arr[0] = p;
		qP->queue_size = 1;
	}
	else{
		qP->end_id = qP->end_id + 1;
		qP->Arr[qP->end_id] = p;
		qP->queue_size = qP->queue_size + 1;
	}
}

POINT dequeue(QUEUE *qP){
	if(!isempty(*qP)){
		POINT popPoint = qP->Arr[qP->start_id];
		qP->start_id = qP->start_id + 1;
		qP->queue_size = qP->queue_size - 1;
		return popPoint;
	}
}
//Queue implementation ends


//BFS to find path existence
int strategy1(char *grid[], int a,int b, POINT src, POINT dest){
	//Visited array initialised to zero
	int **vis = (int **)malloc(a*sizeof(int *));
	for(int i=0;i<a;i++){
		vis[i] = (int *)malloc(b*sizeof(int));
		for(int j=0;j<b;j++){
			vis[i][j] = 0;
		}
	}

	QUEUE q;
	int size = a*b + 1;
	init(&q, size);

	enqueue(&q,src);
	vis[src.x][src.y] = 1;	

	while(!isempty(q)){

		POINT u = dequeue(&q);

		if(u.x == dest.x && u.y == dest.y){
			cout<<"Path Exists\n";
			return 1;
		}

		if(isvalid(u.x-1,u.y,a,b) && !vis[u.x-1][u.y] && grid[u.x-1][u.y] == '*'){
			POINT temp;
			temp.x = u.x - 1;
			temp.y = u.y;
			vis[temp.x][temp.y] = 1;
			enqueue(&q,temp);
		}
		if(isvalid(u.x+1,u.y,a,b) && !vis[u.x+1][u.y] && grid[u.x+1][u.y] == '*'){
			POINT temp;
			temp.x = u.x + 1;
			temp.y = u.y;
			vis[temp.x][temp.y] = 1;
			enqueue(&q,temp);
		}
		if(isvalid(u.x,u.y-1,a,b) && !vis[u.x][u.y-1] && grid[u.x][u.y-1] == '*'){
			POINT temp;
			temp.x = u.x;
			temp.y = u.y - 1;
			vis[temp.x][temp.y] = 1;
			enqueue(&q,temp);
		}
		if(isvalid(u.x,u.y+1,a,b) && !vis[u.x][u.y+1] && grid[u.x][u.y+1] == '*'){
			POINT temp;
			temp.x = u.x;
			temp.y = u.y + 1;
			vis[temp.x][temp.y] = 1;
			enqueue(&q,temp);
		}
	}
	cout<<"No Path Exists\n";
	return 0;
}

//Stack implementation starts
struct STACK{
	POINT head;
	STACK *next;
};

//Stack is implemented is such that first value is a point outside stack, then the proper stack is there
//for ex: if (1,1) is pushed followed by (2,2)
//Then stack look likes  this top->(-1,1)->(2,2)->(1,1)->NULL
//So values are popped and pushed after the first element
//This was done because function definition required use of single pointer instead of double pointer

void init(STACK *s){
	(s->head).x = -1;
	(s->head).y = -1;
	s->next = NULL;
}

int isempty(STACK *s){
	if(s->next == NULL){
		return 1;
	}
	return 0;
}

void push(STACK *s,POINT p){
	STACK *newNode = (STACK *)malloc(sizeof(STACK));
	newNode->head = p;
	newNode->next = s->next;
	s->next = newNode;
}

POINT pop(STACK *s){
	if(!isempty(s)){
		POINT temp = s->next->head;
		s->next = s->next->next;
		return temp;
	}

}
//Stack implement ends

//DFS to find and print path
void strategy2(char *grid[], int a,int b, POINT src, POINT dest){
	STACK s;
	int **vis = (int **)malloc(a*sizeof(int *));
	for(int i=0;i<a;i++){
		vis[i] = (int *)malloc(b*sizeof(int));
		for(int j=0;j<b;j++){
			vis[i][j] = 0;
		}
	}
	init(&s);
	push(&s,src);

	//A flag variable which checks if a path is found
	//Even if BFS isnt called earlier, still this will work
	int pointFound = 0;
	vis[src.x][src.y] = 1;
	while(!isempty(&s)){
		POINT u = (s.next)->head;

		if(u.x == dest.x && u.y == dest.y){
			pointFound = 1;
			break;
		}
		//I check if there is any neighbour of u left unexplored, if not we pop u
		if(isvalid(u.x-1,u.y,a,b) && !vis[u.x-1][u.y] && grid[u.x-1][u.y] == '*'){
			POINT temp;
			temp.x = u.x - 1;
			temp.y = u.y;
			vis[temp.x][temp.y] = 1;
			push(&s,temp);
			continue;
		}
		if(isvalid(u.x+1,u.y,a,b) && !vis[u.x+1][u.y] && grid[u.x+1][u.y] == '*'){
			POINT temp;
			temp.x = u.x + 1;
			temp.y = u.y;
			vis[temp.x][temp.y] = 1;
			push(&s,temp);
			continue;

		}
		if(isvalid(u.x,u.y-1,a,b) && !vis[u.x][u.y-1] && grid[u.x][u.y-1] == '*'){
			POINT temp;
			temp.x = u.x;
			temp.y = u.y - 1;
			vis[temp.x][temp.y] = 1;
			push(&s,temp);
			continue;
		}
		if(isvalid(u.x,u.y+1,a,b) && !vis[u.x][u.y+1] && grid[u.x][u.y+1] == '*'){
			POINT temp;
			temp.x = u.x;
			temp.y = u.y + 1;
			vis[temp.x][temp.y] = 1;
			push(&s,temp);
			continue;
		}
		pop(&s);
	}


	if(pointFound){
		//The order of vertices is destination to source
		//So we push all the vertices in another to stack to reverse order
		//Source to destination
		STACK ans;
		init(&ans);
		while(!isempty(&s)){
			push(&ans,pop(&s));
		}
		while(!isempty(&ans)){
			POINT temp = pop(&ans);
			cout<<"("<<temp.x<<","<<temp.y<<"),";
		}
		cout<<endl;
	}
}


int main(){
	int a,b,n;

	cin>>a>>b>>n;
	POINT src,dest;

	char **grid = (char **)malloc(a*sizeof(char *));

	for(int i=0;i<a;i++){
		grid[i] = (char *)malloc(b*sizeof(char));
		for(int j=0;j<b;j++){
			grid[i][j] = '*';
		}
	}

	int *unstablex = (int *)malloc(n*sizeof(int));
	int *unstabley = (int *)malloc(n*sizeof(int));
	
	for(int i=0;i<n;i++){
		cin>>unstablex[i];
	}
	for(int i=0;i<n;i++){
		cin>>unstabley[i];
	}

	cin>>src.x>>src.y>>dest.x>>dest.y;

	//Marking the unstable rocks
	for(int i=0;i<n;i++){
		grid[unstablex[i]][unstabley[i]] = '-';
	}

	printgrid(grid,a,b);

	int pathExist = strategy1(grid,a,b,src,dest);

	//If a path doesnt exist, then we dont call DFS function and exit
	if(!pathExist){
		return 0;
	}

	//Calling DFS
	strategy2(grid,a,b,src,dest);


}