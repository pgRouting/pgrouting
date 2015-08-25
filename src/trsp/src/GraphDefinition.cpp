
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include "GraphDefinition.h"

// -------------------------------------------------------------------------
GraphDefinition::GraphDefinition(void)
{
    m_lStartEdgeId = -1;
    m_lEndEdgeId = 0;
    m_dStartpart = 0.0;
    m_dEndPart = 0.0;
    m_dCost = NULL;
    m_bIsturnRestrictOn = false;
    m_bIsGraphConstructed = false;
    parent = NULL;
    init();
}

// -------------------------------------------------------------------------
GraphDefinition::~GraphDefinition(void)
{
}


// -------------------------------------------------------------------------
void GraphDefinition::init()
{
    max_edge_id = 0;
    max_node_id = 0;
    isStartVirtual = false;
    isEndVirtual = false;
}


// -------------------------------------------------------------------------
void GraphDefinition::deleteall()
{
    std::vector<GraphEdgeInfo*>::iterator it;
    for(it  = m_vecEdgeVector.begin(); it != m_vecEdgeVector.end(); it++){
        delete *it;
    }
    m_vecEdgeVector.clear();

    delete [] parent;
    delete [] m_dCost;
}


// -------------------------------------------------------------------------
double GraphDefinition::construct_path(int ed_id, int v_pos)
{
    if(parent[ed_id].ed_ind[v_pos] == -1)
    {
        path_element_t pelement;
        GraphEdgeInfo* cur_edge = m_vecEdgeVector[ed_id];
        if(v_pos == 0)
        {
            pelement.vertex_id = cur_edge->m_lStartNode;
            pelement.cost = cur_edge->m_dCost;
        }
        else
        {
            pelement.vertex_id = cur_edge->m_lEndNode;
            pelement.cost = cur_edge->m_dReverseCost;
        }
        pelement.edge_id = cur_edge->m_lEdgeID;

        m_vecPath.push_back(pelement);
        return pelement.cost;
    }
    double ret = construct_path(parent[ed_id].ed_ind[v_pos], parent[ed_id].v_pos[v_pos]);
    path_element_t pelement;
    GraphEdgeInfo* cur_edge = m_vecEdgeVector[ed_id];
    if(v_pos == 0)
    {
        pelement.vertex_id = cur_edge->m_lStartNode;
        pelement.cost = m_dCost[ed_id].endCost - ret;// cur_edge.m_dCost;
        ret = m_dCost[ed_id].endCost;
    }
    else
    {
        pelement.vertex_id = cur_edge->m_lEndNode;
        pelement.cost = m_dCost[ed_id].startCost - ret;
        ret = m_dCost[ed_id].startCost;
    }
    pelement.edge_id = cur_edge->m_lEdgeID;

    m_vecPath.push_back(pelement);

    return ret;
}


// -------------------------------------------------------------------------
double GraphDefinition::getRestrictionCost(
    int edge_ind,
    GraphEdgeInfo& new_edge,
    bool isStart)
{
    double cost = 0.0;
    int edge_id = new_edge.m_lEdgeID;
    if(m_ruleTable.find(edge_id) == m_ruleTable.end())
    {    
        return(0.0);
    }
    std::vector<Rule> vecRules = m_ruleTable[edge_id];
    int ruleIndex;
    int totalRule = vecRules.size();
    int st_edge_ind = edge_ind;
    for(ruleIndex = 0; ruleIndex < totalRule; ruleIndex++)
    {
        bool flag = true;
        int total_edge = vecRules[ruleIndex].precedencelist.size();
        int i;
        int v_pos = (isStart?0:1);
        edge_ind = st_edge_ind;
        for(i = 0; i < total_edge; i++)
        {
            if(edge_ind == -1)
            {
                flag = false;
                break;
            }
            if(vecRules[ruleIndex].precedencelist[i] != m_vecEdgeVector[edge_ind]->m_lEdgeID)
            {
                flag = false;
                break;
            }
            int parent_ind = parent[edge_ind].ed_ind[v_pos];
            v_pos = parent[edge_ind].v_pos[v_pos];
            edge_ind = parent_ind;
        }
        if(flag)
            cost += vecRules[ruleIndex].cost;
    }
    return cost;
}


