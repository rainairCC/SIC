
#include <cstdlib>
#include <stdlib.h>
#include <new>
#include <cstring>
#include <iostream>	                                            // cout, endl
#include <fstream>                                              // open, is_open
#include <string.h>                                               // string
#include <stdio.h>
#include <vector>                                               // vector
#include <cstdlib>                                              // atoi, system
#include <iomanip>                                              // setw
#include <ctime>                                                // clock, CLOCKS_PER_SEC
#include <algorithm>
#include <string>
#define PAGE_SIZE   30                                          // number of output per page

using namespace std;

bool readProblem();
bool readTable();
void work();
void token();
void writeAnswer();

string   fileName ;
char problem[100] [100];
char answer[100] [100];
char table1[101] [101];
char table2[101] [101];
char table3[101] [101];
char table4[101] [101];
char dataType[101] [7];
char table5identifier[100] [101];
int table5subroutine[101];
int table5type[101];
int table5pointer[101];
char table6[1000] [1000];
char table7[100][1000];
int aP=0, aT1=0, aT2=0, aT3=0, aT4=0,aT6=0,aT7 = 0;
int iT6 = 1,iT7  = 1;
int jT6  = 1;
int xA = 0;
int yA = 0;
int yT1 = 0;
int reg = 0;
bool sign = false;
int errorType=0,errorLine = 0;
int scope = 0; //在哪被宣告，放入subroutine用的
int main(void){
    int i = 0;

    if ( readProblem() == true ){
        readTable();

        while( i < 100 ){
            table5identifier[i][0] = '-';
            table4[i][0] = '-';
            table3[i][0] = '-';
            table6[i][0] = '-';
            table7[i][0] = '-';
            table5subroutine[i] = -1;
            table5type[i] = -1;
            table5pointer[i] = -1;
            i = i + 1;
        }

        token();

        cout << problem[1] << "\n";
        work();
        writeAnswer();
//cout << table6[1] << "\n";
//cout << table6[2];
    }  // if problem true

} // main

void writeAnswer(){
    int i = 0;
    int j = 1;
    fstream outFile;
    string fname = fileName + "-output.txt";
    outFile.open( fname.c_str(),fstream::out );

    if( !outFile.is_open() ){
        cout << endl << "## Can't create " << fname << " ! ##"<< endl;
        return;
    }
//cout << aT6;
    while( j <= aT6 ){ //i <= aP && i <= aT6
        //outFile << problem[i] << "\n";
        //outFile << answer[i] << "\n";

        outFile << j << "\t" << table6[j] << "\n";

        //i = i + 1;
        j = j + 1;
    }

    if( errorType > 0 ){
        if( errorType == 1 )
            outFile << "line" << errorLine << ": 最後字元非結束指令 ';' \n" << "Syntax Error" ;
        if( errorType == 2 )
            outFile << "line" << errorLine << ": 後面格式錯誤 \n" << "Syntax Error" ;
        if( errorType == 3 )
            outFile << "line" << errorLine << ": 後面不是ID \n" << "Syntax Error" ;


    }

    outFile.close();
}

bool check( char a, char b ){
    if( a <= 'z' && a >= 'a' ){
        if( b <= 'Z' && b >= 'A' )
            b = b + 32;
    }

    if( a <= 'Z' && a >= 'A' ){
        if( b <= 'z' && b >= 'a' )
            a = a + 32;
    }

    if( a == b )
        return true;
    else
        return false;
}

