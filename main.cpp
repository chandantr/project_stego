/*
************************************************************************************

		CS 293 Course Project  (Steganography)

		Chandan Kumar (100050059)
		
*************************************************************************************  */

#include<iostream>
#include"stego.h"

using namespace std;


int main(int argc,char **argv){
	int type;
	Servant myservant;
	cout<<"i) For encrypting an image into another image, enter '1' ;\nii) For decrypting an image, enter '2' ;"<<endl;
	cout<<"iii) For encrypting text into an image, enter '3';\niv)  For decrypting text message from an image, enter '4' :  "<<endl;
	cin>>type;
	
	//for option (i).
	if(type==1){
		myservant.imageEncrypter(argc,argv);		
	}
	  
	  //for option (ii).
	else if(type==2){
		myservant.imageDecrypter(argc,argv);		
  	}
  	
  	//for option (iii).
  	else if(type==3){
		myservant.textEncrypter(argc,argv);  	
   	}
  	
  	//for option (iv)
  	else {
 		myservant.textDecrypter(argc,argv);  	
	}
	return 0;
}
