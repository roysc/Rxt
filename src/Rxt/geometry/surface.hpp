#pragma once

#include <CGAL/Implicit_surface_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/IO/facets_in_complex_2_to_triangle_mesh.h>
#include <CGAL/make_surface_mesh.h>

namespace Rxt
{
// Generate a triangulated surface from a function f: R3 -> R
struct surface_triangulator
{
    using T3n      = CGAL::Surface_mesh_default_triangulation_3;
    using GT       = T3n::Geom_traits;
    using Implicit = CGAL::Implicit_surface_3<GT, GT::FT(GT::Point_3)>;
    using C2T3     = CGAL::Complex_2_in_triangulation_3<T3n>;

    template <class Mesh, class F>
    auto operator()(F&& f, GT::Sphere_3 bound)
    {
        T3n t3n;
        C2T3 c2t3{t3n};
        Implicit surf{f, bound};
        Mesh mesh;

        // angular bound, radius bound, distance bound
        CGAL::Surface_mesh_default_criteria_3<T3n> criteria {30., 0.1, 0.1};
        CGAL::make_surface_mesh(c2t3, surf, criteria, CGAL::Manifold_with_boundary_tag {});
        CGAL::facets_in_complex_2_to_triangle_mesh(c2t3, mesh);
        return mesh;
    }
};


}
