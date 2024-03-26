#include<stdio.h>
#define NB_Process 10
#define LengthChart 100
#define MaxLength 100
#define done 99999
#define INF 999999

/*-------------------------------RR-------------------------------*/
//Khai bao cau truc pointX
typedef struct{
	int head, tail;
}pointX;

//Khai bao cau truc 1 process
typedef struct{
	int Name_Process; //1->n
	int CPU_Time;
	int Arrival_Time;
	int State[LengthChart];
	int Waiting_Time;
	int Respose_Time;
	int Turnaround_Time;
	pointX a[NB_Process];
	int sizePoint;
}Process1;
//Khai bao cau truc danh sach tien trinh
typedef struct{
	Process1 process[NB_Process];
	int n;
}ListProcess1;
//Khoi tao danh sach tien trinh
void initListProcess1(ListProcess1 *list){
	list->n = 0;
}
//Them 1 tien trinh vao danh sach
void appendListProcess1(ListProcess1 *list, Process1 x){
	list->process[list->n] = x;
	list->n++;
}
//Nhap du lieu
void Input1(ListProcess1 *list, int *q){
	initListProcess1(list);
	printf("Nhap vao he so quantum = ");
	scanf("%d", &(*q));
	int n;
	printf("Nhap vao so luong tien trinh n = "); scanf("%d", &n);
	int i;
	Process1 x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Nhap vao thoi gian tien trinh %d den he thong: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Nhap vao thoi gian chiem dung CPU cua tien trinh %d: ", i+1); scanf("%d", &x.CPU_Time);
		appendListProcess1(list, x);
	}
}

//Sap xep tien trinh theo thoi gian den he thong
void sort1(ListProcess1 *list){
	int i, j;
	for(i = 0; i < list->n-1; i++){
		for(j = i+1; j < list->n; j++){
			if(list->process[j].Arrival_Time < list->process[i].Arrival_Time){
				Process1 temp = list->process[i];
				list->process[i] = list->process[j];
				list->process[j] = temp;
			}
		}
	}
}

//Khai bao cau truc hang doi
typedef struct{
	Process1 Elements[MaxLength];
	int Front, Rear;
}Queue;

//Tao rong
void makeNullQueue(Queue *Q){
	Q->Front = -1;
	Q->Rear = -1;
}

//Kiem tra rong
int isEmptyQueue(Queue Q){
	return Q.Front == -1;
}

//Kiem tra day
int isFullQueue(Queue Q){
	return Q.Rear - Q.Front + 1 == MaxLength;
}

//Them phan tu vao cuoi hang
void appendQueue(Queue *Q, Process1 x){
	if(!isFullQueue(*Q)){
		if(isEmptyQueue(*Q)){
			Q->Front = 0;
		}
		Q->Rear++;
		Q->Elements[Q->Rear] = x;
	}
}

//Lay phan tu
Process1 Front(Queue Q){
	return Q.Elements[Q.Front];
}

//Tra ve positon trong mang ListProcess1
int headToPostion(Process1 p, ListProcess1 list){
	int i;
	for(i = 0; i < list.n; i++){
		if(list.process[i].Name_Process == p.Name_Process){
			return i;
		}
	}
}

//Xoa pha tu
void deQueue(Queue *Q){
	if(!isEmptyQueue(*Q)){
		Q->Front++;
		if(Q->Front > Q->Rear){
			makeNullQueue(Q);
		}
	}
	else printf("Hang doi rong!!!");
}

//Ham equals
int equals(Process1 x, Process1 y){
	return x.Name_Process == y.Name_Process;
}

//ham member
int member(Queue Q, Process1 x){
	while(!isEmptyQueue(Q)){
		Process1 y = Front(Q);
		if(equals(x, y)) return 1;
		deQueue(&Q);
	}
	return 0;
}