// -------------------------------------------------------------------------
void GraphDefinition::explore(
    int cur_node,
    GraphEdgeInfo& cur_edge,
    bool isStart,
    LongVector &vecIndex,
    std::priority_queue<PDP, std::vector<PDP>,
    std::greater<PDP> > &que)
{
    unsigned int i;
    double extCost = 0.0;
    GraphEdgeInfo* new_edge;
    // int new_node;
    double totalCost;
    for(i = 0; i < vecIndex.size(); i++)
    {
        new_edge = m_vecEdgeVector[vecIndex[i]];
        extCost = 0.0;
        if(m_bIsturnRestrictOn)
        {
            extCost = getRestrictionCost(cur_edge.m_lEdgeIndex, *new_edge, isStart);
        }
        if(new_edge->m_lStartNode == cur_node)
        {
            if(new_edge->m_dCost >= 0.0)
            {
                //new_node = new_edge->m_lEndNode;
                
                if(isStart)
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].endCost + new_edge->m_dCost + extCost;
                else
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].startCost + new_edge->m_dCost + extCost;
                if(totalCost < m_dCost[vecIndex[i]].endCost)
                {
                    m_dCost[vecIndex[i]].endCost = totalCost;
                    parent[new_edge->m_lEdgeIndex].v_pos[0] = (isStart?0:1);
                    parent[new_edge->m_lEdgeIndex].ed_ind[0] = cur_edge.m_lEdgeIndex;
                    que.push(std::make_pair(totalCost, std::make_pair(new_edge->m_lEdgeIndex, true)));
                }
            }
        }
        else
        {
            if(new_edge->m_dReverseCost >= 0.0)
            {
                // new_node = new_edge->m_lStartNode;
                if(isStart)
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].endCost + new_edge->m_dReverseCost + extCost;
                else
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].startCost + new_edge->m_dReverseCost + extCost;
                if(totalCost < m_dCost[vecIndex[i]].startCost)
                {
                    m_dCost[vecIndex[i]].startCost = totalCost;
                    parent[new_edge->m_lEdgeIndex].v_pos[1] = (isStart?0:1);
                    parent[new_edge->m_lEdgeIndex].ed_ind[1] = cur_edge.m_lEdgeIndex;
                    que.push(std::make_pair(totalCost, std::make_pair(new_edge->m_lEdgeIndex, false)));
                }
            }
        }
    }
}


// -------------------------------------------------------------------------
int GraphDefinition::multi_dijkstra(
    edge_t *edges,
    unsigned int edge_count,
    std::vector<int> vertices,
    bool directed,
    bool has_reverse_cost, 
    path_element_t **path,
    int *path_count,
    char **err_msg,
    std::vector<PDVI> &ruleList)
{
    construct_graph(edges, edge_count, has_reverse_cost, directed);
    if(ruleList.size() > 0)
    {
    m_ruleTable.clear();
    int total_rule = ruleList.size();
    int i;
    LongVector vecsource;
    // int kk;
    for(i = 0; i < total_rule; i++)
    {
        Rule rule;
        rule.cost = ruleList[i].first;
        int j;
        int seq_cnt = ruleList[i].second.size();
        for(j = 1; j < seq_cnt; j++)
        {
            rule.precedencelist.push_back(ruleList[i].second[j]);
        }
        int dest_edge_id = ruleList[i].second[0];
        if(m_ruleTable.find(dest_edge_id) != m_ruleTable.end())
        {
            m_ruleTable[dest_edge_id].push_back(rule);
        }
        else
        {
            std::vector<Rule> temprules;
            temprules.clear();
            temprules.push_back(rule);
            m_ruleTable.insert(std::make_pair(dest_edge_id, temprules));
        }
    }
    
    m_bIsturnRestrictOn = true;
    }
    parent = new PARENT_PATH[edge_count + 1];
    m_dCost = new CostHolder[edge_count + 1];
    m_vecPath.clear();
    int i;
    int total_vertices = vertices.size();
    for(i = 0; i < total_vertices - 1; i++)
    {
    int ret = my_dijkstra(vertices[i], vertices[i + 1], edge_count, err_msg);
    if(ret < 0)
    {
        deleteall();
        return -1;
    }
    }

    *path = (path_element_t *) malloc(sizeof(path_element_t) * (m_vecPath.size() + 1));
    *path_count = m_vecPath.size();

    for(i = 0; i < *path_count; i++)
    {
    (*path)[i].vertex_id = m_vecPath[i].vertex_id;
    (*path)[i].edge_id = m_vecPath[i].edge_id;
    (*path)[i].cost = m_vecPath[i].cost;
    }
    deleteall();
    return 0;
}


