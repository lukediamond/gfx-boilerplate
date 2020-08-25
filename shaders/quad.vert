#version 400 core

layout(location=0) in vec2 in_pos;
layout(location=1) in vec2 in_coord;

out vec2 pass_coord;

uniform vec2 u_pos;
uniform vec2 u_size;
uniform vec2 u_res;

void main() {
    pass_coord = in_coord;
    vec2 pos = ((in_pos * 0.5 + 0.5) * u_size * 2.0 + u_pos);
    gl_Position.xy = vec2(-1.0, 1.0) + pos * vec2(1.0, -1.0) / u_res;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
}