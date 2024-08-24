/***************************************************/
/*       sqsort  (stable qsort)        ss21        */
/*                                                 */
/*  by 河村 知行 (kawamura tomoyuki)    2024.7.19  */
/*             t-kawa@crux.ocn.ne.jp               */
/***************************************************/

// sqsort は timsortより高速で、安定・安全な比較ソートです。（qsortを元にしています）

/* sqsortの概略　対象配列(ptr)と補助配列(ptr2)(ptrと同じ大きさ)の役目を入れ替えながらソートを行う。
条件が揃った場合は、ポインタ配列(ptr)と補助ポインタ配列(ptr2)による間接ソートに自動的に切り替わる。
以下のようにＡＢ2段階で要素を移動する。ここで「5」はピボットと同じ比較値をもつ要素を表している。
        l             r                 l  r                      l  r
   ptr2:...............  →  ptr2:332223....77787  →  ptr2:332223....77787
   ptr :357358257257237  Ａ  ptr :5555...........  Ｂ  ptr :......5555.....
        m      M                      m
移動Ａで、ピボット及び比較値が同じ要素をptrの左端に集め、5より小さい要素をptr2の左端に集め、
5より大きい要素をptr2の右端に集める。このとき77787は初期順序の逆順になっている。
ここで逆順とは、元の並びの中で逆の順番になることを言う。（正順はその反対）
移動Ｂのとき、もしも5555が逆順ならば、Ｂの処理後は5555が正順になるように移動する。
Ｂの処理後は、5555はソート完成状態の位置に収まっている。
これ以降、部分配列77787と332223に対して同じ処理を繰り返す。（再帰呼び出しを実施）
部分配列が短い場合は、挿入ソートを行う。*/

int _QS_SORT=0;   // 直接ソート・間接ソートの切替え制御  0:自動選択  1:直接ソート  2:間接ソート
int _QS_MID1=120; // 要素数n<=QS_MID1 のときは　3つの要素からピボットを決定する（３点処理）
int _QS_MID2=9;   // 直接ソート時はn<=_QS_MID2で挿入ソートを実施。 120や9や18は実験で得られた値
int _QS_MID3=18;  // 間接ソート時はn<=_QS_MID3で挿入ソートを実施。 cmpが重い時は9や18を7～8にすると速い

#include <malloc.h>
#include <time.h>

#define ASS_CNT(x) {}       /* {ass_cnt += (x);} とすれば、要素の移動回数を計測できる*/

///////////////////　要素の移動に関するもの　別なものに書き換え可能です　////////////////////////
#define  lli    long long int
#define  MV1(i) {                                 a [i] =        b [i];                  } 
#define  MV4(i) {                          ((int*)a)[i] = ((int*)b)[i];                  } 
#define  MV8(i) {                          ((lli*)a)[i] = ((lli*)b)[i];                  } 
#define  SW1(i) {char v; v =        a [i];        a [i] =        b [i];        b [i] = v;} 
#define  SW4(i) { int v; v = ((int*)a)[i]; ((int*)a)[i] = ((int*)b)[i]; ((int*)b)[i] = v;} 
#define  SW8(i) { lli v; v = ((lli*)a)[i]; ((lli*)a)[i] = ((lli*)b)[i]; ((lli*)b)[i] = v;} 

#define HIGHLOW(MOV,WS) {                                                               \
   if (high) {                                                                          \
     char *e = a + high;                                                                \
     do {MOV(0) MOV(1) MOV(2) MOV(3) MOV(4) MOV(5) MOV(6) MOV(7)  b += 8*WS; a += 8*WS; \
     }while (a < e);                                                                    \
   }                                                                                    \
   switch ((low) & 7) {                                                                 \
     case 7: MOV(6)                                                                     \
     case 6: MOV(5)                                                                     \
     case 5: MOV(4)                                                                     \
     case 4: MOV(3)                                                                     \
     case 3: MOV(2)                                                                     \
     case 2: MOV(1)                                                                     \
     case 1: MOV(0)                                                                     \
     case 0: {}                                                                         \
   }                                                                                    \
} 