// -------------------------------------------------------------------------
int GraphDefinition::my_dijkstra(int start_vertex, int end_vertex, unsigned int edge_count, char **err_msg)
{
    if(!m_bIsGraphConstructed)
    {
    *err_msg = (char *)"Graph not Ready!";
    return -1;
    }
    unsigned int i;
    for(i = 0; i <= edge_count; i++)
    {
    m_dCost[i].startCost = 1e15;
    m_dCost[i].endCost = 1e15;
    }

    if(m_mapNodeId2Edge.find(start_vertex) == m_mapNodeId2Edge.end())
    {
    *err_msg = (char *)"Source Not Found";
    deleteall();
    return -1;
    }

    if(m_mapNodeId2Edge.find(end_vertex) == m_mapNodeId2Edge.end())
    {
    *err_msg = (char *)"Destination Not Found";
    deleteall();
    return -1;
    }

    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
    LongVector vecsource = m_mapNodeId2Edge[start_vertex];
    GraphEdgeInfo* cur_edge = NULL;

    for(i = 0; i < vecsource.size(); i++)
    {
    cur_edge = m_vecEdgeVector[vecsource[i]];
    if(cur_edge->m_lStartNode == start_vertex)
    {
        if(cur_edge->m_dCost >= 0.0)
        {
        m_dCost[cur_edge->m_lEdgeIndex].endCost= cur_edge->m_dCost;
        parent[cur_edge->m_lEdgeIndex].v_pos[0] = -1;
        parent[cur_edge->m_lEdgeIndex].ed_ind[0] = -1;
        que.push(std::make_pair(cur_edge->m_dCost, std::make_pair(cur_edge->m_lEdgeIndex, true)));
        }
    }
    else
    {
        if(cur_edge->m_dReverseCost >= 0.0)
        {
        m_dCost[cur_edge->m_lEdgeIndex].startCost = cur_edge->m_dReverseCost;
        parent[cur_edge->m_lEdgeIndex].v_pos[1] = -1;
        parent[cur_edge->m_lEdgeIndex].ed_ind[1] = -1;
        que.push(std::make_pair(cur_edge->m_dReverseCost, std::make_pair(cur_edge->m_lEdgeIndex, false)));
        }
    }
    }
    
    // int new_node;
    int cur_node = -1;

    while(!que.empty())
    {
    PDP cur_pos = que.top();
    que.pop();
    int cured_index = cur_pos.second.first;
    cur_edge = m_vecEdgeVector[cured_index];
    //GraphEdgeInfo* new_edge;

    if(cur_pos.second.second)      // explore edges connected to end node
    {
        cur_node = cur_edge->m_lEndNode;
        if(cur_edge->m_dCost < 0.0)
        continue;
        if(cur_node == end_vertex)
        break;
        explore(cur_node, *cur_edge, true, cur_edge->m_vecEndConnedtedEdge, que);
    }
    else        // explore edges connected to start node
    {
        cur_node = cur_edge->m_lStartNode;
        if(cur_edge->m_dReverseCost < 0.0)
        continue;
        if(cur_node == end_vertex)
        break;
        explore(cur_node, *cur_edge, false, cur_edge->m_vecStartConnectedEdge, que);
    }
    }
    if(cur_node != end_vertex)
    {
    *err_msg = (char *)"Path Not Found";
    deleteall();
    return -1;
    }
    else
    {
    // double total_cost;  //set but not used
    if(cur_node == cur_edge->m_lStartNode)
    {
        // total_cost = m_dCost[cur_edge->m_lEdgeIndex].startCost;
        construct_path(cur_edge->m_lEdgeIndex, 1);
    }
    else
    {
        // total_cost = m_dCost[cur_edge->m_lEdgeIndex].endCost;
        construct_path(cur_edge->m_lEdgeIndex, 0);
    }
    path_element_t pelement;
    pelement.vertex_id = end_vertex;
    pelement.edge_id = -1;
    pelement.cost = 0.0;
    m_vecPath.push_back(pelement);
    }
    return 0;
}


