# How to Compile and Run PBGL algorithm?

## MPI Implementation
To get started with boost.MPI(parallel graphs), we first requires a working MPI(message passing interface) implementation.
There are many conforming MPI implementations available, boost.MPI is fine with any. 
* Open MPI
* MPICH 1.x

**Step 1: Install  MPICH** 
<br>Run following commands to install mpi library(via MPICH)
```
>>> sudo apt-get install libcr-dev mpich mpich-doc
>>> sudo apt install lam-runtime
```

**Step 2: Verify Installation** 
<br>If your build was successful, you should be able to type `mpiexec --version` and see something similar to this.
```
>>> mpiexex --version
mpiexec (OpenRTE) 1.10.2

Report bugs to http://www.open-mpi.org/community/help/
```

**Step 3: Compile & Run**  
<br>Test implementation using mpi_test.cpp program, which passes a message from one processor to another. Each processor prints a message to standard output. You should compile and run this program on two processors. Use the following commands:
```
>>> mpic++ mpi_test.cpp -o mpi_test_output
>>> mpirun -np 2 ./mpi_test_output
```
The following output is perfectly legitimate for this MPI program:
```
Rank 1 OK!
Rank 0 OK!
```

## Install and using Parallel boost-graph

**Step 1: Check libboost Installation:** 
<br>To check whether libboost is already installed with your boost installation or not. Run following commands:
```
>>> apt-cache policy libboost-all-dev
```
If already installed, It should give a output similar to following. In that case, you can skip the next step.
```
libboost-all-dev:
  Installed: 1.58.0.1ubuntu1
  Candidate: 1.58.0.1ubuntu1
  Version table:
 *** 1.58.0.1ubuntu1 500
        500 http://archive.ubuntu.com/ubuntu xenial/universe amd64 Packages
        100 /var/lib/dpkg/status
``` 
**Step 2: Install libboost-all-dev**
If above command's output says that libboost-all-dev is `Not Installed`, then install it.

`>>> sudo apt-get install libboost-all-dev`

After it, Run `dpkg -l|grep boost` to see what have been installed in boost. Specially, verify that `libboost-system`, `libboost-mpi` and ``libboost-graph-parallel` have been installed.

**Step 3: Compile and Run Parallel Graph test code**
Use boost_mpi_test.cpp program to check whether your graph distriution works correctly using two processors. Run below commands:
```
>>> mpic++ -I/path/to/boost/mpi boost_mpi_test.cpp -Llibdir -lboost_mpi -lboost_serialization -lboost_system -lboost_graph_parallel -o boost_mpi_test_output
>>> mpirun -np 2 ./boost_mpi_test_output
```
You should get the following output(or similar), after running above program and we are ready to move on further.
```
>>>Graph vertices distribution among multiple processes(or processors):
 Process: 0: Chicago, population 9500000
Process: 0: Indianapolis, population 791926
>>>Edges distribution among multiple processes(or processors):
 Process: 0: Indianapolis -> Chicago
Process: 0: Indianapolis -> Cincinnati
>>>Graph vertices distribution among multiple processes(or processors):
 Process: 1: Bloomington, population 69291
>>>Edges distribution among multiple processes(or processors):
 Process: 1: Bloomington -> Indianapolis
```
