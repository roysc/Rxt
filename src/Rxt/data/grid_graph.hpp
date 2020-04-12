#pragma once

#include <array>
#include <utility>
// #include <limits>
#include <type_traits>

#include <boost/graph/graph_traits.hpp>
#include <boost/multi_array.hpp>

namespace Rxt
{
using dim_t = std::uint8_t;

enum class grid_boundary { clamp, wrap };

template <dim_t D>
constexpr auto axis_indices()
{
    std::array<dim_t, D> ret;
    for (dim_t axis = 0; axis < std::size(ret); ++axis)
        ret[axis] = axis;
    return ret;
}

template <grid_boundary Bound = grid_boundary::clamp>
struct grid_2d
{
    static constexpr dim_t D = 2;

    using size_type = std::uint32_t;
    using key_type = std::array<size_type, D>;

    // using TileMap = boost::multi_array<bool, D>;
    using WallMap = boost::multi_array<std::array<bool,2>, D>;
    // using KeyDiff = std::array<std::make_signed_t<size_type>, D>;

    // static constexpr size_type max_size = std::numeric_limits<size_type>::max() / D;

    key_type _size;
    WallMap _walls;
    // TileMap _tiles;

    struct vertex
    {
        key_type indices;

        vertex(key_type k) : indices{k} {}
        template <class... T>
        vertex(T... i) : indices{i...} {}
    };

    struct edge
    {
        key_type indices;
        dim_t axis;

        edge(key_type k, dim_t a)
            : indices{k}
            , axis{a}
        {}

        // auto operator<=>(const edge&) const = default;
        bool operator==(edge const& e) const
        {
            return (axis == e.axis) && (indices == e.indices);
        }
    };

    struct vertex_iterator
    {

    };

    struct edge_iterator
    {

    };

    grid_2d(size_type w, size_type h)
        : _size{w, h}
        , _walls{boost::extents[w][h]}
    {}

    auto _neighbors(key_type ix) const
    {
        // back, front for each axis
        using Neighbors = std::array<key_type, 2>;
        std::array<Neighbors, D> ret;
        for (auto axis: axis_indices<D>()) {
            key_type nx = ix;
            if (wrap_boundary() && ix[axis] == 0) {
                nx[axis] = _size[axis] - 1;
            } else {
                nx[axis] -= 1;
            }
            ret[axis][0] = nx;
            nx = ix;
            if (wrap_boundary() && ix[axis] == (_size[axis] - 1)) {
                nx[axis] = 0;
            } else {
                nx[axis] += 1;
            }
            ret[axis][1] = nx;
        }
        return ret;
    }

    constexpr bool wrap_boundary() const
    {
        return Bound == grid_boundary::wrap;
    }

    bool wall(edge e) const
    {
        return _walls[e.indices[0]][e.indices[1]][e.axis];
    }

    friend std::pair<edge, bool> edge(vertex u, vertex v, grid_2d const& g)
    {
        auto u_neighbors = g._neighbors(u.indices);
        for (auto axis: axis_indices<D>()) {
            auto [back, front] = u_neighbors[axis];
            if (v.indices == back) {
                edge e{v.indices, axis};
                if (!g.wall(e)) return {e, true};
            } else if (v.indices == front) {
                edge e{u.indices, axis};
                if (!g.wall(e)) return {e, true};
            }
        }
        return {edge{{0,0},0}, false};
    }
};
}

namespace boost
{
template <Rxt::grid_boundary B>
struct graph_traits<Rxt::grid_2d<B>>
{
    using Grid = Rxt::grid_2d<B>;

    using vertex_descriptor = typename Grid::vertex;
    using edge_descriptor = typename Grid::edge;
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