#define ENINT(x)  ((char*)(x) - (char*)0)
static size_t high, low;

static void   mvfnc8  ( char *a, const char *b ) {        MV8(0)}
static void   mvfnc4  ( char *a, const char *b ) {        MV4(0)}
static void   mvfnc8n ( char *a, const char *b ) {HIGHLOW(MV8,8)}
static void   mvfnc4n ( char *a, const char *b ) {HIGHLOW(MV4,4)}
static void   mvfnc1n ( char *a, const char *b ) {HIGHLOW(MV1,1)}
static void (*mvfnc)  ( char *a, const char *b );
static void   swfnc8  ( char *a,       char *b ) {        SW8(0)}
static void   swfnc4  ( char *a,       char *b ) {        SW4(0)}
static void   swfnc8n ( char *a,       char *b ) {HIGHLOW(SW8,8)}
static void   swfnc4n ( char *a,       char *b ) {HIGHLOW(SW4,4)}
static void   swfnc1n ( char *a,       char *b ) {HIGHLOW(SW1,1)}
static void (*swfnc)  ( char *a,       char *b );

static void mmprepare( void *base, size_t siz ) {
 if       ((sizeof(lli)==8) && (ENINT(base)&(8-1))==0 && (siz&(8-1))==0) {
   high=(siz&(-64)); low=(siz&(64-1))/8;  mvfnc = (siz==8 ? mvfnc8 : mvfnc8n);
                                          swfnc = (siz==8 ? swfnc8 : swfnc8n);
 }else if ((sizeof(int)==4) && (ENINT(base)&(4-1))==0 && (siz&(4-1))==0) {
   high=(siz&(-32)); low=(siz&(32-1))/4;  mvfnc = (siz==4 ? mvfnc4 : mvfnc4n);
                                          swfnc = (siz==4 ? swfnc4 : swfnc4n);
 }else{                                                    
   high=(siz&( -8)); low=(siz&( 8-1))/1;  mvfnc = mvfnc1n;
                                          swfnc = swfnc1n;
 }
}
///////////////////　ここまでが要素の移動に関するもの　別なものに書き換え可能です　//////////////
///////////////////////////　ここから下は、ss21によるソートプログラムです　//////////////////////

#define LT(a,b)  if ((t=CMP(a,b)) <  0) 
#define else_GT  else if (t > 0)
#define med3(a,b,c) (CMP(a,b)<=0 ? (CMP(b,c)<=0 ? b : (CMP(a,c)<=0 ? c : a)) : \
                                   (CMP(b,c)>=0 ? b : (CMP(a,c)<=0 ? a : c)) )
#define I(x)  {x+=Esiz;}  /*注目要素ｘを右へ１つ移動する*/
#define D(x)  {x-=Esiz;}  /*注目要素ｘを左へ１つ移動する*/

static size_t rndm = 0;        /* rndm には再現性のない乱数をセットする */

