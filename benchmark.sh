#!/bin/bash

cc  -O1                             main_prog.c                -o my_qsort
                                                                          
cc  -O1                                         -c sqsort.c               
cc  -O1  -fcommon  -Dqsort=sqsort   main_prog.c    sqsort.o    -o sqsort  
                                                                          
cc  -O1                                         -c timsort.c              
cc  -O1  -fcommon  -Dqsort=timsort  main_prog.c    timsort.o   -o timsort 

prin=benchmark

echo '----------------- benchmark.txt begin -------------------- '           >$prin.txt
echo ' '                                                                    >>$prin.txt
echo '　　キー:同値なし 要素:1万個　要素サイズ:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort    -3      10000         8       4000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3      10000         8       4000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3      10000         8       4000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3      10000        20       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000        80       3000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3      10000        80       3000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3      10000        80       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000      1000        600      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3      10000      1000        600      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3      10000      1000        600      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '　キー種別:100種　要素:1万個　要素サイズ:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort   100      10000         8       6000      -1  -1  -1  0    >>$prin.txt
time ./sqsort     100      10000         8       6000      -1  -1  -1  0    >>$prin.txt
time ./timsort    100      10000         8       6000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./sqsort     100      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./timsort    100      10000        20       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000        80       4000      -1  -1  -1  0    >>$prin.txt
time ./sqsort     100      10000        80       4000      -1  -1  -1  0    >>$prin.txt
time ./timsort    100      10000        80       4000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000      1000        500      -1  -1  -1  0    >>$prin.txt
time ./sqsort     100      10000      1000        500      -1  -1  -1  0    >>$prin.txt
time ./timsort    100      10000      1000        500      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '　　キー種別:2種　要素:1万個　要素サイズ:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort     2      10000         8      12000      -1  -1  -1  0    >>$prin.txt
time ./sqsort       2      10000         8      12000      -1  -1  -1  0    >>$prin.txt
time ./timsort      2      10000         8      12000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000        20       7000      -1  -1  -1  0    >>$prin.txt
time ./sqsort       2      10000        20       7000      -1  -1  -1  0    >>$prin.txt
time ./timsort      2      10000        20       7000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000        80       6000      -1  -1  -1  0    >>$prin.txt
time ./sqsort       2      10000        80       6000      -1  -1  -1  0    >>$prin.txt
time ./timsort      2      10000        80       6000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000      1000        400      -1  -1  -1  0    >>$prin.txt
time ./sqsort       2      10000      1000        400      -1  -1  -1  0    >>$prin.txt
time ./timsort      2      10000      1000        400      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '　 キー:同値なし　百～百万要素　要素サイズ:200byte '                  >>$prin.txt
time ./my_qsort    -3        100       200     200000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3        100       200     200000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3        100       200     200000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3       1000       200      14000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3       1000       200      14000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3       1000       200      14000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000       200       1000      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3      10000       200       1000      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3      10000       200       1000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3     100000       200         60      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3     100000       200         60      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3     100000       200         60      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3    1000000       200          7      -1  -1  -1  0    >>$prin.txt
time ./sqsort      -3    1000000       200          7      -1  -1  -1  0    >>$prin.txt
time ./timsort     -3    1000000       200          7      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '=================  benchmark.txt end  ==================== '          >>$prin.txt
echo 'my_qsort：ベンチマークテスト実行の計算機上の C ライブラリの qsort '   >>$prin.txt
echo 'sqsort　：今回公開した sqsort '                                       >>$prin.txt
echo 'timsort ：https://github.com/patperry/timsort/ の timsort '           >>$prin.txt
echo 'd=キー種類 e=要素数 s=要素サイズ R繰返し回数 c=比較回数/R T=処理秒数 '>>$prin.txt
echo '各行の最後の数値がソート１回あたりの処理時間(10μ秒単位)です '        >>$prin.txt
echo ' '                                                                    >>$prin.txt