void RR(int Chart[], int *index, ListProcess1 *list, float *AVG_T, float *AVG_W, float *AVG_R, int q, int *start){
	*AVG_T = 0;
	*AVG_W = 0;
	*AVG_R = 0;
	Queue Q; makeNullQueue(&Q);
	int count = 0, k, i, check = 1, j, total_Time = 0;
	//Xac dinh process dau tien them vao hang doi
	int min_Arrival_Time = INF, position;
	for(i = 0; i < list->n; i++){
		if(list->process[i].Arrival_Time <= min_Arrival_Time){
			min_Arrival_Time = list->process[i].Arrival_Time;
			position = i;
		}
	}
	//Them tien trinh vao hang doi va *index bat dau tu vi tri nay
	appendQueue(&Q, list->process[position]);
	*start = list->process[position].Arrival_Time;
	*index = list->process[position].Arrival_Time;
	
	//Khoi tao sizePoint
	for(i = 0; i < list->n; i++){
		list->process[i].sizePoint = 0;
	}
	//Lap cho toi khi hang doi rong
	while(!isEmptyQueue(Q)){
		Process1 p1 = Front(Q); 
		deQueue(&Q);
		int pos = headToPostion(p1, *list), cpu_run;
		//Head
		list->process[pos].a[list->process[pos].sizePoint].head = *index;
		//Head
		cpu_run = list->process[pos].CPU_Time;
		for(k = 0; k <= list->process[pos].CPU_Time+*index; k++){
			if(count == q || cpu_run == 0){
				
				//Them process ke tiep vao hang doi voi dk s do toi va ko nam trong hang doi
				for(i = 0; i < list->n; i++){
					while(check){
						//giay do toi/khong nam trong Queue/khac voi tien trinh dang xet
						if(list->process[i].CPU_Time != done && list->process[i].Arrival_Time <= *index && 
						!member(Q, list->process[i]) && list->process[i].Name_Process != list->process[pos].Name_Process){
							appendQueue(&Q, list->process[i]);
						}
						break;
					}
				}
				
				//Giam cpu time va neu cpu time > 0 thi them vao cuoi hang doi
				list->process[pos].CPU_Time -= q;
				if(list->process[pos].CPU_Time > 0){
					appendQueue(&Q, list->process[pos]);
					list->process[pos].a[list->process[pos].sizePoint].tail = *index;
					//Tang sizePoint
					list->process[pos].sizePoint++;
				}
				else{
					list->process[pos].a[list->process[pos].sizePoint].tail = *index;
					list->process[pos].CPU_Time = done;
				}
				total_Time += list->process[pos].CPU_Time;
		        count = 0;
		        break;
			}
			
			else{
				//danh dau tren mang Chart
				Chart[*index] = list->process[pos].Name_Process;
				count++;
				*index += 1;
				cpu_run--;
			}
		}
	}
	//Tinh cac gia tri can thiet (Chu y: phai la chi so index + 1 de lay s chinh xac)
	for(i = 0; i < list->n; i++){
		*AVG_T += list->process[i].a[list->process[i].sizePoint].tail - list->process[i].Arrival_Time;
		*AVG_R += list->process[i].a[0].head - list->process[i].Arrival_Time;
		for(j = 0; j <= list->process[i].sizePoint; j++){
			if(j == 0){
				*AVG_W += list->process[i].a[0].head - list->process[i].Arrival_Time;
			}
			else{
				*AVG_W += list->process[i].a[j].head - list->process[i].a[j-1].tail;
			}
		}
	}
	*AVG_T /= list->n;
	*AVG_R /= list->n;
	*AVG_W /= list->n;
	//Xu ly du kien chuoi dinh thoi tren mang Chart[]
	int z;
	for(i = 0; i < list->n; i++){
		for(j = 0; j < *index; j++){
			if( j >= list->process[i].Arrival_Time && j < list->process[i].a[list->process[i].sizePoint].tail){
				if(list->process[i].Name_Process == Chart[j]){
					list->process[i].State[j] = 'R';
				}
				else list->process[i].State[j] = 'W';
			}
			else
			list->process[i].State[j] = '-';
		}
	}
	
}

/*-------------------------------RR-------------------------------*/
int main(){
	int chart[LengthChart], index, q, i, j, start;
	float avgT, avgR, avgW;
	ListProcess1 list;
	Input1(&list, &q);
	sort1(&list);
	RR(chart, &index, &list, &avgT, &avgW, &avgR, q, &start);
	printf("Grantt Chart = [ ");
	for(i = 0; i < index; i++){
		printf("%d ", chart[i]);
	}
	printf("]");
	printf("\nDinh Thoi RoundRobin\n");
	for(i = 0; i < list.n; i++){
		for(j = start; j <= index; j++){
			printf("%c ", list.process[i].State[j]);
		}
		printf("\n");
	}
	printf("AVGW = %.2f\tAVGR = %.2f\tAVGT = %.2f",avgW, avgR, avgT);
	printf("\n*******************************************\n");
	return 0;
}
