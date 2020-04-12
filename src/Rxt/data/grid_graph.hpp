#pragma once

#include "Rxt/range.hpp"

#include <boost/graph/graph_traits.hpp>
#include <boost/multi_array.hpp>

#include <array>
#include <utility>
#include <type_traits>
#include <unordered_set>

namespace Rxt
{
using dim_t = std::uint8_t;

enum class grid_boundary { clamp, wrap };

template <dim_t D>
constexpr auto axis_indices()
{
    std::array<dim_t, D> ret{};
    for (dim_t axis = 0; axis < std::size(ret); ++axis)
        ret[axis] = axis;
    return ret;
}

// graph-compatible grid with thin, undirected walls
template <grid_boundary Bound = grid_boundary::clamp>
struct grid_graph_2d
{
    static constexpr dim_t D = 2;

    using size_type = std::int32_t;
    using key_type = Rxt::vec::vec<D, size_type>;

    // using wall_map = std::unordered_map<std::array<size_type, D>, std::array<bool,2>>;
    using wall_map = boost::multi_array<std::array<bool,2>, D>;
    using gap_map = std::unordered_set<key_type>;

    key_type m_size;
    wall_map m_walls;
    gap_map m_gaps;

    struct edge_type
    {
        key_type indices;
        dim_t axis;

        edge_type(key_type k, dim_t a)
            : indices{k}
            , axis{a}
        {}

        // auto operator<=>(const edge&) const = default;
        bool operator==(edge_type const& e) const
        {
            return (axis == e.axis) && (indices == e.indices);
        }
    };

    using vertex_type = key_type;

    // struct vertex_iterator { };
    // struct edge_iterator { };

    grid_graph_2d(size_type w, size_type h)
        : m_size{w, h}
        , m_walls{boost::extents[w][h]}
    {}

    auto size() const { return m_size; }

    // if valid, wrap vertex
    bool validate_vertex(vertex_type& v) const
    {
        if (wrap_boundary()) {
            v = (v % m_size + m_size) % m_size; // positive modulo
        }
        if (any(lessThan(v, key_type(0))))
            return false;
        if (any(greaterThanEqual(v, m_size)))
            return false;
        if (m_gaps.contains(v))
            return false;
        return true;
    }

    constexpr bool wrap_boundary() const
    {
        return Bound == grid_boundary::wrap;
    }

    bool wall(edge_type e) const
    {
        return m_walls[e.indices[0]][e.indices[1]][e.axis];
    }

    friend std::pair<edge_type, bool> edge(vertex_type u, vertex_type v, grid_graph_2d const& g)
    {
        edge_type zero{{0,0},0};
        if (!g.validate_vertex(u) || !g.validate_vertex(v) || u == v)
            return {zero, false};

        dim_t edge_axis;        // which axis is the edge along?
        bool forward;           // pointing from u to v?
        // check the difference along each axis (cardinal adjacency only)
        for (auto axis: axis_indices<D>()) {
            auto diff = std::abs(v[axis] - u[axis]);
            if (diff == 0) {
                continue;
            } else if (diff == 1) {
                edge_axis = axis;
                forward = (diff == v[axis] - u[axis]);
            } else if (g.wrap_boundary() && diff == g.size()[axis] - 1) {
                edge_axis = axis;
                forward = (diff == u[axis] - v[axis]);
            } else {
                return {zero, false};
            }
        }
        auto ret = edge_type(forward ? u : v, edge_axis);
        if (!g.wall(ret))
            return {ret, true};
        return {zero, false};
    }

    friend auto adjacent_vertices(vertex_type v, grid_graph_2d const& g)
    {
        struct Iter
        {
            grid_graph_2d const* self;
            vertex_type from;
            dim_t current_axis = 0;
            bool ahead = false;
            vertex_type next{};

            Iter(grid_graph_2d const* g, vertex_type v)
                : self(g), from(v)
            {
                if (!self->validate_vertex(from))
                    current_axis = D;
                satisfy_invariant();
            }

            void advance()
            {
                if (ahead) ++current_axis;
                ahead = !ahead;
            }

            // advance until we find a valid vertex
            void satisfy_invariant()
            {
                while (current_axis < D) {
                    next = from;
                    next[current_axis] += (ahead ? 1 : -1);
                    if (self->validate_vertex(next) &&
                        !self->wall(edge_type(ahead ? from : next, current_axis)))
                        break;
                    advance();
                }
            }

            auto& operator++()
            {
                advance();
                satisfy_invariant();
                return *this;
            }

            vertex_type operator*() { return next; }

            bool operator!=(range_sentinel) { return current_axis < D; }
        };
        return range_adaptor<Iter, range_sentinel>(Iter{&g, v}, range_sentinel{});
    }
};
}

namespace boost
{
template <Rxt::grid_boundary B>
struct graph_traits<Rxt::grid_graph_2d<B>>
{
    using Grid = Rxt::grid_graph_2d<B>;

    using vertex_descriptor = typename Grid::vertex_type;
    using edge_descriptor = typename Grid::edge_type;
    using directed_category = boost::undirected_tag;
    using edge_parallel_category = boost::disallow_parallel_edge_tag;
    // struct traversal_category : public virtual boost::bidirectional_graph_tag,
    //                             public virtual boost::vertex_list_graph_tag,
    //                             public virtual boost::edge_list_graph_tag
    struct traversal_category : public incidence_graph_tag
                              , public adjacency_graph_tag
    {};

    using Size = typename Grid::size_type;
    using vertices_size_type = Size;
    using halfedges_size_type = Size;
    using edges_size_type = Size;
    using faces_size_type = Size;
    using degree_size_type = Size;

    // todo: iterators....
};
}
