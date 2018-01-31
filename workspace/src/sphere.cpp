#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
	vec3 u = ray.direction;
	vec3 p = ray.endpoint - center;
	double deltaPrime = 
	pow(dot(u, p),2) 
	- (p.magnitude_squared() - pow(radius,2));
	
	//Postive Delta prime means a hit
	if (deltaPrime > 0) {
		double b  = (- 1 * dot(u, p));
		double t1 = (b - sqrt(deltaPrime));
		double t2 = (b + sqrt(deltaPrime));
		Hit hit2;
		hit2.object      = this;
		hit2.t           = t2;
		hit2.ray_exiting = true;
		hits.push_back(hit2);
		//Check if there are two intersections (Ray starts outside)
		if (t1 > 0) {
			Hit hit1;
			hit1.object      = this;
			hit1.t           = t1;
			hit1.ray_exiting = false;
			hits.push_back(hit1);
		}
		return true;
	}

    return false;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
	normal = (point - center).normalized();
    return normal;
}
