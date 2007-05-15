CGAL_MAKEFILE = /usr/make/makefile_x86-64_Linux-2.6_g++-3.4.4
BOOST_PATH=/usr/include/boost/graph
GAUL_PATH =/usr/local/include/gaul
GAUL_LIB  =/usr/local/lib64
CGAL_PATH =/usr/include
CGAL_LIB  =/usr/lib64

CXXFLAGS = \
       -I../../include \
$(CGAL_CXXFLAGS) \
$(LONG_NAME_PROBLEM_CXXFLAGS)

EXTRA_FLAGS = -O2 -g -fpic
 
#---------------------------------------------------------------------#
#                    linker flags
#---------------------------------------------------------------------#
 
LIBPATH = \
$(CGAL_LIBPATH)

LDFLAGS = \
       $(LONG_NAME_PROBLEM_LDFLAGS) \
       $(CGAL_LDFLAGS)

MODULE_big = routing

ifeq ($(GAUL_PATH),)
 TSP_OBJS= 
 TSP_LIBS=
else
 TSP_OBJS= tsp.o tsp_solver.o 
 TSP_LIBS= -L$(GAUL_LIB) -lgaul_util -lgaul
endif

ifeq ($(CGAL_MAKEFILE),)
 ALPHA_OBJS= 
 ALPHA_LIBS=
else
 include $(CGAL_MAKEFILE)
 ALPHA_OBJS= alpha.o alpha_drivedist.o
 ALPHA_LIBS= -L$(CGAL_LIB_DIR)/$(CGAL_OS_COMPILER) -lCGAL 
endif

OBJS = dijkstra.o boost_wrapper.o astar.o astar_boost_wrapper.o \
       drivedist.o boost_drivedist.o shooting_star.o shooting_star_boost_wrapper.o \
       $(TSP_OBJS) $(ALPHA_OBJS)

DATA = routing_postgis.sql
DATA_built = routing.sql
DOCS = README.routing



PGXS := $(shell pg_config --pgxs)
include $(PGXS)

SHLIB_LINK += -lstdc++ $(TSP_LIBS) $(ALPHA_LIBS)


boost_wrapper.o: boost_wrapper.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(BOOST_PATH) -c boost_wrapper.cpp

astar_boost_wrapper.o: astar_boost_wrapper.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(BOOST_PATH) -c astar_boost_wrapper.cpp

shooting_star_boost_wrapper.o: shooting_star_boost_wrapper.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(BOOST_PATH) -c shooting_star_boost_wrapper.cpp

boost_drivedist.o: boost_drivedist.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(BOOST_PATH) -c boost_drivedist.cpp

tsp_solver.o: tsp_solver.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(GAUL_PATH) -c tsp_solver.cpp

alpha_drivedist.o: alpha_drivedist.cpp
	$(CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -I$(CGAL_PATH) -I$(CGAL_INCL_CONF_DIR) -c alpha_drivedist.cpp
