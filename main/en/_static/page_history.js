function createInfo(file, newat, altnames = '') {
    this.file = file;
    this.newat = newat;
    this.altnames = altnames;
}

var versionsArr = ['3.5', '3.4', '3.3', '3.2', '3.1', '3.0'];
var unsuportedArr = ['2.6', '2.5', '2.4', '2.3', '2.2', '2.1', '2.0'];
var titles = [
    {k: 'en', v: ['Supported versions', 'Unsupported versions']},
    {k: 'es', v: ['Versiones soportadas', 'Versiones no soportadas']},
];


var newpages = [
    {v: '3.4', pages: ['TRSP-family', 'pgr_withPointsVia', 'pgr_trsp_withPoints', 'pgr_trspVia_withPoints',
            'pgr_trspVia', 'pgr_hawickCircuits', 'pgr_findCloseEdges', 'pgr_cuthillMckeeOrdering', 'ordering-family',
            'migration']},

    {v: '3.3', pages: ['withPoints-category', 'via-category', 'reference', 'pgr_edgeColoring', 'DFS-category',
        'BFS-category']},

    {v: '3.2', pages: [ 'traversal-family', 'pgr_sequentialVertexColoring', 'pgr_makeConnected',
        'pgr_lengauerTarjanDominatorTree', 'pgr_isPlanar',  'pgr_dijkstraNearCost', 'pgr_dijkstraNear',
        'pgr_depthFirstSearch', 'pgr_bipartite', 'coloring-family']},

    {v: '3.0', pages: [ 'VRP-category', 'pgr_pickDeliver', 'pgr_turnRestrictedPath', 'pgr_transitiveClosure',
        'pgr_topologicalSort', 'pgr_stoerWagner', 'spanningTree-family', 'prim-family', 'pgr_primDFS', 'pgr_primDD',
        'pgr_primBFS', 'pgr_prim', 'kruskal-family', 'pgr_kruskalDFS', 'pgr_kruskalDD', 'pgr_kruskalBFS', 'pgr_kruskal',
        'pgr_full_version', 'pgr_maxFlowMinCost_Cost', 'pgr_maxFlowMinCost', 'pgr_extractVertices', 'pgr_edwardMoore',
        'pgr_dagShortestPath', 'chinesePostmanProblem-family', 'pgr_chinesePostmanCost', 'pgr_chinesePostman',
        'pgr_breadthFirstSearch', 'pgr_binaryBreadthFirstSearch', 'pgr_bellmanFord', ] },

    {v: '2.6', pages: ['transformation-family', 'pgr_lineGraphFull',] },

    {v: '2.5', pages: [ 'pgr_strongComponents', 'pgr_lineGraph', 'pgr_connectedComponents', 'pgr_bridges',
        'pgr_biconnectedComponents', 'pgr_bdDijkstraCostMatrix', 'pgr_bdDijkstraCost', 'pgr_bdAstarCostMatrix',
        'pgr_bdAstarCost', 'pgr_articulationPoints', 'components-family', 'bdDijkstra-family', 'bdAstar-family', ] },

    {v: '2.4', pages: [ 'pgr_maxFlow', 'pgr_aStarCostMatrix', 'pgr_aStarCost', 'KSP-category',
        'cost-category', 'aStar-family' ] },
];