/////////////////////////////////　sqsortの本体部分のマクロ定義　///////////////////////////////////
#define SORT_BODY( INS_END, C, S, SORT_TYPE )                                                      \
static void SORT_TYPE ( char *L, char *R, /* 区間L～Rをソートする(両端を含む) */                   \
                        size_t size, int (*cmp)(void *a, void *b), int rev, int hoj, size_t Z ) {  \
 char *l,*l0,*r,*r0; /* 比較値がピボットより小さい要素を l0～l に移動する(右端を含まず) */         \
 char *m,*m0,*M,*p;  /* 比較値がピボットと    同じ要素を m0～m に移動する(右端を含まず) */         \
 int  t;   /* tは作業用 　        */                 /* rev==0 のとき L～Rは正順        */         \
 size_t n; /* nは区間L～Rの要素数 */                 /* hoj==0 のとき L～Rはptr上にある */         \
                                                                                                   \
LOOP:                                                                                              \
 if (R<L)  {                               return; }   /*要素なし*/                                \
 if (L==R) {if (hoj==0) {} else {C(L-Z,L)} return; }   /*要素数１*/                                \
                                                                                                   \
 n = (R - L) / Esiz + 1;                                                                           \
                                                                                                   \
 if (n <= INS_END) { /* n<=INS_END のとき挿入ソートを実行する  結果はptr上に作る           */      \
   if (hoj==0){ if (rev==0) { } /* この４行でソートしたい要素をptr上に移動する(正順にする) */      \
                else        {l=L; r=R;   do {S(l,r) l+=Esiz; r-=Esiz;} while(l< r);}               \
   }else{       if (rev==0) {l=L; p=L-Z; do {C(p,l) l+=Esiz; p+=Esiz;} while(l<=R);}               \
                else        {l=L; p=R-Z; do {C(p,l) l+=Esiz; p-=Esiz;} while(l<=R);}               \
                L-=Z; R-=Z;                                                                        \
   }                                                                                               \
   for (r=L+Esiz; r <= R; r+=Esiz) {  /*hoj判定2種ｘrev判定2種で４種類の挿入ソートにするのが最速*/ \
     if (CMP(r-Esiz,r)<=0) continue;  /*これを実装してみたが、複雑すぎるので採用を見合わせた    */ \
     p=L+Z; C(p,r)  C(r,r-Esiz)       /*挿入する要素を一時的に p に保存する                     */ \
     for (l=r-Esiz-Esiz; L<=l && CMP(l,p)>0; l-=Esiz) C(l+Esiz,l)                                  \
     C(l+Esiz,p)                                                                                   \
   }                                                                                               \
   return;                                                                                         \
 }                                                                                                 \
                                                                                                   \
 if (hoj==0) {l=l0=L+Z; r=r0=R+Z; m=m0=L  ;} /* r:比較値がピボットより大きい要素を次に置く場所 */  \
 else        {l=l0=L  ; r=r0=R-Z; m=m0=L-Z;} /* m:比較値がピボットと    同じ要素を次に置く場所 */  \
                                                                                                   \
 if (n <= _QS_MID1) {              /* ３点処理        「_QS_MID1==120」は実験で得られた値      */  \
   M = L + Esiz * (n >> 1);        /* 必要なら、９点処理と同じ防御策に変更可能(少し遅くなる)   */  \
   M=med3(L, M, R);                /* ３点内の中央要素をMにセットする                          */  \
   if (m==M) {p=L; goto tikamiti;}                                                                 \
 }else{                            /* ９点処理    ９点内の近似的中央要素をMにセットする        */  \
   char *l,*l0,*r,*r0,*m,*m0;      /* 変数名を再利用するために、再度変数宣言をする。           */  \
   size_t u=n/9, q=(rndm%u)*Esiz, v=u*Esiz;  /* 乱数により、悪意のある配列を防御する           */  \
   l=(p=L+q); m=(p+=v); r=(p+=v); l0=med3(l, m, r);                                                \
   l=(p+= v); m=(p+=v); r=(p+=v); m0=med3(l, m, r);                                                \
   l=(p+= v); m=(p+=v); r=(p+ v); r0=med3(l, m, r); M=med3(l0, m0, r0);                            \
 }                                                                                                 \
                                                                                                   \
 /* ピボットと比較して、小要素,大要素,同要素を l,r,m の位置に並べて行く。  mは常にptr上にある */   \
 for (p=L; p<M;) {LT(p,M) {C(l,p) I(l)} else_GT {C(r,p) D(r)} else {C(m,p) I(m)} I(p)}             \
 C(m,p) M=m; tikamiti: I(m) I(p)                                                                   \
 for (  ; p<=R;) {LT(p,M) {C(l,p) I(l)} else_GT {C(r,p) D(r)} else {C(m,p) I(m)} I(p)}             \
 /* この時点で (l-l0)+(m-m0)+(r0-r)==n*Esiz になっている */                                        \
                                                                                                   \
 /* m0～m をptr上の最終位置に移動する。これ以降この要素を参照・移動することはない */               \
 if (rev==0) {                                                                                     \
   if (l0<l) { p=(l-l0)+m; do {D(m) D(p) C(p,m)} while (m0<m);}  /*m0～mには少なくとも１個ある*/   \
 }else{ /* rev==1 */                                                                               \
   char *ll=l-Z;                                                                                   \
   if (m<=ll) {                                                                                    \
     p=ll; do {D(m) C(p,m) I(p)} while(m0<m);                                                      \
   }else{                                                                                          \
     for (M=m-Esiz, p=ll; p<M;) {S(p,M) I(p) D(M)}                                                 \
     for (p=m, m=ll; m0<m;) {D(m) C(p,m) I(p)}                                                     \
   }                                                                                               \
 }                                                                                                 \
                                                                                                   \
 /*ピボットより 小さい要素 と 大きい要素 を別々にソートする。要素数の少ない方からソートする。*/    \
 if (l-l0 < r0-r)                                                                                  \
      { SORT_TYPE(l0, l-Esiz, size, cmp, rev  ,     1, Z);   L=r+Esiz; R=r0; rev^=1; hoj^=1; }     \
 else { SORT_TYPE(r+Esiz, r0, size, cmp, rev^1, hoj^1, Z);   L=l0; R=l-Esiz;         hoj =1; }     \
 goto LOOP;                                                                                        \
}                                                                                                  \


