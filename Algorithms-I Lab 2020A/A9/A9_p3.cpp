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


/*
	I have added an index array which maps jobId to index in heap array
*/
typedef struct _heap{
	job list[MAX_SIZE];
	int index[MAX_SIZE];
	int numJobs;
}newheap;



typedef struct _jobPair{
	int jobid_from;
	int jobid_to;
} jobpair;

bool ismin(job a,job b){
	if(a.remLength == b.remLength){
		return a.jobId < b.jobId;
	}
	return a.remLength < b.remLength;
}

int max(int a,int b){
	return (a>b)?a:b;
}

void updateUp(newheap *H,int idx){
	int par = idx/2;
	while(par>=1 && ismin(H->list[idx],H->list[par])){
		job temp = H->list[par];
		H->list[par] = H->list[idx];
		H->list[idx] = temp;

		H->index[H->list[idx].jobId] = idx;
		H->index[H->list[par].jobId] = par;

		idx = par;
		par = idx/2;
	}
}

void Heapify(newheap *H,int i){
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
		job temp = H->list[smallest];
		H->list[smallest] = H->list[i];
		H->list[i] = temp;

		H->index[H->list[i].jobId] = i;
		H->index[H->list[smallest].jobId] = smallest;
		Heapify(H,smallest);
	}
}

void initHeap(newheap *H){
	H->numJobs = 0;
	for(int i=0;i<MAX_SIZE;i++){
		H->index[i] = -1;
	}
}

void insertJob(newheap *H,job j){
	H->numJobs = H->numJobs + 1;
	H->list[H->numJobs] = j;
	H->index[H->list[H->numJobs].jobId] = H->numJobs;
	updateUp(H,H->numJobs);
}

int extractMinJob(newheap *H, job *j){
	if(H->numJobs == 0){
		return -1;
	}
	job t = H->list[H->numJobs];
	H->list[H->numJobs] = H->list[1];
	H->list[1] = t;

	*j = H->list[H->numJobs];

	H->index[H->list[1].jobId] = 1;
	H->index[H->list[H->numJobs].jobId] = H->numJobs;

	H->numJobs = H->numJobs - 1;
	Heapify(H,1);
	return 0;
}


void decreasekey(newheap *H, int jid){
	int i = H->index[jid];
	if(i!=-1 && H->list[i].remLength == H->list[i].jobLength){
		H->list[i].remLength = H->list[i].remLength/2;
		H->list[i].jobLength = H->list[i].remLength;
		updateUp(H,i);
	}
}


void newScheduler(job jobList[], int n,jobpair pairList[],int m){
	newheap *H = (newheap *)malloc(sizeof(newheap));
	initHeap(H);
	int curjob = -1;
	int i = 0;
	int jobDone = 0;


	//jobList is sorted by starting time
	//b stores the elements by jobId(there may be some elements which are not so to update remTime for those elements b is useful)
	int maxStartTime=0;
	for(int i=0;i<n;i++){
		maxStartTime = max(maxStartTime,jobList[i].startTime);
	}
	int count[maxStartTime+1];
	job b[n+1];
	for(int i=0;i<=maxStartTime;i++){
		count[i] = 0;
	}
	for(int i=0;i<n;i++){
		count[jobList[i].startTime]++;
	}
	for(int i=1;i<=maxStartTime;i++){
		count[i] += count[i-1];
	}
	for(int i=maxStartTime;i>0;i--){
		count[i] = count[i-1];
	}
	count[0] = 0;
	for(int i=0;i<n;i++){
		b[count[jobList[i].startTime]] = jobList[i];
		count[jobList[i].startTime]++;
	}
	for(int i=0;i<n;i++){
		job t = jobList[i];
		jobList[i] = b[i];
		b[i] = t;
	}
	
	for(int i=n;i>0;i--){
		b[i] = b[i-1];
	}

	int trntime = 0;
	cout<<"Jobs scheduled at each timestep are: \n";
	for(int T=0;;T++){
		while(i < n && jobList[i].startTime == T){
				jobList[i].remLength = b[jobList[i].jobId].remLength;
				jobList[i].jobLength = b[jobList[i].jobId].jobLength;
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

			H->index[H->list[1].jobId] = -1;
			extractMinJob(H,&cur);
			for(int k=0;k<m;k++){
				if(pairList[k].jobid_from == curjob){
					decreasekey(H,pairList[k].jobid_to);
					if(b[pairList[k].jobid_to].remLength == b[pairList[k].jobid_to].jobLength){
						b[pairList[k].jobid_to].remLength /= 2;
						b[pairList[k].jobid_to].jobLength /= 2;
					}
				}
			}

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

int main(){
	int n;
	cout<<"Enter no. of jobs (n): ";
	cin>>n;
	job j[n];
	cout<<"Enter the jobs: \n";
	for(int i=0;i<n;i++){
		cin>>j[i].jobId>>j[i].startTime>>j[i].jobLength;
		j[i].remLength = j[i].jobLength;
	}
	// scheduler(j,n);
	int m;
	cout<<"Enter the number of dependency pairs\n";
	cin>>m;
	cout<<"Enter the dependency pairs\n";
	jobpair pairList[m];
	for(int i=0;i<m;i++){
		cin>>pairList[i].jobid_from>>pairList[i].jobid_to;
	}
	newScheduler(j,n,pairList,m);



}