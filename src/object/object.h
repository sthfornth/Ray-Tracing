
#ifndef _OBJECT_OBJECT_H
#define _OBJECT_OBJECT_H
#include "../core/vec3f.h"
#include "../core/ray.h"
#include "../material/material.h"

class Intersection;
class Object;
class Scene;

class Intersection {
public:
    Object *object;
    double distance;
    Vec3f position;
    Vec3f norm;
    const static Intersection null;

    Intersection(void) : object(NULL), distance(INF) { }
    Intersection(Object* obj, double d, Vec3f p, Vec3f n)
    	:object(obj), distance(d), position(p), norm(n){

    }
    bool operator <(const Intersection i) const{
    	return this->distance < i.distance;
    }
};

class Scene {
public:
    vector<Object *> objects;

    Scene(void) { }

    Scene *add(Object *obj) {
        objects.push_back(obj);
        return this;
    }

    Intersection intersect(const Ray &ray);
};


class Object {
public:
    union {
        Material *material;
        Light *light;
    };
    bool is_light;

    Object(void){// : material(NULL), is_light(false) {

    }

    virtual Intersection intersect(const Ray &ray) { 
        //puts("inside object intersect");
    	return Intersection::null; 
    }
    //virtual void sample(RandomStream *rng, Ray &ray, double &pdf) = 0;

    Object *set_material(Material *const m) {
        material = m, is_light = false;
        return this;
    }
    Object *set_light(Light *const l) {
        light = l, is_light = true;
        return this;
    }
	Object *add_to_scene(Scene *s) {
        s->add(this);
        return this;
    }
};

class Triangle : public Object
{
public:
	Triangle(Vec3f a, Vec3f b, Vec3f c)
		:a(a), b(b), c(c) 
		{
			n = (a - b) ^ (a - c);
			n.Normalize();
		}
	~Triangle();
	Vec3f a, b, c;	//vertices
	Vec3f n;	//normal vector
	virtual Intersection intersect(const Ray& ray){
        //puts("inside tri.intersect");
        double d = (a - ray.origin) & n;
		double v = ray.direct & n;
		double t = d / v;
        //printf("%lf\n", t);
		if(t < EPS)	//no intersection of the plane
			return Intersection :: null;
		Vec3f p = ray.get(t);
		if(!(sameSide(a, b, c, p) && sameSide(c, a, b, p) && sameSide(b, c, a, p)))
			return Intersection :: null;
		return Intersection(this, t, p, n);
	}
	bool sameSide(Vec3f a, Vec3f b, Vec3f c, Vec3f p){//p&c at the same side of ab
		//printf("c p\n");
        return (((a - b) ^ (a - c)) & ((a - b) ^ (a - p))) > EPS;
	}
};

class Mesh : public Object
{
public:
	Mesh():mesh(){}
	~Mesh();
	vector<Triangle*> mesh;
	virtual Intersection intersect(const Ray& ray){
        //puts("inside mesh.intersect");
		Intersection res = Intersection :: null;
		for (int i = 0; i < mesh.size(); ++i)
			res = min(res, mesh[i]->intersect(ray));
        res.object = this;
		return res;
	}
	void read(const char* path, Vec3f scale = Vec3f(1, 1, 1), Vec3f shift = Vec3f::ZERO);
};

class BBox : public Object  //Bounding Box
{
public:
    BBox(Mesh* mesh): mesh(mesh){
        vmax = Vec3f(-INF, -INF, -INF);
        vmin = Vec3f(INF, INF, INF);
        for (auto i : mesh->mesh)
            for (int j = 0; j < 3; ++j){
                vmax[j] = max(vmax[j], max(i->a[j], max(i->b[j], i->c[j])));
                vmin[j] = min(vmin[j], min(i->a[j], min(i->b[j], i->c[j])));
            }
    }
    ~BBox();
    Mesh* mesh;
    Vec3f vmax, vmin;
    virtual Intersection intersect(const Ray& ray){
        if(!is_intersect(ray))
            return Intersection::null;
        return mesh->intersect(ray);
    }

    inline bool is_intersect(const Ray &ray) {
        Vec3f result_min = vmin - ray.origin;
        Vec3f result_max = vmax - ray.origin;
        result_min = result_min / (ray.direct + EPS);
        result_max = result_max / (ray.direct + EPS);
        for (int i = 0; i < 3; ++i)
            if (result_min[i] > result_max[i])
                swap(result_min[i], result_max[i]);
        double near = result_min[0], far = result_max[0];
        for (int i = 1; i < 3; ++i){
            near = max(near, result_min[i]);
            far = min(far, result_max[i]);
        }
        return !(near > far || far < 0);
    }
};

class Quad: public Object
{
public:
    Quad(Vec3f a, Vec3f b, Vec3f c, Vec3f d){
        t1 = new Triangle(a, b, c);
        t2 = new Triangle(a, c, d);
    }
    Triangle* t1;
    Triangle* t2;
    virtual Intersection intersect(const Ray& ray){
        Intersection inter = min(t1->intersect(ray), t2->intersect(ray));
        inter.object = this;
        return inter;
    }
};

class Sphere: public Object
{
public:
    Sphere(const Vec3f& center, double radius): center(center), radius(radius){
        sqrrad = radius * radius;
    }
    Sphere(double x, double y, double z, double r) {
        center.x = x, center.y = y, center.z = z;
        radius = r, sqrrad = r * r;
    }
    Vec3f center;
    double radius;
    double sqrrad;
    virtual Intersection intersect(const Ray& ray){
        Vec3f v = center - ray.origin;
        double b = dot(v, ray.direct);
        double det = b * b - v.L2Norm_Sqr() + sqrrad;
        if (det > 0) {
            double sdet = sqrt(det);
            double distance = 0;
            if (b - sdet > EPS)
                distance = b - sdet;
            else if (b + sdet > EPS)
                distance = b + sdet;
            if (distance > EPS) {
                Intersection res;
                res.object = static_cast<Object *>(this);
                res.distance = distance;
                res.position = ray.get(distance);
                res.norm = (res.position - center).norm();
                return res;
            }
        }
        return Intersection::null;
    }

};

#endif