// -------------------------------------------------------------------------
int GraphDefinition::my_dijkstra(edge_t *edges, unsigned int edge_count, int start_edge_id, double start_part, int end_edge_id, double end_part, bool directed, bool has_reverse_cost,
                path_element_t **path, int *path_count, char **err_msg, std::vector<PDVI> &ruleList)
{
    if(!m_bIsGraphConstructed)
    {
        init();
        construct_graph(edges, edge_count, has_reverse_cost, directed);
        m_bIsGraphConstructed = true;
    }
    GraphEdgeInfo* start_edge_info = m_vecEdgeVector[m_mapEdgeId2Index[start_edge_id]];
    edge_t start_edge;
    int start_vertex, end_vertex;
    m_dStartpart = start_part;
    m_dEndPart = end_part;
    m_lStartEdgeId = start_edge_id;
    m_lEndEdgeId = end_edge_id;

    if(start_part == 0.0)
    {
        start_vertex = start_edge_info->m_lStartNode;
    }
    else if(start_part == 1.0)
    {
        start_vertex = start_edge_info->m_lEndNode;
    }
    else
    {
        isStartVirtual = true;
        m_lStartEdgeId = start_edge_id;
        start_vertex = max_node_id + 1;
        max_node_id++;
        start_edge.id = max_edge_id + 1;
        max_edge_id++;
        start_edge.source = start_vertex;
        start_edge.reverse_cost = -1.0;
        if(start_edge_info->m_dCost >= 0.0)
        {
            start_edge.target = start_edge_info->m_lEndNode;
            start_edge.cost = (1.0 - start_part) * start_edge_info->m_dCost;
            addEdge(start_edge);
            edge_count++;
        }
        if(start_edge_info->m_dReverseCost >= 0.0)
        {
            start_edge.id = max_edge_id + 1;
            max_edge_id++;
            start_edge.target = start_edge_info->m_lStartNode;
            start_edge.cost = start_part * start_edge_info->m_dReverseCost;
            addEdge(start_edge);
            edge_count++;
        }
    }

    GraphEdgeInfo* end_edge_info = m_vecEdgeVector[m_mapEdgeId2Index[end_edge_id]];
    edge_t end_edge;
    
    if(end_part == 0.0)
    {
        end_vertex = end_edge_info->m_lStartNode;
    }
    else if(end_part == 1.0)
    {
        end_vertex = end_edge_info->m_lEndNode;
    }
    else
    {
        isEndVirtual = true;
        m_lEndEdgeId = end_edge_id;
        end_vertex = max_node_id + 1;
        max_node_id++;
        end_edge.id = max_edge_id + 1;
        max_edge_id++;
        end_edge.target = end_vertex;
        end_edge.reverse_cost = -1.0;
        if(end_edge_info->m_dCost >= 0.0)
        {
            end_edge.source = end_edge_info->m_lStartNode;
            end_edge.cost = end_part * end_edge_info->m_dCost;
            addEdge(end_edge);
            edge_count++;
        }
        if(end_edge_info->m_dReverseCost >= 0.0)
        {
            end_edge.source = end_edge_info->m_lEndNode;
            end_edge.id = max_edge_id + 1;
            end_edge.cost = (1.0 - end_part) * end_edge_info->m_dReverseCost;
            addEdge(end_edge);
            edge_count++;
        }
    }

    return(my_dijkstra(edges, edge_count, start_vertex, end_vertex, directed, has_reverse_cost, path, path_count, err_msg, ruleList));
}


// -------------------------------------------------------------------------
int GraphDefinition:: my_dijkstra(edge_t *edges, unsigned int edge_count, int start_vertex, int end_vertex, bool directed, bool has_reverse_cost,
                                  path_element_t **path, int *path_count, char **err_msg, std::vector<PDVI> &ruleList)
{
    m_ruleTable.clear();
    int total_rule = ruleList.size();
    int i;
    LongVector vecsource;
    unsigned int kk;
    for(i = 0; i < total_rule; i++)
    {
        Rule rule;
        rule.cost = ruleList[i].first;
        int j;
        int seq_cnt = ruleList[i].second.size();
        for(j = 1; j < seq_cnt; j++)
        {
            rule.precedencelist.push_back(ruleList[i].second[j]);
        }
        int dest_edge_id = ruleList[i].second[0];
        if(m_ruleTable.find(dest_edge_id) != m_ruleTable.end())
        {
            m_ruleTable[dest_edge_id].push_back(rule);
        }
        else
        {
            std::vector<Rule> temprules;
            temprules.clear();
            temprules.push_back(rule);
            m_ruleTable.insert(std::make_pair(dest_edge_id, temprules));
        }
    
        if(isStartVirtual)
        {
            if(seq_cnt == 2 && ruleList[i].second[1] == m_lStartEdgeId)
            {
                vecsource = m_mapNodeId2Edge[start_vertex];
                for(kk = 0; kk < vecsource.size(); kk++)
                {
                    rule.precedencelist.clear();
                    rule.precedencelist.push_back(m_vecEdgeVector[vecsource[kk]]->m_lEdgeID);
                    m_ruleTable[dest_edge_id].push_back(rule);
                }
            }
        }
    }
    if(isEndVirtual)
    {
        if(m_ruleTable.find(m_lEndEdgeId) != m_ruleTable.end())
        {
            std::vector<Rule> tmpRules = m_ruleTable[m_lEndEdgeId];
            vecsource = m_mapNodeId2Edge[end_vertex];
            for(kk = 0; kk < vecsource.size(); kk++)
            {
                m_ruleTable.insert(std::make_pair(m_vecEdgeVector[vecsource[kk]]->m_lEdgeID, tmpRules));
            }
        }
    }
    m_bIsturnRestrictOn = true;
    return(my_dijkstra(edges, edge_count, start_vertex, end_vertex, directed, has_reverse_cost, path, path_count, err_msg));
}


