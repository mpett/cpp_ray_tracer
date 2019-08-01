#define MAXFLOAT FLT_MAX
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;

    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;

        if (depth < 50 && rec.mat_ptr -> scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        } else {
            return vec3(0, 0, 0);
        }

    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0 * a);
    }
}

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = (rand() / (RAND_MAX + 1.0));
            vec3 center(a + 0.9 * (rand() / (RAND_MAX + 1.0)), 0.2, b + 0.9 * (rand() / (RAND_MAX + 1.0)));

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3((rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)) * (rand() / (RAND_MAX + 1.0)))));
                } else if (choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + (rand() / (RAND_MAX + 1.0))), 0.5 * ( 1 + (rand() / (RAND_MAX + 1.0)) ), 0.5 * (rand() / (RAND_MAX + 1.0))), 0.3));
                } else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            } 
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    
    return new hitable_list(list, i);
}

int main() {
    srand(time(0));
    int nx = 1920;
    int ny = 1080;
    int ns = 15;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[4];

    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, 5);
    world = random_scene();

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx) / float(ny), aperture, dist_to_focus);

    std::cerr << "\033[1;30;42mCOMPUTATION STARTING. DO NOT TERMINATE THIS MACHINE.\033[0m\n\n";

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            
            for (int s = 0; s < ns; s++) {
                float u = float(i + (rand() / (RAND_MAX + 1.0))) / float(nx);
                float v = float(j + (rand() / (RAND_MAX + 1.0))) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3 ( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }

        
        int ra_foreground = 37;
        int ra_background = int( (rand() / (RAND_MAX + 1.0)) * (7.0 + 1.0) + 40.0);
        std::cerr << "\033[1;" << ra_foreground << ";" << ra_background << "m" << " ROW " << j << " " << "\033[0m";
    }

    std::cerr << std::endl;
    std::cerr << "\033[1;30;42mCOMPUTATION WAS A GLORIOUS SUCCESS!!!\033[0m\n";
    std::cerr << "\n\033[1;31mALL OF YOUR BASE ARE BELONG TO US.\033[0m\n\n";
    std::getchar();
}
