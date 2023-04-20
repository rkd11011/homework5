/* postfix.c
 * infix룰 입력 받아 postfix로 변경하고 계산하는 프로그램
 * 자료구조 5번째 과제
 * 충북대학교 컴퓨터공학부 2017015040 물리학과 강수
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //스택 최대 크기
#define MAX_EXPRESSION_SIZE 20// 수식 표현 최대 크기



//구별자
typedef enum{
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7, /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5, /* + 덧셈 */
    minus = 4, /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];//수식을 저장할 문자열 변수
char postfixExp[MAX_EXPRESSION_SIZE];//postfix로 변환할 문자열을 저장할 변수
char postfixStack[MAX_STACK_SIZE];//postfix로 변환시 사용할 스택
int evalStack[MAX_STACK_SIZE];//postfix로 변환된 문자열을 계산할 때 사용될 스택

int postfixStackTop = -1;//postfixStack TOP위치
int evalStackTop = -1;//postfixStack TOP위치

int evalResult = 0;//계산결과

void postfixPush(char x);//postfixStack의 Push
char postfixPop();//postfixStack의 Pop
void evalPush(int x);//evalStack의 Push
int evalPop();//evalStack의 Pop
void getInfix();//Infix 입력 함수
precedence getToken(char symbol);//받은 문자열을 구별해 식별자를 반환
precedence getPriority(char x);//연산자 우선순위 반환
precedence getStackPriority(char x);//스택내 연산자 우선순위 반환

void charCat(char* c);//문자하나를 postfixExp에 추가 하는 함수
void toPostfix();//infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경하는 함수
void debug();//infixExp,postfixExp,evalResult,postfixStack을 확인하는 변수

//infixExp,postfixExp,postfixStack,postfixStackTop,evalStackTop,evalResult 초기화 함수
void reset();
void evaluation();//postfix로 저장된값으로 계산함수

int main()
{
    char command;//명령어
    printf("\n[----- [kang su] [2017015040] -----]\n");
    do{

        printf("----------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d, Reset=r,Quit=q \n");
        printf("----------------------------------------------------------------\n");
        printf("Command = ");
    
        scanf(" %c", &command);
        switch(command) {
        case 'i': case 'I'://수식 입력
            getInfix();
            break;
        case 'p': case 'P'://Postfix로 변경
            toPostfix();
            break;
        case 'e': case 'E'://postfix로 변경된걸 계산함
            evaluation();
            break;
        case 'd': case 'D'://디버그
            debug();
            break;
        case 'r': case 'R'://리셋
            reset();
            break;
        case 'q': case 'Q'://나가기
            break;
        default:
            printf("\n >>>>> Concentration!! <<<<<\n");
            break;
        }
    }while(command != 'q' && command != 'Q');//종료전 까지 반복
    return 1;
}

//postfixStack의 Push
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;//postfixStackTop의 값을 증가 시킨후 입력된 값을 저장
}

//postfixStack의 POP
char postfixPop()
{
    char x;//스택에서 꺼낼 값
    if(postfixStackTop == -1)//스택이 비어있는지 확인
        return '\0';//공백문자 반환
    else {
        x = postfixStack[postfixStackTop--];//postfixStackTop위치의 값 꺼내고 postfixStackTop에 -1
    }
    return x;
}


//evalStack의 Push
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;//top 증가후 해당위치에 값대입
}

//evalStack의 POP
int evalPop()
{
   if(evalStackTop == -1)//스택이 비었는지 확인
       return -1;
   else
    return evalStack[evalStackTop--];//top위치에 값 반환 하고 top위치-1
}

