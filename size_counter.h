#ifndef _SIZE_COUNTER_
#define _SIZE_COUNTER_
#include "exception.h"
#include <vector>
#include <iterator>
#include <string>
using namespace std;
char hex_format[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

class SizeCounter {
private:
	vector<char> blocs_size_in_hex;
	vector<char> size_in_hex;
	static const bool DELETE = true;
	static const bool NOT_DELETE = false;
	bool increment(vector<char>& size);
	void createNewBlock(vector<char>& size);
	bool nextInHex(char& cs);
	string reverse(vector<char> size, bool delete_zero_in_start) const;
	bool sizeIsMore127() const;
	bool blockSizeIsOne() const;
public:
	SizeCounter(){		
		this->size_in_hex = {'0', '0'};
		this->blocs_size_in_hex = {'1', '0'};
	}
	bool isShortForm() const;
	bool isEmpty() const {
		if(size_in_hex.size() == 2 && size_in_hex[0] == '0' && size_in_hex[1] == '0')
			return true;
		return false;
	}
	string blocksToASN();
	void increment();
	string getSize() const {return this->reverse(this->size_in_hex, this->NOT_DELETE);}
	string getBlocksSize() const{return this->reverse(this->blocs_size_in_hex, this->DELETE);}
	~SizeCounter(){this->blocs_size_in_hex.clear(); this->size_in_hex.clear();}
};
#endif
string SizeCounter::reverse(vector<char> size, bool delete_zero_in_start) const{
	vector<char>::iterator iter = size.end()-1;
	if(delete_zero_in_start == true && *iter == '0'){
		iter--;
	}
	string res = "";
	while(iter >= size.begin()){
		res+=(*iter--);
	}
	return res;
}
void SizeCounter::increment(){
	if(this->increment(this->size_in_hex) == true){
		this->increment(this->blocs_size_in_hex);
		if(this->blocs_size_in_hex.size() > 2){
			throw Exception("Too much data\nYour data is more 7F bytes\n");
		}
	}
}
string SizeCounter::blocksToASN(){
	switch(blocs_size_in_hex[1]){
		case '7':
			blocs_size_in_hex[1] = 'F';
			break;
		case '6':
			blocs_size_in_hex[1] = 'E';
			break;
		case '5':
			blocs_size_in_hex[1] = 'D';
			break;
		case '4':
			blocs_size_in_hex[1] = 'C';
			break;
		case '3':
			blocs_size_in_hex[1] = 'B';
			break;
		case '2':
			blocs_size_in_hex[1] = 'A';
			break;
		case '1':
			blocs_size_in_hex[1] = '9';
			break;
		case '0':
			blocs_size_in_hex[1] = '8';
			break;
		default:
			throw Exception("Too much data\nYour data is more 7F bytes\n");
			break;
	}
	return this->reverse(this->blocs_size_in_hex, this->NOT_DELETE);
}
bool SizeCounter::isShortForm() const {
	if(this->blockSizeIsOne() && !this->sizeIsMore127())
		return true;
	else
		return false;
}

bool SizeCounter::blockSizeIsOne() const {
	if(this->blocs_size_in_hex.size() > 2){
		return false;
	}
	if(this->blocs_size_in_hex[0] == '1' && this->blocs_size_in_hex.size() <= 2){
		return true;
	}
	return false;
}
bool SizeCounter::sizeIsMore127() const {
	if(this->size_in_hex.size() > 2){
		return false;
	}
	switch(size_in_hex[1]){
		case '8':
			return true;
		case '9':
			return true;
	}
	if(size_in_hex[1] >= 'A' && size_in_hex[1] <= 'F'){
		return true;
	}
	return false;
}
void SizeCounter::createNewBlock(vector<char>& size){
	size.push_back('0');
	size.push_back('0');
}
bool SizeCounter::nextInHex(char& c){
	for(int i = 0; i < 16; i++){
		if(c == hex_format[i]){
			c = hex_format[(++i)%16];
			return (i == 16) ? false : true;
		}
	}
	return false;
}
bool SizeCounter::increment(vector<char>& size){
	bool stop_flag = false;
	bool create_flag = false;
	vector<char>::iterator iter = size.begin();
	while(stop_flag == false){
		stop_flag = this->nextInHex(*iter);
		if(iter+1 == size.end() && stop_flag == false){
			create_flag = true;		
			this->createNewBlock(size);
			iter = size.end()-2;
		} else {
			iter++;
		}
	}
	return create_flag;
}