#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform vec2 u_size;

void main() {
    vec2 coord = u_size * (pass_coord * 2.0 - 1.0);
    float radius = 64.0;
    if (
        abs(coord.x) > u_size.x - radius * 2.0 &&
        abs(coord.y) > u_size.y - radius * 2.0
    ) {
        vec2 corner = coord / abs(coord);
        corner = corner * u_size - corner * radius * 2.0;
        if (distance(corner, coord) >= radius * 2.0) discard;
    }

    out_color = vec4(vec3(1.0), 1.0);
}