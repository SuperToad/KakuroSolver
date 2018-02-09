parser: parser.o csp.cpp main.cpp
	g++ -g3 -o $@ $^

%.o: %.cpp %.hpp
	g++ -g3 -c $<

clean:
	rm -f parser *.o
