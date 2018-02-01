#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 r = (ray.direction - 2.0 * dot(ray.direction, same_side_normal) * same_side_normal).normalized();
    Ray reflection;
    reflection.endpoint = intersection_point;
    reflection.direction = r;
    vec3 reflected_color = world.Cast_Ray(reflection, recursion_depth + 1);
    vec3 shader_color = shader->Shade_Surface(
        reflection, 
        intersection_point, 
        same_side_normal, 
        recursion_depth, 
        is_exiting);
    vec3 color = reflectivity * reflected_color + (1 - reflectivity) * shader_color;
    return color;
}
