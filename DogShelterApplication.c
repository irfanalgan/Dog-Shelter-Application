//IRFAN KAAN ALGAN 2328599//
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <math.h>

#define FALSE 0
#define TRUE 1

struct hash{
    int data;       //holds dog's unique id
    int size;
    struct dog *dogDetail;
};

struct datetime{
	int day;
	int month;
	int year;
};

struct dog{
    int uniqueId;
    char name[30];
    float weight;
    float height;
    char status[30];
    struct datetime entryDate;
    struct datetime leaveDate;
};

struct hash *initializeHashTable(int);
struct Node *initializeDogTable();
struct hash *addDog(struct hash*,float *,int , char[],float,float);
struct hash *reHashing(struct hash *);
struct hash *searchForDog(struct hash *, int, int);
struct hash *adoptDog(struct hash *,int, int,int);
int hashFunction(int, int);
int checkId(struct hash*,int,int);
void nextPrime(struct hash *);


int main(){

    int choice, c, i, flag, key; // "c" for continue
    int idFlag = 0,ID;  // if it is 0 that means id is unique
    float dogCount = 0.0;       // counts the dog number
    char dogName[30];
    float dogWeight, dogHeight;
    struct hash * hashTable;
    int size = 5;
    hashTable = initializeHashTable(size);
    

    do {
        printf("Welcome to  A Simple Dog Shelter Application\n\n");
        printf("\t\tMENU\n\n");
        printf("1. Add a new dog to Dog Shelter\n");
        printf("2. Search for a dog in Dog Shelter\n");
        printf("3. Adopt a dog form Dog Shelter\n");
        printf("4. EXIT\n");
        printf("\nEnter your choice: ");
		scanf("%d", &choice);
        fflush(stdin);
		switch(choice){
		case 1:
            printf("\nEnter unique identifier: ");      // I take the id and check it if it is unique or not before  (newDog + yer)->uniqueId = ID
            scanf("%d",&ID);    
            fflush(stdin);
            idFlag = checkId(hashTable,idFlag,ID);
            if(idFlag == 0){
                printf("\nEnter name: ");
                scanf("%s", dogName);
                fflush(stdin);
                printf("\nEnter weight: ");
                scanf("%f",&dogWeight);
                fflush(stdin);
                printf("\nEnter height: ");
                scanf("%f",&dogHeight);
                fflush(stdin);
                hashTable = addDog(hashTable,&dogCount,ID,dogName,dogWeight,dogHeight);
            }
            else{
                printf("Id should be unique!");
                idFlag = 0;
            }
		    break;
		case 2:
		    flag = 0;           //go for just search
            printf("\nEnter unique identifier: ");
            scanf("%d",&key);
            hashTable = searchForDog(hashTable, flag, key);
		    break;
		case 3:
            flag = 1;           // go for search and adopt a dog
            printf("\nEnter unique identifier: ");
            scanf("%d",&key);
            hashTable = searchForDog(hashTable, flag, key);
		    break;
		case 4:
            exit(1);
		    break;
		default:
		    printf("Wrong Input\n");
 
		}
		printf("\n Do you want to continue(press 1): ");
		scanf("%d", &c);
        fflush(stdin);
        printf("\n");
 
	}while(c == 1);

    getch();
    return 0;
}

int hashFunction(int key, int size){

    return key % size;
}//returns hash function value


struct hash *initializeHashTable(int newSize){

    int i;
    struct hash * hashTableInitial; 
    hashTableInitial = malloc(sizeof(struct hash) * newSize);
    if (hashTableInitial == NULL) { 
        printf("Memory not allocated.\n"); 
        exit(0); 
    }
    hashTableInitial->dogDetail = malloc(sizeof(struct dog) * newSize); 
    hashTableInitial->size = newSize;
    for(i = 0; i < hashTableInitial->size; i++)
         hashTableInitial[i].data= -1;
    return hashTableInitial;     
}