// -------------------------------------------------------------------------
int GraphDefinition:: my_dijkstra(edge_t *edges, unsigned int edge_count, int start_vertex, int end_vertex, bool directed, bool has_reverse_cost,
                path_element_t **path, int *path_count, char **err_msg)
{
    if(!m_bIsGraphConstructed)
    {
        init();
        construct_graph(edges, edge_count, has_reverse_cost, directed);
        m_bIsGraphConstructed = true;
    }
    
    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
    parent = new PARENT_PATH[edge_count + 1];
    m_dCost = new CostHolder[edge_count + 1];
    m_vecPath.clear();

    unsigned int i;
    for(i = 0; i <= edge_count; i++)
    {
        m_dCost[i].startCost = 1e15;
        m_dCost[i].endCost = 1e15;
    }
    
    if(m_mapNodeId2Edge.find(start_vertex) == m_mapNodeId2Edge.end())
    {
        *err_msg = (char *)"Source Not Found";
        deleteall();
        return -1;
    }

    if(m_mapNodeId2Edge.find(end_vertex) == m_mapNodeId2Edge.end())
    {
        *err_msg = (char *)"Destination Not Found";
        deleteall();
        return -1;
    }

    LongVector vecsource = m_mapNodeId2Edge[start_vertex];
    GraphEdgeInfo* cur_edge = NULL;

    for(i = 0; i < vecsource.size(); i++)
    {
        cur_edge = m_vecEdgeVector[vecsource[i]];
        if(cur_edge->m_lStartNode == start_vertex)
        {
            if(cur_edge->m_dCost >= 0.0)
            {
                m_dCost[cur_edge->m_lEdgeIndex].endCost= cur_edge->m_dCost;
                parent[cur_edge->m_lEdgeIndex].v_pos[0] = -1;
                parent[cur_edge->m_lEdgeIndex].ed_ind[0] = -1;
                que.push(std::make_pair(cur_edge->m_dCost, std::make_pair(cur_edge->m_lEdgeIndex, true)));
            }
        }
        else
        {
            if(cur_edge->m_dReverseCost >= 0.0)
            {
                m_dCost[cur_edge->m_lEdgeIndex].startCost = cur_edge->m_dReverseCost;
                parent[cur_edge->m_lEdgeIndex].v_pos[1] = -1;
                parent[cur_edge->m_lEdgeIndex].ed_ind[1] = -1;
                que.push(std::make_pair(cur_edge->m_dReverseCost, std::make_pair(cur_edge->m_lEdgeIndex, false)));
            }
        }
    }
    //parent[start_vertex].v_id = -1;
    //parent[start_vertex].ed_id = -1;
    //m_dCost[start_vertex] = 0.0;

    // int new_node;
    int cur_node = -1;

    while(!que.empty())
    {
        PDP cur_pos = que.top();
        que.pop();
        int cured_index = cur_pos.second.first;
        cur_edge = m_vecEdgeVector[cured_index];
        //GraphEdgeInfo* new_edge;

        if(cur_pos.second.second)      // explore edges connected to end node
        {
            cur_node = cur_edge->m_lEndNode;
            if(cur_edge->m_dCost < 0.0)
                continue;
            if(cur_node == end_vertex)
                break;
            explore(cur_node, *cur_edge, true, cur_edge->m_vecEndConnedtedEdge, que);
        }
        else        // explore edges connected to start node
        {
            cur_node = cur_edge->m_lStartNode;
            if(cur_edge->m_dReverseCost < 0.0)
                continue;
            if(cur_node == end_vertex)
                break;
            explore(cur_node, *cur_edge, false, cur_edge->m_vecStartConnectedEdge, que);
        }
    }
    if(cur_node != end_vertex)
    {
        if(m_lStartEdgeId == m_lEndEdgeId)
        {
            if(get_single_cost(1000.0, path, path_count))
            {
                return 0;
            }
        }
        *err_msg = (char *)"Path Not Found";
        deleteall();
        return -1;
    }
    else
    {
        double total_cost;
        if(cur_node == cur_edge->m_lStartNode)
        {
            total_cost = m_dCost[cur_edge->m_lEdgeIndex].startCost;
            construct_path(cur_edge->m_lEdgeIndex, 1);
        }
        else
        {
            total_cost = m_dCost[cur_edge->m_lEdgeIndex].endCost;
            construct_path(cur_edge->m_lEdgeIndex, 0);
        }
        path_element_t pelement;
        pelement.vertex_id = end_vertex;
        pelement.edge_id = -1;
        pelement.cost = 0.0;
        m_vecPath.push_back(pelement);

        if(m_lStartEdgeId == m_lEndEdgeId)
        {
            if(get_single_cost(total_cost, path, path_count))
            {
                return 0;
            }
        }
        
        *path = (path_element_t *) malloc(sizeof(path_element_t) * (m_vecPath.size() + 1));
        *path_count = m_vecPath.size();

        for(int i = 0; i < *path_count; i++)
        {
            (*path)[i].vertex_id = m_vecPath[i].vertex_id;
            (*path)[i].edge_id = m_vecPath[i].edge_id;
            (*path)[i].cost = m_vecPath[i].cost;
        }
        if(isStartVirtual)
        {
            (*path)[0].vertex_id = -1;
            (*path)[0].edge_id = m_lStartEdgeId;
        }
        if(isEndVirtual)
        {
            *path_count = *path_count - 1;
            (*path)[*path_count - 1].edge_id = m_lEndEdgeId;
        }
    }
    deleteall();
    return 0;
}


