psql  -U postgres -h localhost -A -t -q -f common-any-00.data pgr_test 
psql  -U postgres -h localhost -A -t -q -f common-any-01.data pgr_test 

psql  -U postgres -h localhost -A -t -q -f versionless-any-01.test pgr_test &> versionless-any-01.aaa
sed s/psql:versionless-any-01.test:[0-9]*:// <versionless-any-01.aaa >versionless-any-01.rest

psql  -U postgres -h localhost -A -t -q -f common-any-01.test pgr_test &> common-any-01.aaa
sed s/psql:common-any-01.test:[0-9]*:// <common-any-01.aaa >common-any-01.rest

psql  -U postgres -h localhost -A -t -q -f common-any-02.test pgr_test &> common-any-02.aaa
sed s/psql:common-any-02.test:[0-9]*:// <common-any-02.aaa >common-any-02.rest

psql  -U postgres -h localhost -A -t -q -f common-any-03.test pgr_test &> common-any-03.aaa
sed s/psql:common-any-03.test:[0-9]*:// <common-any-03.aaa >common-any-03.rest

psql  -U postgres -h localhost -A -t -q -f common-any-04.test pgr_test &> common-any-04.aaa
sed s/psql:common-any-04.test:[0-9]*:// <common-any-04.aaa >common-any-04.rest

psql  -U postgres -h localhost -A -t -q -f common-any-05.test pgr_test &> common-any-05.aaa
sed s/psql:common-any-05.test:[0-9]*:// <common-any-05.aaa >common-any-05.rest

psql  -U postgres -h localhost -A -t -q -f gettablename-any-01.test pgr_test &> gettablename-any-01.aaa
sed s/psql:gettablename-any-01.test:[0-9]*:// <gettablename-any-01.aaa >gettablename-any-01.rest

psql  -U postgres -h localhost -A -t -q -f createTopology-any-01.test pgr_test &> createTopology-any-01.aaa
sed s/psql:createTopology-any-01.test:[0-9]*:// <createTopology-any-01.aaa >createTopology-any-01.rest

psql  -U postgres -h localhost -A -t -q -f createVerticesTable-any-01.test pgr_test &> createVerticesTable-any-01.aaa
sed s/psql:createVerticesTable-any-01.test:[0-9]*:// <createVerticesTable-any-01.aaa >createVerticesTable-any-01.rest

psql  -U postgres -h localhost -A -t -q -f routweb-any-01.test pgr_test &> routweb-any-01.aaa
sed s/psql:routweb-any-01.test:[0-9]*:// <routweb-any-01.aaa >routweb-any-01.rest

psql  -U postgres -h localhost -A -t -q -f topoweb-any-01.test pgr_test &> topoweb-any-01.aaa
sed s/psql:topoweb-any-01.test:[0-9]*:// <topoweb-any-01.aaa >topoweb-any-01.rest

psql  -U postgres -h localhost -A -t -q -f pgr_getColumnName_any_01.test pgr_test &> pgr_getColumnName_any_01.test.aaa
sed s/psql:pgr_getColumnName_any_01.test:[0-9]*:// <pgr_getColumnName_any_01.test.aaa >pgr_getColumnName_any_01.rest

psql  -U postgres -h localhost -A -t -q -f pgr_getColumnType_any_01.test pgr_test &> pgr_getColumnType_any_01.aaa
sed s/psql:pgr_getColumnType_any_01.test:[0-9]*:// <pgr_getColumnType_any_01.aaa >pgr_getColumnType_any_01.rest

psql  -U postgres -h localhost -A -t -q -f pgr_checkVertTab_any_01.test pgr_test &> pgr_checkVertTab_any_01.aaa
sed s/psql:pgr_checkVertTab_any_01.test:[0-9]*:// <pgr_checkVertTab_any_01.aaa >pgr_checkVertTab_any_01.rest

psql  -U postgres -h localhost -A -t -q -f incrementalCreateTopology_any_01.test  pgr_test &> incrementalCreateTopology_any_01.aaa
sed s/psql:incrementalCreateTopology_any_01.test:[0-9]*:// <incrementalCreateTopology_any_01.aaa >incrementalCreateTopology_any_01.rest 

rm *.aaa
