#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void die( char *s ) {fprintf(stderr, "***** %s *****\n", s); printf("***** %s *****\n", s); exit(1);}

typedef struct { int key; int data; } el_t;
#define KEY(i)  (((el_t*)(vec+(i)*rec_siz))->key)
#define DATA(i) (((el_t*)(vec+(i)*rec_siz))->data)

unsigned int cmp_cnt,ass_cnt;
int _QS_MID1,_QS_MID2,_QS_MID3;

int strcmp0(char *s1, char *s2) { int d;
 while ((d=(*s1++ - *s2++)) == 0) if (s1[-1] == 0) return(0);
 return(d);
}

int cmp_loop,cmp_val;
int cmpfnc( xp, yp ) el_t *xp, *yp; {cmp_cnt++;
 if (cmp_loop) {int c; for (c=cmp_loop; c>0; c--) cmp_val+=strcmp0("abc","def");}
 return xp->key - yp->key;
}

int counter, div_val, itarate, chk_chk1, chk_chk2;
size_t arr_max, rec_siz;
char *vec, *chk;

void do_qsort(int do_qs) {int i,j,x,t,h;

 srand( div_val + arr_max + 556  ); //乱数の初期化 

 for (counter=0; counter<itarate; counter++) {
   /*データを用意する*/
   if (div_val == 0 ) for (i = 0; i < arr_max; i++) KEY(i)= 5;         /*一定*/
   else if (div_val == -1) for (i = 0; i < arr_max; i++) KEY(i)= i+1;       /*昇順*/
   else if (div_val == -2) for (i = 0; i < arr_max; i++) KEY(i)= arr_max-i; /*降順*/
   else if (div_val == 1 ) for (i = 0; i < arr_max; i++) KEY(i)= rand();  /*乱数*/
   else if (div_val >= 2 ) for (i = 0; i < arr_max; i++) KEY(i)= rand()%div_val;
   else if (div_val == -3) {
     for (i = 0; i < arr_max; i++) KEY(i)= i;       /*同値キーがない乱数　入れ替えで*/
     for (i = 0; i < arr_max; i++) {x=rand()%arr_max; t=KEY(i); KEY(i)=KEY(x); KEY(x)=t;}
   }
   else die("ill div_val");

   if (rec_siz >= 8)
     for (i = 0; i < arr_max; i++) DATA(i) = i;   /*検査のための準備*/

   if (do_qs) qsort( (char*)vec, arr_max, rec_siz, cmpfnc );   /*ソートの実行*/

   /*以下でソートできたことを検査する*/
   for (i = 1; i < arr_max; i++)
     if (div_val>=0 ? KEY(i-1)>KEY(i) : KEY(i-1)>=KEY(i)) {
       if (do_qs==0) continue;
       puts("");
       {for (h = 0; h < arr_max && h<40; h++) printf(" %d",KEY(h)); puts(" arr_max error");}
       {for (h = 0; h <= i && h<40; h++) printf(" ."); puts("← error is here");}
       printf("  counter=%d   error i=%d  ",counter,i);
       die("not sorted  do_qsort(1)");
     }else{
       if (do_qs==0) continue;  // do_qsort(1) do_qsort(0) の時間をできるだけ合わせるための処理
     }
   if (rec_siz >= 8) {
     for (i = 0; i < arr_max; i++) chk[i] = 0;
     for (i = 0; i < arr_max; i++) chk[DATA(i)] = 123;
     for (i = 0; i < arr_max; i++) if (chk[i] != 123) die("chk err");
   }
 }
}


int main( int argc, char **argv ) {
 clock_t clk_start, clk_end, clk_end2;
 cmp_cnt=ass_cnt=0;

 if (sizeof(char*)!=8 && sizeof(char*)!=4) die("sizeof(char*)!=8 && sizeof(char*)!=4");
 if (sizeof(char*)!=sizeof(size_t)) die("sizeof(char*)!=sizeof(size_t)");
 if (argc != 9) die("Usage: main.exe div_val arr_max rec_siz itarate MID1 MID2 MID3 cmp");
 
 div_val = atoi(argv[1]);       /*テストデータの種類を指定する rand()%div_val等*/
 arr_max = atoi(argv[2]);       /*要素の個数(要素数)*/
 rec_siz = atoi(argv[3]);       /*要素の大きさ(要素サイズ)*/
 itarate = atoi(argv[4]);       /*繰り返し回数*/
 if (atoi(argv[5])>=0) _QS_MID1 = atoi(argv[5]);
 if (atoi(argv[6])>=0) _QS_MID2 = atoi(argv[6]);
 if (atoi(argv[7])>=0) _QS_MID3 = atoi(argv[7]);
 cmp_loop  = atoi(argv[8]);     /*比較関数の重たさを調整する*/
 if (rec_siz == 0) rec_siz = sizeof(char*);
                                
 fprintf(stderr,"\n%-7s d=%d e=%d s=%d %dMB R%d ",
      argv[0]+2,div_val,   arr_max,   rec_siz,   arr_max*rec_siz/1000000 ,itarate);
 fprintf(stderr,"%c=%d:%d:%d:%d: ",(sizeof(char*)==8?'M':'m'),_QS_MID1,_QS_MID2,_QS_MID3,cmp_loop);

 printf("%-8s d=%d e=%d s=%d R%d ", argv[0]+2,div_val,   arr_max,   rec_siz ,itarate);
 printf("%c%03d:%03d:%03d:%d:",(sizeof(char*)==8?'M':'m'),_QS_MID1,_QS_MID2,_QS_MID3,cmp_loop);
 fflush(stdout);

 if (rec_siz < 4 || 100000 < rec_siz) die("本プログラムでは「要素のバイトサイズは４以上&10万以下」");
 if (rec_siz % 4) die("本プログラムでは「要素のバイトサイズは４の倍数」を仮定");
 if ((vec = (char*)malloc((arr_max+5)*rec_siz)) == NULL) die("vec NULL");
 if ((chk = (char*)malloc(arr_max)) == NULL) die("chk NULL");
 
 clk_start=clock();
 do_qsort(1);
 clk_end=clock();
 do_qsort(0);
 clk_end2=clock();

 {unsigned int cmp_av=cmp_cnt/itarate,
               ass_av=ass_cnt/itarate;
  double sum_av=(double)cmp_av+ass_av,
         etime= (double)((clk_end-clk_start)-(clk_end2-clk_end))/CLOCKS_PER_SEC;
  fprintf(stderr,"  T=%1.2f %4.0f ",etime,etime/itarate*100000);
  if (ass_av) printf(" c=%-6u %10u a=%-6u %10u i=%1.0f T=%1.2f %4.0f \n",
                   cmp_av,cmp_cnt, ass_av,ass_cnt,sum_av,etime,etime/itarate*100000);
  else        printf(" c=%-6u %10u T=%1.2f %4.0f \n",
                   cmp_av,cmp_cnt,                       etime,etime/itarate*100000);
 }

 fflush(stdout);  
 return 0;
}