var filesArr = [


    new createInfo('TSP-family', '2.3', [
        { v: '2.3', n: 'tsp', d: 'tsp'},
    ]),
    new createInfo('costMatrix-category', '2.3', [
        { v: '2.3', n: 'CostMatrix', d: 'costMatrix'},
    ]),
    new createInfo('pgr_withPointsCostMatrix', '2.3', [
        { v: '2.3', n: 'pgr_withPointsCostMatrix', d: 'costMatrix'},
    ]),
    new createInfo('pgr_maxCardinalityMatch', '2.3', [
        { v: '2.3', n: 'pgr_maximumCardinalityMatching', d: 'max_flow'},
        { v: '2.4', n: 'pgr_maximumCardinalityMatching', d: ''},
    ]),
    new createInfo('pgr_pushRelabel', '2.3', [
        { v: '2.3', n: 'pgr_maxFlowPushRelabel', d: 'max_flow'},
        { v: '2.4', n: 'pgr_maxFlowPushRelabel', d: ''},
    ]),
    new createInfo('pgr_edmondsKarp', '2.3', [
        { v: '2.3', n: 'pgr_maxFlowEdmondsKarp', d: 'max_flow'},
        { v: '2.4', n: 'pgr_maxFlowEdmondsKarp', d: ''},
    ]),
    new createInfo('pgr_boykovKolmogorov', '2.3', [
        { v: '2.3', n: 'pgr_maxFlowBoykovKolmogorov', d: 'max_flow'},
        { v: '2.4', n: 'pgr_maxFlowBoykovKolmogorov', d: ''},
    ]),
    new createInfo('pgr_TSPeuclidean', '2.3', [
        { v: '2.3', n: 'pgr_eucledianTSP', d: 'tsp'},
        { v: '2.4', n: 'pgr_eucledianTSP', d: ''},
        { v: '2.5', n: 'pgr_eucledianTSP', d: ''},
        { v: '2.6', n: 'pgr_eucledianTSP', d: ''},
    ]),
    new createInfo('pgr_edgeDisjointPaths', '2.3', [
        { v: '2.3', n: 'pgr_edgeDisjointPaths', d: 'max_flow'},
    ]),
    new createInfo('pgr_dijkstraCostMatrix', '2.3', [
        { v: '2.3', n: 'pgr_dijkstraCostMatrix', d: 'costMatrix'},
    ]),
    new createInfo('contraction-family', '2.3', [
        { v: '2.3', n: 'contraction', d: 'contraction'},
    ]),
    new createInfo('pgr_contraction', '2.3', [
        { v: '2.3', n: 'pgr_contractGraph', d: 'contraction'},
        { v: '2.4', n: 'pgr_contractGraph', d: ''},
        { v: '2.5', n: 'pgr_contractGraph', d: ''},
        { v: '2.6', n: 'pgr_contractGraph', d: ''},
    ]),
    new createInfo('flow-family', '2.3', [
        { v: '2.3', n: 'maxFlow', d: 'max_flow'},
    ]),

    new createInfo('drivingDistance-category', '2.2', [
        { v: '2.2', n: 'drivingDistance', d: 'driving_distance'},
        { v: '2.3', n: 'drivingDistance', d: 'driving_distance'},
      ]),

    new createInfo('withPoints-family', '2.2', [
        { v: '2.2', n: 'withPoints', d: 'withPoints'},
        { v: '2.3', n: 'withPoints', d: 'withPoints'},
    ]),
    new createInfo('withPoints-family', '2.2', [
        { v: '2.2', n: 'withPoints', d: 'withPoints'},
        { v: '2.3', n: 'withPoints', d: 'withPoints'},
    ]),
    new createInfo('experimental', '2.2', [
        { v: '2.2', n: 'src/proposed', d: ''},
        { v: '2.3', n: 'src/proposed', d: ''},
        { v: '2.4', n: 'proposed', d: ''},
        { v: '2.5', n: 'proposed', d: ''},
        { v: '2.6', n: 'proposed', d: ''},
    ]),
    new createInfo('proposed', '2.2', [
        { v: '2.2', n: 'src/proposedNext', d: ''},
        { v: '2.3', n: 'src/proposedNext', d: ''},
    ]),
    new createInfo('pgr_withPointsKSP', '2.2', [
        { v: '2.2', n: 'pgr_withPointsKSP', d: 'withPoints'},
        { v: '2.3', n: 'pgr_withPointsKSP', d: 'withPoints'},
    ]),
    new createInfo('pgr_withPoints', '2.2', [
        { v: '2.2', n: 'pgr_withPoints', d: 'withPoints'},
        { v: '2.3', n: 'pgr_withPoints', d: 'withPoints'},
    ]),
    new createInfo('pgr_withPointsDD', '2.2', [
        { v: '2.2', n: 'pgr_withPointsDD', d: 'withPoints'},
        { v: '2.3', n: 'pgr_withPointsDD', d: 'withPoints'},
    ]),
    new createInfo('pgr_withPointsCost', '2.2', [
        { v: '2.2', n: 'pgr_withPointsCost', d: 'withPoints'},
        { v: '2.3', n: 'pgr_withPointsCost', d: 'withPoints'},
    ]),
    new createInfo('pgr_withPointsCost', '2.2', [
        { v: '2.2', n: 'pgr_withPointsCost', d: 'withPoints'},
        { v: '2.3', n: 'pgr_withPointsCost', d: 'withPoints'},
    ]),
    new createInfo('pgr_dijkstraVia', '2.2', [
        { v: '2.2', n: 'pgr_dijkstraVia', d: 'dijkstra'},
        { v: '2.3', n: 'pgr_dijkstraVia', d: 'dijkstra'},
    ]),
    new createInfo('pgr_dijkstraCost', '2.2', [
        { v: '2.2', n: 'pgr_dijkstraCost', d: 'dijkstra'},
        { v: '2.3', n: 'pgr_dijkstraCost', d: 'dijkstra'},
    ]),
    new createInfo('dijkstra-family', '2.2', [
        { v: '2.2', n: 'dijkstra', d: 'dijkstra'},
        { v: '2.3', n: 'dijkstra', d: 'dijkstra'},
    ]),
    new createInfo('allpairs-family', '2.2', [
        { v: '2.2', n: 'allpairs', d: 'allpairs'},
        { v: '2.3', n: 'allpairs', d: 'allpairs'},
    ]),
    new createInfo('pgr_pickDeliverEuclidean', '2.1', [
        { v: '2.1', n: 'index', d: 'vrppdtw'},
        { v: '2.2', n: 'index', d: 'vrppdtw'},
        { v: '2.3', n: 'pgr_gsoc_vrppdtw', d: 'vrppdtw'},
        { v: '2.4', n: 'pgr_gsoc_vrppdtw', d: ''},
        { v: '2.5', n: 'pgr_gsoc_vrppdtw', d: ''},
        { v: '2.6', n: 'pgr_gsoc_vrppdtw', d: ''},
    ]),
    new createInfo('pgr_vrpOneDepot', '2.1', [
        { v: '2.1', n: 'index', d: 'vrp_basic'},
        { v: '2.2', n: 'pgr_vrpOneDepot', d: 'vrp_basic'},
        { v: '2.3', n: 'pgr_vrpOneDepot', d: 'vrp_basic'},
    ]),
    new createInfo('pgr_version', '2.0', [
        { v: '2.0', n: 'utilities/version', d: 'common'},
        { v: '2.1', n: 'utilities/version', d: 'common'},
        { v: '2.2', n: 'pgr_version', d: 'common'},
        { v: '2.3', n: 'pgr_version', d: 'common'},
    ]),
    new createInfo('routingFunctions', '2.0', [
        { v: '2.0', n: 'src/index', d: ''},
        { v: '2.1', n: 'src/index', d: ''},
        { v: '2.2', n: 'src/routingFunctions', d: ''},
        { v: '2.3', n: 'src/routingFunctions', d: ''},
    ]),
    new createInfo('index', '2.0', [
        { v: '2.0', n: 'index', d: ''},
        { v: '2.1', n: 'index', d: ''},
        { v: '2.2', n: 'index', d: ''},
        { v: '2.3', n: 'index', d: ''},
    ]),
    new createInfo('pgr_TSP', '2.0', [
        { v: '2.0', n: 'index', d: 'tsp'},
        { v: '2.1', n: 'index', d: 'tsp'},
        { v: '2.2', n: 'pgr_tsp', d: 'tsp'},
        { v: '2.3', n: 'pgr_tsp', d: 'tsp'},
    ]),
    new createInfo('pgr_trsp', '2.0', [
        { v: '2.0', n: 'index', d: 'trsp'},
        { v: '2.1', n: 'index', d: 'trsp'},
        { v: '2.2', n: 'pgr_trsp', d: 'trsp'},
        { v: '2.3', n: 'pgr_trsp', d: 'trsp'}
    ]),
    new createInfo('pgr_KSP', '2.0', [
        { v: '2.0', n: 'index', d: 'ksp'},
        { v: '2.1', n: 'ksp_v3', d: 'ksp'},
        { v: '2.2', n: 'pgr_ksp', d: 'ksp'},
        { v: '2.3', n: 'pgr_ksp', d: 'ksp'},
    ]),
    new createInfo('pgr_dijkstra', '2.0', [
        { v: '2.0', n: 'index', d: 'dijkstra'},
        { v: '2.1', n: 'dijkstra_v3', d: 'dijkstra'},
        { v: '2.2', n: 'pgr_dijkstra', d: 'dijkstra'},
        { v: '2.3', n: 'pgr_dijkstra', d: 'dijkstra'}
    ]),
    new createInfo('pgr_bdDijkstra', '2.0', [
        { v: '2.0', n: 'index', d: 'bd_dijkstra'},
        { v: '2.1', n: 'index', d: 'bd_dijkstra'},
        { v: '2.2', n: 'pgr_bdDijkstra', d: 'bd_dijkstra'},
        { v: '2.3', n: 'pgr_bdDijkstra', d: 'bd_dijkstra'}
    ]),
    new createInfo('pgr_bdAstar', '2.0', [
        { v: '2.0', n: 'index', d: 'bd_astar'},
        { v: '2.1', n: 'index', d: 'bd_astar'},
        { v: '2.2', n: 'pgr_bdAstar', d: 'bd_astar'},
        { v: '2.3', n: 'pgr_bdAstar', d: 'bd_astar'}
    ]),
    new createInfo('pgr_aStar', '2.0', [
        { v: '2.0', n: 'index', d: 'astar'},
        { v: '2.1', n: 'index', d: 'astar'},
        { v: '2.2', n: 'pgr_astar', d: 'astar'},
        { v: '2.3', n: 'pgr_astar', d: 'astar'},
    ]),
    new createInfo('pgr_floydWarshall', '2.0', [
        { v: '2.0', n: 'index', d: 'apsp_warshall'},
        { v: '2.1', n: 'index', d: 'apsp_warshall'},
        { v: '2.2', n: 'pgr_floydWarshall', d: 'allpairs'},
        { v: '2.3', n: 'pgr_floydWarshall', d: 'allpairs'}
    ]),
    new createInfo('pgr_johnson', '2.0', [
        { v: '2.0', n: 'index', d: 'apsp_johnson'},
        { v: '2.1', n: 'index', d: 'apsp_johnson'},
        { v: '2.2', n: 'pgr_johnson', d: 'allpairs'},
        { v: '2.3', n: 'pgr_johnson', d: 'allpairs'}
    ]),
    new createInfo('pgr_nodeNetwork', '2.0', [
        { v: '2.0', n: 'functions/node_network', d: 'common'},
        { v: '2.1', n: 'functions/node_network', d: 'common'},
        { v: '2.2', n: 'pgr_nodeNetwork', d: 'topology'},
        { v: '2.3', n: 'pgr_nodeNetwork', d: 'topology'},
    ]),
    new createInfo('topology-functions', '2.0', [
        { v: '2.0', n: 'functions/index', d: 'common'},
        { v: '2.1', n: 'functions/index', d: 'common'},
        { v: '2.2', n: 'topology', d: 'topology'},
        { v: '2.3', n: 'topology', d: 'topology'},
    ]),
    new createInfo('pgr_createVerticesTable', '2.0', [
        { v: '2.0', n: 'functions/create_vert_table', d: 'common'},
        { v: '2.1', n: 'functions/create_vert_table', d: 'common'},
        { v: '2.2', n: 'pgr_createVerticesTable', d: 'topology'},
        { v: '2.3', n: 'pgr_createVerticesTable', d: 'topology'},
    ]),
    new createInfo('pgr_createTopology', '2.0', [
        { v: '2.0', n: 'functions/create_topology', d: 'common'},
        { v: '2.1', n: 'functions/create_topology', d: 'common'},
        { v: '2.2', n: 'pgr_createTopology', d: 'topology'},
        { v: '2.3', n: 'pgr_createTopology', d: 'topology'},
    ]),
    new createInfo('pgr_analyzeOneWay', '2.0', [
        { v: '2.0', n: 'functions/analyze_oneway', d: 'common'},
        { v: '2.1', n: 'functions/analyze_oneway', d: 'common'},
        { v: '2.2', n: 'pgr_analyzeOneWay', d: 'topology'},
        { v: '2.3', n: 'pgr_analyzeOneWay', d: 'topology'},
    ]),
    new createInfo('pgr_analyzeGraph', '2.0', [
        { v: '2.0', n: 'functions/analyze_graph', d: 'common'},
        { v: '2.1', n: 'functions/analyze_graph', d: 'common'},
        { v: '2.2', n: 'pgr_analyzeGraph', d: 'topology'},
        { v: '2.3', n: 'pgr_analyzeGraph', d: 'topology'},
    ]),
    new createInfo('pgRouting-concepts', '2.0', [
        { v: '2.0', n: 'doc/src/tutorial/index', d: ''},
        { v: '2.1', n: 'doc/src/tutorial/index', d: ''},
        { v: '2.2', n: 'doc/src/tutorial/index', d: ''},
        { v: '2.3', n: 'doc/src/tutorial/index', d: ''},
    ]),
    new createInfo('support', '2.0', [
        { v: '2.0', n: 'doc/src/introduction/support', d: ''},
        { v: '2.1', n: 'doc/src/introduction/support', d: ''},
        { v: '2.2', n: 'doc/src/introduction/support', d: ''},
        { v: '2.3', n: 'doc/src/introduction/support', d: ''},
    ]),
    new createInfo('pgRouting-introduction', '2.0', [
        { v: '2.0', n: 'doc/src/introduction/introduction', d: ''},
        { v: '2.1', n: 'doc/src/introduction/introduction', d: ''},
        { v: '2.2', n: 'doc/src/introduction/introduction', d: ''},
        { v: '2.3', n: 'doc/src/introduction/introduction', d: ''},
    ]),
    new createInfo('pgRouting-installation', '2.0', [
        { v: '2.0', n: 'doc/src/installation/index', d: ''},
        { v: '2.1', n: 'doc/src/installation/index', d: ''},
        { v: '2.2', n: 'doc/src/installation/installation', d: ''},
        { v: '2.3', n: 'doc/src/installation/installation', d: ''},
    ]),
    new createInfo('sampledata', '2.0', [
        { v: '2.0', n: 'doc/src/developer/sampledata', d: ''},
        { v: '2.1', n: 'doc/src/developer/sampledata', d: ''},
        { v: '2.2', n: 'doc/src/developer/sampledata', d: ''},
        { v: '2.3', n: 'doc/src/developer/sampledata', d: ''},
    ]),
    new createInfo('release_notes', '2.0', [
        { v: '2.0', n: 'doc/src/changelog/index', d: ''},
        { v: '2.1', n: 'doc/src/changelog/index', d: ''},
        { v: '2.2', n: 'doc/src/changelog/index', d: ''},
        { v: '2.3', n: 'doc/src/changelog/release_notes', d: ''},
    ]),
    new createInfo('pgr_drivingDistance', '2.0', [
        { v: '2.0', n: 'dd_driving_distance', d: 'driving_distance'},
        { v: '2.1', n: 'dd_driving_distance_v3', d: 'driving_distance'},
        { v: '2.2', n: 'pgr_drivingDistance', d: 'driving_distance'},
        { v: '2.3', n: 'pgr_drivingDistance', d: 'driving_distance'}
    ]),
    new createInfo('pgr_alphaShape', '2.0', [
        { v: '2.0', n: 'dd_alphashape', d: 'driving_distance'},
        { v: '2.1', n: 'dd_alphashape', d: 'driving_distance'},
        { v: '2.2', n: 'pgr_alphaShape', d: 'alpha_shape'},
        { v: '2.3', n: 'pgr_alphaShape', d: 'alpha_shape'}
    ]),

];

