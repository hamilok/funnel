funnel:
	g++ -I ./include -l boost_system -l boost_thread src/main.cpp -o bin/funnel

clean:
	rm bin/funnel
