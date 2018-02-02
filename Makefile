parser: parser.o main.cpp
	gcc -g3 -o $@ $^

%.o: %.cpp %.h
	gcc -g3 -c $<

clean:
	rm -f parser *.o
