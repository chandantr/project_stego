/*
************************************************************************************

		CS 293 Course Project  (Steganography)

		Chandan Kumar (100050059)
		
*************************************************************************************  */

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include"CImg.h"
#include"stego.h"
using namespace std;

//function for combining four higher bits from cover image's pixel value and four from secret image's pixel value.
unsigned char Servant::encrypt(const unsigned char& source_pixel,const unsigned char& secret_pixel){
	return ((16*((int)source_pixel/16))+((int)secret_pixel/16));
}


//function for deriving the secret image's pixel value from the encrypted image
unsigned char Servant::decrypt_secret(const unsigned char& coded_pixel){
	return (16*((int)coded_pixel%16));
}

//This function hides a character's ASCII value into R,G,B values of a pixel in the cover image.
//The new R,G,B values are returned in a vector.
vector<unsigned char> Servant::encryptText(const unsigned char& r,const unsigned char& g,const unsigned char& b,unsigned char secret){
	vector<unsigned char> vec;
	unsigned char newpixel=((int)r/8)*8+((int)secret/16);
	secret=secret%16;
	vec.push_back(newpixel);
	newpixel=((int)g/4)*4+((int)secret/4);
	secret=secret%4;
	vec.push_back(newpixel);
	newpixel=((int)b/4)*4+((int)secret);
	vec.push_back(newpixel);
	return vec;
}

//This function extracts and returns the hidden character's ASCII value from the RGB values of a pixel in the encrypted image.
unsigned char Servant::decryptText(const unsigned char& r,const unsigned char& g,const unsigned char& b){
	return ((r%8)*16+(g%4)*4+(b%4));
}

void Servant::imageEncrypter(int argc,char **argv){
	//initialization and input section.
		vector<unsigned char> temp;
		char coverImage[40],secretImage[40],newImage[40];
		string passwd;
		cout<<"Give cover image name :   ";
		cin>>coverImage;
		cout<<"Give secret image name :   ";
		cin>>secretImage;
		cout<<"Encrypted image name  (to be created):   ";
		cin>>newImage;
		cout<<"Enter password :  ";
  		cin>>passwd;
  		
		//file pointers created
		const char *file_i1 = cimg_option("-i1",coverImage,"Cover Image ");
		const char *file_i2 = cimg_option("-i2",secretImage,"Secret Image ");
		const char *file_o  = cimg_option("-o",newImage,"Output Image");
		const bool visu     = cimg_option("-visu",true,"Visualization mode");   //for displaying the result image after encryption.
		cimg_library::CImg<unsigned char> img1(file_i1), img2(file_i2);           
		
		//Resizing the two images to same values.
		if (!img2.is_sameXYZC(img1)){
		    int
      			dx = cimg_library::cimg::max(img1.width(),img2.width()),
      			dy = cimg_library::cimg::max(img1.height(),img2.height()),
     			dz = cimg_library::cimg::max(img1.depth(),img2.depth()),
      			dv = cimg_library::cimg::max(img1.spectrum(),img2.spectrum());
    		img1.resize(dx,dy,dz,dv,3);
    		img2.resize(dx,dy,dz,dv,3);
  		}
  		
  		cimg_library::CImg<unsigned char> dest(img1);      //for new image to be formed.
  		
  		//Encryption being performed.
  		cimg_forXY(dest,x,y) {
		    dest(x,y,0)=(unsigned char)encrypt(img1(x,y,0),img2(x,y,0));
		    dest(x,y,1)=(unsigned char)encrypt(img1(x,y,1),img2(x,y,1));
		    dest(x,y,2)=(unsigned char)encrypt(img1(x,y,2),img2(x,y,2));
		}
		dest(0,0,0)=(unsigned char)passwd.length();		//will be used while decryption.
		
		//Password stored into the new image.
		for(int x=0,y=1;y<=passwd.length();y++){
			dest(x,y,0)=passwd[y-1];
		}

		//Final part: save, display final image formed and exit.
		if (file_o) dest.save(file_o);
	  	if (visu) dest.display("Encrypted Image");
}

