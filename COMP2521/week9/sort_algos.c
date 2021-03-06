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

Item numsnr[] = {{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9},{"j",10},{"k",11}};
Item numsnrr[] = {{"k",11},{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9},{"j",10}};

Item numsnr1[] = {{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9},{"j",10},{"k",11}};
Item numsnrr1[] = {{"k",11},{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9},{"j",10}};

Item numsss[10] = {{"a",10},{"b",10},{"c",8},{"d",7},{"e",6},{"f",5},{"g",4},{"h",3},{"i",2},{"j",1}};
Item numsse[10] = {{"a",10},{"b",9},{"c",8},{"d",7},{"e",6},{"f",5},{"g",4},{"h",3},{"i",1},{"j",1}};
Item numssm[10] = {{"a",10},{"b",9},{"c",8},{"d",7},{"e",5},{"f",5},{"g",4},{"h",3},{"i",2},{"j",1}};
 
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
   for (i = lo; i < hi; i++) {
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
   int i, j, nswaps = 0, ncomps = 0;
   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         ncomps++;
         if (less(a[j], a[j-1])) {
            swap(a[j], a[j-1]);
            nswaps++;
         }
      }
      if (nswaps == 0) break;
   }
   printf("swaps: %d comps %d\n",nswaps,ncomps);
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
int partition(Item a[], int lo, int hi)
{
   Item v = a[lo];  // pivot
   int  i = lo+1, j = hi;
   for (;;) {
      while (less(a[i],v) && i < j) i++;
      while (less(v,a[j]) && j > i) j--;
      if (i == j) break;
      swap(a[i],a[j]);
   }
   j = less(a[i],v) ? i : i-1;
   swap(a[lo],a[j]);
   return j;
}

void quicksort(Item a[], int lo, int hi)
{
   int i; // index of pivot
   if (hi <= lo) return;
   i = partition(a, lo, hi);
   quicksort(a, lo, i-1);
   quicksort(a, i+1, hi);
}

void mergesort(Item a[], int lo, int hi)
{
   int mid = (lo+hi)/2; // mid point
   if (hi <= lo) return;
   mergesort(a, lo, mid);
   mergesort(a, mid+1, hi);
   merge(a, lo, mid, hi);
}

void medianOfThree(Item a[], int lo, int hi)
{
   int mid = (lo+hi)/2;
   if (less(a[mid],a[lo])) swap(a[lo],a[mid]);
   if (less(a[hi],a[mid])) swap(a[mid],a[hi]);
   if (less(a[mid],a[lo])) swap(a[lo],a[mid]);
   // now, we have a[lo] < a[mid] < a[hi]
   // swap a[mid] to a[lo+1] to use as pivot
   swap(a[mid],a[lo+1]);
}
void quicksort_mo3(Item a[], int lo, int hi)
{
   if (hi <= lo) return;
   medianOfThree(a, lo, hi);
   int i = partition(a, lo+1, hi-1);
   quicksort(a, lo, i-1);
   quicksort(a, i+1, hi);
}


int main(void){
   printlist(numsnr,10);
   printf("\n");
   printlist(numsnrr,10);
   printf("\n");

   printf("Insertion Sort\n");

   insertionSort(numsnr,0,10);
   printf("\n");
   insertionSort(numsnrr,0,10);
   printf("\n");

   printf("Bubble Sort\n");
   
   bubbleSort(numsnr1,0,10);
   printf("\n");
   bubbleSort(numsnrr1,0,10);
   printf("\n");
    
   //  printlist(numsr,9);
   //  printf("\n");
   //  quicksort_mo3(numsr,0,9);
   //  printf("\n");
   //  printlist(numsnr,9);
   //  printf("\n");
    

   //  printlist(numsss,9);
   //  printf("\n");
   //  quicksort_mo3(numsss,0,9);
   //  printf("\n");
   //  printlist(numsss,9);
   //  printf("\n");
    
   //  printlist(numsse,9);
   //  printf("\n");
   //  quicksort_mo3(numsse,0,9);
   //  printf("\n");
   //  printlist(numsse,9);
   //  printf("\n");
    
   //  printlist(numssm,9);
   //  printf("\n");
   //  quicksort_mo3(numssm,0,9);
   //  printf("\n");
   //  printlist(numssm,9);
   //  printf("\n");
    
    
    

    return 0;
}
