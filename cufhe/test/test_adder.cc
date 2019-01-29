/**
 * Copyright 2018 Wei Dai <wdai3141@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// Include these two files for CPU computing.

#include <iostream>
using namespace std;
#include <chrono>
#include <ctime>
#include <ratio>

void Fa(int& z, int& co, int& a, int& b, int& ci) {
  int t0, t1, t2;

  t0 = a ^ b;
  t1 =  a & b;
  t2 = ci & t0;
  z = ci^ t0;
  co = t1 | t2;

  // cout << "FA z:"<< flush;
  // cout<< z << flush;
  // cout<< "  co: " << flush;
  // cout<< co << endl;
}

void Ha(int& z, int& co, int& a, int& b) {
  z = a ^ b;
  co = a & b;
}

void Rca(int* z, int* co, int* a, int* b, int n) {
  Ha(z[0], co[0], a[0], b[0]);

  for (int i = 1; i < n; i++) {
    Fa(z[i], co[i], a[i], b[i], co[i-1]);
  }
}

void Mux(int* out, int* inp1, int* inp2, int sel, int n){
  int p0[n];
  int p1[n];
  int is;

  is = !sel;
  for (int i = 0; i < n; i++) {
    p0[i] = inp1[i] * is;
    p1[i] = inp2[i] * sel;
  }

  for (int i = 0; i < n; i++) {
    out[i] = p1[i] + p0[i];
  }
}

 void RCS(int* z, int* co, int* a, int* b, int n){
    int tempb[n];
  for(int i =0; i<n; i++){
    tempb[i] = b[i] ^ 1;
  }
  int one[n];
  one[0] = 1;
  for(int i =1; i<n; i++){
    one[i] = 0;

  }
  int finalb [n];
  Rca(finalb,co, one, tempb ,n);
  Rca(z,co,a,finalb,n);
}

void sixteenMux(int* out, int in[][10], int* sel, int size, int n){
  int out1[n/2][size];
  int out2[n/4][size];
  int out3[n/8][size];

cout <<"sixteen Mux in 0-16 is: "<<endl;
//  for(int i =0; i < 16; i ++){
//  cout<< in[i][i] << endl;
//  }
 cout<< *in[1] << endl;
cout <<"Mux 1: "<<endl;
for(int i=0; i< ((n/2)); i++){
    Mux(out1[i], in[i], in[(n/2) + i], sel[0], size);
    cout<< out1[i][i] <<endl;
}
  cout<<"done one"<<endl;
for(int i=0; i< ((n/4)); i++){
   Mux(out2[i], out1[i], out1[(n/4) + i], sel[1], size);
 }
for(int i=0; i< ((n/8)); i++){
  Mux(out3[i], out2[i], out2[(n/8) + i], sel[2], size);
}
for(int i=0; i< ((n/16)); i++){
  Mux(out, out3[0], out3[(n/16  + i)], sel[3], size);
}
}

void floatAdder(int* out, int* in1, int* in2, int* test){
    int in1exp[5];
    int in2exp[5];
    int in1mantisaR[13];
    int in2mantisaR[13];
    int negcheck = 1;
    int co[5] = {0,0,0,0,0};

    int tempexpo[5];
    int smallIn[16];
    int bigIn[16];
    int smallInman[13];
    int bigInman[13];
    int exposum[5];

  for(int i = 0; i < 5; i++){
    in1exp[i] = in1[10+i];
    in2exp[i] = in2[10+i];
  }
  for(int i = 0; i< 10; i++){             
    in1mantisaR[i+3] = in1[i];  //leae last 3 bits for the round bits
    in2mantisaR[i+3] = in2[i];
  }
  cout<< "exposum is:" << endl;
  RCS(exposum,co, in1exp, in2exp, 5);
  for(int i=0;i<5;i++){
    cout<<exposum[i]<<flush;
  }
  cout<<""<<endl;
  //check if negative to determine which exponent is larger
  negcheck = exposum[4] & negcheck;
  cout<< negcheck << endl;
//if "negcheck" is positive, then input2 is larger, else input1 larger. 
  Mux(tempexpo, in1exp, in2exp, negcheck, 5);       //make tempexpo into whichever exponent is higher 
  Mux(smallIn, in2, in1,  negcheck, 16);            //chosing which input is the "smaller" one , aka the one with smaller input
  Mux(bigIn, in1, in2, negcheck, 16);            // which input is bigger
  Mux(smallInman, in2mantisaR, in1mantisaR, negcheck, 13); //chose the smaller mantisa
  Mux(bigInman, in1mantisaR, in2mantisaR,  negcheck, 13);    //chose the larger mantisa

  //testing line
  for(int i =0; i< 13; i++){
     test[i] = bigInman[i];
  }
 
}
int main() {
 int inp1[16] = {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
 int inp2[16] = {1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0};
 int out[16];
 int test[13];
 // cout<< "Testing Part 1" << endl;
 // floatAdder(out, inp1, inp2, test);
 // cout<<"big mantisa is:" <<endl;
 //  for(int i = 0; i < 13; i ++){
 //   cout << test[i] << flush;
 // }
 return 0;
}
