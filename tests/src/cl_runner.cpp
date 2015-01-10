/* 
* @Author: BlahGeek
* @Date:   2015-01-07
* @Last Modified by:   BlahGeek
* @Last Modified time: 2015-01-10
*/

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "hp/trace_runner_cl.h"
#include "hp/scene_cl/base.h"

using namespace hp;

#define ASSIGN_F3(X, Y) \
    do { \
        (X).s[0] = (Y)[0]; \
        (X).s[1] = (Y)[1]; \
        (X).s[2] = (Y)[2]; \
    } while(0)

TEST(CLRunner, run) {
    auto scene = std::make_unique<cl::Scene>(std::string("obj/cornell_box.obj"));

    Vec _view_p(250, 250, -500);
    cl_float3 view_p;
    ASSIGN_F3(view_p, _view_p);
    std::vector<cl_float3> view_dir;
    // std::vector<Vec> view_dir;
    for(int i = 0 ; i < 500 ; i += 1) {
        for(int j = 0 ; j < 500 ; j += 1) {
            Vec dir = Vec(i, j, 0) - _view_p;
            dir.normalize();
            cl_float3 x;
            ASSIGN_F3(x, dir);
            view_dir.push_back(x);
        }
    }

    auto runner = std::make_unique<cl::TraceRunner>(std::move(scene),
                                                    std::move(view_dir),
                                                    view_p);
    runner->run();

    std::ofstream fout("out.ppm");
    fout << "P3\n";
    fout << "500 500\n255\n";
    for(int y = 500 - 1 ; y >= 0 ; y --) {
        for(int x = 0 ; x < 500 ; x ++) {
            for(int k = 0 ; k < 3 ; k += 1) {
                auto val = runner->result[x * 500 + y + k];
                fout << int(255 * (1.0 - std::exp(-val))) << " ";
            }
        }
    }
    fout.close();
}