// -------------------------------------------------------------------------
bool GraphDefinition::get_single_cost(double total_cost, path_element_t **path, int *path_count)
{
    GraphEdgeInfo* start_edge_info = m_vecEdgeVector[m_mapEdgeId2Index[m_lStartEdgeId]];
    if(m_dEndPart >= m_dStartpart)
    {
        if(start_edge_info->m_dCost >= 0.0 && start_edge_info->m_dCost * (m_dEndPart - m_dStartpart) <= total_cost)
        {
            *path = (path_element_t *) malloc(sizeof(path_element_t) * (1));
            *path_count = 1;
            (*path)[0].vertex_id = -1;
            (*path)[0].edge_id = m_lStartEdgeId;
            (*path)[0].cost = start_edge_info->m_dCost * (m_dEndPart - m_dStartpart);

            return true;
        }
    }
    else
    {
        if(start_edge_info->m_dReverseCost >= 0.0 && start_edge_info->m_dReverseCost * (m_dStartpart - m_dEndPart) <= total_cost)
        {
            *path = (path_element_t *) malloc(sizeof(path_element_t) * (1));
            *path_count = 1;
            (*path)[0].vertex_id = -1;
            (*path)[0].edge_id = m_lStartEdgeId;
            (*path)[0].cost = start_edge_info->m_dReverseCost * (m_dStartpart - m_dEndPart);

            return true;
        }
    }
    return false;
    
}


// -------------------------------------------------------------------------
bool GraphDefinition::construct_graph(edge_t* edges, int edge_count, bool has_reverse_cost, bool directed)
{
    int i;
    for(i = 0; i < edge_count; i++)
    {
        if(!has_reverse_cost)
        {
        if(directed)
        {
            edges[i].reverse_cost = -1.0;
        }
        else
        {
            edges[i].reverse_cost = edges[i].cost;
        }
        }
        addEdge(edges[i]);
    }
    m_bIsGraphConstructed = true;
    return true;
}


