
all: MessageReceiverTest CharactersTest

MessageReceiverTest: MessageReceiverTest.cpp ../MessageReceiver.cpp ../MessageReceiver.h
	g++ MessageReceiverTest.cpp -o MessageReceiver.test
	./MessageReceiver.test 01234 abcd EFGH ijkl MNOPQ rstuvxyz

CharactersTest: CharactersTest.cpp ../Characters.cpp
	g++ CharactersTest.cpp ../Characters.cpp -o Characters.test
	./Characters.test 01234 abcd EFGH ijkl MNOPQ rstuvxyz

clean: 
	rm -f *.test