struct hash *reHashing(struct hash *hashTable){

    int i,j = 0;
    int position;
    int result = 0;   
    int *tempArray;   // this array holds the data before rehashing 
    int checkOk = 0;  //if the number can be stored in hash table, the value of the checkOk will be 1    
    int flag;       //checks how many time that number is used    
    int tempSize = hashTable->size;
    struct dog *dogTable;
    dogTable = malloc(sizeof(struct hash) * tempSize);

    int newSize = hashTable->size/2;
    tempArray= (int *)malloc(newSize * sizeof(int));  

    if (tempArray == NULL) { 
        printf("Memory not allocated.\n"); 
        exit(0); 
    } 

    hashTable->size = hashTable->size * 2;  //we doubled hasTable size
    nextPrime(hashTable);   //send it to function to make its size prime

    for(i = 0; i < tempSize; i++){            //we need to check all the elements in old hash table with its old size
        if(hashTable[i].data != -1){
            tempArray[j] = hashTable[i].data;    // we hold the old datas in temp array 
            j++;
        }
    }
    /* j= 0;
    for(i = 0; i < tempSize; i++){            //we need to check all the elements in old hash table with its old size
        if(hashTable[i].data != -1){
            dogTable[i].uniqueId = hashTable[i].data;    // we hold the old datas in temp array 
            strcpy(dogTable[i].name ,hashTable->dogDetail[i].name);
            dogTable[i].weight = hashTable->dogDetail[i].weight;
            dogTable[i].height = hashTable->dogDetail[i].height;
            j++;
        }
    }
    for(i = 0; i < tempSize; i++){            //we need to check all the elements in old hash table with its old size
        if(hashTable[i].data != -1){
            printf("  dog uniq id: %d",dogTable[i].uniqueId);
            printf("  dog uniq id: %s",dogTable[i].name);
            printf("  dog uniq id: %f",dogTable[i].weight);
            printf("  dog uniq id: %f",dogTable[i].height);
            printf("\n");
        }
    }*/

    hashTable = initializeHashTable(hashTable->size);       //Rehashing
  
    for(i = 0; i <= newSize; i++){
        flag = 1;                               //started at 1 
        checkOk = 0;
        position = hashFunction(tempArray[i],hashTable->size);      // determine position
        if(hashTable[position].data == -1)        
            hashTable[position].data = tempArray[i];        //if the position empty place the value
        else{           
            do{
               result =  flag * hashFunction(tempArray[i],hashTable->size);         //we need do (how many times was the number used) * key % size
               result = hashFunction((result + position),hashTable->size);          // doing (result+position)%size
               if(hashTable[result].data == -1){
                    hashTable[result].data= tempArray[i];                       //if the position empty place the value
                    checkOk = 1;
               }
               flag++;
            }while(checkOk != 1);                           //if it placed checkOk will be 1 and terminate the loop
        }
    }
    return hashTable;
}

void nextPrime(struct hash *hashTable){

    int i, flag = 0;
    do{
        flag = 0;
        for (i = 2; i <= hashTable->size / 2; ++i) {
            if (hashTable->size % i == 0) {
                flag = 1; 
            }
        }
        if(flag == 1)
            hashTable->size = hashTable->size + 1;  
    }while(flag == 1);
}//this function checks if the size value prime or not. if it is not, we convert it to the nearest prime number


struct hash *addDog(struct hash *hashTable, float *dogCount,int dogId, char dogName[30], float dogWeight, float dogHeight){
    
    float loadFactor;
    int i,j = 0;
    int position = 0;
    int result = 0;
    int dogPosition = (int)*dogCount;
    int checkOk = 0;  //if the number can be stored in hash table, the value of the checkOk will be 1    
    int flag;       //checks how many time that number is used  

