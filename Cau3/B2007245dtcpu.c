#include<stdio.h>
#define NB_Process 10
#define LengthChart 100
#define MaxLength 100
#define done 99999
#define INF 999999

/*-----------------------------------FCFS-----------------------------------*/

//Khai bao cau truc 1 process
typedef struct{
	int Name_Process; // 1->n
	int CPU_Time;
	int Arrival_Time;
	char State[LengthChart]; //chuoi dinh thoi 'R'=>run/'W'=>waiting/'-'=>tra CPU va khong thuc hien nua
	int Waiting_Time;
	int Response_Time;
	int Turnaround_Time;
	int head, tail;
}Process;

//Khai bao cau truc danh sach tien trinh
typedef struct{
	Process process[NB_Process];
	int n;
}ListProcess;

//Khoi tao danh sach tien trinh
void initListProcess(ListProcess *list){
	list->n = 0;
}

//Them 1 tien trinh vao danh sach
void appendListProcess(ListProcess *list, Process x){
	list->process[list->n] = x;
	list->n++;
}

//Nhap du lieu
void Input(ListProcess *list){
	initListProcess(list);
	int n;
	printf("Input number of process: "); scanf("%d", &n);
	int i;
	Process x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Input arrival time of the process %d come system: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Input CPU time of the process %d: ", i+1); scanf("%d", &x.CPU_Time);
		appendListProcess(list, x);
	}
}

//Sap xep tien trinh theo thoi gian den he thong
void sort(ListProcess *list){
	int i, j;
	for(i = 0; i < list->n-1; i++){
		for(j = i+1; j < list->n; j++){
			if(list->process[j].Arrival_Time < list->process[i].Arrival_Time){
				Process temp = list->process[i];
				list->process[i] = list->process[j];
				list->process[j] = temp;
			}
		}
	}
}