////////////////////////////////　直接ソート用に マクロを展開　////////////////////////////////////
#define C1(x,y)  {ASS_CNT(1) mvfnc(x,y);}
#define S1(x,y)  {ASS_CNT(2) swfnc(x,y);}
#define CMP(a,b)  cmp(a,b)
#define Esiz      size                             /* Esiz は実際にソートされる配列の要素の大きさ*/
 SORT_BODY( _QS_MID2, C1, S1, sort_direct )        /************* 直接 ソート の本体 *************/


////////////////////////////////　ptr_t ソート用に マクロを展開　//////////////////////////////////
#define C2(x,y) {ASS_CNT(1)                          *(char**)(x)=*(char**)(y);                   }
#define S2(x,y) {ASS_CNT(2) {char *tmp=*(char**)(x); *(char**)(x)=*(char**)(y); *(char**)(y)=tmp;}}
#undef  Esiz
#define Esiz  sizeof(char*)                        /* Esiz は実際にソートされる配列の要素の大きさ*/
 SORT_BODY( _QS_MID3, C2, S2, sort_ptr_t )         /************* ptr_t ソート の本体 ************/


////////////////////////////////　間接ソート用に マクロを展開　////////////////////////////////////
#undef  CMP
#define CMP(a,b)  cmp( *(void**)(a), *(void**)(b) )
 SORT_BODY( _QS_MID3, C2, S2, sort_indirect )      /************* 間接 ソート の本体 *************/


