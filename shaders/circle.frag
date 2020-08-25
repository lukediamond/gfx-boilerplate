#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform vec2 u_size;

void main() {
    vec2 coord = (pass_coord * 2.0 - 1.0);
    if (length(coord) >= 1.0) discard;
    out_color = vec4(vec3(1.0), 1.0);
}