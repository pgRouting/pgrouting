psql -U postgres  -h localhost -A -t -q -f ksp-any-00.data ksp_test

psql -U postgres  -h localhost -A -t -q -f ksp-any-01.test ksp_test &> ksp-any-01.aaa
sed s/psql:ksp-any-01.test:[0-9]*:// <ksp-any-01.aaa > ksp-any-01.rest

psql -U postgres  -h localhost -A -t -q -f ksp-any-02.test ksp_test &> ksp-any-02.aaa
sed s/psql:ksp-any-02.test:[0-9]*:// <ksp-any-02.aaa >ksp-any-02.rest

psql -U postgres  -h localhost -A -t -q -f ksp-parallel-any-03.test ksp_test &> ksp-parallel-any-03.aaa
sed s/psql:ksp-parallel-any-03.test:[0-9]*:// <ksp-parallel-any-03.aaa >ksp-parallel-any-03.rest

rm *.aaa