///////////////　ソートの入り口　直接ソート・ptr_tソート・間接ソートへ振り分ける　/////////////////
int sqsort( void *base, size_t nel, size_t size,  int (*cmp)(void *a, void *b) )
    // base : ソートしようとする配列へのポインタ
    // nel  : 配列baseの要素数
    // size : 配列baseの要素の大きさ（バイト単位）
    // cmp  : 要素の大小比較をする関数へのポインタ     // sqsort関数の戻り値　0:正常  -1:メモリ不足
{
 char *ptr,*ptr2; /* ptrとptr2 は実際にソートする配列と補助配列を指す */ 
 size_t Z;        /* Z は配列・補助配列間の距離 */
 if (rndm == 0) {                  /* 初回だけ、値のセットをする                       */
   rndm = (clock() & 0x7FFFFFFFL); /* clock()を乱数発生器として使用。正整数を保証      */
   if (rndm == 0) {rndm=12345;}    /* clock()が 0 だったとき、２度目の初期化を防ぐため */
 }

 if (_QS_SORT == 1)                                                 goto direct;
 if (_QS_SORT == 2)                                                 goto indirect;
 if (size == sizeof(char*) && (ENINT(base)&(sizeof(char*)-1)) == 0) goto ptr_type;
 if (size >= 520)                                                   goto indirect;
 if (size <   16)                                                   goto direct;
 if (nel  <   70)                                                   goto direct;

 // 直接ソート選択に必要なeqcnt(下記参照)は、nelとsizeごとに変化する。実験により必要な値を求めた。
 // 下はnel[10000000～100] X size[16～480]でのeqcntの必要値。eqcntがこの値以上で直接ソートを選択。
 // 下はnel代表 右はsize値 16 24 32 40 48 56 64 72 80 88 96 104112120160200240280320360400440480
 static char eq_tab10M []={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 4, 6,10,10,14,14};
 static char eq_tab4M  []={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,10,10,10,10,14,14,14,14};
 static char eq_tab2M  []={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4,14,14,14,14,14,33,33,33};
 static char eq_tab1M  []={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 6,14,14,14,14,14,33,33,33};
 static char eq_tab400K[]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2,14,14,33,33,33,33,33,33,33};
 static char eq_tab200K[]={ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 6,10,14,33,33,33,33,33,33,33,33};
 static char eq_tab100K[]={ 0, 1, 1, 1, 1, 2, 2, 2, 4, 6,10,10,14,14,33,33,33,33,33,33,33,33,33};
 static char eq_tab40K []={ 1, 1, 1, 1, 4,10,14,14,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33};
 static char eq_tab20K []={ 1, 1, 1, 2, 6,14,14,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33};
 static char eq_tab10K []={ 1, 1,14,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33};
 static char eq_tab4K  []={ 1, 1, 1, 1, 1, 1, 3,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33};
 static char eq_tab2K  []={ 1, 1, 1, 1, 1, 2, 3, 4, 4, 5, 6,10,10,14,33,33,33,33,33,33,33,33,33};
 static char eq_tab1K  []={ 1, 1, 1, 1, 1, 2, 3, 4, 4, 5, 6, 8,10,10,14,33,33,33,33,33,33,33,33};
 static char eq_tab400 []={ 1, 1, 1, 2, 2, 2, 4, 4, 5, 5, 6, 8,10,14,14,33,33,33,33,33,33,33,33};
 static char eq_tab200 []={ 1, 1, 1, 2, 2, 2, 4, 4, 4, 5, 5, 6, 8, 8,14,33,33,33,33,33,33,33,33};
 static char eq_tab100 []={ 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 5, 6, 6, 8,10,33,33,33,33,33,33,33,33};
 static char*eq_tab;

 if      (nel <    150) eq_tab = eq_tab100 ;
 else if (nel <    300) eq_tab = eq_tab200 ;
 else if (nel <    700) eq_tab = eq_tab400 ;
 else if (nel <   1500) eq_tab = eq_tab1K  ;
 else if (nel <   3000) eq_tab = eq_tab2K  ;
 else if (nel <   7000) eq_tab = eq_tab4K  ;
 else if (nel <  15000) eq_tab = eq_tab10K ;
 else if (nel <  30000) eq_tab = eq_tab20K ;
 else if (nel <  70000) eq_tab = eq_tab40K ;
 else if (nel < 150000) eq_tab = eq_tab100K;
 else if (nel < 300000) eq_tab = eq_tab200K;
 else if (nel < 700000) eq_tab = eq_tab400K;
 else if (nel <1500000) eq_tab = eq_tab1M  ;
 else if (nel <3000000) eq_tab = eq_tab2M  ;
 else if (nel <7000000) eq_tab = eq_tab4M  ;
 else /*  nel>=7000000*/eq_tab = eq_tab10M ;

 //eq_tab[nel,size]の値を取り出す。eqcnt(値は0～32)がこの値以上なら直接ソート,それ以外なら間接ソート
 {
  int eq_low = ( size < 120 ? eq_tab[(size-16)/8] : eq_tab[(size-120)/40+13] ); //eq_tabは左右２部制
  if (eq_low == 0 ) goto   direct;     // eqcntの値を計算するまでもなく eqcnt(0～32) >= eq_low(0)
  if (eq_low == 33) goto indirect;     // eqcntの値を計算するまでもなく eqcnt(0～32) <  eq_low(33)

#define VBUN 64          /* 比較する組(ペアー)の数の２倍==VBUN。     nel>=70 なので nel>=VBUN となる。*/

  //適当な32組の要素を比較して、等しい組の数を eqcnt とする。eqcnt==0～32になる。
  int eqcnt=0; size_t vsize=(nel/VBUN)*size; size_t half=vsize*(VBUN/2); char *ip_end=base+half;
  for (char *ip=base; ip<ip_end; ip+=vsize) if ((*cmp)(ip,ip+half)==0) eqcnt++;

  if (eqcnt >= eq_low) goto direct; else goto indirect;
 }


 direct:       /************* 直接 ソート の開始 *******************/
 ptr = base;
 ptr2 = malloc( nel * size );          //ptr2はソートのための補助配列
 if (ptr2 == NULL) {goto indirect;}    //直接ソートが不可なので、間接ソートを試す
 Z = ptr2 - ptr;                       //Zは２個の配列間の距離。正負両方あり

 mmprepare( base, size );
 sort_direct( ptr, ptr+nel*size-size, size, cmp, 0/*rev*/, 0/*hoj*/, Z );  // 直接ソートの実行

 free( ptr2 );
 return 0;     /************* 直接 ソート の終了 *******************/


 ptr_type:     /************* ptr_t ソート の開始 ******************/
 ptr = base;
 ptr2 = malloc( nel * sizeof(char*) );   //ptr2はソートのための補助配列
 if (ptr2 == NULL) {return -1;}          //作業領域が確保できないので、ソートは失敗
 Z = ptr2 - ptr;                         //Zは２個の配列間の距離。正負両方あり

 sort_ptr_t( ptr, ptr+nel*sizeof(char*)-sizeof(char*), size, cmp, 0/*rev*/, 0/*hoj*/, Z );  //ptr_t

 free( ptr2 );
 return 0;     /************* ptr_t ソート の終了 ******************/


 indirect:     /************* 間接 ソート の開始 *******************/
 Z = nel * sizeof(char*) ;                        //Zはポインタ配列１個のバイト数
 ptr = malloc( Z * 2 + (size > Z ? size-Z : 0 )); //２個のポインタ配列を同時に確保する
 if (ptr == NULL) {return -1;}                    //作業領域が確保できないので、ソートは失敗
 ptr2 = ptr + Z;                                  //ptrは実際にソートする配列。ptr2は補助配列
 {
  void **tp=(void**)ptr; char *ip_end=(char*)base+nel*size;
  for (char *ip=base; ip<ip_end; ip+=size, tp++) *tp=(void*)ip;  //ポインタ配列ptrの要素をセット
 }

 sort_indirect( ptr, ptr2-sizeof(char*), size, cmp, 0/*rev*/, 0/*hoj*/, Z );  // 間接ソートの実行

 mmprepare( base, size );
 // ソート終了後に、ポインタ配列ptrを用いて、本来の配列baseの要素を移動してから終了する
 {void **tp;  char *ip, *kp, *tmp=ptr2; size_t i;
  // tp[i]はループの糸口(先頭)を指す。ipは退避要素の元の位置,kpは空き地を指す
  for (tp = (void **)ptr, i = 0, ip = base; i < nel; i++, ip += size)
    if ((kp = tp[i]) != ip) {
      size_t j = i;
      char *jp = ip;
      C1(tmp, ip);
      do {
        size_t k = (kp - (char *) base) / size;
        tp[j] = jp;
        C1(jp, kp);
        j = k;
        jp = kp;
        kp = tp[k];
      }while (kp != ip);
      tp[j] = jp;
      C1(jp, tmp);
    }
 }

 free( ptr );
 return 0;     /************* 間接 ソート の終了 *******************/
}