void work() {
    int x = 0;
    int i = 0;
    fstream outFile;
    string fname = fileName + "-output.txt";
cout << i << " " << aP  << "\n";
    while(i <= aP ){
            //cout << aT6;
        if( problem[i][ strlen( problem[i] )-1 ] != ';' ){ // 每行最後應為;
            errorLine = i + 1;
            errorType = 1;

            i = 100000; // 結束文法判斷
        }
        else{
            if( answer[i][1] == '2' ){    //  table2的處理
                if( answer[i][3] == '2' && answer[i][4] == '1' && answer[i][5] == ')' ){  // 是PROGRAM // PROGRAM id; //(2,21)(5, )(1,1)
                    if( answer[i][7] == '5' ) { // 可能符合文法，先算出是在table5哪行
                        int row = 0;
                        x = 12;

                        if( answer[i][10] == ')' )    // 算出是在table5哪行
                            row = answer[i][9] - '0';
                        else{
                            if( answer[i][11] == ')' ) {
                                row = (answer[i][9] - '0' )*10;
                                row = answer[i][10] - '0' + row;
                                x = 13;
                       //cout<< x <<"a" << answer[i][x];
                            }
                            else{
                                if( answer[i][12] == ')' ) {
                                    row = (answer[i][9] - '0' )*100;
                                    row = (answer[i][10] - '0' )*10 + row;
                                    row = answer[i][11] - '0' + row;
                                    x = 14;
                                }
                            }
                        } // 算出是在table5哪行


                        if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == ')' ) {  //確認後面是;(1,1)，符合文法，+ table5的pointer
                            table5pointer[row] = i+1;
                            scope = row;
                            i = i + 1;
                        }
                        else {  // PROGRAM後面不只有ID && ';'
                            errorLine = i + 1;
                            errorType = 2;

                            i = 100000; // 結束文法判斷
                        }
                    } //可能符合文法
                    else{   //後面不是加id，文法錯誤
                        errorLine = i + 1;
                        errorType = 3;

                        i = 100000; // 結束文法判斷
                    }  //後面不是加id，文法錯誤

                } // 是PROGRAM
                else{    // 處理table2其他可能

                    if( answer[i][3] == '2' && answer[i][4] == '5' && answer[i][5] == '\)') { // 是VARIABLE
                    // VARIABLE INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                    if( answer[i][7] == '2' && answer[i][9] == '1' && answer[i][10] == '4' && answer[i][13] == '1' && answer[i][15] == '1' && answer[i][16] == '2'){
                   //INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                        if( answer[i][19] == '5' ) { // 可能符合文法，先算出是在table5哪行
                            int row = 0;
                            x = 24;

                            if( answer[i][22] == ')' )    // 算出是在table5哪行
                                row = answer[i][21] - '0';
                            else{
                                if( answer[i][23] == ')' ) {
                                    row = (answer[i][21] - '0' )*10;
                                    row = answer[i][22] - '0' + row;
                                    x = 25;
                                }
                                else{
                                    if( answer[i][24] == ')' ) {
                                        row = (answer[i][21] - '0' )*100;
                                        row = (answer[i][22] - '0' )*10 + row;
                                        row = answer[i][23] - '0' + row;
                                        x = 26;
                                    }
                                }
                            } // 算出是在table5哪行
                            // ((5,73)	,   ,	,	)	I
                            table5subroutine[row] = scope;
                            table5type[row] = 4;   // 先直接判斷type4
                            table6[iT6][0] = '(';
                            table6[iT6][1] = '(';
                            table6[iT6][2] = '5';
                            table6[iT6][3] = ',';
                            jT6 = 5;
cout << answer[i] ;
                            if( x == 24 )
                                table6[iT6][4] = answer[i][21];
                            if( x == 25 ){
                                table6[iT6][4] = answer[i][21];
                                table6[iT6][5] = answer[i][22];
                                jT6 = 6;
                            }
                            if( x == 26 ){
                                table6[iT6][4] = answer[i][21];
                                table6[iT6][5] = answer[i][22];
                                table6[iT6][6] = answer[i][23];
                                jT6 = 7;
                            }
                             //cout << table6[iT6][4]<< "ddd";
                            table6[iT6][jT6] = ')';
                            table6[iT6][jT6+1] = ' ';
                            table6[iT6][jT6+2] = ',';
                            table6[iT6][jT6+3] = ' \t';
                            table6[iT6][jT6+4] = ',';
                            table6[iT6][jT6+5] = ' \t';
                            table6[iT6][jT6+6] = ',';
                            table6[iT6][jT6+7] = ' \t';
                            table6[iT6][jT6+8] = ')';
                            table6[iT6][jT6+9] = ' \t';
                            table6[iT6][jT6+10] = table5identifier[row][0];
                            jT6 = jT6 + 11;
                            if( strlen(table5identifier[row]) > 1 ){
                                table6[iT6][jT6] = table5identifier[row][1];
                                jT6 = jT6 + 1;
                            }
                            if( strlen(table5identifier[row]) > 2 ){
                                table6[iT6][jT6] = table5identifier[row][2];
                                jT6 = jT6 + 1;
                            }
                            aT6 = iT6 ;
                            iT6 = iT6 + 1;
                            jT6 = 0;
// 是VARIABLE // VARIABLE INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                            int tt = 0;

                            while( x < strlen(answer[i])-5 && answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == '1' ) {   // 如果接下來是， 確認下一個是否為ID
                                x = x + 6;
                                tt = x;
                                if( answer[i][x] == '2' && answer[i][x+2] == '1' && answer[i][x+3] == '2' )  {
                                    answer[i][x] = '5';
                                    answer[i][x+2] = '7';
                                    answer[i][x+3] = '3';
                                    table5identifier[73][0]='I';
                                }

                                if( answer[i][x] == '5' ) { // 可能符合文法，先算出是在table5哪行
                                    row = 0;
                                    //x = x + 2;

                                    if( answer[i][x+3] == ')' ) {   // 算出是在table5哪行
                                        row = answer[i][x+2] - '0';
                                        x = x + 5;
                                    }
                                    else{
                                        if( answer[i][x+4] == ')' ) {
                                            row = (answer[i][x+2] - '0' )*10;
                                            row = answer[i][x+3] - '0' + row;
                                            x = x + 6;
                                        }
                                        else{
                                            if( answer[i][x+5] == ')' ) {
                                                row = (answer[i][x+2] - '0' )*100;
                                                row = (answer[i][x+3] - '0' )*10 + row;
                                                row = answer[i][x+4] - '0' + row;
                                                x = x + 7;
                                            }
                                        }
                                    } // 算出是在table5哪行

                                    table5subroutine[row] = scope;
                                    table5type[row] = 4;   // 先直接判斷type4

                                    table6[iT6][jT6] = '(';
                                    table6[iT6][jT6+1] = '(';
                                    table6[iT6][jT6+2] = '5';
                                    table6[iT6][jT6+3] = ',';
                                    jT6 = jT6+4;

                                    if( (x-tt) == 5 )
                                        table6[iT6][jT6] = answer[i][tt+2];
                                    if( (x-tt) == 6 ){
                                        table6[iT6][jT6] = answer[i][tt+2];
                                        table6[iT6][jT6+1] = answer[i][tt+3];
                                        jT6 = jT6+2;
                                    }
                                    if( (x-tt) == 7 ){
                                        table6[iT6][jT6] = answer[i][tt+2];
                                        table6[iT6][jT6+1] = answer[i][tt+3];
                                        table6[iT6][jT6+2] = answer[i][tt+4];
                                        jT6 = jT6+3;
                                    }
                                    table6[iT6][jT6] = ')';
                                    table6[iT6][jT6+1] = ' ';
                                    table6[iT6][jT6+2] = ',';
                                    table6[iT6][jT6+3] = ' \t';
                                    table6[iT6][jT6+4] = ',';
                                    table6[iT6][jT6+5] = ' \t';
                                    table6[iT6][jT6+6] = ',';
                                    table6[iT6][jT6+7] = ' \t';
                                    table6[iT6][jT6+8] = ')';
                                    table6[iT6][jT6+9] = ' \t';
                                    table6[iT6][jT6+10] = table5identifier[row][0];
                                    jT6 = jT6 + 11;
                                    if( strlen(table5identifier[row]) > 1 ){
                                        table6[iT6][jT6] = table5identifier[row][1];
                                        jT6 = jT6 + 1;
                                    }
                                    if( strlen(table5identifier[row]) > 2 ){
                                        table6[iT6][jT6] = table5identifier[row][2];
                                        jT6 = jT6 + 1;
                                    }
                                    aT6 = iT6 ;
                                    iT6 = iT6 + 1;
                                    jT6 = 0;
                                }
                                else{  // 後面不是table5 ID
                                    errorLine = i + 1;
                                    errorType = 3;
                                    x = 100000;
                                    i = 100000; // 結束文法判斷

                                    //aT6 = aT6 - 1;
                                }
                            } // 如果接下來是， 確認下一個是否為ID

                            //要往下看是不是逗號，如果是，繼續做ID table5判斷，不是的話，應該要是;(1,1)
                            // ID要加subroutine(用scope)&&type(用(2,14直接判斷),  (2,2)....等等其他的之後再寫吧)
                            // table6, ID位置加上去，全加完再判斷文法，有錯的話看加幾個ID就減aT6&&error輸出


                            if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == ')' ) {  //確認後面是;(1,1)，符合文法，+ table5的pointer
                                table5pointer[row] = i+1;
                                i = i + 1;
                            }
                            else {  // 後面不只有ID && ';'
                                errorLine = i + 1;
                                errorType = 2;
                                aT6 = aT6 - 1;
                                i = 100000; // 結束文法判斷
                                 //cout << "sss";
                            }
                        } //可能符合文法
                        else{   //後面不是加id，文法錯誤
                            errorLine = i + 1;
                            errorType = 3;

                            i = 100000; // 結束文法判斷
                        }  //後面不是加id，文法錯誤

                    }   // INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                    else {   //後面沒接 INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                        errorLine = i + 1;
                        errorType = 2;

                        i = 100000; // 結束文法判斷

                    }
                    }// 是VARIABLE // VARIABLE INTERGER : id {,id}; //(2,25)(2,14)(1,12)(5, ) {(1,11)(5, )} (1,1)
                    else{   //不是VARIABLE，換下一個statement

                        if( answer[i][3] == '4' )  {
                        // 是DIMENSION // DIMENSION REAL(這裡我先只用INTERGER) : id ( num{,num} ){,id( num{,num} )};
                      //(2,4)(2,14)(1,12)(5, )(1,2)(3, ){(1,11)(3, )} (1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)

                        if( answer[i][6] == '2' && answer[i][8] == '1' && answer[i][9] == '4' && answer[i][12] == '1' && answer[i][14] == '1' && answer[i][15] == '2' ){
               // REAL(這裡我先只用INTERGER) : id ( num{,num} ){,id( num{,num} )};
                      //(2,14)(1,12)(5, )(1,2)(3, ){(1,11)(3, )} (1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)
                            if( answer[i][18] == '5' ) { // 可能符合文法，先算出是在table5哪行
                                int row = 0;
                                x = 23;

                                if( answer[i][21] == ')' )    // 算出是在table5哪行
                                    row = answer[i][20] - '0';
                                else{
                                    if( answer[i][22] == ')' ) {
                                        row = (answer[i][20] - '0' )*10;
                                        row = answer[i][21] - '0' + row;
                                        x = 24;
                                    }
                                    else{
                                        if( answer[i][23] == ')' ) {
                                            row = (answer[i][20] - '0' )*100;
                                            row = (answer[i][21] - '0' )*10 + row;
                                            row = answer[i][22] - '0' + row;
                                            x = 25;
                                        }
                                    }
                                } // 算出是在table5哪行
                            // ((5,73)	,   ,	,	)	I
                                //table5subroutine[row] = scope;
                                table5type[row] = 4;   // 先直接判斷type4
                                table6[iT6][0] = '(';
                                table6[iT6][1] = '(';
                                table6[iT6][2] = '5';
                                table6[iT6][3] = ',';
                                jT6 = 5;

                                if( x == 23 )
                                    table6[iT6][4] = answer[i][20];
                                if( x == 24 ){
                                    table6[iT6][4] = answer[i][20];
                                    table6[iT6][5] = answer[i][21];
                                    jT6 = 6;
                                }
                                if( x == 25 ){
                                    table6[iT6][4] = answer[i][20];
                                    table6[iT6][5] = answer[i][21];
                                    table6[iT6][6] = answer[i][22];
                                    jT6 = 7;
                                }
                             //cout << table6[iT6][4]<< "ddd";
                                table6[iT6][jT6] = ')';
                                table6[iT6][jT6+1] = ' ';
                                table6[iT6][jT6+2] = ',';
                                table6[iT6][jT6+3] = ' \t';
                                table6[iT6][jT6+4] = ',';
                                table6[iT6][jT6+5] = ' \t';
                                table6[iT6][jT6+6] = ',';
                                table6[iT6][jT6+7] = ' \t';
                                table6[iT6][jT6+8] = ')';
                                table6[iT6][jT6+9] = ' \t';
                                table6[iT6][jT6+10] = table5identifier[row][0];
                                jT6 = jT6 + 11;
                                if( strlen(table5identifier[row]) > 1 ){
                                    table6[iT6][jT6] = table5identifier[row][1];
                                    jT6 = jT6 + 1;
                                }
                                if( strlen(table5identifier[row]) > 2 ){
                                    table6[iT6][jT6] = table5identifier[row][2];
                                    jT6 = jT6 + 1;
                                }
                                aT6 = iT6 ;
                                iT6 = iT6 + 1;
                                jT6 = 0;
                    // 是DIMENSION // DIMENSION REAL(這裡我先只用INTERGER) : id ( num{,num} ){,id( num{,num} )};
                      //(2,4)(2,14)(1,12)(5, )(1,2)(3, ){(1,11)(3, )} (1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)

                      //table5已經出來了 // (1,2)(3, ){(1,11)(3, )} (1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)

                                table7[iT7][0] = '4'; //INTEGER ARRAY

                                if( answer[i][x] == '1' && answer[i][x+2] == '2' ){   // 是(  ，往下判斷是不是數字
                                    x = x + 5;

                                    if( answer[i][x] == '3' && answer[i][x+1] == ',' ){   // 是 3,  ，將數字放入table7
                                        x = x + 2;
                                        int i7 = 0;
                                        int r = iT7+2 ;
                                        while( answer[i][x] != '\)')  {
                                            table7[r][i7] = answer[i][x];
                                            i7 = i7 + 1;
                                            x = x + 1;
                                        }

                                        i7 = 0;
                                        x = x + 2;
                                  // {(1,11)(3, )} (1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)

                                        if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == '1' && answer[i][x+6] == '3' ){  //看有沒有， + num
                                            //放NUM
                                            x = x + 8;
                                            i7 = 0;
                                            r = iT7+3 ;

                                            while( answer[i][x] != '\)')  {
                                                table7[r][i7] = answer[i][x];
                                                i7 = i7 + 1;
                                        //cout<< x <<"a" << answer[i][x];
                                                x = x + 1;
                                            }

                                            i7 = 0;
                                            x = x + 2;

                                            table7[iT7+1][0] = '2';
                                            table5pointer[row] = iT7;
                                            iT7 = iT7 + 4;
                                            aT7 = iT7 - 1;

                                        }//看有沒有， + num
                                        else{  //不是，num

                                            table7[iT7+1][0] = '1';  //如果num只有一個
                                            table5pointer[row] = iT7;
                                            iT7 = iT7 + 3;
                                            aT7 = iT7 - 1;
                                        }
                                        // 看是不是(1,3)
                                        //(1,3){(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)
                                        //cout<< x <<"a" << answer[i][x];
                                        if( answer[i][x] == '1' && answer[i][x+2] == '3' && answer[i][x+3] == ')' ){   //看是不是) (必須是)
                                            x = x + 5;
                                            // {(1,11)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)
                                            row = 0;
                                            if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == '1' ){  //有第二個ID

                                                // (5, )(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)
                                                x = x + 6;
                                                int tt = x;
//cout << x << "aaqqq" << tt <<"\n";
                                                if( answer[i][x] == '5' ) { // 可能符合文法，先算出是在table5哪行

                                                    if( answer[i][x+3] == ')' ) {   // 算出是在table5哪行
                                                        row = answer[i][x+2] - '0';
                                                        x = x + 5;
                                                    }
                                                    else{
                                                        if( answer[i][x+4] == ')' ) {
                                                            row = (answer[i][x+2] - '0' )*10;
                                                            row = answer[i][x+3] - '0' + row;
                                                            x = x + 6;
                                                        }
                                                        else{
                                                            if( answer[i][x+5] == ')' ) {
                                                                row = (answer[i][x+2] - '0' )*100;
                                                                row = (answer[i][x+3] - '0' )*10 + row;
                                                                row = answer[i][x+4] - '0' + row;
                                                                x = x + 7;
                                                            }

                                                        }
                                                    } // 算出是在table5哪行
//cout << x << "aaqqq" << tt <<"\n";
                                                    //table5subroutine[row] = scope;
                                                    table5type[row] = 4;   // 先直接判斷type4

                                                    table6[iT6][jT6] = '(';
                                                    table6[iT6][jT6+1] = '(';
                                                    table6[iT6][jT6+2] = '5';
                                                    table6[iT6][jT6+3] = ',';
                                                    jT6 = jT6+4;

                                                    if( (x-tt) == 5 )
                                                        table6[iT6][jT6] = answer[i][tt+2];
                                                    if( (x-tt) == 6 ){
                                                        table6[iT6][jT6] = answer[i][tt+2];
                                                        table6[iT6][jT6+1] = answer[i][tt+3];
                                                        jT6 = jT6+2;
                                                    }
                                                    if( (x-tt) == 7 ){
                                                        table6[iT6][jT6] = answer[i][tt+2];
                                                        table6[iT6][jT6+1] = answer[i][tt+3];
                                                        table6[iT6][jT6+2] = answer[i][tt+4];
                                                        jT6 = jT6+3;
                                                    }
                                                    table6[iT6][jT6] = ')';
                                                    table6[iT6][jT6+1] = ' ';
                                                    table6[iT6][jT6+2] = ',';
                                                    table6[iT6][jT6+3] = ' \t';
                                                    table6[iT6][jT6+4] = ',';
                                                    table6[iT6][jT6+5] = ' \t';
                                                    table6[iT6][jT6+6] = ',';
                                                    table6[iT6][jT6+7] = ' \t';
                                                    table6[iT6][jT6+8] = ')';
                                                    table6[iT6][jT6+9] = ' \t';
                                                    table6[iT6][jT6+10] = table5identifier[row][0];
                                                    jT6 = jT6 + 11;
                                                    if( strlen(table5identifier[row]) > 1 ){
                                                        table6[iT6][jT6] = table5identifier[row][1];
                                                        jT6 = jT6 + 1;
                                                    }
                                                    if( strlen(table5identifier[row]) > 2 ){
                                                        table6[iT6][jT6] = table5identifier[row][2];
                                                        jT6 = jT6 + 1;
                                                    }
                                                    aT6 = iT6 ;
                                                    iT6 = iT6 + 1;
                                                    jT6 = 0;


                                                    //(1,2)(3, ){(1,11)(3, )} (1,3)}(1,1)

//cout << x << "aaqqq" << tt <<"\n";

                                                    if( answer[i][x] == '1' && answer[i][x+2] == '2' ){   // 是(  ，往下判斷是不是數字
                                                        x = x + 5;
                                                        if( answer[i][x] == '3' && answer[i][x+1] == ',' ){   // 是 3,  ，將數字放入table7
                                                            x = x + 1;
                                                            int i7 = 0;
                                                            int r = iT7+2;
                                                            table7[iT7][0] = '4'; //INTEGER ARRAY

                                                            while( answer[i][x] != '\)')  {
                                                                table7[r][i7] = answer[i][x];
                                                                i7 = i7 + 1;
                                                                x = x + 1;
                                                            }

                                                            i7 = 0;
                                                            x = x + 2;
                                                        // {(1,11)(3, )} (1,3)}(1,1)

                                                            if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == '1' && answer[i][x+6] == '3' ){  //看有沒有， + num

                                                                //放NUM
                                                                x = x + 8;
                                                                i7 = 0;
                                                                r = iT7+3 ;

                                                                while( answer[i][x] != ')')  {
                                                                    table7[r][i7] = answer[i][x];
                                                                    i7 = i7 + 1;
                                                                    x = x + 1;
                                                                }

                                                                i7 = 0;
                                                                x = x + 2;

                                                                table7[iT7+1][0] = '2';
                                                                table5pointer[row] = iT7;
                                                                iT7 = iT7 + 4;
                                                                aT7 = iT7 - 1;

                                                            }//看有沒有， + num
                                                            else{  //不是，numcout << ">>>>" <<answer[i][x] << "<<<<\n";
                                                                table7[iT7+1][0] = '1';  //如果num只有一個
                                                                table5pointer[row] = iT7;
                                                                iT7 = iT7 + 3;
                                                                aT7 = iT7 - 1;

                                                            }
//cout << ">>>>" <<answer[i][x] << "<<<<\n";
                                                            // ((1,1)
                                                            if( answer[i][x] == '1' && answer[i][x+2] == '3' && answer[i][x+5] == '1' && answer[i][x+7] == '1' ) //是 ); 沒錯
                                                                i = i +1;
                                                            else{   //x = x + 3;
                                                                    //cout << "qqqq\n";
                                                                errorLine = i + 1;
                                                                errorType = 2;
                                                                aT6 = aT6 - 1;
                                                                i = 100000; // 結束文法判斷
                                                            }

                                                        }// 是 3,  ，將數字放入table7
                                                    } // 不是 (
                                                    else{
                                                        errorLine = i + 1;
                                                        errorType = 2;
                                                        aT6 = aT6 - 1;
                                                        i = 100000; // 結束文法判斷
                                                    }
                                                } // 不是table5 但前面有，
                                                else{
                                                    errorLine = i + 1;
                                                    errorType = 2;
                                                    //aT6 = aT6 - 1;
                                                    i = 100000; // 結束文法判斷
                                                }
                                            }
                                            else{  // 沒有的二個ID
                                                //(1,1)

                                                if( answer[i][x] != '1' || answer[i][x+2] != '1' ){
                                                    errorLine = i + 1;
                                                    errorType = 2;
                                                    aT6 = aT6 - 1;
                                                    i = 100000; // 結束文法判斷
                                                }
                                                else i = i + 1;
                                            }
                                        } // 看是不是) (必須是)
                                        else{
                                            errorLine = i + 1;
                                            errorType = 2;
                                            aT6 = aT6 - 1;
                                            i = 100000; // 結束文法判斷
                                        }

                                    } // 是 3,  ，將數字放入table7
                                    else {// 竟然不是數字阿

                                        errorLine = i + 1;
                                        errorType = 2;
                                        aT6 = aT6 - 1;
                                        i = 100000; // 結束文法判斷
                                    }

                                }  // 竟然不是， (
                                else {
                                    errorLine = i + 1;
                                    errorType = 2;
                                    aT6 = aT6 - 1;
                                    i = 100000; // 結束文法判斷

                                }



                                }
                                else{  // 後面不是table5 ID
                                    errorLine = i + 1;
                                    errorType = 3;
                                    x = 100000;
                                    i = 100000; // 結束文法判斷

                                    //aT6 = aT6 - 1;
                                }
                        }
                        else{    // 不是 接REAL : id ( num{,num} ); //(2,4)(2,22)(1,12)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)(1,1)
                            errorLine = i + 1;
                            errorType = 3;

                            i = 100000; // 結束文法判斷
                        }
