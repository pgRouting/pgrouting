function createInfo(file, newat, altnames = '', removedat = '') {
    this.file = file;
    this.newat = newat;
    this.altnames = altnames;
    this.removedat = removedat;
}

const versionsArr = ['3.8','3.7', '3.6', '3.5', '3.4', '3.3', '3.2', '3.1', '3.0'];
var unsuportedArr = ['2.6', '2.5', '2.4', '2.3', '2.2', '2.1', '2.0'];
var titles = [
    {k: 'en', v: ['Supported versions', 'Unsupported versions']},
    {k: 'es', v: ['Versiones soportadas', 'Versiones no soportadas']},
    {k: 'zh-Hans', v: ['Supported versions', 'Unsupported versions']},
];


var newpages = [
    {v: '3.8', pages: ['pgr_contractionDeadEnd', 'pgr_contractionLinear', 'pgr_separateCrossing',
        'pgr_separateTouching']},

    {v: '3.7', pages: ['metrics-family', 'pgr_betweennessCentrality']},

    {v: '3.4', pages: ['TRSP-family', 'pgr_withPointsVia', 'pgr_trsp_withPoints', 'pgr_trspVia_withPoints',
            'pgr_trspVia', 'pgr_hawickCircuits', 'pgr_findCloseEdges', 'pgr_cuthillMckeeOrdering', 'ordering-family',
            'migration', 'pgr_degree']},

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
        'drivingDistance-category', 'costMatrix-category', 'cost-category', 'aStar-family' ] },
];

