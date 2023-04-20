/* circularQ.c 
 *  원형큐 큐를 만들고 큐값을 입력 삭제 확인 하는 프로그램
 *
 * 자료구조 5번째 과제
 * 충북대학교 컴퓨터공학부 2017015040 물리학과 강수
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4//큐의 최대크기

//구조체 정의
typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];//큐값을 저장할 배열
	int front, rear;
}QueueType;


QueueType *createQueue();//큐를 생성하는 함수,반환 값을 포인터 사용
int freeQueue(QueueType *cQ);//큐 해제하는 함수
int isEmpty(QueueType *cQ);//큐가 비어있는지 확인 하는 함수
int isFull(QueueType *cQ);//큐가 차있는지 확인하는 함수
void enQueue(QueueType *cQ, element item);//큐에 요소 삽입 함수
void deQueue(QueueType *cQ, element* item);//큐의 요소 삭제 함수
void printQ(QueueType *cQ);//큐의 모든 요소 출력 함수
void debugQ(QueueType *cQ);//디버그 함수 front rear queue에 있는 값 확인
element getElement();//입력 처리함수

int main(void)
{
	//변수 선언
	//구조체 형식으로 동적 배열을 할당하고,cQ의 주소에 할당된 동적 배열의 주소를 지정함
	QueueType *cQ = createQueue();
	//입력받는 값 저장할 변수
	element data;
	char command;
    printf("\n[----- [kang su] [2017015040] -----]\n");
	do{

		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I'://추가
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D'://삭제
			deQueue(cQ, &data);
			break;
		case 'p': case 'P'://표시
			printQ(cQ);
			break;
		case 'b': case 'B'://디버그
			debugQ(cQ);
			break;
		case 'q': case 'Q'://끝내고 동적 메모리 해제
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');//종료 전까지 반복


	return 1;
}

//큐생성 함수
QueueType *createQueue()
{
	QueueType *cQ;//구조체 메모리 동적 할당 받을 위치를 저장할 포인터 변수
	cQ = (QueueType *)malloc(sizeof(QueueType));//구조체 크기만큼 메모리 동적 할당
	cQ->front = 0;//변수 초기화
	cQ->rear = 0;//변수 초기화
	return cQ;//동적 배열로 할당된 실제로 값이 저장될 주소반환
}

//큐 메모리 해제 함수
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;//메모리가 할당되지 않은 경우 리턴 1
    free(cQ);//메모리 해제
    return 1;
}

//요소를 입력 받는 함수
element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}

//큐가 비어있는지 확인 함수
int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear){//front와 rear가 같으면 비어있는 상태
		printf("Circular Queue is empty!");
		return 1;
	}
	else return 0;
}

//큐가 차있는지 확인 함수
int isFull(QueueType *cQ)
{
    //front와 (rear+1%최대크기)가 같으면 가득 차있는 상태 
    //rear가 증가하다 최대크기를 넘어서면 0부터 시작하기 때문에 최대크기의 나머지를 사용한다.
    //더 추가할경우 front와 rear이 같아져 비어있는 상태로 취급된다.
    if (cQ->front == ((cQ->rear)+1)%MAX_QUEUE_SIZE){
		printf("Circular Queue is full!");
		return 1;
	}
    else return 0;
}

//큐를 추가하는 함수
void enQueue(QueueType *cQ, element item)
{
    
    //큐가 차있는지 확인
    if(isFull(cQ))
    {
    	return ;
    }
    else{
         //rear증가 시키고 최대치를 넘을 경우 0부터시작
        (cQ->rear)=((cQ->rear)+1)%MAX_QUEUE_SIZE;
   
        //rear증가 된 위치에 입력된 값을 집어 넣는다
        cQ->queue[cQ->rear]=item;
		
    	return ;
    }


}

//큐를 제거하는 함수
void deQueue(QueueType *cQ, element *item)
{
    //큐가 비어있는지 확인
    if(isEmpty(cQ))
    {
    	return ;
    }
    else{
        //front증가시키고 최대치를 넘을 경우 0부터시작, 형식적으로 값을 제거
        (cQ->front)=((cQ->front)+1)%MAX_QUEUE_SIZE;
		//큐에서 형식상 삭제된 값을 item에 저장
		*item = cQ->queue[cQ->front];
		printf("\nDelete: %c",*item);
    
    }
    return ;
}

//큐출력
void printQ(QueueType *cQ)
{
	int i, first, last;

	//첫번째 요소와 마지막 요소 위치 지정
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	//첫번째 요소와 마지막 위치 전까지 출력
	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

//디버그 함수 front rear queue에 있는 값 확인
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	//큐 값 확인
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		//front일 경우 확인
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		//큐에 저장된 값 확인
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	//front와 rear값 확인
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

