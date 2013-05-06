all : main.cpp stego.cpp stego.h
	g++ -o stego main.cpp stego.cpp -lpthread -lX11

clean:
	rm *.o stego
