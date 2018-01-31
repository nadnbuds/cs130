#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 color = world.ambient_color * world.ambient_intensity * color_ambient;

    for(Light *light : world.lights)
    {
        if(world.enable_shadows){
            Ray shadowCheck;
            Hit hit;
            shadowCheck.endpoint = light->position;
            shadowCheck.direction = (intersection_point - light->position).normalized();
            world.Closest_Intersection(shadowCheck, hit);
            vec3 firstHit = shadowCheck.Point(hit.t);
            if((firstHit - intersection_point).magnitude() > small_t)
                continue;

        }
        vec3 l = (light->position - intersection_point);
        vec3 lightColor = light->Emitted_Light(ray)/l.magnitude_squared();
        l = l.normalized();

        double d = dot(l, same_side_normal);
        d = std::max(d, 0.0);
        color += d * lightColor * color_diffuse;

        vec3 r = (2.0 * dot(l,same_side_normal) * same_side_normal - l).normalized();
        vec3 eye = (world.camera.position - intersection_point).normalized();
        double s = dot(r, eye);
        s = std::max(s, 0.0);
        color += pow(s, specular_power) * color_specular * lightColor;
    }
    return color;
}