function get_titles(data, k) {
    for (var i = 0; i < data.length; i++) {
        if (data[i].k == k) { return data[i]; }
    }
    return '';
}
function get_vdata(data, v) {
    for (var i = 0; i < data.length; i++) {
        if (data[i].v == v) { return data[i]; }
    }
    return '';
}

function get(name) {
    for (var i = 0; i < filesArr.length; i++) {
        if (filesArr[i].file === name) { return filesArr[i]; }
    }
}

function prepare() {
    for (var i = 0; i < newpages.length; i++) {
        for (var j = 0; j < newpages[i].pages.length; j++) {
            filesArr.push(new createInfo(newpages[i].pages[j], newpages[i].v));
        }
    }
}

function get_history(name, lang) {
    prepare();
    var data = get(name);
    var title = get_titles(titles, lang);
    var home = `<a class="reference external" href="https://docs.pgrouting.org/`;
    var validlang = lang;

    var result;
    result = '<strong>' + title.v[0] + ':</strong>';
    result += home + 'latest/' + validlang + '/' + data.file + '.html"> latest </a> ';
    latest = versionsArr[0];
    for (var i = 0; i < versionsArr.length; i++) {
        if (versionsArr[i] < data.newat) break;
        var link = home + versionsArr[i] + '/';
        link += validlang + '/' + data.file + '.html">' + versionsArr[i] + '</a>';
        if (versionsArr[i] == latest) result += '(' + link + ') ';
        else result += link + ' ';
    }
    result += home + 'main/' + validlang + '/' + data.file + '.html"> main </a> ';
    result += home + 'dev/' + validlang + '/' + data.file + '.html"> dev </a> ';

    if (data.newat <= unsuportedArr[0]) {
        result += '<br/><strong>' + title.v[1] + ':</strong>';
        for (var i = 0; i < unsuportedArr.length; i++) {
            /* for spanish non translated versions use english */
            validlang = (lang == 'es' && unsuportedArr[i] == '2.0')? lang : 'en';

            if (data.newat > unsuportedArr[i]) break;
            var link = home + unsuportedArr[i] + '/';
            switch(unsuportedArr[i]) {
                case '2.4':
                    var info = get_vdata(data.altnames, unsuportedArr[i]);
                    if (info === '') {
                        link += validlang + '/' + data.file + '.html">' + unsuportedArr[i] + '</a>';
                    } else {
                        link += validlang + '/' + info.n + '.html">' + unsuportedArr[i] + '</a>';
                    }
                    break;
                case '2.3':
                case '2.2':
                case '2.1':
                case '2.0':
                    var info = get_vdata(data.altnames, unsuportedArr[i]);
                    if (info.d == '') {
                        link += validlang + '/' + info.n + '.html">' + unsuportedArr[i] + '</a>';
                    } else {
                        link += validlang + '/src/' + info.d + '/doc/' + info.n + '.html">' + unsuportedArr[i] + '</a>';
                    }
                    break;
                default:
                    link += validlang + '/' + data.file + '.html">' + unsuportedArr[i] + '</a>';
            }
            result += link + ' ';
        }
    }
    return result;
}

