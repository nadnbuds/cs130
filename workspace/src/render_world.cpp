#include <vector>
#include <limits>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
	Object* closestObj = NULL;
	double minT = std::numeric_limits<double>::max();
	for(Object* var : objects)
	{
		std::vector<Hit> hits;
		var->Intersection(ray, hits);
		for(Hit h : hits) {
			if (h.t < minT && h.t > small_t) {
				minT = h.t;
				hit = h;
				closestObj = var;
			}
		}
	}
    return closestObj;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray;
	ray.endpoint = camera.position;
	ray.direction = (camera.position - camera.World_Position(pixel_index)).normalized();
    vec3 color=Cast_Ray(ray,0);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
	int rd = recursion_depth;
	Hit objHit;
	objHit.object	   = NULL;
	objHit.t		   = 0;
	objHit.ray_exiting = false;
	Object* obj = Closest_Intersection(ray, objHit);
    vec3 color;
	//If there is an Obj for the ray
	if (obj != NULL) {
		vec3 point = ray.Point(objHit.t);
		vec3 normal = obj->Normal(point);
		if (objHit.ray_exiting) {
			normal *= -1;
		}
		color = obj->material_shader->Shade_Surface(ray,
			point,
			normal,
			rd,
			objHit.ray_exiting);
	}
	//If there is no Obj, use the background
	else {
		vec3 dummy;
		color = background_shader->Shade_Surface(ray, dummy, dummy, rd, false);
	}
	rd++;
    return color;
}
