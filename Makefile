funnel: main.o server.o
	g++ -o bin/funnel -I ./include -l boost_system -l boost_thread obj/network_v4.o obj/server.o obj/main.o

main.o:
	g++ -o obj/main.o -c src/main.cpp -I ./include
server.o: network_v4.o
	g++ -o obj/server.o -c src/server.cpp -I ./include

network_v4.o:
	g++ -o obj/network_v4.o -c src/network_v4.cpp -I ./include


.PHONY: clean

clean:
	rm -f bin/funnel obj/main.o obj/server.o obj/network_v4.o
