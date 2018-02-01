#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 reflection_color;
    vec3 refraction_color;
    
    double reflectance_ratio=-1;

    if(!world.disable_fresnel_refraction)
    {
        double cosR;
        double cosI;
        double nR = refractive_index;
        double nI = REFRACTIVE_INDICES::AIR;
        if(is_exiting){
            nR = nI;
            nI = refractive_index;
        }

        vec3 r = (ray.direction - dot(ray.direction, same_side_normal) * same_side_normal);
        cosI = dot(-1.0 * ray.direction, same_side_normal);
        reflectance_ratio = 1;
        cosR = 1.0 - pow(nI/nR, 2) * (1.0 - pow(cosI, 2));
        if(cosR >= 0){
            cosR = sqrt(cosR);
            Ray refraction;
            refraction.endpoint = intersection_point;
            refraction.direction = (nI/nR) * r - (cosR * same_side_normal);
            refraction.direction = refraction.direction.normalized();
            refraction_color = world.Cast_Ray(refraction, recursion_depth + 1);

            double rPar = pow((nR * cosI - nI * cosR)/(nR * cosI + nI * cosR), 2);
            double rPer = pow((nI * cosI - nR * cosR)/(nI * cosI + nR * cosR), 2);
            reflectance_ratio = (rPar + rPer)/2.0;
        }
    }

    if(!world.disable_fresnel_reflection){
        vec3 r = (ray.direction - 2.0 * dot(ray.direction, same_side_normal) * same_side_normal).normalized();
        Ray reflection;
        reflection.endpoint = intersection_point;
        reflection.direction = r;
        reflection_color = world.Cast_Ray(reflection, recursion_depth + 1);
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

