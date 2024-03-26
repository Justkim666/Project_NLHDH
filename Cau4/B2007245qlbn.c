#include<stdio.h>
#include<math.h>
#define MaxLength 100

//Khai bao cau truc 1 memory
typedef struct{
	char c;
	int number_page;
}element;

//Khai bao 1 mang memory
typedef struct{
	element M[MaxLength];
	int size;
}memory;

//Khoi tao mang memory
void initMemmory(memory *m){
	m->size = 0;
}

//Them phan tu
void addMemory(memory *m, char x){
	m->M[m->size].c = x;
	m->size++;
}

//Doc du lieu
void Input(memory *Logical, int PageTable[], int *indexPageTable, int *byte){
	initMemmory(Logical);
	freopen("filename.txt", "r", stdin);
	int numberPage, byteAPage;
	scanf("%d%d", &numberPage, &byteAPage);
	*byte = byteAPage;
	int i, count = 0, indexPage = 0;
	char c;
	for(i = 0; i < numberPage * byteAPage; i++){
		getchar();
		scanf("%c", &c);
		if(count == *byte){
			count = 0;
			indexPage++;
		}
		count++;
		addMemory(Logical, c);
		Logical->M[i].number_page = indexPage;
	}
	scanf("%d", &(*indexPageTable));
	int frame;
	for(i = 0; i < *indexPageTable; i++){
		scanf("%d", &frame);
		PageTable[i] = frame;
	}
}

//Tim max => length Physical
int max(int PageTable[], int indexPageTable){
	int maxval = -99999, i;
	for(i = 0; i < indexPageTable; i++){
		if(PageTable[i] > maxval){
			maxval = PageTable[i];
		}
	}
	return maxval;
}

//Tim index trong Physical
int findIndexFrame(int frame, memory Physical){
	int i;
	for(i = 0; i < Physical.size; i++){
		if(Physical.M[i].number_page == frame){
			return i;
		}
	}
}

//Khoi tao Physical value - frame
void initPhysical(int PageTable[], int indexPageTable, int byte, memory *Physical){
	int maxval = max(PageTable, indexPageTable);
	int length = maxval * byte + byte;
	Physical->size = length + byte;
	int i, count = 0, frame_nb = 0;
	for(i = 0; i < Physical->size; i++){
		if(count == byte){
			count = 0;
			frame_nb++;
		}
		Physical->M[i].c = ' ';
		Physical->M[i].number_page = frame_nb;
		count++;
	}
}

void LogicalToPhysical(memory Logical, int PageTable[], int indexPageTable, memory *Physical, int byte){
	initPhysical(PageTable, indexPageTable, byte, Physical);
	int i;
	char CharAt;
	for(i = 0; i < Logical.size; i++){
		CharAt = Logical.M[i].c;
		int page = Logical.M[i].number_page;
		int frame = PageTable[page];
		int offset = 0;
		int j;
		for(j = page*byte; j < page*byte+byte; j++){
			if(Logical.M[j].c == CharAt){
				break;
			}
			else offset++;
		}
		int indexStart = findIndexFrame(frame, *Physical);
		Physical->M[indexStart+offset].c = CharAt;
	}
	
}
int main(){
	memory Logical, Physical;
	int pageTable[MaxLength], indexPageTable, byte;
	Input(&Logical, pageTable, &indexPageTable, &byte);
	LogicalToPhysical(Logical, pageTable, indexPageTable, &Physical, byte);
	int i;
	printf("\n  PHYSICAL MEMORY\n");
	for(i = 0; i < Physical.size; i++){
		printf("%d\t%c\t%d\n", i, Physical.M[i].c, Physical.M[i].number_page);
	}
	return 0;
}