// -------------------------------------------------------------------------
bool GraphDefinition::connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge, bool bIsStartNodeSame)
{
    if(bIsStartNodeSame)
    {
        if(firstEdge.m_dReverseCost >= 0.0)
            firstEdge.m_vecStartConnectedEdge.push_back(secondEdge.m_lEdgeIndex);
        if(firstEdge.m_lStartNode == secondEdge.m_lStartNode)
        {
            if(secondEdge.m_dReverseCost >= 0.0)
                secondEdge.m_vecStartConnectedEdge.push_back(firstEdge.m_lEdgeIndex);
        }
        else
        {
            if(secondEdge.m_dCost >= 0.0)
                secondEdge.m_vecEndConnedtedEdge.push_back(firstEdge.m_lEdgeIndex);
        }
    }
    else
    {
        if(firstEdge.m_dCost >= 0.0)
            firstEdge.m_vecEndConnedtedEdge.push_back(secondEdge.m_lEdgeIndex);
        if(firstEdge.m_lEndNode == secondEdge.m_lStartNode)
        {
            if(secondEdge.m_dReverseCost >= 0.0)
                secondEdge.m_vecStartConnectedEdge.push_back(firstEdge.m_lEdgeIndex);
        }
        else
        {
            if(secondEdge.m_dCost >= 0.0)
                secondEdge.m_vecEndConnedtedEdge.push_back(firstEdge.m_lEdgeIndex);
        }
    }

    return true;
}


// -------------------------------------------------------------------------
bool GraphDefinition::addEdge(edge_t edgeIn)
{
    // long lTest;
    Long2LongMap::iterator itMap = m_mapEdgeId2Index.find(edgeIn.id);
    if(itMap != m_mapEdgeId2Index.end())    
        return false;

    
    GraphEdgeInfo* newEdge = new GraphEdgeInfo();
    newEdge->m_vecStartConnectedEdge.clear();
    newEdge->m_vecEndConnedtedEdge.clear();
    newEdge->m_vecRestrictedEdge.clear();
    newEdge->m_lEdgeID = edgeIn.id;
    newEdge->m_lEdgeIndex = m_vecEdgeVector.size();    
    newEdge->m_lStartNode = edgeIn.source;
    newEdge->m_lEndNode = edgeIn.target;
    newEdge->m_dCost = edgeIn.cost;
    newEdge->m_dReverseCost = edgeIn.reverse_cost;

    if(edgeIn.id > max_edge_id)
    {
        max_edge_id = edgeIn.id;
    }

    if(newEdge->m_lStartNode > max_node_id)
    {
        max_node_id = newEdge->m_lStartNode;
    }
    if(newEdge->m_lEndNode > max_node_id)
    {
        max_node_id = newEdge->m_lEndNode;
    }

    //Searching the start node for connectivity
    Long2LongVectorMap::iterator itNodeMap = m_mapNodeId2Edge.find(edgeIn.source);
    if(itNodeMap != m_mapNodeId2Edge.end())
    {
        //Connect current edge with existing edge with start node
        //connectEdge(
        long lEdgeCount = itNodeMap->second.size();
        long lEdgeIndex;
        for(lEdgeIndex = 0; lEdgeIndex < lEdgeCount; lEdgeIndex++)
        {
            long lEdge = itNodeMap->second.at(lEdgeIndex);    
            connectEdge(*newEdge, *m_vecEdgeVector[lEdge], true);
        }
    }


    //Searching the end node for connectivity
    itNodeMap = m_mapNodeId2Edge.find(edgeIn.target);
    if(itNodeMap != m_mapNodeId2Edge.end())
    {
        //Connect current edge with existing edge with end node
        //connectEdge(
        long lEdgeCount = itNodeMap->second.size();
        long lEdgeIndex;
        for(lEdgeIndex = 0; lEdgeIndex < lEdgeCount; lEdgeIndex++)
        {
            long lEdge = itNodeMap->second.at(lEdgeIndex);    
            connectEdge(*newEdge, *m_vecEdgeVector[lEdge], false);
        }
    }



    //Add this node and edge into the data structure
    m_mapNodeId2Edge[edgeIn.source].push_back(newEdge->m_lEdgeIndex);
    m_mapNodeId2Edge[edgeIn.target].push_back(newEdge->m_lEdgeIndex);


    //Adding edge to the list
    m_mapEdgeId2Index.insert(std::make_pair(newEdge->m_lEdgeID, m_vecEdgeVector.size()));
    m_vecEdgeVector.push_back(newEdge);

    //


    return true;
}
