#include "block.hpp"

int main(){
    auto  blocks=
    repeat(7,
        repeat(16,
            right_degrees(29.0f),
            forward_steps(-38.0f)
        ),
        repeat(16,
            right_degrees(-19.0f),
            forward_steps(10.0f)
        ),
        right_degrees(47.0f),
        forward_steps(10.0f)
    );
    uint32_t dump_size{1000};
    float one_step_length = 2.0f / dump_size;
    Turtle turtle = defaul_turtle(one_step_length);
    // auto blocks = repeat(1,right_degrees(45),forward_steps(100));
    blocks->drive(turtle);
    draw_axis();
    PPMDumper::dump("turtle", dump_size, dump_size);
    return 0;
}