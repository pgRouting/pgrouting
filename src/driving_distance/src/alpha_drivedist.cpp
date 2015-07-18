/*
 * Alpha-Shapes for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * As a special exception, you have permission to link this program
 * with the CGAL library and distribute executables, as long as you
 * follow the requirements of the GNU GPL in regard to all of the
 * software in the executable aside from CGAL.
 *
 */


/***********************************************************************
Takes a list of points and returns a list of segments 
corresponding to the Alpha shape.
************************************************************************/
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif
#ifdef __MINGW64__
namespace boost {
  void tss_cleanup_implemented() { }
}
#endif
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/algorithm.h>

#include <vector>
#include <list>

#ifndef _MSC_VER
#include "alpha.h"
#endif // _MSC_VER

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_hierarchy_vertex_base_2.h>
#include <CGAL/Triangulation_hierarchy_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_euclidean_traits_2.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>

#ifdef _MSC_VER
#include "alpha.h"
#endif // _MSC_VER

typedef double coord_type;

typedef CGAL::Simple_cartesian<coord_type>  SC;
typedef CGAL::Filtered_kernel<SC> K;

typedef K::Point_2  Point;
typedef K::Segment_2  Segment;

typedef CGAL::Alpha_shape_vertex_base_2<K> Avb;
typedef CGAL::Triangulation_hierarchy_vertex_base_2<Avb> Av;

typedef CGAL::Triangulation_face_base_2<K> Tf;
typedef CGAL::Alpha_shape_face_base_2<K,Tf> Af;

typedef CGAL::Triangulation_default_data_structure_2<K,Av,Af> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Dt;
typedef CGAL::Triangulation_hierarchy_2<Dt> Ht;
typedef CGAL::Alpha_shape_2<Ht> Alpha_shape_2;

typedef Alpha_shape_2::Face_circulator  Face_circulator;
typedef Alpha_shape_2::Vertex_circulator  Vertex_circulator;

typedef Alpha_shape_2::Alpha_iterator Alpha_iterator;
typedef Alpha_shape_2::Alpha_shape_edges_iterator Alpha_shape_edges_iterator;

//---------------------------------------------------------------------

void find_next_edge(Segment s, std::vector<Segment>& segments, 
                    std::vector<Segment>& res)
{
  if(res.size() == segments.size())
    return;
    
  res.push_back(s);

  Point end = s.target();
  
  for(int i=0;i < segments.size(); i++)
    {
      Point source = segments.at(i).source();
      if(source == end)
        {
          find_next_edge(segments.at(i), segments, res);
        }
    }
}

template <class OutputIterator>
void
alpha_edges( const Alpha_shape_2&  A,
             OutputIterator out)
{ 

  for(Alpha_shape_edges_iterator it =  A.alpha_shape_edges_begin();
      it != A.alpha_shape_edges_end();
      ++it){
    *out++ = A.segment(*it);
  }
}


int alpha_shape(vertex_t *vertices, unsigned int count, 
                vertex_t **res, int *res_count, char **err_msg)
{
  std::list<Point> points;

  //std::copy(begin(vertices), end(vertices), std::back_inserter(points)); 
  
  for (std::size_t j = 0; j < count; ++j)
    {
      Point p(vertices[j].x, vertices[j].y);
      points.push_back(p);
    }
  

  Alpha_shape_2 A(points.begin(), points.end(),
                  coord_type(10000),
                  Alpha_shape_2::GENERAL);
  
  std::vector<Segment> segments;
  std::vector<Segment> result;

  Alpha_shape_2::Alpha_shape_vertices_iterator vit;
  Alpha_shape_2::Vertex_handle vertex;
  Alpha_shape_2::Alpha_shape_edges_iterator eit;
  Alpha_shape_2::Edge edge;
  Alpha_shape_2::Face_iterator fit;
  Alpha_shape_2::Face_handle face;
  
  A.set_alpha(*A.find_optimal_alpha(1)*6); 

  alpha_edges( A, std::back_inserter(segments));

  Segment s = segments.at(0);
  find_next_edge(s, segments, result);

  *res = (vertex_t *) malloc(sizeof(vertex_t) * (result.size() + 1));
  *res_count = result.size();

  for(int i=0;i < result.size(); i++)
    {
      (*res)[i].x = result.at(i).target().x();
      (*res)[i].y = result.at(i).target().y();
    }

  return EXIT_SUCCESS;
}
