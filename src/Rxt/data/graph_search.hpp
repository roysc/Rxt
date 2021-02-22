#pragma once

#include "graph.hpp"
#include "Rxt/io.hpp"

#include <unordered_map>
#include <queue>

namespace Rxt
{

namespace _det
{
struct greater_of_first
{
    template <class T, class U>
    bool operator()(T const& t, U const& u)
    {
        return t.first > u.first;
    }
};

template<typename T, typename priority_t>
struct priority_queue {
    using element = std::pair<priority_t, T>;
    std::priority_queue<element, std::vector<element>,
                        greater_of_first> elements;

    bool empty() const { return elements.empty(); }

    void put(T item, priority_t priority) { elements.emplace(priority, item); }

    T get()
    {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};
}


template <class G, class H, class PathMap, class CostMap, class Vdesc = vertex_descriptor_t<G>>
void a_star_search(
    G const& graph,
    Vdesc start, Vdesc goal,
    PathMap& came_from,
    CostMap cost_map,
    H&& heuristic
) {
    std::unordered_map<Vdesc, double> cost_so_far;
    _det::priority_queue<Vdesc, double> frontier;
    frontier.put(start, 0);

    came_from[start] = start;
    cost_so_far[start] = 0;
  
    while (!frontier.empty()) {
        Vdesc current = frontier.get();

        if (current == goal) {
            break;
        }

        for (auto [nbr, nbr_end] = adjacent_vertices(current, graph);
             nbr != nbr_end; ++nbr) {
            double new_cost = cost_so_far[current] + cost_map[edge(current, *nbr, graph).first];
            if (cost_so_far.find(*nbr) == cost_so_far.end()
                || new_cost < cost_so_far[*nbr]) {
                cost_so_far[*nbr] = new_cost;
                double priority = new_cost + heuristic(*nbr, goal);
                frontier.put(*nbr, priority);
                came_from[*nbr] = current;
            }
        }
    }
}

template <class Vdesc, class PathMap>
std::vector<Vdesc> reconstruct_path(
    Vdesc start, Vdesc goal,
    PathMap const& came_from
) {
    std::vector<Vdesc> path;
    Vdesc current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from.at(current);
    }
    path.push_back(start); // optional
    std::reverse(path.begin(), path.end());
    return path;
}
}