/**인픽스 문자열 입력받고 저장하는 함수
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

//문자 구별 기능
precedence getToken(char symbol)
{
   switch(symbol) {
    case '(' : return lparen;
    case ')' : return rparen;
    case '+' : return plus;
    case '-' : return minus;
    case '/' : return divide;
    case '*' : return times;
    default : return operand;//숫자
    }
}


//연산자 우선순위 반환
precedence getPriority(char x)
{
    switch(x) {
    case '(' : return rparen;
    case ')' : return rparen;
    case '+' : return plus;
    case '-' : return plus;
    case '/' : return divide;
    case '*' : return divide;
    default : return operand;//숫자
    }
}

//스택내 연산자 우선순위 반환
precedence getStackPriority(char x)
{
    switch(x) {
    case '(' : return lparen;
    case ')' : return rparen;
    case '+' : return plus;
    case '-' : return plus;
    case '/' : return divide;
    case '*' : return divide;
    default : return operand;//숫자
    }
}

//문자하나를 postfixExp에 추가 하는 함수
void charCat(char* c)//문자 포인터로 받아처리
{
    if (postfixExp == '\0')//postfixExp가 비어있다면
        strncpy(postfixExp, c, 1);//postfixExp에 c를 복사
    else
        strncat(postfixExp, c, 1);//postfixExp문자열 끝에 c를 추가
}

//infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경하는 함수
void toPostfix()
{

   //infixExp 문자열을 하나씩 읽기위한 포인터
    char *exp = infixExp;
    char x; //문자 임시저장 변수
    //
    while(*exp != '\0')//exp값이 NULL이 아니면 반복
    {
        if(getToken(*exp)==operand){//operand인 경우
            x = *exp;//X에 값저장
            charCat(&x);//X를 postfixExp에 추가
        }
        else if(getToken(*exp) == lparen) {//(인 경우
            postfixPush(*exp);//(를 postfixStack에 Push
        }
        else if(getToken(*exp) == rparen)//)인 경우
        {
            while((x = postfixPop()) != '(') {// 스택에 저장된 문자가 (일 때까지 스택의 내용을 postfixExp에 추가
                charCat(&x);
            }
        }
        else
        {
            //getStackPriority 판단 할 때 postfixStack의 top에 있는 문자 우선순위가 getPriority로 판단한 *exp 문자 보다 우선순위가 높거나 같으면
            while(getStackPriority(postfixStack[postfixStackTop]) >=getPriority(*exp))
            {
                x = postfixPop();//postfixStack에서 top이 가르키는 값 가져와 x에 대입
                charCat(&x);//X를 postfixExp에 추가
            }
            postfixPush(*exp);//*exp가 postfixStack의 top보다 우선 순위가 낮은 경우 스택에 Push
        }

        exp++;//다음문자로 위치이동
    }

    while(postfixStackTop != -1)//스택에 값이 남은경우
    {
        x = postfixPop();//스택 값을 x에 저장
        charCat(&x);//postfixExp에 x값 저장
    }
}

//infixExp,postfixExp,evalResult,postfixStack을 확인하는 변수
void debug()
{
    printf("\n---DEBUG\n");
    printf("infixExp = %s\n", infixExp);
    printf("postExp = %s\n", postfixExp);
    printf("eval result = %d\n", evalResult);
    
    printf("postfixStack : ");
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        printf("%c ", postfixStack[i]);
    printf("\n");
}

//infixExp,postfixExp,postfixStack,postfixStackTop,evalStackTop,evalResult 초기화 함수
void reset()
{
    infixExp[0] = '\0';
    postfixExp[0] = '\0';
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        postfixStack[i] = '\0';

    postfixStackTop = -1;
    evalStackTop = -1;
    evalResult = 0;
}

//postfix로 저장된값으로 계산함수
void evaluation()
{
    /* postfixExp, evalStack을 이용한 계산 */
    int opr1, opr2, i; //연산자 1 ,연산자2 그리고 postfixExp 배열 위치

    int length = strlen(postfixExp);//postfixExp의 길이
    char symbol;//문자
    evalStackTop = -1;//evalStackTop 초기화
    
    for(i = 0; i < length; i++)//postfixExp처음부터 끝까지 반복
    {
        symbol = postfixExp[i];//문자를 받아옴
        if(getToken(symbol) == operand) {//문자가 operand일 경우
          evalPush(symbol - '0');//ASCII 코드 정수 문자에서 '0'을 빼서 정수로 만든다.
        }
        else {//symbol이 연산자일 경우
            opr2 = evalPop();//evalStack에서 값을 꺼내서 저장한다
            opr1 = evalPop();//evalStack에서 값을 꺼내서 저장한다
            //printf("opr = %d %d\n", opr2,opr1);
            switch(getToken(symbol)) {//getStackToken을 사용해 symbol 값을 판별
            case plus: evalPush(opr1 + opr2); break;//합을 evalStack에 Push
            case minus: evalPush(opr1 - opr2); break;//차을 evalStack에 push
            case times: evalPush(opr1 * opr2); break;//곱을 evalStack에 push
            case divide: evalPush(opr1 / opr2); break;//나누기를 evalStackdp에 push
            default: break;//기타 연산자 무시
            }
        }
    }
    evalResult = evalPop();//결과값 저장
}