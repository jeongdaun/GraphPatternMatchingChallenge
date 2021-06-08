/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

/*
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
int cnt = 0;                //backtrack 함수 호출 횟수 count

void backtrack(vector<size_t> &list, vector<bool> &check, size_t id, const Graph &data, const Graph &query, const CandidateSet &cs, size_t level) {
    for (size_t i = cs.GetCandidateSize(id) - 1; i != -1; i--) {                                            //현재 방문한 노드의 모든 candidate에 대해 수행
        bool is_fit = true;
        for (int j = 0; j < list.size(); j++) {
            if (!check[j])
                continue;
            if ((!data.IsNeighbor(list[j], cs.GetCandidate(id, i)) && query.IsNeighbor(id, j))              //선택한 candidate이 올바르게 연결되었는지
              || cs.GetCandidate(id, i) == list[j]) {                                                       //이미 사용된 candidate은 아닌지 검사
                is_fit = false;
                break;
            }
        }
        if (is_fit) {
            list[id] = cs.GetCandidate(id, i);
            check[id] = true;
            if (level == check.size()) {                    //모든 노드를 방문했다면 print out
                printf("a ");
                for (int l = 0; l < list.size(); l++)
                    printf("%d ", list[l]);
                printf("\n");
                cnt++;
                if (cnt == 100000) {
                    exit(0);
                }
                check[id] = false;
                list[id] = -1;
                continue;
            }

            //다음으로 방문할 노드 찾기
            //cs size가 1인 인접 노드가 있다면 선택
            //그렇지 않다면 first-fit

            size_t idx = -1;
            size_t len = query.GetNumVertices();
            bool flag1 = false, flag2 = false;
            for (size_t j = 0; j < len; j++) {
                if (!check[j])
                    continue;
                for (size_t k = j + 1; k < len; k++) {
                    if (check[k])
                        continue;
                    if (query.IsNeighbor(j, k)) {
                        if (!flag2) {
                            idx = k;
                            flag2 = true;
                        }
                        if (cs.GetCandidateSize(k) == 1) {
                            idx = k;
                            flag1 = true;
                            break;
                        }
                    }
                }
                if (flag1)
                    break;
            }
            if (idx != -1)
                backtrack(list, check, idx, data, query, cs, level + 1);
            check[id] = false;
            list[id] = -1;
        }
    }
}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    cout << "t " << query.GetNumVertices() << "\n";
    vector<size_t> list(query.GetNumVertices(), -1);
    vector<bool> check(query.GetNumVertices(), false);
    backtrack(list, check, 0, data, query, cs, 1);
}