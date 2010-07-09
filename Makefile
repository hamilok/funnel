funnel: main.o server.o
	g++ -o bin/funnel -I ./include -l boost_system -l boost_thread obj/main.o obj/server.o

main.o:
	g++ -o obj/main.o -c src/main.cpp -I ./include
server.o:
	g++ -o obj/server.o -c src/server.cpp -I ./include


.PHONY: clean

clean:
	rm -f bin/funnel obj/main.o obj/server.o