//cout <<iT6 <<"   " << i;
                    }  // 是DIMENSION // DIMENSION REAL : id ( num{,num} ); //(2,4)(2,22)(1,12)(5, )(1,2)(3, ){(1,11)(3, )} (1,3)(1,1)
                    else{ //  不是DIMENSION，換下一個statement
                        int row = 0;

                        if( answer[i][3] == '1' && answer[i][4] == '5' && answer[i][5] == '\)') { // 是LABEL
                             // 是LABEL // LABEL  id { , id }; //(2,15)(5, ){(1,11)(5, )}(1,1)


                            if( answer[i][7] == '5' ) { // 可能符合文法，先算出是在table5哪行
                                int row = 0;
                                x = 12;

                                if( answer[i][10] == ')' )    // 算出是在table5哪行
                                    row = answer[i][9] - '0';
                                else{
                                    if( answer[i][11] == ')' ) {
                                        row = (answer[i][9] - '0' )*10;
                                        row = answer[i][10] - '0' + row;
                                        x = 13;
                                    }
                                else{
                                    if( answer[i][12] == ')' ) {
                                        row = (answer[i][9] - '0' )*100;
                                        row = (answer[i][10] - '0' )*10 + row;
                                        row = answer[i][11] - '0' + row;
                                        x = 13;
                                    }
                                }
                            } // 算出是在table5哪行
                            // ((5,73)	,   ,	,	)	I
                            table5subroutine[row] = scope;

                            if( table5type[row] >= 0 ) { // 重複了 ,要加入中間碼&& 加原本LABEL的pointer
////////////////!!!!!!!!!!!!!!!!!!!!!  L92 	ENP;
/////////////////!!!!!!!!!!!!!!!!!!!
///////////////!!!!!!!!!!!!!!!!
                                table5pointer[row] = iT6;
                                table6[iT6][0] = '(';
                                table6[iT6][1] = '(';
                                table6[iT6][2] = '1';
                                table6[iT6][3] = ',';
                                table6[iT6][4] = '5';
                                table6[iT6][5] = ')';
                                table6[iT6][6] = ' ';
                                table6[iT6][7] = ',';
                                table6[iT6][8] = ' \t';
                                table6[iT6][9] = ',';
                                table6[iT6][10] = ' \t';
                                table6[iT6][11] = ',';
                                table6[iT6][12] = ' \t';
                                table6[iT6][13] = ')';
                                table6[iT6][14] = ' \t';
                                aT6 = iT6 ;
                                iT6 = iT6 + 1;
                                jT6 = 0;

                            }
                            else{
                                table5type[row] = 5;   // type5
                                table6[iT6][0] = '(';
                                table6[iT6][1] = '(';
                                table6[iT6][2] = '5';
                                table6[iT6][3] = ',';
                                jT6 = 5;

                                if( x == 12 )
                                    table6[iT6][4] = answer[i][9];
                                if( x == 13 ){
                                    table6[iT6][4] = answer[i][9];
                                    table6[iT6][5] = answer[i][10];
                                    jT6 = 6;
                                }
                                if( x == 14 ){
                                    table6[iT6][4] = answer[i][9];
                                    table6[iT6][5] = answer[i][10];
                                    table6[iT6][6] = answer[i][11];
                                    jT6 = 7;
                                }
                                //cout << table6[iT6][4]<< "ddd";
                                table6[iT6][jT6] = ')';
                                table6[iT6][jT6+1] = ' ';
                                table6[iT6][jT6+2] = ',';
                                table6[iT6][jT6+3] = ' \t';
                                table6[iT6][jT6+4] = ',';
                                table6[iT6][jT6+5] = ' \t';
                                table6[iT6][jT6+6] = ',';
                                table6[iT6][jT6+7] = ' \t';
                                table6[iT6][jT6+8] = ')';
                                table6[iT6][jT6+9] = ' \t';
                                table6[iT6][jT6+10] = table5identifier[row][0];
                                jT6 = jT6 + 11;
                                if( strlen(table5identifier[row]) > 1 ){
                                    table6[iT6][jT6] = table5identifier[row][1];
                                    jT6 = jT6 + 1;
                                }
                                if( strlen(table5identifier[row]) > 2 ){
                                    table6[iT6][jT6] = table5identifier[row][2];
                                    jT6 = jT6 + 1;
                                }
                                aT6 = iT6 ;
                                iT6 = iT6 + 1;
                                jT6 = 0;
                        // LABEL  id { , id }; //(2,15)(5, ){(1,11)(5, )}(1,1)
                                int tt = 0;

                                while( x < strlen(answer[i])-5 && answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == '1' ) {   // 如果接下來是， 確認下一個是否為ID
                                    x = x + 6;
                                    tt = x;
                                    // cout << answer[i] <<"\n";
//cout << x <<"   " << answer[i];
                                    if( answer[i][x] == '5' ) { // 可能符合文法，先算出是在table5哪行
                                        row = 0;
                                        //x = x + 2;

                                        if( answer[i][x+3] == ')' ) {   // 算出是在table5哪行
                                            row = answer[i][x+2] - '0';
                                            x = x + 5;
                                        }
                                        else{
                                            if( answer[i][x+4] == ')' ) {
                                                row = (answer[i][x+2] - '0' )*10;
                                                row = answer[i][x+3] - '0' + row;
                                                x = x + 6;
                                            }
                                            else{
                                                if( answer[i][x+5] == ')' ) {
                                                    row = (answer[i][x+2] - '0' )*100;
                                                    row = (answer[i][x+3] - '0' )*10 + row;
                                                    row = answer[i][x+4] - '0' + row;
                                                    x = x + 7;
                                                }
                                            }
                                        } // 算出是在table5哪行

                                        table5subroutine[row] = scope;
                                        table5type[row] = 4;   // 先直接判斷type4

                                        table6[iT6][jT6] = '(';
                                        table6[iT6][jT6+1] = '(';
                                        table6[iT6][jT6+2] = '5';
                                        table6[iT6][jT6+3] = ',';
                                        jT6 = jT6+4;

                                        if( (x-tt) == 5 )
                                            table6[iT6][jT6] = answer[i][tt+2];
                                        if( (x-tt) == 6 ){
                                            table6[iT6][jT6] = answer[i][tt+2];
                                            table6[iT6][jT6+1] = answer[i][tt+3];
                                            jT6 = jT6+2;
                                        }
                                        if( (x-tt) == 7 ){
                                            table6[iT6][jT6] = answer[i][tt+2];
                                            table6[iT6][jT6+1] = answer[i][tt+3];
                                            table6[iT6][jT6+2] = answer[i][tt+4];
                                            jT6 = jT6+3;
                                        }
                                        table6[iT6][jT6] = ')';
                                        table6[iT6][jT6+1] = ' ';
                                        table6[iT6][jT6+2] = ',';
                                        table6[iT6][jT6+3] = ' \t';
                                        table6[iT6][jT6+4] = ',';
                                        table6[iT6][jT6+5] = ' \t';
                                        table6[iT6][jT6+6] = ',';
                                        table6[iT6][jT6+7] = ' \t';
                                        table6[iT6][jT6+8] = ')';
                                        table6[iT6][jT6+9] = ' \t';
                                        table6[iT6][jT6+10] = table5identifier[row][0];
                                        jT6 = jT6 + 11;

                                        if( strlen(table5identifier[row]) > 1 ){
                                            table6[iT6][jT6] = table5identifier[row][1];
                                            jT6 = jT6 + 1;
                                        }
                                        if( strlen(table5identifier[row]) > 2 ){
                                            table6[iT6][jT6] = table5identifier[row][2];
                                            jT6 = jT6 + 1;
                                        }
                                        aT6 = iT6 ;
                                        iT6 = iT6 + 1;
                                        jT6 = 0;

                                    }
                                    else{  // 後面不是table5 ID
                                        errorLine = i + 1;
                                        errorType = 3;
                                        x = 100000;
                                        i = 100000; // 結束文法判斷

                                        //aT6 = aT6 - 1;
                                    }
                                } // 如果接下來是， 確認下一個是否為ID

                            //要往下看是不是逗號，如果是，繼續做ID table5判斷，不是的話，應該要是;(1,1)
                            // ID要加subroutine(用scope)&&type(用(2,14直接判斷),  (2,2)....等等其他的之後再寫吧)
                            // table6, ID位置加上去，全加完再判斷文法，有錯的話看加幾個ID就減aT6&&error輸出


                                if( answer[i][x] == '1' && answer[i][x+2] == '1' && answer[i][x+3] == ')' ) {  //確認後面是;(1,1)，符合文法，+ table5的pointer
                                    table5pointer[row] = i+1;
                                    i = i + 1;
                                }
                                else {  // 後面不只有ID && ';'
                                    errorLine = i + 1;
                                    errorType = 2;
                                    aT6 = aT6 - 1;
                                    i = 100000; // 結束文法判斷
                                    //cout << "sss";
                                }
                            }
                            } //可能符合文法
                            else{   //後面不是加id，文法錯誤
                                errorLine = i + 1;
                                errorType = 3;

                                i = 100000; // 結束文法判斷
                            }  //後面不是加id，文法錯誤

                        }  //LABEL
                        else {  //不是LABEL，換下一個statement
                           /*
                            if( answer[i][3] == '1' && answer[i][4] == '1' && answer[i][5] == '\)') { // 是LABEL
                             // 是GTO // GTO  id ; //(2,11)(5, )(1,1)

                                if( answer[i][7] == '5' ) { // 可能符合文法，先算出是在table5哪行

                                    int row = 0;
                                    x = 12;

                                    if( answer[i][10] == ')' )    // 算出是在table5哪行
                                        row = answer[i][9] - '0';
                                    else{
                                        if( answer[i][11] == ')' ) {
                                            row = (answer[i][9] - '0' )*10;
                                            row = answer[i][10] - '0' + row;
                                            x = 13;
                                        }
                                        else{
                                            if( answer[i][12] == ')' ) {
                                                row = (answer[i][9] - '0' )*100;
                                                row = (answer[i][10] - '0' )*10 + row;
                                                row = answer[i][11] - '0' + row;
                                                x = 13;
                                            }
                                        }
                                    }
                                } // 算出是在table5哪行

                                table6[iT6][0] = '(';
                                table6[iT6][1] = '(';
                                table6[iT6][2] = '2';
                                table6[iT6][3] = ',';
                                table6[iT6][4] = '1';
                                jT6 = 5;

                                if( x == 12 )
                                    table6[iT6][4] = answer[i][9];
                                if( x == 13 ){
                                    table6[iT6][4] = answer[i][9];
                                    table6[iT6][5] = answer[i][10];
                                    jT6 = 6;
                                }
                                if( x == 14 ){
                                    table6[iT6][4] = answer[i][9];
                                    table6[iT6][5] = answer[i][10];
                                    table6[iT6][6] = answer[i][11];
                                    jT6 = 7;
                                }
                                //cout << table6[iT6][4]<< "ddd";
                                table6[iT6][jT6] = ')';
                                table6[iT6][jT6+1] = ' ';
                                table6[iT6][jT6+2] = ',';
                                table6[iT6][jT6+3] = ' \t';
                                table6[iT6][jT6+4] = ',';
                                table6[iT6][jT6+5] = ' \t';
                                table6[iT6][jT6+6] = ',';
                                table6[iT6][jT6+7] = ' \t';
                                table6[iT6][jT6+8] = ')';
                                table6[iT6][jT6+9] = ' \t';
                                table6[iT6][jT6+10] = table5identifier[row][0];
                                jT6 = jT6 + 11;
                                if( strlen(table5identifier[row]) > 1 ){
                                    table6[iT6][jT6] = table5identifier[row][1];
                                    jT6 = jT6 + 1;
                                }
                                if( strlen(table5identifier[row]) > 2 ){
                                    table6[iT6][jT6] = table5identifier[row][2];
                                    jT6 = jT6 + 1;
                                }
                                aT6 = iT6 ;
                                iT6 = iT6 + 1;
                                jT6 = 0;









                                } //  可能符合文法，先算出是在table5哪行
                                else{
                                    errorLine = i + 1;
                                    errorType = 3;

                                    i = 100000; // 結束文法判斷
                                }  //後面不是加id，文法錯誤

                            } // 是LABEL
*/
i = i + 1;
                        } //不是LABEL，換下一個statement

                    } //  不是DIMENSION ，換下一個statement
                }  //不是VARIABLE，換下一個statement
                }
           } //  table2的處理
        } // 每行最後是;

    } // while

} // work

