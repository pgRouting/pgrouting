all: vrp_tester


vrp_tester: Tester.cpp VRP_Solver.cpp VRP_Solver.h Utils.h  VRP.h
	   	 g++ -g Tester.cpp VRP_Solver.cpp  -o  vrp_tester

clean:
	rm -f vrp_tester  result.txt
