#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 16) out;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

vec4 X = vec4(1.0, 0.0, 0.0, 0.0);
vec4 Y = vec4(0.0, 1.0, 0.0, 0.0);
vec4 Z = vec4(0.0, 0.0, 1.0, 0.0);
vec4 XY = vec4(1.0, 1.0, 0.0, 0.0);
vec4 YZ = vec4(0.0, 1.0, 1.0, 0.0);
vec4 XZ = vec4(1.0, 0.0, 1.0, 0.0);
vec4 XYZ = vec4(1.0, 1.0, 1.0, 0.0);

void main() {
    // Note: 15 Vert Method, May one day be optimized to 12.

    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + X);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + Z);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + YZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XYZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + Y);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XY);
    EmitVertex();

    EndPrimitive();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XYZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + XY);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + X);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + Y);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + YZ);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position + Z);
    EmitVertex();

    gl_Position = MatrixMult * (gl_in[0].gl_Position); // Root: Local (0,0)
    EmitVertex();

    EndPrimitive();
}