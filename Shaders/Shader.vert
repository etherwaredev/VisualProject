#version 460 core
layout(location = 0) in vec3 Pos;

out VData {
	vec3 VPos;
} VertData;

void main() {
	VertData.VPos = Pos;
    gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0) - vec4(.5, .5, .5, 0.0);
}