void token(){
    int yP = 0;
    int xP = 0;
    int xT = 0;
    int yT = 1;
    bool right = false;

    while( yP <= aP ){   //yP <= aP

        while( xP < strlen( problem[yP] ) ){             //切第一條字串

            if( problem[yP][xP] == ' ' || problem[yP][xP] == '\t' || problem[yP][xP] == ';' || problem[yP][xP] == '        '  ){
                if( problem[yP][xP] == ';' ){
                    xP = 1000000;   //  problem換行
                    answer[yA][xA] = '(';
                    answer[yA][xA+1] = '1';
                    answer[yA][xA+2] = ',';
                    answer[yA][xA+3] = '1';
                    answer[yA][xA+4] = ')';
                    xA = xA + 5;
                }
                else
                    xP = xP + 1;

            }
            else{    // 如果不是空白，要開始找table位置
                sign = false;
                reg = xP;
                yT = 1;
                while( right == false && yT <= aT2 ){    // 找是不是在table2

                    if( check( problem[yP][xP], table2[yT][xT] ) ){ //如果第一個字母一樣，往後比看是不是同一個字串
                        right = true;

                        while( sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){

                            if( xT == strlen( table2[yT] ) ){   // 題目資料已超出table該框內容，看是不是標點符號
                                yT1 = 1;
                                right = false;
                                while( right == false && yT1 <= aT1 ){    // 找是不是在table1

                                    if( problem[yP][xP] == table1[yT1][0] ){ //如果第一個字母一樣，就正確
                                        right = true;
                                        sign = true;
                                    }
                                    else                                 //第一個字母不一樣，table要往下找
                                        yT1 = yT1 + 1;
                                }
                            }
                            else {
                                if( check( problem[yP][xP], table2[yT][xT] ) == false )
                                    right = false;

                            }
// cout<< problem[yP][xP-1] << "aaa" << problem[yP][xP] << "\n";
                           // if( problem[yP][xP] ='I' && problem[yP][xP+1] == ' ' )
                         //       right = false;

                            xP = xP + 1;
                            xT = xT + 1;
                        }

                        if( right == false ){
                            yT = yT + 1;
                            xT = 0;

                            //xP = reg;
                        }
                        else{

                            answer[yA][xA] = '(';
                            answer[yA][xA+1] = '2';
                            answer[yA][xA+2] = ',';
                            if( yT > 99 ){
                                answer[yA][xA+3] = yT/100+48;
                                answer[yA][xA+4] = (yT-100)/10+48;
                                answer[yA][xA+5] = (yT-100)%10+48;
                                answer[yA][xA+6] = ')';
                                xA = xA + 7;
                            }
                            else{
                                if( yT > 9 ){
                                    answer[yA][xA+3] = yT/10+48;
                                    answer[yA][xA+4] = yT%10+48;
                                    answer[yA][xA+5] = ')';
                                    xA = xA + 6;
                                }
                                else{
                                    answer[yA][xA+3] = yT+48;
                                    answer[yA][xA+4] = ')';
                                    xA = xA + 5;
                                }
                            }

                            if( sign == true ){
                                //xP = xP - 1;
                                answer[yA][xA] = '(';
                                answer[yA][xA+1] = '1';
                                answer[yA][xA+2] = ',';
                                if( yT1 > 99 ){
                                    answer[yA][xA+3] = yT1/100+48;
                                    answer[yA][xA+4] = (yT1-100)/10+48;
                                    answer[yA][xA+5] = (yT1-100)%10+48;
                                    answer[yA][xA+6] = ')';
                                    xA = xA + 7;
                                }
                                else{
                                    if( yT1 > 9 ){
                                        answer[yA][xA+3] = yT1/10+48;
                                        answer[yA][xA+4] = yT1%10+48;
                                        answer[yA][xA+5] = ')';
                                        xA = xA + 6;
                                    }
                                    else{
                                        answer[yA][xA+3] = yT1+48;
                                        answer[yA][xA+4] = ')';
                                        xA = xA + 5;
                                    }
                                }
                            }
                        }
                    }
                    else {                                //第一個字母不一樣，table要往下找
                        yT = yT + 1;
                        xT = 0;
                        xP = reg;
                    };
                }

                if( right == true ){
                    if( sign == true ){                             //此位置是table4標點符號
                        if( table1[yT1][0] == ';' )
                            xP = 1000000;   //  problem換行
                        else
                            xP = xP ;
                        sign = false;

                    }
                    else// 成功找到table位置了，往右找其他字串位置
                        xP = xP + 1;
                    yT = 1;
                    xT = 0;
                    right = false;

                }
                else{                                    //table2沒有，要找找看其他table
                    xP = reg;
                    yT = 1;
                    sign = false;
                    while( right == false && yT <= aT1 ){    // 找是不是在table1  (4改1)   找標點符號

                        if( problem[yP][xP] == table1[yT][xT] ){ //如果第一個字母一樣，就正確
                            right = true;

                            answer[yA][xA] = '(';
                            answer[yA][xA+1] = '1';
                            answer[yA][xA+2] = ',';
                            if( yT > 99 ){
                                answer[yA][xA+3] = yT/100+48;
                                answer[yA][xA+4] = (yT-100)/10+48;
                                answer[yA][xA+5] = (yT-100)%10+48;
                                answer[yA][xA+6] = ')';
                                xA = xA + 7;
                            }
                            else{
                                if( yT > 9 ){
                                    answer[yA][xA+3] = yT/10+48;
                                    answer[yA][xA+4] = yT%10+48;
                                    answer[yA][xA+5] = ')';
                                    xA = xA + 6;
                                }
                                else{
                                    answer[yA][xA+3] = yT+48;
                                    answer[yA][xA+4] = ')';
                                    xA = xA + 5;
                                }
                            }
                        }
                        else                                 //第一個字母不一樣，table要往下找
                                yT = yT + 1;
                    }

                    if( right == true ){                       // 成功找到table位置了，往右找其他字串位置
                        xP = xP + 1;
                        yT = 1;
                        xT = 0;
                        right = false;
                    }
// else xP = xP + 1;

                    else{                                    //table1沒有，要找找看其他table
                        // 先判斷是不是數字 table3 (6改3)
                        xP = reg;
                        yT = 1;
                        int ascii = 0;
                        char aReg[300];
                        int i = 0 ;
                        int checkInteger = 1;     // 判斷是純數字or小數
                        // 判斷有沒有可能是純數字
                        sign = false;
                        ascii = 0;
                        i = 0;
                        memset(aReg,'\0',sizeof(aReg));
                        if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' ) {  // 第一個是數字
                            right = true;

                            while(  sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){
//cout << xP<<"，" << strlen( problem[yP] ) << "，"<< problem[yP][xP] << "<<ssss\n";
                                if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' ){
                                    ascii = ascii + problem[yP][xP] ;
                                    aReg[i] = problem[yP][xP];

                                    i = i + 1;
                                }
                                else{  // 若不是數字，看有沒有可能是標點符號0

                                    if( problem[yP][xP] == '.' ){
                                        ascii = ascii + problem[yP][xP] ;
                                        aReg[i] = problem[yP][xP];
                                        checkInteger = 0;
                                        i = i + 1;

                                    }
                                    else {
                                        yT1 = 1;
                                        right = false;
                                        while( right == false && yT1 <= aT1 ){
 //cout <<  table1[yT1]<<"000"<< yT1<<"00" << problem[yP][xP]<< "\n";
                                            if( problem[yP][xP] == table1[yT1][0] ){ //如果第一個字母一樣，就正確
                                                right = true;
                                                sign = true;
                                            }
                                            else                                 //第一個字母不一樣，table要往下找
                                                yT1 = yT1 + 1;
                                        }
                                    }
                                }

                                xP = xP + 1;
                            }


                            if( right == true ){
                                ascii = ascii % 100;

                                if ( checkInteger == 1 ){
                                    while( table3[ascii][0] != '-' && strcmp( table3[ascii], aReg ) != 0 ){
                                        ascii = ascii + 1;

                                    }

                                    strcpy( table3[ascii], aReg );
                                }
                                else{
                                    while( table4[ascii][0] != '-' && strcmp( table4[ascii], aReg ) != 0 ){
                                        ascii = ascii + 1;

                                    }

                                    strcpy( table4[ascii], aReg );
                                }

                                answer[yA][xA] = '(';

                                if ( checkInteger == 1 )
                                    answer[yA][xA+1] = '3';
                                else
                                    answer[yA][xA+1] = '4';

                                answer[yA][xA+2] = ',';
                                if( ascii > 9 ){
                                    answer[yA][xA+3] = ascii/10+48;
                                    answer[yA][xA+4] = ascii%10+48;
                                    answer[yA][xA+5] = ')';
                                    xA = xA + 6;
                                    // cout << "++++++++++" << ascii << "EEEEEE\n";
                                }
                                else{
                                    answer[yA][xA+3] = ascii+48;
                                    answer[yA][xA+4] = ')';
                                    xA = xA + 5;

                                }
                            }
//cout << "++++++++++" << xA << "EEEEEE\n";
                            if( sign == true ){
                                answer[yA][xA] = '(';
                                answer[yA][xA+1] = '1';
                                answer[yA][xA+2] = ',';
                                if( yT1 > 99 ){
                                    answer[yA][xA+3] = yT1/100+48;
                                    answer[yA][xA+4] = (yT1-100)/10+48;
                                    answer[yA][xA+5] = (yT1-100)%10+48;
                                    answer[yA][xA+6] = ')';
                                    xA = xA + 7;
                                }
                                else{
                                    if( yT1 > 9 ){
                                        answer[yA][xA+3] = yT1/10+48;
                                        answer[yA][xA+4] = yT1%10+48;
                                        answer[yA][xA+5] = ')';
                                        xA = xA + 6;
                                    }
                                    else{
                                        answer[yA][xA+3] = yT1+48;
                                        answer[yA][xA+4] = ')';
                                        xA = xA + 5;
                                    }
                                }

                            }  // 若第一個為數字
                        }

                        if( right == true ){
                            if( sign == true ){            //此位置是table1標點符號
                                if( table1[yT1][0] == '.' )
                                        xP = 1000000;   //  problem換行
                                else
                                    xP = xP ;
                                sign = false;

                            }
                            else
                                xP = xP + 1;

                            yT1 = 1;
                            xT = 0;
                            right = false;
//cout <<
                        }
                        else{    // 不是table3 or table4 (數字) ， 直接放在table5
xP = reg;
                                memset(aReg,'\0',sizeof(aReg));
                                i = 0;
                                ascii = 0;
                                sign = false;
                                while(  sign == false && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){
                                    yT1 = 1;
                                    right = false;

                                    while( yT1 <= aT1 && sign == false ){

                                        if( problem[yP][xP] == table1[yT1][0] ) //如果第一個字母一樣，就正確
                                            sign = true;
                                        else                                 //第一個字母不一樣，table要往下找
                                            yT1 = yT1 + 1;

                                    }

                                    if( sign == false ){
                                        ascii = ascii + problem[yP][xP] ;
                                        aReg[i] = problem[yP][xP];
                                        i = i + 1;

                                    }

                                    xP = xP + 1;
                                }


                                ascii = ascii % 100;
                                while( table5identifier[ascii][0] != '-' && strcmp( table5identifier[ascii], aReg ) != 0  ){
                                    ascii = ascii + 1;
                                }

                                strcpy( table5identifier[ascii], aReg );

                                answer[yA][xA] = '(';
                                answer[yA][xA+1] = '5';
                                answer[yA][xA+2] = ',';
                                if( ascii > 9 ){
                                    answer[yA][xA+3] = ascii/10+48;
                                    answer[yA][xA+4] = ascii%10+48;
                                    answer[yA][xA+5] = ')';
                                    xA = xA + 6;
                                }
                                else{
                                    answer[yA][xA+3] = ascii+48;
                                    answer[yA][xA+4] = ')';
                                    xA = xA + 5;
                                }

                                if( sign == true ){

                                    answer[yA][xA] = '(';
                                    answer[yA][xA+1] = '1';
                                    answer[yA][xA+2] = ',';
                                    if( yT1 > 99 ){
                                        answer[yA][xA+3] = yT1/100+48;
                                        answer[yA][xA+4] = (yT1-100)/10+48;
                                        answer[yA][xA+5] = (yT1-100)%10+48;
                                        answer[yA][xA+6] = ')';
                                        xA = xA + 7;
                                    }
                                    else{
                                        if( yT1 > 9 ){
                                            answer[yA][xA+3] = yT1/10+48;
                                            answer[yA][xA+4] = yT1%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = yT1+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }

                                    }
                                }
                                else {
                                    xP = xP + 1;
                                    yT = 1;
                                    xT = 0;
                                    right = false;
                                    sign = false;
                                }
                        }

                    }

                }

            }//if 非空白

        }
