#include<stdio.h>
#define NB_Process 10
#define LengthChart 100
/*-----------------------------------FCFS-----------------------------------*/

//Khai bao cau truc 1 process
typedef struct{
	int Name_Process; // 1->n
	int CPU_Time;
	int Arrival_Time;
	char State[LengthChart]; //chuoi dinh thoi 'R'=>run/'W'=>waiting/'-'=>tra CPU va khong thuc hien nua
	int Waiting_Time;
	int Response_Time;
	int Turnarrond_Time;
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
	printf("Nhap vao so luong tien trinh n = "); scanf("%d", &n);
	int i;
	Process x;
	for(i = 0; i < n; i++){
		x.Name_Process = i+1;
		printf("Nhap vao thoi gian tien trinh %d den he thong: ", i+1); scanf("%d", &x.Arrival_Time);
		printf("Nhap vao thoi gian chiem dung CPU cua tien trinh %d: ", i+1); scanf("%d", &x.CPU_Time);
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
		list->process[i].Turnarrond_Time = list->process[i].tail - list->process[i].Arrival_Time;
		*AVG_T += list->process[i].Turnarrond_Time;
		list->process[i].Response_Time = list->process[i].head - list->process[i].Arrival_Time;
		*AVG_R += list->process[i].Response_Time;
		list->process[i].Waiting_Time = list->process[i].head - list->process[i].Arrival_Time;
		*AVG_W += list->process[i].Waiting_Time;
	}
	*AVG_T /= 3;
	*AVG_R /= 3;
	*AVG_W /= 3;
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
	printf("Grantt Chart = [ ");
	for(i = 0; i <= index; i++){
		printf("%d ", Chart[i]);
	}
	printf("]");
}

/*-----------------------------------FCFS-----------------------------------*/
int main(){
	int chart[LengthChart], index, start;
	float avgT, avgR, avgW;
	ListProcess list;
	Input(&list);
	sort(&list);
	start = list.process[0].Arrival_Time;
	FCFS(chart, &index, &list, &avgT, &avgW, &avgR);
	int i, j;
	output(chart, index);
	printf("\nDinh thoi FCFS\n");
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
