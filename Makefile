build:
	g++ -std=c++11 -o project2.out src/main.cpp src/header.h

tasks: multiply flip subtract

multiply:
	./project2.out output/out.tga input/circles.tga multiply input/layer1.tga

flip:
	./project2.out output/flip.tga input/layer1.tga flip

subtract:
	./project2.out output/subtract.tga input/layer1.tga subtract input/layer2.tga