var filesArr = [

    new createInfo('spanningTree-category', '3.0', [
        { v: '3.7', n: 'spanningTree-family'},
    ]),

    /* Documentation from version 2.3 */
    new createInfo('TSP-family', '2.3', [
        { v: '2.3', n: 'src/tsp/doc/tsp', d: 'tsp'},
    ]),

    new createInfo('pgr_withPointsCostMatrix', '2.3', [
        { v: '2.3', n: 'src/costMatrix/doc/pgr_withPointsCostMatrix'},
    ]),

    new createInfo('pgr_maxCardinalityMatch', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/pgr_maximumCardinalityMatching'},
        { v: '2.4', n: 'pgr_maximumCardinalityMatching'},
    ]),
    new createInfo('pgr_pushRelabel', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/pgr_maxFlowPushRelabel'},
        { v: '2.4', n: 'pgr_maxFlowPushRelabel'},
    ]),
    new createInfo('pgr_edmondsKarp', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/pgr_maxFlowEdmondsKarp'},
        { v: '2.4', n: 'pgr_maxFlowEdmondsKarp'},
    ]),
    new createInfo('pgr_boykovKolmogorov', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/pgr_maxFlowBoykovKolmogorov'},
        { v: '2.4', n: 'pgr_maxFlowBoykovKolmogorov'},
    ]),
    new createInfo('pgr_TSPeuclidean', '2.3', [
        { v: '2.3', n: 'src/tsp/doc/pgr_eucledianTSP'},
        { v: '2.6', n: 'pgr_eucledianTSP'},
    ]),
    new createInfo('pgr_edgeDisjointPaths', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/pgr_edgeDisjointPaths'},
    ]),
    new createInfo('pgr_dijkstraCostMatrix', '2.3', [
        { v: '2.3', n: 'src/costMatrix/doc/pgr_dijkstraCostMatrix'},
    ]),
    new createInfo('pgr_contraction', '2.3', [
        { v: '2.3', n: 'src/contraction/doc/pgr_contractGraph'},
        { v: '2.6', n: 'pgr_contractGraph'},
    ]),
    new createInfo('flow-family', '2.3', [
        { v: '2.3', n: 'src/max_flow/doc/maxFlow'},
    ]),

    /* Documentation from version 2.2 */
    new createInfo('withPoints-family', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/withPoints'},
    ]),
    new createInfo('withPoints-family', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/withPoints'},
    ]),
    new createInfo('experimental', '2.2', [
        { v: '2.2', n: 'src/proposed'},
        { v: '2.3', n: 'src/proposed'},
        { v: '2.4', n: 'proposed'},
        { v: '2.5', n: 'proposed'},
        { v: '2.6', n: 'proposed'},
    ]),
    new createInfo('proposed', '2.2', [
        { v: '2.2', n: 'src/proposed'},
        { v: '2.3', n: 'src/proposed'},
    ]),
    new createInfo('pgr_withPointsKSP', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/pgr_withPointsKSP'},
    ]),
    new createInfo('pgr_withPoints', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/pgr_withPoints'},
    ]),
    new createInfo('pgr_withPointsDD', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/pgr_withPointsDD'},
    ]),
    new createInfo('pgr_withPointsCost', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/pgr_withPointsCost'},
    ]),
    new createInfo('pgr_withPointsCost', '2.2', [
        { v: '2.3', n: 'src/withPoints/doc/pgr_withPointsCost'},
    ]),
    new createInfo('pgr_dijkstraVia', '2.2', [
        { v: '2.3', n: 'src/dijkstra/doc/pgr_dijkstraVia'},
    ]),
    new createInfo('pgr_dijkstraCost', '2.2', [
        { v: '2.3', n: 'src/dijkstra/doc/pgr_dijkstraCost'},
    ]),
    new createInfo('dijkstra-family', '2.2', [
        { v: '2.3', n: 'src/dijkstra/doc/dijkstra'},
    ]),
    new createInfo('contraction-family', '2.2', [
        { v: '2.3', n: 'src/contraction/doc/contraction'},
    ]),
    new createInfo('allpairs-family', '2.2', [
        { v: '2.3', n: 'src/allpairs/doc/allpairs'},
    ]),

    /* Documentation from version 2.1 */
    new createInfo('pgr_pickDeliverEuclidean', '2.1', [
        { v: '2.2', n: 'src/vrppdtw/doc/index'},
        { v: '2.3', n: 'src/vrppdtw/doc/pgr_gsoc_vrppdtw'},
        { v: '2.6', n: 'pgr_gsoc_vrppdtw'},
    ]),
    new createInfo('pgr_vrpOneDepot', '2.1', [
        { v: '2.1', n: 'src/vrp_basic/doc/index'},
        { v: '2.3', n: 'src/vrp_basic/doc/pgr_vrpOneDepot'},
    ]),

    /* Documentation from version 2.0 */
    new createInfo('pgr_version', '2.0', [
        { v: '2.1', n: 'src/common/doc/utilities/version'},
        { v: '2.3', n: 'src/common/doc/pgr_version'},
    ]),
    new createInfo('routingFunctions', '2.0', [
        { v: '2.1', n: 'src/index'},
        { v: '2.3', n: 'src/routingFunctions'},
    ]),
    new createInfo('index', '2.0', [
        { v: '2.3', n: 'doc/index'},
    ]),
    new createInfo('pgr_TSP', '2.0', [
        { v: '2.1', n: 'src/tsp/doc/index'},
        { v: '2.3', n: 'src/tsp/doc/pgr_tsp'},
    ]),
    new createInfo('pgr_trsp', '2.0', [
        { v: '2.1', n: 'src/trsp/doc/index'},
        { v: '2.3', n: 'src/trsp/doc/pgr_trsp'}
    ]),
    new createInfo('pgr_KSP', '2.0', [
        { v: '2.0', n: 'src/ksp/doc/index'},
        { v: '2.1', n: 'src/ksp/doc/ksp_v3'},
        { v: '2.3', n: 'src/ksp/doc/pgr_ksp'},
    ]),
    new createInfo('pgr_dijkstra', '2.0', [
        { v: '2.0', n: 'src/dijkstra/doc/index'},
        { v: '2.1', n: 'src/dijkstra/doc/dijkstra_v3'},
        { v: '2.3', n: 'src/dijkstra/doc/pgr_dijkstra'}
    ]),
    new createInfo('pgr_bdDijkstra', '2.0', [
        { v: '2.1', n: 'src/bd_dijkstra/doc/index'},
        { v: '2.3', n: 'src/bd_dijkstra/doc/pgr_bdDijkstra'}
    ]),
    new createInfo('pgr_bdAstar', '2.0', [
        { v: '2.1', n: 'src/bd_astar/doc/index'},
        { v: '2.3', n: 'src/bd_astar/doc/pgr_bdAstar'}
    ]),
    new createInfo('pgr_aStar', '2.0', [
        { v: '2.1', n: 'src/astar/doc/index'},
        { v: '2.3', n: 'src/astar/doc/pgr_astar'},
    ]),
    new createInfo('pgr_floydWarshall', '2.0', [
        { v: '2.1', n: 'src/apsp_warshall/doc/index'},
        { v: '2.3', n: 'src/allpairs/doc/pgr_floydWarshall'}
    ]),
    new createInfo('pgr_johnson', '2.0', [
        { v: '2.1', n: 'src/apsp_johnson/doc/index'},
        { v: '2.3', n: 'src/allpairs/doc/pgr_johnson'}
    ]),
    new createInfo('utilities-family', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/index'},
        { v: '2.3', n: 'src/topology/doc/topology'},
        { v: '3.8', n: 'topology-functions'},
    ]),
    new createInfo('pgRouting-concepts', '2.0', [
        { v: '2.3', n: 'doc/src/tutorial/tutorial'},
    ]),
    new createInfo('support', '2.0', [
        { v: '2.3', n: 'doc/src/introduction/support'},
    ]),
    new createInfo('pgRouting-introduction', '2.0', [
        { v: '2.3', n: 'doc/src/introduction/introduction'},
    ]),
    new createInfo('pgRouting-installation', '2.0', [
        { v: '2.1', n: 'doc/src/installation/index'},
        { v: '2.3', n: 'doc/src/installation/installation'},
    ]),
    new createInfo('sampledata', '2.0', [
        { v: '2.3', n: 'doc/src/developer/sampledata'},
    ]),
    new createInfo('release_notes', '2.0', [
        { v: '2.2', n: 'doc/src/changelog/index'},
        { v: '2.3', n: 'doc/src/changelog/release_notes'},
    ]),
    new createInfo('drivingDistance-category', '2.0', [
        { v: '2.1', n: 'src/driving_distance/doc/index'},
        { v: '2.3', n: 'src/driving_distance/doc/drivingDistance'},
    ]),
    new createInfo('pgr_drivingDistance', '2.0', [
        { v: '2.0', n: 'src/driving_distance/doc/dd_driving_distance'},
        { v: '2.1', n: 'src/driving_distance/doc/dd_driving_distance_v3'},
        { v: '2.3', n: 'src/driving_distance/doc/pgr_drivingDistance'}
    ]),

    new createInfo('pgr_nodeNetwork', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/node_network'},
        { v: '2.3', n: 'src/topology/doc/pgr_nodeNetwork'}, 3.8
    ]),
    new createInfo('pgr_createVerticesTable', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/create_vert_table'},
        { v: '2.3', n: 'src/topology/doc/pgr_createVerticesTable'}, 3.8
    ]),
    new createInfo('pgr_createTopology', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/create_topology'},
        { v: '2.3', n: 'src/topology/doc/pgr_createTopology'}, 3.8
    ]),
    new createInfo('pgr_analyzeOneWay', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/analyze_oneway'},
        { v: '2.3', n: 'src/topology/doc/pgr_analyzeOneWay'}, 3.8
    ]),
    new createInfo('pgr_analyzeGraph', '2.0', [
        { v: '2.1', n: 'src/common/doc/functions/analyze_graph'},
        { v: '2.3', n: 'src/topology/doc/pgr_analyzeGraph'}, 3.8
    ]),
    new createInfo('pgr_alphaShape', '2.0', [
        { v: '2.1', n: 'src/driving_distance/doc/dd_alphashape'},
        { v: '2.3', n: 'src/alpha_shape/doc/pgr_alphaShape'}, 3.8
    ]),

];

