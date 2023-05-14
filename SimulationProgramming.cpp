

#include <iostream>
#include <string>
#include<stdio.h>
#include<iomanip>

using namespace std;

struct adder {
	int Cout = 0;
	int result = 0;
};

adder ALU1bit(int a, int b, int Binv, int Cin, string OP);
void bitAdder16(int a[], int b[], int Binv, string OP, int result[]);
void BoothMul(int md[], int mq[]);

int main() {
	string md, mq;
	int MD[16];
	int MQ[16];
	cout << "Plese Enter MD: ";
	getline(cin, md);
	cout << "Please Enter MQ: ";
	getline(cin, mq);
	
	//converting string to int array 
	for (int i = 0; i < 16; i++) {
		if (md[i] == '0') {
			MD[i] = 0;
		}
		else{
			MD[i] = 1;
		}
		if (mq[i] == '0') {
			MQ[i] = 0;
		}
		else {
			MQ[i] = 1;
		}

	}
	cout << endl;
	BoothMul(MD, MQ);
	cout << endl;

	system("pause");
	return 0;
}

adder ALU1bit(int a, int b, int Binv, int Cin, string OP) {
	adder result;

	//if subtracting negate b and perform addition operation
	if (Binv == 1 && OP == "10") {
		if (b == 0) {
			b = 1;
		}
		else if (b == 1) {
			b = 0;
		}
	}
	//addition operation
	if (a == 1 && b == 1 && Cin == 1) {
		result.result = 1;
		result.Cout = 1;
	}
	else if (((a == 1 || b == 1) && Cin == 1) || ((a == 1 && b == 1) && Cin == 0)) {
		result.result = 0;
		result.Cout = 1;
	}
	else if ((a == 1 || b == 1) || Cin == 1) {
		result.result = 1;
		result.Cout = 0;
	}
	else {
		result.result = 0;
		result.Cout = 0;
	}
	return result;

}

//16 bit adder
void bitAdder16(int a[], int b[], int Binv, string OP, int result[]) {
	
	adder result16;
	int Cin = Binv;
	if (Binv == 0) {
		for (int i = 15; i >= 0; i--) {
			result16 = ALU1bit(a[i], b[i], 0, result16.Cout, OP);
			result[i] = result16.result;
		}
	}
	else {
		result16.Cout = 1;
		for (int i = 15; i >= 0; i--) {
			result16 = ALU1bit(a[i], b[i], 1, result16.Cout, OP);
			result[i] = result16.result;
		}
	}
}

void BoothMul(int md[], int mq[]) {
	
	int cycleCounter[4];
	int AC[16] = {};
	adder result;
	int mqMinus1 = 0;
	int op[2] = { mq[15] , mqMinus1 };

	cout << "CycleCounter" << setw(10) << "MD" << setw(20) << "AC" << setw(20) << "MQ" << setw(23) << "MQ - 1" << endl;
	cout << "-----------------------------------------------------------------------------------------------------" << endl;

	for (int i = 15; i >= 0; i--) {
		op[0] = mq[15];
		op[1] = mqMinus1;

		int temp = i;
		for (int j = 3; j >= 0; j--) {
			if (temp % 2 == 1) {
				cycleCounter[j] = 1;
			}
			else {
				cycleCounter[j] = 0;
			}
			temp = temp / 2;
		}

		//Booth's Multiplication
		if (op[0] == 0 && op[1] == 1) {
			bitAdder16(AC, md, 0, "10", AC);
		}
		else if (op[0] == 1 && op[1] == 0) {
			bitAdder16(AC, md, 1, "10", AC);
		}

		//print before shift (sar)
		for (int k = 0; k < 4; k++) {
			cout << cycleCounter[k];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << md[z];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << AC[z];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << mq[z];
		}
		cout << "\t" << mqMinus1 << endl;

		//Shift right (sar)
		mqMinus1 = mq[15];
		for (int i = 15; i >= 1; i--) {
			mq[i] = mq[i - 1];
		}
		mq[0] = AC[15];

		for (int i = 15; i >= 1; i--) {
			AC[i] = AC[i - 1];
		}

		//print after shift(sar)
		for (int k = 0; k < 4; k++) {
			cout << cycleCounter[k];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << md[z];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << AC[z];
		}
		cout << "\t";
		for (int z = 0; z < 16; z++) {
			cout << mq[z];
		}
		cout << "\t" << mqMinus1 << endl;
	}
}