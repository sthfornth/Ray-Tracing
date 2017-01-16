#include "core/common.h"
#include "object/object.h"
#include "renderer/camera.h"
#include "renderer/render.h"
#include "core/canvas.h"

Vec3f expand(const Vec3f &source, int axis, double delta) {
    Vec3f result = source;
    result[axis] += delta;
    return result;
}

void make_scene2(Scene* scene){
	Vec3f l00(-5.0, 9.99, -5.0);
    Vec3f l01(-5.0, 9.99,  5.0);
    Vec3f l10( 5.0, 9.99, -5.0);
    Vec3f l11( 5.0, 9.99,  5.0);
    Object *lt1 = new Quad(l00, l10, l11, l01);
    lt1->set_light(new SimpleLight(Vec3f(4., 4., 4.)))->add_to_scene(scene);

    Vec3f delta(0, 0, 15);
    Object *lt2 = new Quad(l00 + delta, l10 + delta, l11 + delta, l01 + delta);
    lt2->set_light(new SimpleLight(Vec3f(4., 4., 4.)))->add_to_scene(scene);

    delta = delta * 2;
    Object *lt3 = new Quad(l00 + delta, l10 + delta, l11 + delta, l01 + delta);
    lt3->set_light(new SimpleLight(Vec3f(4., 4., 4.)))->add_to_scene(scene);

    Vec3f v000(-10.0, -10.0, -10.0);
    Vec3f v100( 10.0, -10.0, -10.0);
    Vec3f v010(-10.0,  10.0, -10.0);
    Vec3f v001(-10.0, -10.0,  50.0);
    Vec3f v110( 10.0,  10.0, -10.0);
    Vec3f v101( 10.0, -10.0,  50.0);
    Vec3f v011(-10.0,  10.0,  50.0);
    Vec3f v111( 10.0,  10.0,  50.0);

    Object *ceilWall = new Quad(v010, v110, v111, v011);
    Object *floorWall = new Quad(v000, v001, v101, v100);
    Object *backWall = new Quad(v010, v000, v100, v110);
    Object *leftWall = new Quad(expand(v000, 0, 0.1), expand(v010, 0, 0.1), expand(v011, 0, 0.1), expand(v001, 0, 0.1));
    Object *rightWall = new Quad(expand(v100, 0, -0.1), expand(v101, 0, -0.1), expand(v111, 0, -0.1), expand(v110, 0, -0.1));

    floorWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture(Vec3f(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    ceilWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture(Vec3f(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    backWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture(Vec3f(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    leftWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture(Vec3f(0.75, 0.25, 0.25)))
    ))->add_to_scene(scene);
    rightWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture(Vec3f(0.25, 0.25, 0.75)))
    ))->add_to_scene(scene);

}

void make_scene1(Scene* scene){
	Mesh* mesh = new Mesh();
	mesh->read("cornell_box.obj");
	scene->add(mesh);

	// Object* light = new Quad(Vec3f(643, 548.7, 27), Vec3f(643 , 548.7, 632), 
			// Vec3f(1, 548.7, 632), Vec3f(1, 548.7, 27));
	Object* light = new Quad(Vec3f(343, 548.7, 227), Vec3f(343 , 548.7, 332), 
			Vec3f(213, 548.7, 332), Vec3f(213, 548.7, 227));
	light -> set_light(new SimpleLight(Vec3f(36, 36, 36)))->add_to_scene(scene);
}

int main()
{

	Camera* camera = new Camera(Vec3f(277, 272, -400), Vec3f(0, 0, 1), Vec3f(1, 0, 0), 60);
	Scene* scene = new Scene();
	make_scene1(scene);
	int h = 300, w = 300;
	Canvas* canvas = new Canvas(h, w, 3);

	//Render* render = new DepthRender(scene);
	int Times;
	scanf("%d", &Times);
	Render* render = new PathTracingRender(scene, Times);
	render->render(camera, canvas);
	canvas->show("depth");
	canvas->write("pt1.jpg");

	return 0;
}