    hashTable->dogDetail[dogPosition].uniqueId = dogId;  
    strcpy(hashTable->dogDetail[dogPosition].name,dogName);               
    hashTable->dogDetail[dogPosition].weight = dogWeight;
    hashTable->dogDetail[dogPosition].height = dogHeight;
    strcpy(hashTable->dogDetail[dogPosition].status ,"in shelter");

      
    loadFactor = *dogCount / hashTable->size;

    if(loadFactor < 0.5){
        flag = 1;                    //started at 1 
        checkOk = 0;
        result = 0;
        position = hashFunction(hashTable->dogDetail[dogPosition].uniqueId,hashTable->size);      // determine position
        if(hashTable[position].data == -1) {      
            hashTable[position].data = hashTable->dogDetail[dogPosition].uniqueId;        //if the position empty place the value
            printf("\n%s has been added to the dog shelter",hashTable->dogDetail[dogPosition].name);
            *dogCount = *dogCount + 1;
        }
        else{
            do{
                result = flag * hashFunction(hashTable->dogDetail[dogPosition].uniqueId,hashTable->size);         //we need do (how many times was the number used) * key % size
                result = hashFunction((result + position),hashTable->size);          // doing (result+position)%size
                printf("\nRESULT :%d \n",result);
                if(hashTable[result].data == -1){
                    hashTable[result].data = hashTable->dogDetail[dogPosition].uniqueId;                       //if the position empty place the value
                    checkOk = 1;
                     printf("\n%s has been added to the dog shelter",hashTable->dogDetail[dogPosition].name);
                     *dogCount = *dogCount + 1;
                }
                flag++;
            }while(checkOk != 1);                           //if it placed checkOk will be 1 and terminate the loop
        }
    
         printf("\nHASH TABLE!\n");
         for(i = 0; i < hashTable->size; i++){      
             printf("  %d",hashTable[i].data);
             printf("\n");
        }
    }
    else{
        hashTable = reHashing(hashTable);
    }
    return hashTable;
}

int checkId(struct hash* hashTable,int idFlag,int uniqueId){

    int i;
    for(i = 0; i < hashTable->size; i++){
        if(uniqueId == (hashTable + i)->data)  
            idFlag = 1; 
    }
    return idFlag;
}//this function checks the dog id if it is unique or not

struct hash *searchForDog(struct hash *hashTable, int flag, int key){
    int i,j;
    int beingFlag = 0;    //checks the value is in our table

    for(i = 0; i < hashTable->size; i++){                   //the first loop scan all the elements in hash table
        if(hashTable[i].data == key){
            for(j = 0; j< hashTable->size ;j++){             //the first loop scan all the elements in hashtable.dogDetail
                if(hashTable->dogDetail[j].uniqueId == key){    
                    beingFlag =1;
                    break;
                }
            }
        }
       
    }
    if(flag == 1)           // if it is 1 that means we will need to go adoptDog function
        hashTable = adoptDog(hashTable,key,j,i);
    
    if(beingFlag == 1 && flag == 0){                    // if being flag is 1 it means that the dog is in shelter 
        printf("\nName: %s\n",hashTable->dogDetail[j].name);
        printf("\nWeight: %2.6f\n",hashTable->dogDetail[j].weight);
        printf("\nHeight: %2.6f\n",hashTable->dogDetail[j].height);
        printf("\n%s is %s",hashTable->dogDetail[j].name,hashTable[j].dogDetail->status);
    }
    if(beingFlag == 0)
        printf("No dog is found");

    return hashTable;
}

struct hash *adoptDog(struct hash *hashTable, int key,int j, int i){
    
    hashTable[i].data = -1;
    strcpy(hashTable->dogDetail[j].status, "Adopted");
    printf("\nName: %s\n",hashTable->dogDetail[j].name);
    printf("\nWeight: %2.6f\n",hashTable->dogDetail[j].weight);
    printf("\nHeight: %2.6f\n",hashTable->dogDetail[j].height);
    printf("\n%s is %s",hashTable->dogDetail[j].name,hashTable->dogDetail[j].status);

    return hashTable; 
}

