psql  -U postgres -h localhost -A -t -q -f sampledata.test pgr_test &>tmp.aaa
sed s/psql:sampledata.test:[0-9]*:// <tmp.aaa >sampledata.rest



psql  -U postgres -h localhost -A -t -q -f alphashape-any.test pgr_test &> tmp.aaa
sed s/psql:alphashape-any.test:[0-9]*:// <tmp.aaa >alphashape-any.rest

psql  -U postgres -h localhost -A -t -q -f apspJohnson-any.test pgr_test &> tmp.aaa
sed s/psql:apspJohnson-any.test:[0-9]*:// <tmp.aaa >apspJohnson-any.rest

psql  -U postgres -h localhost -A -t -q -f apspWarshall-any.test pgr_test &> tmp.aaa
sed s/psql:apspWarshall-any.test:[0-9]*:// <tmp.aaa >apspWarshall-any.rest

psql  -U postgres -h localhost -A -t -q -f astar-any.test pgr_test &> tmp.aaa
sed s/psql:astar-any.test:[0-9]*:// <tmp.aaa >astar-any.rest

psql  -U postgres -h localhost -A -t -q -f bdstar-any.test pgr_test &> tmp.aaa
sed s/psql:bdstar-any.test:[0-9]*:// <tmp.aaa >bdstar-any.rest

psql  -U postgres -h localhost -A -t -q -f bdDijkstra-any.test pgr_test &> tmp.aaa
sed s/psql:bdDijkstra-any.test:[0-9]*:// <tmp.aaa >bdDijkstra-any.rest

psql  -U postgres -h localhost -A -t -q -f dijkstra-any.test pgr_test &> tmp.aaa
sed s/psql:dijkstra-any.test:[0-9]*:// <tmp.aaa >dijkstra-any.rest

psql  -U postgres -h localhost -A -t -q -f drivingDistance-any.test pgr_test &> tmp.aaa
sed s/psql:dt01-drivingDistance-any.test:[0-9]*:// <tmp.aaa >drivingDistance-any.rest

psql  -U postgres -h localhost -A -t -q -f pointsAsPolygon-any.test pgr_test &> tmp.aaa
sed s/psql:pointsAsPolygon-any.test:[0-9]*:// <tmp.aaa >pointsAsPolygon-any.rest

psql  -U postgres -h localhost -A -t -q -f kdijkstra-any.test pgr_test &> tmp.aaa
sed s/psql:kdijkstra-any.test:[0-9]*:// <tmp.aaa >kdijkstra-any.rest

psql  -U postgres -h localhost -A -t -q -f ksp-any.test pgr_test &> tmp.aaa
sed s/psql:ksp-any.test:[0-9]*:// <tmp.aaa >ksp-any.rest

psql  -U postgres -h localhost -A -t -q -f tsp-any.test pgr_test &> tmp.aaa
sed s/psql:tsp-any.test:[0-9]*:// <tmp.aaa >tsp-any.rest

psql  -U postgres -h localhost -A -t -q -f trsp-any.test pgr_test &> tmp.aaa
sed s/psql:trsp-any.test:[0-9]*:// <tmp.aaa >trsp-any.rest


psql  -U postgres -h localhost -A -t -q -f utilities-any.test pgr_test &> tmp.aaa
sed s/psql:utilities-any.test:[0-9]*:// <tmp.aaa >utilities-any.rest



psql  -U postgres -h localhost -A -t -q -f createTopology-any.test pgr_test &> tmp.aaa
sed s/psql:createTopology-any.test:[0-9]*:// <tmp.aaa >createTopology-any.rest

psql  -U postgres -h localhost -A -t -q -f createVertTab-any.test pgr_test &> tmp.aaa
sed s/psql:createVertTab-any.test:[0-9]*:// <tmp.aaa >createVertTab-any.rest

psql  -U postgres -h localhost -A -t -q -f analyzeGraph-any.test pgr_test &> tmp.aaa
sed s/psql:analyzeGraph-any.test:[0-9]*:// <tmp.aaa >analyzeGraph-any.rest

psql  -U postgres -h localhost -A -t -q -f analyzeOneway-any.test pgr_test &> tmp.aaa
sed s/psql:analyzeOneway-any.test:[0-9]*:// <tmp.aaa >analyzeOneway-any.rest

psql  -U postgres -h localhost -A -t -q -f nodeNetwork-any.test pgr_test &> tmp.aaa
sed s/psql:nodeNetwork-any.test:[0-9]*:// <tmp.aaa >nodeNetwork-any.rest
