
SET(LOCAL_FILES
    doc-johnson.queries
    doc-pgr_bdDijkstra.queries
    doc-pgr_edgeDisjointPaths.queries
    doc-pgr_pointsAsPolygon.queries
    doc-pgr_withPoints.queries
    doc-aStarCost.queries
    doc-ksp.queries
    doc-pgr_boykovKolmogorov.queries
    doc-pgr_edmondsKarp.queries
    doc-pgr_pushRelabel.queries
    doc-pgr_withPointsVia.queries
    doc-astar.queries
    doc-pgr_alphashape.queries
    doc-pgr_createTopology.queries
    doc-pgr_eucledianTSP.queries
    doc-pgr_TSP.queries
    doc-pickDeliverEuclidean.queries
    doc-contractGraph.queries
    doc-pgr_analyzeGraph.queries
    doc-pgr_dijkstraCost.queries
    doc-pgr_fooDmatrix.queries
    doc-pgr_version.queries
    doc-pickDeliver.queries
    doc-contraction.queries
    doc-pgr_bdAstarCost.queries
    doc-pgr_dijkstra.queries
    doc-pgr_labelGraph.queries
    doc-pgr_withPointsCost.queries
    doc-trsp.queries
    doc-floydWarshall.queries
    doc-pgr_bdAstar.queries
    doc-pgr_dijkstraVia.queries
    doc-pgr_maxCardinalityMatch.queries
    doc-pgr_withPointsDD.queries
    doc-pgr_bdDijkstraCost.queries
    doc-pgr_drivingDistance.queries
    doc-pgr_maxFlow.queries
    doc-pgr_withPointsKSP.queries
	doc-pgr_connectedComponents.queries
    doc-pgr_strongComponents.queries
	doc-pgr_biconnectedComponents.queries
	doc-pgr_articulationPoints.queries
	doc-pgr_bridges.queries
    doc-pgr_vrpOneDepot.queries
    doc-gsoc_vrppdtw.queries
    doc-pgr_lineGraph.queries
    doc-pgr_lineGraphFull.queries
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