//cout << answer[yP] << "\n" ;
        yP = yP + 1;
        yA = yA + 1;
        xP = 0;
        xA = 0;
        right = false;
        sign = false;

    }




}// token

bool readProblem() {                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf[255];
    int i = 0 ;

    cout << "請輸入檔名：" ;
    cin >> fileName ;                                           // input file name is fixed
    fileName = fileName + ".txt";

    inFile.open(fileName.c_str(), fstream::in);                 // open file to read

    if (!inFile.is_open()){                                  // unable to open file
        cout << endl << "### " << fileName << " does not exist! ###" << endl;
        return false;
    } // if
    else
        while (inFile.getline(rBuf, 255, '\n')){                // get each input record line by line
            strcpy( problem[i], ( rBuf ) );
            i = i + 1;
        }

    inFile.close();                                             // close file
    if (i == 0) {
        cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
        return false;
    } // if
    else
        aP = i - 1;

    return true;
}   // end readProblem


bool readTable() {                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf0[255];
    int i = 1 ;
    string tableName;
    tableName = "Table1.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf0, 255, '\n')){                // get each input record line by line
        if( i == 9 )
            table1[9][0] = '↑' ;
        else
            strcpy( table1[i], ( rBuf0 ) );
        i = i + 1;
    }
    aT1 = i - 1;
    inFile.close();                                             // close file

    char        rBuf2[255];
    i = 1 ;
    tableName = "Table2.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf2, 255, '\n')){                // get each input record line by line
        strcpy( table2[i], ( rBuf2 ) );
        i = i + 1;
    }

    aT2 = i - 1;
    inFile.close();

    char        rBuf3[255];
    i = 1 ;
    tableName = "DataType.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf3, 255, '\n')){                // get each input record line by line
        strcpy( dataType[i], ( rBuf3 ) );
        i = i + 1;
    }

    inFile.close();

}   // end readTable
