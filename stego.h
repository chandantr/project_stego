#include<vector>
using namespace std;

class Servant{
	public:
		unsigned char encrypt(const unsigned char& source_pixel,const unsigned char& secret_pixel);	//encryption of pixel RGB values
		unsigned char decrypt_secret(const unsigned char& coded_pixel);			//decryption of pixel RGB values
		vector<unsigned char> encryptText(const unsigned char& r,const unsigned char& g,const unsigned char& b,unsigned char secret);		//encryption of characters into RGB values of the image.
		unsigned char decryptText(const unsigned char& r,const unsigned char& g,const unsigned char& b);		//decryption of characters.
		void imageEncrypter(int argc,char **argv);			//image into image encrypter
		void imageDecrypter(int argc,char **argv);			//image into image decrypter
		void textEncrypter(int argc,char **argv);				//text into image encrypter
		void textDecrypter(int argc,char **argv);				//text decrypter.
};
