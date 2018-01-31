#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    double top = dot(normal, (x1 - ray.endpoint));
    double bot = dot(normal, ray.direction);
    double t = (top/bot);
    if(bot != 0){
        if (t >= 0){
            Hit h;
            h.object = this;
            h.ray_exiting = false;
            h.t = t;
            hits.push_back(h);
            return true;
        }
    }
    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