function get_titles(data, k) {
    for (var i = 0; i < data.length; i++) {
        if (data[i].k == k) { return data[i]; }
    }
    return '';
}

function get_vdata(data, v, file, lang) {
    let info = '';
    let link = '';
    for (var i = 0; i < data.length; i++) {
        if (v <= data[i].v) { info = data[i]; break; }
    }
    if (info === '') {
        link = `${lang}/${file}.html">${v}</a>`;
    } else {
        link = `${lang}/${info.n}.html">${v}</a>`;
    }
    return link;
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
        validlang = lang;

        if (versionsArr[i] < data.newat) break;
        if (data.removedat != '' && versionsArr[i] > data.newat) break;

        /* for zh-Hans non translated versions use english */
        validlang = (lang == 'zh-Hans' && versionsArr[i] >= '3.7')? 'zh_Hans' : 'en';

        let link = `${home}${versionsArr[i]}/`
        link += get_vdata(data.altnames, versionsArr[i], data.file, validlang);
        if (versionsArr[i] == latest) result += '(' + link + ') ';
        else result += link + ' ';
    }
    result += home + 'main/' + lang + '/' + data.file + '.html"> main </a> ';
    result += home + 'dev/' + lang + '/' + data.file + '.html"> dev </a> ';

    if (data.newat <= unsuportedArr[0]) {
        result += '<br/><strong>' + title.v[1] + ':</strong>';
        for (var i = 0; i < unsuportedArr.length; i++) {
            /* basically 2.x has only english */
            validlang = 'en';

            /* for spanish non translated versions use english */
            validlang = (lang == 'es' && unsuportedArr[i] == '2.0')? 'es' : validlang;

            if (data.newat > unsuportedArr[i]) break;
            var link = home + unsuportedArr[i] + '/';
            link += get_vdata(data.altnames, unsuportedArr[i], data.file, validlang);
            result += link + ' ';
        }
    }
    return result;
}

