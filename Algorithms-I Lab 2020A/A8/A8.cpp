/*
	Aaditya Agrawal
	19CS10003
*/
#include<iostream>
using namespace std;
#define INF 100000000
/*
	Algorithm: First we find minimum and maximum elevation. Then we binary search over
	the elevation and then in an iteration we fix the max elevation, then we see if its possible to 
	reach from source to destination(by DFS) only using points having elevation lower than the limit.
	Time complexity: O((V+E)log(maxTime))
	where V = number of vertices = O(n^2)
	and E = number of edges = O(n^2)
*/

struct point{
	int x,y;
	int h;
};

bool isvalid(int x,int y,int n){
	return x>=0&&y>=0&&x<n&&y<n;
}

int max(int a,int b){
	return (a>b)?a:b;
}
int min(int a,int b){
	return (a<b)?a:b;
}

/*Part 1 Solution*/
/*
	We do dfs to find if source and destination are connected.
	dir array stores the the direction in which a parent of a node is located.
	Left(L), Up(U), Right(R),Down(D)
*/
bool dfs(int **grid,int x,int y,int **vis,char **dir,int n,int maxElevation,point f){
	vis[x][y] = 1;
	if(grid[x][y] > maxElevation){
		return false;
	}
	if(x==f.x && y == f.y){
		return true;
	}
	bool isfound = false;
	if(isvalid(x-1,y,n) && !vis[x-1][y]){
		isfound = isfound || dfs(grid,x-1,y,vis,dir,n,maxElevation,f);
		dir[x-1][y] = 'D';
	}
	if(isvalid(x+1,y,n) && !vis[x+1][y]){
		isfound = isfound || dfs(grid,x+1,y,vis,dir,n,maxElevation,f);
		dir[x+1][y] = 'U';
	}
	if(isvalid(x,y-1,n) && !vis[x][y-1]){
		isfound = isfound || dfs(grid,x,y-1,vis,dir,n,maxElevation,f);
		dir[x][y-1] = 'R';
	}
	if(isvalid(x,y+1,n) && !vis[x][y+1]){
		isfound = isfound || dfs(grid,x,y+1,vis,dir,n,maxElevation,f);
		dir[x][y+1] = 'L';
	}
	return isfound;
}


void reconstructPath(int x,int y,char **dir,int *BlocksVisited){
	*BlocksVisited = *BlocksVisited + 1;
	if(dir[x][y] != 'S'){
		if(dir[x][y] == 'U'){
			reconstructPath(x-1,y,dir,BlocksVisited);
		}
		else if(dir[x][y] == 'D'){
			reconstructPath(x+1,y,dir,BlocksVisited);
		}
		else if(dir[x][y] == 'L'){
			reconstructPath(x,y-1,dir,BlocksVisited);
		}
		else if(dir[x][y] == 'R'){
			reconstructPath(x,y+1,dir,BlocksVisited);
		}
		cout<<",("<<x<<","<<y<<")";
		return;
	}
	cout<<"("<<x<<","<<y<<")";
}



void findMinTime(int **grid,int n,point s,point f){
	//An array to store points and sort them
	if(s.x==f.x && s.y == f.y){
		cout<<"Minimum time taken is: "<<0<<endl;
		cout<<"Path from ("<<s.x<<","<<s.y<<") to ("<<f.x<<","<<f.y<<") is:\n("<<f.x<<','<<f.y<<")";
		cout<<"\nThe Number of Blocks traversed are: "<<1;
		cout<<endl;
		return;
	}
	int lo = 0,hi = 0;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			hi = max(hi,grid[i][j] + 1);
		}
	}


	int **vis = new int *[n];
	char **dir = new char *[n];
	for(int i=0;i<n;i++){
		vis[i] = new int[n];
		dir[i] = new char[n];
		for(int j=0;j<n;j++){
			dir[i][j] = 'S';
		}
	}

	while(lo<hi){
		int mid = (lo + hi)/2;
		
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				vis[i][j] = 0;
			}
		}

		bool result = dfs(grid,s.x,s.y,vis,dir,n,mid,f);
		if(result == true){
			hi = mid;
		}
		else{
			lo = mid+1;
		}
	}

	//Counting number of blacks travelled
	int BlocksVisited = 0;
	
	cout<<"Minimum time taken is: "<<lo<<endl;
	cout<<"Path from ("<<s.x<<","<<s.y<<") to ("<<f.x<<","<<f.y<<") is:\n";

	reconstructPath(f.x,f.y,dir,&BlocksVisited);

	cout<<"\nThe Number of Blocks traversed are: "<<BlocksVisited;

	cout<<endl;
}
/*Part 1 solution ends*/


/*Bonus question solution starts*/
void leastTimeDijkstra(int **grid,int n,point s,point f){
	int inSet[n][n];
	point par[n][n];
	int dis[n][n];


	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			inSet[i][j] = 0;
			dis[i][j] = INF;
			par[i][j] = {-1,-1};
		}
	}
	inSet[s.x][s.y] = 1;
	dis[s.x][s.y] = 0;

	int dx[4] = {0,0,1,-1};
	int dy[4] = {1,-1,0,0};
	for(int i=0;i<4;i++){
		int nx = s.x + dx[i];
		int ny = s.y + dy[i];
		if(isvalid(nx,ny,n)){
			dis[nx][ny] = max(grid[s.x][s.y],grid[nx][ny]);
		}
	}


	for(int k=1;k<=n*n-1;k++){
		point vnear;int mindis = INF;
		for(int x=0;x<n;x++){
			for(int y=0;y<n;y++){
				if(inSet[x][y] == 0 && dis[x][y] < mindis){
					vnear = {x,y};
					mindis = dis[x][y];
				}
			}
		}
		inSet[vnear.x][vnear.y] = 1;
		for(int i=0;i<4;i++){
			int nx = vnear.x + dx[i];
			int ny = vnear.y + dy[i];
			if(isvalid(nx,ny,n)){
				dis[nx][ny] = min(dis[nx][ny],max(dis[vnear.x][vnear.y],grid[nx][ny]));
			}
		}
	}



	cout<<"++++DIJKSTRA'S ALGORITHM++++\n";
	cout<<"++++BONUS QUESTION++++\n";
	cout<<"Minimum time taken is: "<<dis[f.x][f.y]<<endl;
}
/*Bonus question solution ends*/


int main(){
	int n;
	cin>>n;
	int **grid = new int *[n];
	for(int i=0;i<n;i++){
		grid[i] = new int[n];
		for(int j=0;j<n;j++){
			cin>>grid[i][j];			
		}
	}
	point s,f;
	cin>>s.x>>s.y>>f.x>>f.y;
	findMinTime(grid,n,s,f);

	/*+++++++++++++++BONUS PART+++++++++++++++*/
	leastTimeDijkstra(grid,n,s,f);
}