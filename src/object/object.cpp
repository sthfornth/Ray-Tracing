#include "object.h"
#include "../material/material.h"
const Intersection Intersection::null = Intersection();

Intersection Scene::intersect(const Ray &ray){
	//puts("inside scene.intersect");
	// printf("%d\n", objects.size());
    Intersection res = Intersection :: null;
    for (int i = 0; i < objects.size(); ++i)
        res = min(res, objects[i]->intersect(ray));
    return res;
}

void Mesh::read(const char* path, Vec3f scale, Vec3f shift){
    FILE* fp = fopen(path, "r");
    if (fp == NULL){
        printf("File not exists\n");
        return;
    }
    char type[10];
    char buf[1005];  
    int lines = 0;
    vector<Vec3f> pSet;
    Material* cur;
	while (fscanf(fp, "%s", type)!=EOF){  
		if ((++ lines) % 10000 == 0)  
			printf("line %d %c\n",lines, type[0]);
		if (type[0] == '#')  //usemtl
			fgets(buf, sizeof(buf), fp);
		else if (type[0] == 'u'){
			fscanf(fp, "%s", buf);
			Vec3f color;
			if (buf[0] == 'w') //white
				color = Vec3f(0.8, 0.8, 0.8);
			else if (buf[0] == 'b') //blue
				color = Vec3f(0.2, 0.2, 0.8);
			else if (buf[0] == 'r') //red
				color = Vec3f(0.8, 0.2, 0.2);
			else if (buf[0] == 'g') //green
				color = Vec3f(0.2, 0.8, 0.2);
			cur = new SimpleMaterial(new LambertianBRDF(new ConstantTexture(color)));
		}else if (type[0] == 'v'){  
			double x, y, z;  
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);  
			Vec3f pos(x, y, z);
			pos = pos * scale + shift;
			pSet.push_back(pos);
		}else if (type[0] == 'f'){  
			int a, b, c, n = pSet.size();
			fscanf(fp, "%d %d %d", &a, &b, &c);
			a = (a + n - 1)%n;
			b = (b + n - 1)%n;
			c = (c + n - 1)%n;
 			Triangle* tri = new Triangle(pSet[a], pSet[b], pSet[c]);
 			tri->set_material(cur);
			mesh.push_back(tri);
		}  
	}  
}