void FCFS(int Chart[], int *index, ListProcess *list, float *AVG_T, float *AVG_W, float *AVG_R){
	*AVG_T = 0;
	*AVG_W = 0;
	*AVG_R = 0;
	int i, check = 1, k;
	*index = 0;
	for(i = 0; i < list->n; i++){
		sort(list);
		while(check){
			if(list->process[i].Arrival_Time <= *index){
				list->process[i].head = *index;
				for(k = list->process[i].head; k <= list->process[i].CPU_Time+*index; k++){
					Chart[k] = list->process[i].Name_Process;
				}
				*index += list->process[i].CPU_Time;
				list->process[i].tail = *index;
				break;
			}
			else{
				Chart[*index] = 0;
				*index++;
			}
		}
	}
	//Xu ly cac du kien turnaround, waiting, response time
	for(i = 0; i < list->n; i++){
		list->process[i].Turnaround_Time = list->process[i].tail - list->process[i].Arrival_Time;
		*AVG_T += list->process[i].Turnaround_Time;
		list->process[i].Response_Time = list->process[i].head - list->process[i].Arrival_Time;
		*AVG_R += list->process[i].Response_Time;
		list->process[i].Waiting_Time = list->process[i].head - list->process[i].Arrival_Time;
		*AVG_W += list->process[i].Waiting_Time;
	}
	*AVG_T /= list->n;
	*AVG_R /= list->n;
	*AVG_W /= list->n;
	//Xu ly du kien chuoi dinh thoi dua tren mang Chart[]
	int j;
	for(i = 0; i < list->n; i++){
		for(j = 0; j < *index; j++){ //State[0->index-1]
			if(j >= list->process[i].Arrival_Time && j < list->process[i].tail){ // [ )
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

//in Chart
void output(int Chart[], int index){
	int i;
	printf("\nGrantt Chart = [ ");
	for(i = 0; i <= index; i++){
		printf("%d ", Chart[i]);
	}
	printf("]");
}

/*-----------------------------------FCFS-----------------------------------*/






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
	printf("Input quantum q = ");
	scanf("%d", &(*q));
	int n;
	printf("Input number of process n = "); scanf("%d", &n);
	int i;
	Process1 x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Input arrival time of the process %d come system: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Input CPU time of the process %d: ", i+1); scanf("%d", &x.CPU_Time);
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





/*-----------------------------------------PremtivePriority-----------------------------------------*/
//Khai bao cau truc sub
typedef struct{
	int head, tail;
}sub;

//Khai bao cau truc 1 process
typedef struct{
	int Name_Process; //1->n
	int CPU_Time;
	int Arrival_Time;
	int Priority;
	int State[LengthChart];
	int Waiting_Time;
	int Respose_Time;
	int Turnaround_Time;
	sub a[NB_Process];
	int sizePoint;
}Process2;

//Khai bao cau truc danh sach tien trinh
typedef struct{
	Process2 process[NB_Process];
	int n;
}ListProcess2;

//Khoi tao danh sach tien trinh
void initListProcess2(ListProcess2 *list){
	list->n = 0;
}

//Ham kiem tra rong
int isEmpty(ListProcess2 list){
	return list.n == 0;
}

//Ham kiem tra day
int isFull(ListProcess2 list, int nbProcess){
	return list.n == nbProcess;
}

//Them 1 tien trinh vao danh sach
void appendListProcess2(ListProcess2 *list, Process2 x){
	list->process[list->n] = x;
	list->n++;
}

//Xoa 1 tien trinh
void deleteListProcess2(ListProcess2 *list, int index){
	int i;
	for(i = index+1; i < list->n; i++){
		list->process[i-1] = list->process[i];
	}
	list->n -= 1;
}

//Nhap du lieu
void Input2(ListProcess2 *list){
	initListProcess2(list);
	int n;
	printf("Input number of process: "); scanf("%d", &n);
	int i;
	Process2 x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Input arrival time of the process %d come system: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Input CPU time of the process %d: ", i+1); scanf("%d", &x.CPU_Time);
		printf("Input priority of the process %d: ", i+1); scanf("%d", &x.Priority);
		appendListProcess2(list, x);
	}
}

//ham getNextProcess
Process2 getNextProcess(ListProcess2 list){
	Process2 p = list.process[0];
	int i;
	for(i = 1; i < list.n; i++){
		if(list.process[i].Priority < p.Priority){
			p = list.process[i];
		}
	}
	return p;
}

//Ham PtoIndex
int PtoIndex(Process2 p, ListProcess2 list){
	int i;
	for(i = 0; i < list.n; i++){
		if(list.process[i].Name_Process == p.Name_Process){
			return i;
		}
	}
}

//Ham thanh vien
int member1(Process2 x, ListProcess2 result){
	int i;
	for(i = 0; i < result.n; i++){
		if(x.Name_Process == result.process[i].Name_Process){
			return 1;
		}
	}
	return 0;
}

//ham smallest Priority
int smallest_Priority_current(ListProcess2 list){
	int minValue = INF, i, position;
	for(i = 0; i < list.n; i++){
		if(list.process[i].Priority < minValue){
			minValue = list.process[i].Priority;
			position = i;
		}
	}
	return list.process[position].Name_Process;
}

//Thuat toan
void Priority_Premtive(int Chart[], int *index, ListProcess2 *list, float *AVG_T, float *AVG_W, float *AVG_R, int *start){
	*AVG_T = 0;
	*AVG_W = 0;
	*AVG_R = 0;
	
	//Tim tien trinh den dau tien
	int i, j, min_Arrival_Time = INF, position;
	for(i = 0; i < list->n; i++){
		if(list->process[i].Arrival_Time <= min_Arrival_Time){
			min_Arrival_Time = list->process[i].Arrival_Time;
			position = i;
		}
	}
	
	//Them tien trinh vao LitsProcess va *index bat dau tu vi tri nay
	ListProcess2 result; initListProcess2(&result);
	appendListProcess2(&result, list->process[position]);
	*start = list->process[position].Arrival_Time;
	*index = list->process[position].Arrival_Time;
	
	//Khoi tao sub
	for(i = 0; i < list->n; i++){
		list->process[i].sizePoint = 0;
	}
	
	//Loop
	int k, have_process_come, count_seconds, donefill;
	while(!isEmpty(result)){
		Process2 p = getNextProcess(result);
		have_process_come = 0;
		//Tham chieu den ListProcess chinh
		int pos_root = PtoIndex(p, *list);
		//Tham chieu den ListProcess phu (chu yeu de xoa)
		int pos_child = PtoIndex(p, result);
		//Xac dinh sub(head)
		list->process[pos_root].a[list->process[pos_root].sizePoint].head = *index;
		donefill = 0;
		while(list->process[pos_root].CPU_Time+1 != 0){
			if(list->process[pos_root].CPU_Time == 0){
				if(donefill == 0){
					list->process[pos_root].a[list->process[pos_root].sizePoint].tail = *index;
				}
				//Danh dau xong tien trinh
				list->process[pos_root].CPU_Time = done;
				//Xoa tien trinh khoi danh sach
				deleteListProcess2(&result, pos_child);
				//Thoat
				break;
			}
			Chart[*index] = list->process[pos_root].Name_Process;
			list->process[pos_root].CPU_Time -= 1;
			*index += 1;
			//Duyet tim kiem tien trinh den trong thoi gian do (*index)
			for(i = 0; i < list->n; i++){
				if(list->process[i].Arrival_Time <= *index && !member1(list->process[i], result) && list->process[i].CPU_Time != done){
					appendListProcess2(&result, list->process[i]);
					//Ton tai tien trinh toi
					have_process_come = 1;
				}
			}
			//Neu co tien trinh den thi ngat Tail, neu CPU_Time > 0 cap phat them sub
			if(have_process_come){
				if(smallest_Priority_current(result) != list->process[pos_root].Name_Process){
				    list->process[pos_root].a[list->process[pos_root].sizePoint].tail = *index;
				    donefill = 1;
				    if(list->process[pos_root].CPU_Time > 0){
					    list->process[pos_root].sizePoint += 1;
				    }
				    break;
				}
			}
		}
	}
	//Tinh cac du kien can thiet
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
	//Xu ly du kien tren mang Chart[]
	int z;
	for(i = 0; i < list->n; i++){
		for(j = *start; j <= *index; j++){
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

/*-----------------------------------------PremtivePriority-----------------------------------------*/


int main(){
	int check = 1, n;
	printf("\n-------------------------------------MENU ALGORITHM-------------------------------------\n");
	printf("\t\t\t1. FSFS algorithm\n");
	printf("\t\t\t2. RounRobin algorithm\n");
	printf("\t\t\t3. PriorityPremtive algorithm\n");
	printf("\t\t\t0. exit\n\n\n");
	while(check){
		printf("Choose the algorithm which you want: "); scanf("%d", &n);
		if(n == 0){
			check = 0;
			continue;
		}
		else{
			if(n == 1){
				int chart[LengthChart], index, start;
				float avgT, avgR, avgW;
				ListProcess list;
				Input(&list);
				sort(&list);
				start = list.process[0].Arrival_Time;
				FCFS(chart, &index, &list, &avgT, &avgW, &avgR);
				int i, j;
				output(chart, index); // Grantt Chart
				printf("\nDinh thoi FCFS\n");
				for(i = 0; i < list.n; i++){
					for(j = start; j <= index; j++){
						printf("%c ", list.process[i].State[j]);
					}
					printf("\n");
				}
				printf("AVGW = %.2f\tAVGR = %.2f\tAVGT = %.2f",avgW, avgR, avgT);
				printf("\n*******************************************\n");
			}
			if(n == 2){
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
			}
			if(n == 3){
				int chart[LengthChart], index, q, i, j, start;
				float avgT, avgR, avgW;
				ListProcess2 list;
				Input2(&list);
				Priority_Premtive(chart, &index, &list, &avgT, &avgW, &avgR, &start);
				printf("Grantt Chart = [ ");
				for(i = start; i < index; i++){
					printf("%d ", chart[i]);
				}
				printf("]");
				printf("\nDinh Thoi Priority Premtive\n");
				for(i = 0; i < list.n; i++){
					for(j = start; j <= index; j++){
						printf("%c ", list.process[i].State[j]);
					}
					printf("\n");
				}
				printf("AVGW = %.2f\tAVGR = %.2f\tAVGT = %.2f",avgW, avgR, avgT);
				printf("\n*******************************************\n");
			}
		}
	}
	return 0;
}








