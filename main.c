#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <windows.h>



#define KEY_ESC 27
#define KEY_ENTER '\r'
#define KEY_BS '\b'

#define MAX_WORD_LENGTH 24
#define MAX_SCREEN_WORD_COUNT 19
#define WORD_COUNT 10
#define COL GetStdHandle(STD_OUTPUT_HANDLE)
#define YELLOW SetConsoleTextAttribute(COL, 0x000e);



int g_word_count = 11;                          // 확장을 위해 변수로 선언
char *g_words[11] = 
{
   "programming",
    "school",
    "student",
    "chair",
    "desk",
    "average",
    "screen",
    "mouse",
    "window",
    "door",
    "학교"
};
int g_fail_count = 0;                           // 실패한 개수
int g_score = 0;                                // 정수
char g_input_word[MAX_WORD_LENGTH + 1];         // 입력 단어 저장
int g_input_word_length = 0;                    // 입력 단어 길이
int an=0;
struct ScreenWord
{
    int index;
    int x;
    int y;
};
typedef struct ScreenWord ScreenWord ;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// 화면에 나타난 단어들(words의 원소 index 저장)
int g_screen_word_count = 0;                    // 화면에 나타난 단어 개수

clock_t g_start_time;                           // 기준 시각
double g_falling_speed = 1.5;                   // 단어 낙하 시각(초 단위)

void InitScreen(void);
void InitData(void);                            // 아직 내용 없음 (확장 가능)
void Run(void);
void CompareWords(void);
void ProcessInput(int key);
void DrawWord(int i);                           // 화면에 나타난 i번째 단어 출력
void EraseWord(int i);                          // 화면에 나타난 i번째 단어 지우기
void UpdateInputWord(void);
void UpdateScore(void);
void UpdateFailCount(void);


int main(void)
{
   
    SetCursorVisible(FALSE);
   srand(time(NULL));
    InitScreen();
    Run();
   
   
   UpdateFailCount();
   system("cls");
   printf("게임종료ㅠㅇㅠ\n");
   printf("아쉽지만 다음 기회에...\n");

    
    return 0;
}

void InitScreen(void)
{
    int i;

   // system("cls");
    
    printf("┌─────────────────────────────────────┐\n");
    for (i = 0; i < 19; i++)
        printf("│                                                                          │\n");
    printf("└─────────────────────────────────────┘\n");

    // Programmed by
    printf("┌────────────┐\n");
    printf("│  Programmed by 안선영  │\n");
    printf("└────────────┘");

    // 입력칸
    GotoXY(28, 21); printf("┌────────────┐\n");
   GotoXY(28, 22);   printf("│                        │\n");
   GotoXY(28, 23);   printf("└────────────┘");
   // 성공횟수
   GotoXY(58, 21);printf("┌───┐\n");
   GotoXY(58, 22);printf("│  %d   │\n",g_score);   
   GotoXY(58, 23);printf("└───┘");
   // 실패횟수
   GotoXY(68, 21);printf("┌───┐\n");
   GotoXY(68, 22);printf("│  %d   │\n",g_fail_count);   
   GotoXY(68, 23);printf("└───┘");
   //GotoXY(75, 4); printf("in");
}

void Run(void)
{
   int i=0,j;
   int key;
   
   while(1)
   {
      g_screen_word[i].x = (rand() % 61) + 2;
      g_screen_word[i].y = 0;
      g_screen_word[i].index = rand() % 11;
      while(1)
      {
           if(kbhit())
                 {
                     key=_getch();
                     ProcessInput(key);
                  }
         if(GetDiffTime(g_start_time) > g_falling_speed)
         {
            for(j = 0; j < 19; j++)
            {
               
               if( g_screen_word[j].x != 0 && g_screen_word[j].y != 0 )
                  EraseWord(j);

               if( g_screen_word[j].y == 19 )
               {
                     i = j - 1;
                      UpdateFailCount();
               }
               else
               {
                  if(g_screen_word[j].x > 1)
                  {
                     g_screen_word[j].y++;
                     DrawWord(j);
                  }
               }       
            }
            g_start_time=clock();
            break;
         }
     }
      i++;
      if(g_fail_count>=5)
   {
     break;
   }
   }
}

void ProcessInput(int key)
{
   int a;
   
   if(key==KEY_ENTER)
   {
      CompareWords();
   }
   if(key==KEY_BS)
      {
         if(an>=0)
         {
            GotoXY(30,22);
         for(a=0;a<strlen(g_input_word);a++)
         {
            printf(" ");
         }
            g_input_word[--an]=0;
         UpdateInputWord();
         }
      }
   else
   {
      g_input_word[an]=key;
      an++;
      UpdateInputWord();
   }

}
void DrawWord(int i)
{
   int j;

   GotoXY(g_screen_word[i].x, g_screen_word[i].y);
   for(j = 0; j < strlen(g_words[g_screen_word[i].index]); j++)
   {
      if( g_screen_word[i].x + j >= 75 || g_screen_word[i].y == 0 )
      {
         continue;
      }
      else
      {
         YELLOW printf("%c", *(g_words[g_screen_word[i].index]+j));
      }
   }
}

void EraseWord(int i)
{
    int c;

    GotoXY(g_screen_word[i].x, g_screen_word[i].y);
    for (c = 0; c < strlen(g_words[g_screen_word[i].index]); c++)
    {
        if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) < 77)
            printf(" ");
    }
}

void CompareWords()
{
int i=0,j;
int t;

for(i=0;i<MAX_SCREEN_WORD_COUNT ;i++)
{
   t=strcmp(g_words[g_screen_word[i].index],g_input_word);
   if(t==0)
   {
      GotoXY(30,22);
      for(j=0;j<strlen(g_input_word);j++)
      printf(" ");
       while(an!=-1)
       {
      g_input_word[an]=0;
      an--;
        }
      EraseWord(*g_words[g_screen_word[i].index]);
      UpdateScore();
   }
}
}

void UpdateScore(void)
{
   GotoXY(62,22);
   g_score++;
   printf("%d",g_score);
}

void UpdateFailCount(void)
{
   GotoXY(72,22);
   g_fail_count++;
   printf("%d",g_fail_count);
   
}
void UpdateInputWord(void)
{
   int a;
   GotoXY(30,22);
   for(a=0;a<strlen(g_input_word);a++)
   {
      printf(" ");
   }
   GotoXY(30,22);
   for(a=0;a<strlen(g_input_word);a++)
   {
      printf("%c",g_input_word[a]);
   }

}