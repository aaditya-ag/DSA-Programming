//Aaditya Agrawal
//19CS10003
#include "assign2.h"
#include <iostream>
using namespace std;


int max(int a, int b){
	return (a>b)?a:b;
}

Point* mergeLines(Point a[],Point b[],int sizel,int sizer,int* outputsize){
	int i=0,j=0;
	//size is the index that will mantain the size of merged array
	int size = 0;
	//curah,curbh store the current height of the outlines from a and b upto the current index
	int curah = 0;
	int curbh = 0;
	
	int totalSize = sizel + sizer;
	Point* ans = new Point[totalSize];
	
	while(i<sizel && j<sizer){
		
		if(a[i].x < b[j].x){
			//If the next point from a is higher than current b height, then a new point is generated
			if(a[i].y > curbh){
				ans[size] = a[i];		
				size++;		
			}
			else{
				//If current height of b is higher than new point but lower than olden point then a new point is generated
				if(curah > curbh){
					ans[size].x = a[i].x;
					ans[size].y = curbh;
					size++;
				}
			}
			curah = a[i].y;
			i++;
		}
		
		else if(a[i].x == b[j].x){
			//If new height is equal to previous height then its just a continuation of previous outline
			if(max(curah,curbh) != max(a[i].y,b[j].y)){
				ans[size].y = max(a[i].y,b[j].y);
				ans[size].x = a[i].x;				
				size++;
			}
			curah = a[i].y;
			curbh = b[j].y;
			i++;
			j++;
		}
		
		else{
			if(b[j].y > curah){
				ans[size] = b[j];		
				size++;		
			}
			else{
				if(curah < curbh){
					ans[size].x = b[j].x;
					ans[size].y = curah;
					size++;
				}
			}
			curbh = b[j].y;
			j++;
		}
	}
	
	while(i<sizel){
		ans[size++] = a[i++];
	}
	
	while(j<sizer){
		ans[size++] = b[j++];
	}
	

	*outputsize = size;
	return ans;
}
//The algorithm is that we obtain skylines from left and right and then continuously merge them
Point* mergeSort(Box boxes[],int lo,int hi,int* outputsize){
	if(lo==hi){
		Point* arr = new Point[2];
		arr[0].x = boxes[lo].left;
		arr[0].y = boxes[lo].ht;
		arr[1].x = boxes[lo].right;
		arr[1].y = 0;
		*outputsize = 2;
		return arr;
	}

	int mid = (lo + hi)/2;
	//sizel,sizer stores the size of outlines returned from left and right
	int sizel,sizer;
	Point* leftLine = mergeSort(boxes,lo,mid,&sizel);
	Point* rightLine = mergeSort(boxes,mid+1,hi,&sizer);

	Point* totalLine = mergeLines(leftLine,rightLine,sizel,sizer,outputsize);

	return totalLine;
}

Point* findOutLine(Box boxes[],int size,int* outputsize){
	Point* point_array;
	point_array = mergeSort(boxes,0,size-1,outputsize);
	return point_array;
}

int main(){
	process();
	return 0;
}