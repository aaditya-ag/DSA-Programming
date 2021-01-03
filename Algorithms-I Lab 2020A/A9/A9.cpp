//Aaditya Agrawal
//19CS10003
#include<iostream>
using namespace std;

#define MAX_SIZE 100
typedef struct _job{
	int jobId;
	int startTime;
	int jobLength;
	int remLength;
}job;

typedef struct _heap{
	job list[MAX_SIZE];
	int numJobs;
}heap;

/*+++++++++++++UTILITY FUNCTIONS START++++++++++*/
bool ismin(job a,job b){
	if(a.remLength == b.remLength){
		return a.jobId < b.jobId;
	}
	return a.remLength < b.remLength;
}



void updateUp(heap *H,int idx){
	int par = idx/2;
	while(par>=1 && ismin(H->list[idx],H->list[par])){
		job temp = H->list[par];
		H->list[par] = H->list[idx];
		H->list[idx] = temp;

		idx = par;
		par = idx/2;
	}
}

void Heapify(heap *H,int i){
	int smallest = i;
	int leftch = 2*i;
	int rightch = 2*i + 1;
	int n = H->numJobs;

	if(leftch <=n && ismin(H->list[leftch],H->list[smallest])){
		smallest = leftch;
	}
	if(rightch <=n && ismin(H->list[rightch],H->list[smallest])){
		smallest = rightch;
	}


	if(smallest != i){
		//Swapping two jobs
		job temp = H->list[smallest];
		H->list[smallest] = H->list[i];
		H->list[i] = temp;


		Heapify(H,smallest);
	}
}

/*+++++++++++++UTILITY FUNCTIONS END++++++++++*/


/*+++++++++++++HEAP FUNCTIONS START++++++++++*/

void initHeap(heap *H){
	H->numJobs = 0;
}

void insertJob(heap *H,job j){
	H->numJobs = H->numJobs + 1;
	H->list[H->numJobs] = j;
	updateUp(H,H->numJobs);
}

int extractMinJob(heap *H, job *j){
	if(H->numJobs == 0){
		return -1;
	}

	//Swapping two jobs
	job t = H->list[H->numJobs];
	H->list[H->numJobs] = H->list[1];
	H->list[1] = t;

	//The last element is removed minimum job
	*j = H->list[H->numJobs];
	H->numJobs = H->numJobs - 1;


	Heapify(H,1);
}
/*+++++++++++++HEAP FUNCTIONS END++++++++++*/




/*+++++++++++++SCHEDULER START++++++++++++*/
void scheduler(job jobList[], int n){
	heap *H = (heap *)malloc(sizeof(heap));
	initHeap(H);
	int curjob = -1;
	int i = 0;
	int jobDone = 0;

	int trntime = 0;
	cout<<"Jobs scheduled at each timestep are: \n";
	for(int T=0;;T++){
		while(i < n && jobList[i].startTime == T){
				insertJob(H,jobList[i]);
				curjob = H->list[1].jobId;
				i++;
		}
		cout<<curjob<<" ";
		if(H->list[1].remLength == H->list[1].jobLength){
			trntime += (T - H->list[1].startTime);
		}
		H->list[1].remLength = H->list[1].remLength - 1;
		if(H->list[1].remLength == 0){
			job cur;
			jobDone++;
			extractMinJob(H,&cur);
			if(H->numJobs == 0){
				curjob = -1;
			}
			else{
				curjob = H->list[1].jobId;
			}
		} 
		if(jobDone == n){
			break;
		}
	}
	cout<<"\nAverage Turnaround Time is "<<double(1.0*trntime)/double(n);
	cout<<endl;

}
/*+++++++++++++SCHEDULER END++++++++++++*/




int main(){
	int n;
	cout<<"Enter no. of jobs (n): ";
	cin>>n;

	job j[n];
	int maxStartTime = 0;

	cout<<"Enter the jobs: \n";
	for(int i=0;i<n;i++){
		cin>>j[i].jobId>>j[i].startTime>>j[i].jobLength;
		j[i].remLength = j[i].jobLength;
		maxStartTime = max(maxStartTime,j[i].startTime);
	}

	/*+++++++++++++COUNTING SORT START++++++++++++*/
	int count[maxStartTime+1];
	job b[n];
	for(int i=0;i<=maxStartTime;i++){
		count[i] = 0;
	}
	for(int i=0;i<n;i++){
		count[j[i].startTime]++;
	}
	for(int i=1;i<=maxStartTime;i++){
		count[i] += count[i-1];
	}
	for(int i=maxStartTime;i>0;i--){
		count[i] = count[i-1];
	}
	count[0] = 0;
	for(int i=0;i<n;i++){
		b[count[j[i].startTime]] = j[i];
		count[j[i].startTime]++;
	}
	for(int i=0;i<n;i++){
		j[i] = b[i];
	}
	/*+++++++++++++COUNTING SORT END++++++++++++*/


	scheduler(j,n);


}