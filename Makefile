main:main.cc
	g++ -g -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf main
