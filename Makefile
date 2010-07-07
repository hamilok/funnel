funnel:
	g++ -I ./include -l boost_system -l boost_thread src/main.cpp -o bin/funnel

grub:
	g++ -I ./include -l boost_system -l boost_thread src/grub.cpp -o bin/grub

clean:
	rm bin/funnel
