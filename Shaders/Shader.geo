#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 16) out;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

float CubeScale = 1.0;
vec4 X = vec4(CubeScale, 0.0, 0.0, 0.0);
vec4 Y = vec4(0.0, CubeScale, 0.0, 0.0);
vec4 Z = vec4(0.0, 0.0, CubeScale, 0.0);
vec4 XY = vec4(CubeScale, CubeScale, 0.0, 0.0);
vec4 YZ = vec4(0.0, CubeScale, CubeScale, 0.0);
vec4 XZ = vec4(CubeScale, 0.0, CubeScale, 0.0);
vec4 XYZ = vec4(CubeScale, CubeScale, CubeScale, 0.0);

in VData {
	vec3 VPos;
} VertData[1];

out VData {
	vec3 VPos;
} FVertData;

void main() {
	FVertData.VPos = VertData[0].VPos;
    // Note: 15 Vert Method, May one day be optimized to 12.

    gl_PrimitiveID = 5;
    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    gl_PrimitiveID = 4;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + X);
    EmitVertex();

    gl_PrimitiveID = 4;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + Z);
    EmitVertex();

    gl_PrimitiveID = 6;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XZ);
    EmitVertex();

    gl_PrimitiveID = 3;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + YZ);
    EmitVertex();

    gl_PrimitiveID = 1;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XYZ);
    EmitVertex();

    gl_PrimitiveID = 2;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + Y);
    EmitVertex();

    gl_PrimitiveID = 3;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XY);
    EmitVertex();

    EndPrimitive();

    gl_PrimitiveID = 1;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XYZ);
    EmitVertex();

    gl_PrimitiveID = 6;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XZ);
    EmitVertex();

    gl_PrimitiveID = 3;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + XY);
    EmitVertex();

    gl_PrimitiveID = 4;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + X);
    EmitVertex();

    gl_PrimitiveID = 2;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + Y);
    EmitVertex();

    gl_PrimitiveID = 5;
    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    gl_PrimitiveID = 3;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + YZ);
    EmitVertex();

    gl_PrimitiveID = 4;
    gl_Position = MatrixMult * (gl_in[0].gl_Position + Z);
    EmitVertex();

    gl_PrimitiveID = 5;
    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    EndPrimitive();
}