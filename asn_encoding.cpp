#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>
#include "octet_string_asn.h"

string Message = "[X] Enter source filename and result filename for encodin data from file to ASN foramat\n[X] Or input '-' for use input/output stream\n";

int main(int argc, char const *argv[])
{
	if(argc == 3){
		OctetStringASN os;
		try{
			os.encode(argv[1], argv[2]);
			cout << "[+]File data was successfully encoded\n";
		} catch(Exception err){
			cout << err.getMessage();
			return -1;
		}
	} else if(argc == 2){
		if(string(argv[1]) == "-"){
			OctetStringASN os;
			printf("Input data: ");
			try{
				os.encode();
			} catch(Exception err){
				cout << err.getMessage();
				return -1;
			}
		} else {
			cout << Message;
		}
	} else {
		cout << Message;
	}
	return 0;
}