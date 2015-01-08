/* 
* @Author: BlahGeek
* @Date:   2015-01-07
* @Last Modified by:   BlahGeek
* @Last Modified time: 2015-01-08
*/

#include "./trace_runner.h"
#include "./scene/base.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace hp;

void TraceRunner::log() {
    std::cerr << "Loop "
        << ": s0 " << s0.size() 
        << ", s1 " << s1.size() 
        << ", s2_refract " << s2_refract.size() 
        << ", s2_specular " << s2_specular.size() 
        << ", s2_diffuse " << s2_diffuse.size() << std::endl;
}

void TraceRunner::run() {
    std::srand(std::time(0));

    s0.clear(); s1.clear(); 
    s2_specular.clear(); s2_refract.clear(); s2_diffuse.clear();

    result.resize(view_dir.size(), Color(0, 0, 0));
    int orig_id = 0;

    for(auto & dir: view_dir)
        s0.push_back({
            .orig_id = orig_id++,
            .depth = 0,
            .strength = Color(1, 1, 1),
            .start_p = view_p,
            .in_dir = dir
        });

    for(int i = 0 ; i < 5 ; i += 1) {

        log();

        for(auto & x: s0)
            x.run(scene.get(), s1);
        s0.clear();

        log();

        for(auto & x: s1)
            x.run(result, s2_refract, s2_specular, s2_diffuse);
        s1.clear();
        
        log();

        for(auto & x: s2_refract)
            x.run(s0);
        s2_refract.clear();
        
        log();

        for(auto & x: s2_diffuse)
            x.run(s0);
        s2_diffuse.clear();
        
        log();

        for(auto & x: s2_specular)
            x.run(s0);
        s2_specular.clear();
        
        log();
    }
    
}