void Servant::imageDecrypter(int argc,char **argv){
	//initialization and input section.
	  	char sourceImage[40],originalCoverImage[40];
	  	string passwd;
	  	int ctr=1,n;
		vector<unsigned char> temp;
  		cout<<"Encrypted image name  :   ";
 		cin>>sourceImage;
  		char secretImage[40];
  		cout<<"Secret image name (to be created) :   ";
  		cin>>secretImage;
  		cout<<"Enter password :   ";
  		cin>>passwd;
  		const char *file_i1 = cimg_option("-i1",sourceImage,"Source Image ");
 		const char *file_o1  = cimg_option("-o1",secretImage,"Secret Image");
		const bool visu = cimg_option("-visu",true,"Visualization mode");
  		cimg_library::CImg<unsigned char> img1(file_i1);
  		cimg_library::CImg<unsigned char> dest1(img1) , dest2(img1);
  		  		
  		n=img1(0,0,0);		//n stores the correct password's length.
  		
  		//we first ensure that lengths of correct and input passwords are equal.
  		if(n!=passwd.length()){				
  			cout<<"Error: wrong password \n";
  			return;
  		}
  		//Password check before decryption.
  		while(ctr<=passwd.length()){
  			if(img1(0,ctr,0)!=passwd[ctr-1]){
  				cout<<"Error: wrong password \n";
  				return;
  			}
  			ctr++;
  		}
  		
  		//Decryption being done.
  		cimg_forXY(dest1,x,y) {
    			dest1(x,y,0)=(unsigned char)decrypt_secret(img1(x,y,0));
    			dest1(x,y,1)=(unsigned char)decrypt_secret(img1(x,y,1));
    			dest1(x,y,2)=(unsigned char)decrypt_secret(img1(x,y,2));
  		}
  		//save and display the secret image and exit.
  		if (file_o1) dest1.save(file_o1);
  		if (visu) dest1.display("Secret Image");
}

void Servant::textEncrypter(int argc,char **argv){
	//initialization and input section.
  		char coverImage[40],textFile[100],newImage[100];
  		string passwd;
		ifstream inFile;
		vector<unsigned char> temp;
		cout<<"Give cover image name  :   ";
		cin>>coverImage;
		cout<<"Give secret text file name  :    ";
		cin>>textFile;
		cout<<"Give encrypted image name (to be created) :   ";
		cin>>newImage;
		cout<<"Enter a password :  ";
  		cin>>passwd;
		const char *file_i1 = cimg_option("-i1",coverImage,"Cover Image ");
		const char *file_o  = cimg_option("-o",newImage,"Output Image");
		const bool visu     = cimg_option("-visu",true,"Visualization mode");
		inFile.open(textFile);
		if (!inFile.is_open()){
			cout << "Error opening file\n";
			exit (1);
		}
		cimg_library::CImg<unsigned char> img1(file_i1);	
		cimg_library::CImg<unsigned char> dest(img1);
		vector<unsigned char> v;
		int n=passwd.length(),flag=1;		
		dest(0,0,0)=(unsigned char)n;		//will be used while decryption.
		
		//Password stored into the new image's first row of pixels.
		for(int x=1,y=0;x<=n;x++){
			dest(x,y,0)=passwd[x-1];
		}
		//Encryption being performed(in the subsequent rows of pixels.
    	char c=inFile.get();
    	cimg_forXY(dest,x,y){
    		if(y!=0){
    			if(!inFile.eof()){	
    				v=encryptText(img1(x,y,0),img1(x,y,1),img1(x,y,2),c);
    				dest(x,y,0)=v[0];
    				dest(x,y,1)=v[1];
    				dest(x,y,2)=v[2];
    				v.clear();
    				c=inFile.get();
    			}
    			else if(flag==1){
    				v=encryptText(img1(x,y,0),img1(x,y,1),img1(x,y,2),0);
    				dest(x,y,0)=v[0];
    				dest(x,y,1)=v[1];
    				dest(x,y,2)=v[2];
    				v.clear();
    				flag++;
    			}
			}
		}
		inFile.close();
		// Save,display final image and exit
		if (file_o) dest.save(file_o);
		if (visu) dest.display("Encrypted Image");

}

void Servant::textDecrypter(int argc,char **argv){
		//initialization and input section
  		char encryptedImage[100],secretTextFile[100];
  		string passwd;
  		int n,ctr=1,flag=1;
		ofstream outFile;
  		cout<<"encrypted image  :   " ;
  		cin>>encryptedImage;
		cout<<"Secret text file name (to be created) :    ";
		cin>>secretTextFile;
		cout<<"Enter password :  ";
		cin>>passwd;
		vector<unsigned char> temp;
  		const char *file_i1 = cimg_option("-i1",encryptedImage,"Source Image ");
		cimg_library::CImg<unsigned char> img1(file_i1);
		n=img1(0,0,0);		//n stores the correct password's length.
		
		//we first ensure that lengths of correct and input passwords are equal.
 	 	if(n!=passwd.length()){
  			cout<<"Error: wrong password \n";
  			return ;
  		}
 	 	//Password check before decryption.
  		while(ctr<=passwd.length()){
  			if(img1(ctr,0,0)!=passwd[ctr-1]){
  				cout<<"Error: wrong password \n";
  				return ;
  			}
  			ctr++;
  		}
		
		//Decryption being done and secret text file being formed.
		outFile.open(secretTextFile);
		cimg_forXY(img1,x,y){
			if((y!=0)&&(flag==1)){
				char c=decryptText(img1(x,y,0),img1(x,y,1),img1(x,y,2));
				if(flag==1){
					if(c>0) {
						outFile.put(c);
					}
					else flag++;			
				}
			}
		}
		outFile.close();		//Task complete.
}








