


#include<iostream>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<fstream>
#include<string>
#include<iomanip>

using namespace std;

int BtoD(string);
string DtoB(int);
bool mhs(string);	// returns true if hit, returns false if miss
bool lw(string, int);
bool sw(string, int);
void init_data();
void display();

struct Block{

	int hist;	//history
	bool v;		//valid
	string tag;
	string data;
};

struct Set{

	Block block0;
	Block block1;
};

vector<string>reg;	// data construtor for register
vector<Set>cache;	// data construtor for cache
vector<string>mem;	// data construtor for memory

int main()
{
	string instruction;
	ifstream input;
	input.open("input.txt", ios::in);

	if (input.is_open()){

		init_data();
		cout << "Instruction" << setw(60) << "Cache-Access" << endl;
		while (!input.eof()){

			input >> instruction;
			input.get();
			cout << instruction << setw(30) << (mhs(instruction) == true ? "hit" : "miss") << endl;
			if (input.peek() == EOF)
				break;
		}
		cout << endl;
		display();
		input.close();
	}
	else{
		cout << "fail to open file." << endl;
	}
	return 0;
}

string DtoB(int num){
	string res;

	while (num != 0){
		int temp = num % 2;
		res.push_back(temp + '0');
		num /= 2;
	}

	while (res.length() < 7) {
		res.push_back('0');
	}

	for (int i = 0; i < res.length() / 2; i++){
		swap(res[i], res[res.length() - 1 - i]);
	}
	return res;
}

int BtoD(string num){

	int len = num.length() - 1;
	int power = 0;
	int sum = 0;
	int temp;

	while (len >= 0){

		temp = num[len--] == '1' ? pow(2, power) : 0;
		power++;
		sum += temp;
	}
	return sum;
}
void init_data(){

	string zeros = "00000000000000000000000000000000";
	int num = 5;	// data in main memoroy starts from 5
	Block bl;
	Set st;
	bl.hist = 0;
	bl.data = zeros;
	bl.tag = "0000";
	bl.v = 0;
	st.block0 = st.block1 = bl;

	for (int i = 0; i < 128; i++){

		string temp = DtoB(num);
		mem.push_back(zeros.substr(0, zeros.length() - temp.length()) + temp);
		num++;
	}

	for (int i = 0; i < 8; i++){
		reg.push_back(zeros);
		cache.push_back(st);
	}
}
bool mhs(string ins){

	string addr = DtoB(BtoD(ins.substr(16, ins.length() - 16)) / 4);
	int rt = BtoD(ins.substr(13, 3));
	if (ins.substr(0, 6) == "100011") {
		return lw(addr, rt);
	}
	else{
		return sw(addr, rt);
	}
	return 1;
}

bool lw(string addr, int rt){

	int index = BtoD(addr) % 8;
	string tag = addr.substr(0, 4);
	bool hit = false;
	if (cache[index].block0.tag == tag){

		if (cache[index].block0.v){
			hit = true;
			reg[rt] = cache[index].block0.data;
			cache[index].block0.hist = 1;
			cache[index].block1.hist = 0;
		}
	}
	else if (cache[index].block1.tag == tag){

		if (cache[index].block1.v){
			hit = true;
			reg[rt] = cache[index].block1.data;
			cache[index].block1.hist = 1;
			cache[index].block0.hist = 0;
		}

	}
	if (!hit){

		string data = mem[BtoD(addr)];
		reg[rt] = data;
		if (!cache[index].block0.v){

			cache[index].block0.v = true;
			cache[index].block0.tag = tag;
			cache[index].block0.data = data;
			cache[index].block0.hist = 1;
			cache[index].block1.hist = 0;
		}
		else if (!cache[index].block1.v){

			cache[index].block1.v = true;
			cache[index].block1.tag = tag;
			cache[index].block1.data = data;
			cache[index].block1.hist = 1;
			cache[index].block0.hist = 0;
		}
		else{
			if (!cache[index].block0.hist){

				cache[index].block0.v = true;
				cache[index].block0.tag = tag;
				cache[index].block0.data = data;
				cache[index].block0.hist = 1;
				cache[index].block1.hist = 0;
			}
			else{

				cache[index].block1.v = true;
				cache[index].block1.tag = tag;
				cache[index].block1.data = data;
				cache[index].block1.hist = 1;
				cache[index].block0.hist = 0;
			}
		}
	}
	return hit;
}
bool sw(string addr, int rt){

	int index = BtoD(addr) % 8;
	string tag = addr.substr(0, 4);
	bool hit = true;

	if (cache[index].block0.tag == tag and cache[index].block0.v){

		cache[index].block0.data = reg[rt];
		cache[index].block0.v = true;
		cache[index].block0.hist = 1;
		cache[index].block1.hist = 0;
	}
	else if (cache[index].block1.tag == tag and cache[index].block1.v){

		cache[index].block1.data = reg[rt];
		cache[index].block1.v = true;
		cache[index].block0.hist = 0;
		cache[index].block1.hist = 1;
	}
	else{
		mem[BtoD(addr)] = reg[rt];
		hit = false;
	}
	return hit;
}

void display(){

	string letterS = "s";
	int i;

	cout << "Register File" << endl;
	for (int i = 0; i < reg.size(); i++){
		cout << (letterS + to_string(i)) << "     " << reg[i] << endl;
	}

	cout << endl;
	cout << setw(30) << "Cache" << endl;
	cout << "block0" << setw(56) << "block1" << endl;
	cout << "set" << setw(8) << "valid" << setw(6) << "tag" << setw(8) << "data" << setw(36) << "valid" << setw(8) << "tag" << setw(8) << "data" << endl;
	
	for (int i = 0; i < cache.size(); i++){
		cout << i << setw(6) << cache[i].block0.v << setw(11) << cache[i].block0.tag << "   " << cache[i].block0.data << setw(4) << cache[i].block1.v << setw(13) << cache[i].block1.tag << "   " << cache[i].block1.data << endl;
	}

	cout << endl;
	cout << setw(32) << "Main Memory" << endl;
	cout << "Address" << setw(14) << "Data" << setw(32) << "Address" << setw(14) << "Data" << endl;
	
	for (i = 0; i <= 63; i++){
		cout << i << (i >= 10 ? "      " : "       ") << mem[i] << (i >= 100 ? "       " : "      ") << i + 64 << (i >= 36 ? "      " : "       ") << mem[i + 64] << endl;
	}
}