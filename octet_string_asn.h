#include "size_counter.h"
#include "exception.h"
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <cstdio>
#ifndef _OCTET_STRING_
#define _OCTET_STRING_
using namespace std;
class OctetStringASN{
private:
	string ASNsize(SizeCounter sc) const;
	string charToHex(char c) const;
	list<string> encodeToList() const;
	list<string> encodeToList(ifstream& file) const;
	istream_iterator<char> eos;
public:	
	void encode(string filename, string result_filename) const;
	void encode() const;
	OctetStringASN(){};
	~OctetStringASN(){};
};

#endif
void OctetStringASN::encode() const{
	list<string> res;
	try{
		res = this->encodeToList();
	} catch(Exception err){
		throw err;
	}
	for(auto iter = res.begin(); iter != res.end(); iter++){
		cout << *iter;
	}
	cout << '\n';
}
void OctetStringASN::encode(string filename, string result_filename) const {
	ifstream file;
	file.open(filename);
	if(!file){
		throw Exception("No such file exsist\n");
	}
	list<string> res;
	try {
		res = this->encodeToList(file);
	} catch(Exception err){
		file.close();
		throw err;
	}
	file.close();
	ofstream res_file(result_filename);
	for(auto iter = res.begin(); iter != res.end(); iter++){
		res_file << *iter;
	}
	res_file.close();
}
list<string> OctetStringASN::encodeToList() const{
	list<string> result;
	SizeCounter sc;
	for(char c; (c = getchar()) != '\n';){
		result.push_back(this->charToHex(c));
		try{
			sc.increment();
		} catch (Exception& err){
			throw err;
		}
	}
	try {
		result.push_front(this->ASNsize(sc));
	} catch (Exception& err){
		throw err;
	}
	if(sc.isEmpty()){
		throw Exception("No data\n");
	}
	result.push_front("04");
	return result;
}
list<string> OctetStringASN::encodeToList(ifstream& file) const{
	list<string> result;
	SizeCounter sc;
	char c;
	file >> noskipws;
	while((file >> c) && (!file.eof())){
		result.push_back(this->charToHex(c));
		try{
			sc.increment();
		} catch (Exception& err){
			throw err;
		}
	}
	try {
		result.push_front(this->ASNsize(sc));
	} catch (Exception& err){
		throw err;
	}
	if(sc.isEmpty()){
		throw Exception("File is Empty\n");
	}
	result.push_front("04");
	return result;
}
string OctetStringASN::ASNsize(SizeCounter sc) const{
	string asn_format_size = "";
	string size = sc.getSize();
	if(sc.isShortForm()){
		asn_format_size += size;
	}else{
		try{
			asn_format_size += sc.blocksToASN();
		} catch (Exception& err){
		throw err;
	}
		asn_format_size += size;
	}
	return asn_format_size;
}
string OctetStringASN::charToHex(char c) const{
	short bytes = (short)c;
	string str = "";
	str += hex_format[bytes / 16];
	str += hex_format[bytes % 16];
	return str;
}
