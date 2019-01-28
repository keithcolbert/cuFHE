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


void sixteenMux(int* out, int in[][13], int* sel, int size, int n) {
  int out1[n/2][size];
  int out2[n/4][size];
  int out3[n/8][size];

  cout <<"sixteen Mux in 0-16 is: "<<endl;

  for(int i=0; i < ((n/2)); i++) {
  	Mux(out1[i], in[i], in[(n/2) + i], sel[3], size);
  }

  for(int i=0; i < ((n/4)); i++) {
  	Mux(out2[i], out1[i], out1[(n/4) + i], sel[2], size);
  }

  for(int i=0; i < ((n/8)); i++) {
  	Mux(out3[i], out2[i], out2[(n/8) + i], sel[1], size);
  }

  for(int i=0; i < ((n/16)); i++) {
  	Mux(out, out3[0], out3[(n/16 + i)], sel[0], size);
  }
}


//updated shift 
void Shift(int smallout[], int smallin[], int smallzero, int n, int nshift) {
	int temp = (n - 1) - nshift;
	for(int i = 0; i <= temp; i++) {
		smallout[i] = smallin[nshift + i];
	}
	for(int i = (temp + 1); i < 13; i++) {
		smallout[i] = smallzero;
	}
}

void totalShift(int o[], int smallI[]){
	int smallO[16][13];
	int sel[4] = {0,1,0,0};

	for(int i = 0; i <= 12; i++) {
		if(i < 3){
			Shift(smallO[i], smallI, 0, 13, i);
		}
		else {
			Shift(smallO[i], smallI, 0 ,13, i);
			smallO[i][2] = smallI[i-1];
			smallO[i][1] = smallI[i-2];
			smallO[i][0] = smallI[i-3];
			smallO[i][0] = smallO[i][0] + smallO[i-1][0];
		}
	}

	for (int i = 13; i < 16; i++){
		for (int z = 0; z < 13; z++) {
			smallO[i][z] = 0;
		}
	}

	sixteenMux(o, smallO, sel, 13, 16);

}



int main() {
	int inp[13] = {0,0,1,0,0,0,0,0,0,1,0,0,1};
	int outp[13];
	int out2p[13];

	Shift(out2p, inp, 0, 13, 2);
	for(int i = 12; i >= 0; i--){
		cout << out2p[i] << flush;
	}
	cout << endl;

	totalShift(outp, inp);
	for(int i = 12; i >= 0; i--){
		cout << outp[i] << flush;
	}
	cout << endl;

	return 0;
}
