#include <stdio.h>
#include <stdlib.h>
struct item{
    char* string;
    int number;
};
typedef struct item Item;
#define swap(A,B) {Item t; t = A; A = B; B = t;}


Item nums[10] = {{"a",32},{"b",45},{"c",17},{"d",22},{"e",94},{"f",78},{"g",64},{"h",25},{"i",55},{"j",42}};
Item numsr[10] = {{"a",10},{"b",9},{"c",8},{"d",7},{"e",6},{"f",5},{"g",4},{"h",3},{"i",2},{"j",1}};
Item numsnr[10] = {{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9},{"j",10}};
 
void printlist(Item a[],int len){
    len = len + 1;
    for(int i = 0; i < len ; i++){
        printf("%s %d\n",a[i].string,a[i].number);
    }

}

int less(Item A,Item B){
    if(A.number < B.number){
        return 1;
    }
    return 0;
}
/*
void swap(Item A,Item B){
    Item t; 
    t = A; 
    A = B; 
    B = t;
}
*/
void selectionSort(Item a[], int lo, int hi)
{   
    
   int i, j, min,nswaps = 0;
   for (i = lo; i < hi-1; i++) {
      min = i;
      for (j = i+1; j <= hi; j++) {
         if (less(a[j],a[min])) min = j;
      }
      swap(a[i], a[min]);
      nswaps++;
   }
   printf("swaps: %d\n",nswaps);
   printlist(a,hi);
}

void bubbleSort(Item a[], int lo, int hi)
{
   int i, j, nswaps = 0;
   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         if (less(a[j], a[j-1])) {
            swap(a[j], a[j-1]);
            nswaps++;
         }
      }
      if (nswaps == 0) break;
   }
   printf("swaps: %d\n",nswaps);
   printlist(a,hi);
}

void insertionSort(Item a[], int lo, int hi)
{
   int i, j,ncomps = 0;
   Item val;
   for (i = lo+1; i <= hi; i++) {
      val = a[i];
      for (j = i; j > lo; j--) {
        
        ncomps++;
        //stops scan when position found?
         if (!less(val,a[j-1])) break;
         a[j] = a[j-1];
      }
      a[j] = val;
   }
   printf("comparisons: %d\n",ncomps);
   printlist(a,hi);
}

void merge(Item a[], int lo, int mid, int hi)
{
    int  i, j, k, nitems = hi-lo+1;
    Item *tmp = malloc(nitems*sizeof(Item));

    i = lo; j = mid+1; k = 0;
    // scan both segments, copying to tmp
    while (i <= mid && j <= hi) {
        if (less(a[i],a[j])){
            tmp[k++] = a[i++];
        }
        else{
            tmp[k++] = a[j++];
        }
    }
    // copy items from unfinished segment
    while (i <= mid){ tmp[k++] = a[i++];}
    while (j <= hi){ tmp[k++] = a[j++];}

    //copy tmp back to main array
    for (i = lo, k = 0; i <= hi; i++, k++){
      a[i] = tmp[k];
    }
    free(tmp);
}

void mergesort(Item a[], int lo, int hi)
{
   int mid = (lo+hi)/2; // mid point
   if (hi <= lo) return;
   mergesort(a, lo, mid);
   mergesort(a, mid+1, hi);
   merge(a, lo, mid, hi);
}



int main(void){
    printlist(nums,9);
    printf("\n");
    selectionSort(nums,0,9);
    printf("\n");
    printlist(numsr,9);
    printf("\n");
    selectionSort(numsr,0,9);
    printf("\n");
    printlist(numsnr,9);
    printf("\n");
    selectionSort(numsnr,0,9);
    printf("\n");
    
    
    

    return 0;
}
