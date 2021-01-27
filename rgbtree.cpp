/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{
    for(auto const & [colour,photo]:photos){
        tree.push_back(colour);

    }
/* your code here! */

    rgbtreehelper(0,tree.size()-1,0);

}

void rgbtree::rgbtreehelper(int start,int end,int d){
    int median = (end + start)/2;
   
    if(start >= end){
        return;
    }

    quickSelect(start,end,median,d);

    rgbtreehelper(start,median-1,(d+1)%3);
    rgbtreehelper(median+1,end,(d+1)%3);

}

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{

/* your code here! */
    RGBAPixel currNearest(0,0,0);
    currNearest = findNearestNeighborHelper(query,0,tree.size()-1,0);
    return currNearest;

}

RGBAPixel rgbtree::findNearestNeighborHelper(const RGBAPixel & query,int start,int end, int d) const
{
    //declare in h yo
    if(start == end){
        return tree[start];
    }
    else if(start < end){
        int root = (start + end)/2;
        RGBAPixel currNearest = tree[root];
        int currNearestDist = distance(query, tree[root]); // Make this function later (get square ddistnac ebetween two points)
        RGBAPixel inNearest = currNearest; 

        if(smallerByDim(query,tree[root],d)){
            if (root > start){
                inNearest = findNearestNeighborHelper(query,start,root - 1,(d+1)%3);
            }

        } else {
            inNearest = findNearestNeighborHelper(query,root + 1,end,(d+1)%3);
        }

        int inNearestdistance = distance(query,inNearest);

        if(inNearestdistance < currNearestDist){
            currNearestDist = inNearestdistance;
            currNearest = inNearest;
        }

        RGBAPixel outNearest = currNearest;
        if(distToSplit(query,tree[root],d) < currNearestDist){

            if(smallerByDim(query,tree[root],d)){
                outNearest = findNearestNeighborHelper(query,root + 1,end,(d+1)%3);

            }
            else {       
                if (root > start){
                    outNearest = findNearestNeighborHelper(query,start,root - 1,(d+1)%3);
                }
            }

        }
        int outNearestdistance = distance(query,outNearest);

        if(outNearestdistance < currNearestDist){
            currNearestDist = outNearestdistance;
            currNearest = outNearest;
        }
        return currNearest;

    }


}
double rgbtree::distance(const RGBAPixel& pixa, const RGBAPixel& pixb) const{
    double distance = 0;
    distance += (pixa.r - pixb.r) *  (pixa.r - pixb.r);
    distance += (pixa.g - pixb.g) *  (pixa.g - pixb.g);
    distance += (pixa.b - pixb.b) *  (pixa.b - pixb.b);
    return distance;
}



bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{

/* your code here! */

	if(curDim == 0){
		if(first.r < second.r){
			return true;
		} else if(first.r > second.r){
			return false;
		} else{
			return first < second;
		}
	}
	if(curDim == 1){
		if(first.g < second.g){
			return true;
		}
		else if(first.g > second.g){
			return false;
		}
		else{
			return first < second;
		}
	}
	if(curDim == 2){
		if(first.b < second.b){
			return true;
		}
		else if(first.b > second.b){
			return false;
		}
		else{
			return first < second;
		}
	}

}

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{

/* your code here! */

	if(start >= end){
        return;
    }
    int index = partition(start,end,d);



    if(index > k){
        quickSelect(start, index-1, k,d);
    }
    if(index < k){
        quickSelect(index + 1,end,k,d);
    }

}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    // this is the partition code from HW2 it doesn't work!!
   /* int p = lo;
    for( int i=lo+1; i <= hi; i++ )
      if( tree[i] < tree[lo] ) { p++; swap(tree[p], tree[i]); }
    swap(tree[lo], tree[p]);
    return p;*/
// check condition for if statement again
    int p = lo; 
    for (int i = lo+1; i <= hi; i++) { 
        if(smallerByDim(tree[i],tree[lo],d)) { 
            p++;
            swap(tree[p], tree[i]);  
            } 
        }
        swap(tree[lo], tree[p]); 
        return p;      
}



/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{

/* your code here! */
    if(dimension == 0){
        return (query.r - curr.r)*(query.r - curr.r);
    }
    if(dimension == 1){
        return (query.g - curr.g)*(query.g - curr.g);
    }
    if(dimension == 2){
        return (query.b - curr.b)*(query.b - curr.b);
    }

}

