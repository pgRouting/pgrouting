#ifndef SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
#define SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_

#include <deque>
#include <set>
#include "basePath.h"
#include "pgr_dijkstra.hpp"

template < class G >
class Pgr_ksp: public Pgr_dijkstra< G > {

    // typedef typename std::pair<pgr_path_t*, float8> Paths;

    class compPaths {
      public:
        bool operator()(const Path &p1, const Path  &p2) const {
          if (p1.cost < p2.cost) return true;
          if (p1.cost > p2.cost) return false;

          // paths costs are equal now we check by length
          if (p1.path.size()  < p2.path.size()) return true;
          if (p1.path.size()  > p2.path.size()) return false;

          // paths weights & lengths are equal now we check by ID
          int i;
          for ( i = 0; i < p1.path.size() ; i++)
               if ( p1.path[i].source < p2.path[i].source) return true;

          // we got here and everything is equal
          return false;
       }
   };



 public:
        explicit Pgr_ksp(graphType gtype, const int initial_size) 
                : Pgr_dijkstra <G>(gtype, initial_size)
           {}

        ~Pgr_ksp(void) {} //  {clear();}
        std::deque<Path> Yen(int64_t source, int64_t target, int K);
        void clear();

 private:
        //! the actual algorithm 
        void executeYen(int top_k);

        /** @name Auxiliary function for yen's algorithm */
        ///@{

        //! Performs the first Dijkstra of the algorithm
        void getFirstSolution();
        //! Performs the next cycle of the algorithm
        void doNextCycle();
        //! stores in subPath the first i elements of path
        void removeVertices(const Path &path);
        ///@}

 private:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    V v_source;
    V v_target;
    int64_t m_start;
    int64_t m_end;

        // unsigned int sourceID;  //!< local ID of the source
        // unsigned int targetID;  //!< local ID of the target
        //! ordered set of paths found
    

    Path curr_result_path;
    
    typedef typename  std::set<Path, compPaths> pSet;
    typedef typename  std::set<Path, compPaths>::iterator pSet_i;
    pSet m_ResultSet;
    pSet m_Heap; //!< the heap
    pSet_i pIt;

 private:
        /** @name For parallel paths */
        ///@{
        //void getParallelShort(const BasePath &solutionPath);
        ///@}
};

#include "./YenTopKShortestPathsAlg.cpp"

#endif  // SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
