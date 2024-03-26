#include<stdio.h>
#define NB_Process 10
#define LengthChart 100
#define MaxLength 100
#define done 99999
#define INF 999999


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
	printf("Nhap vao so luong tien trinh n = "); scanf("%d", &n);
	int i;
	Process2 x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Nhap vao thoi gian tien trinh %d den he thong: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Nhap vao thoi gian chiem dung CPU cua tien trinh %d: ", i+1); scanf("%d", &x.CPU_Time);
		printf("Nhap vao priority cua tien trinh %d: ", i+1); scanf("%d", &x.Priority);
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
int member(Process2 x, ListProcess2 result){
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
				if(list->process[i].Arrival_Time <= *index && !member(list->process[i], result) && list->process[i].CPU_Time != done){
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
	int chart[LengthChart], index, q, i, j, start;
	float avgT, avgR, avgW;
	ListProcess2 list;
	Input2(&list);
	Priority_Premtive(chart, &index, &list, &avgT, &avgW, &avgR, &start);
	printf("Grantt Chart = [ ");
	for(i = start; i <= index; i++){
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
	return 0;
}





















