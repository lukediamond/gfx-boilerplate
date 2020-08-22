#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform sampler2D u_image;

void main() {
    out_color = vec4(texture(u_image, pass_coord).rgb, 1.0);
}