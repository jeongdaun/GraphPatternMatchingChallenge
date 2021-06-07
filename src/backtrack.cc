/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

/*
graph

Graph::GetGraphID() : graph ID return
Graph::GetNumVertices() : graph |V| return
Graph::GetNumEdges() : graph |E| return
Graph::GetNumLabels() : graph |labels| return
Graph::GetLabelFrequency(Label l) : graph 내의 label l의 frequency return
Graph::GetNeighborLabelFrequency(Vertex v, Lable l) : v의 neighbor 중 label l 개수 return
Graph::GetDegree(Vertex v) : v의 degree return
Graph::GetNeighborStartOffset(Vertex v) : v의 neighbor의 start offset return
Graph::GetNeighborEndOffset(Vertex v) : v의 neighbor의 end offset return
Graph::GetNeighborStartOffset(Vertex v, Label l) : v의 neighbor 중 label이 l인 것의 start offset return
Graph::GetNeighborEndOffset(Vertex v, Label l) : v의 neighbor 중 label이 l인 것의 end offset return
Graph::GetLabel(Vertex v) : v의 label return
Graph::GetNeighbor(size_t offset) : offset 보고 해당하는 vertex return
Graph::IsNeighbor(Vertex u, Vertex v) : u, v 사이에 edge 있는지 확인

candidate set

CandidateSet::GetCandidateSize(Vertex u) : u에 mapped 된 vertice 개수 return
CandidateSet::GetCandidate(Vertex u, size_t i) : u의 candidate set에서 ith candidate return
*/

#include <ctime>
#include <vector>

using namespace std;
int cnt = 0;
clock_t start;

void Check(vector<size_t> list, const Graph &data, const Graph &query, const CandidateSet &cs)
{
  for (int i = 0; i < query.GetNumVertices(); i++)
  {
    for (int j = i + 1; j < query.GetNumVertices(); j++)
    {
      if (query.IsNeighbor(i, j) &&
          !data.IsNeighbor(list[i], list[j]))
      {
        printf("wrong edge! %d %d\n", i, j);
      }
      if (list[i] == list[j])
        printf("same vertex! %d %d\n", list[i], list[j]);
    }
  }
}

void backtrack(vector<size_t> &list, vector<bool> &check, size_t id, const Graph &data, const Graph &query, const CandidateSet &cs, size_t level)
{
  if ((double)(clock() - start) / CLOCKS_PER_SEC > 60)
  {
    cout << "TIMEOUT " << endl
         << cnt << endl;
    exit(0);
  }

  for (size_t i = 0; i < cs.GetCandidateSize(id); i++)
  {
    bool is_fit = true;
    for (int j = 0; j < list.size(); j++)
    {
      if (!check[j])
        continue;
      if ((!data.IsNeighbor(list[j], cs.GetCandidate(id, i)) && query.IsNeighbor(id, j)) || cs.GetCandidate(id, i) == list[j])
      {
        is_fit = false;
        break;
      }
    }
    if (is_fit)
    {
      list[id] = cs.GetCandidate(id, i);
      check[id] = true;
      if (level == check.size())
      {
        printf("a ");
        for (int l = 0; l < list.size(); l++)
          printf("%d ", list[l]);
        printf("\n");
        Check(list, data, query, cs);
        cnt++;
        if (cnt == 100000)
        {
          cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
          cout << cnt << endl;
          exit(0);
        }
        check[id] = false;
        list[id] = -1;
        continue;
      }
      size_t idx = -1;
      size_t min = -1;
      for (size_t j = 0; j < query.GetNumVertices(); j++)
      {
        if (!check[j])
          continue;
        for (size_t k = j + 1; k < query.GetNumVertices(); k++)
        {
          if (check[k])
            continue;
          if (query.IsNeighbor(j, k) && min > cs.GetCandidateSize(k))
          {
            idx = k;
            min = cs.GetCandidateSize(k);
          }
        }
      }
      if (idx != -1)
        backtrack(list, check, idx, data, query, cs, level + 1);

      check[id] = false;
      list[id] = -1;
    }
  }
}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs)
{
  start = clock();
  cout << "t " << query.GetNumVertices() << "\n";
  vector<size_t> list(query.GetNumVertices(), -1);
  vector<bool> check(query.GetNumVertices(), false);
  backtrack(list, check, 0, data, query, cs, 1);
  cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
  cout << cnt << endl;
}