#version 460 core
out vec4 FragColor;

in VData {
	vec3 VPos;
} FVertData;

vec3 XPosNorm = normalize(FVertData.VPos + vec3(0.5, 0.0, 0.0));
vec3 XNegNorm = normalize(FVertData.VPos + vec3(-0.5, 0.0, 0.0));
vec3 YPosNorm = normalize(FVertData.VPos + vec3(0.0, 0.5, 0.0));
vec3 YNegNorm = normalize(FVertData.VPos + vec3(0.0, -0.5, 0.0));
vec3 ZPosNorm = normalize(FVertData.VPos + vec3(0.0, 0.0, 0.5));
vec3 ZNegNorm = normalize(FVertData.VPos + vec3(0.0, 0.0, -0.5));

uniform vec3 LightPos;
uniform vec3 CameraPos;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

void main() {
	vec3 ObjColor = {.09f, .15f, .95f};

	vec3 Ambient = (0.1 * vec3(1.0f, 1.0f, 1.0f)) * ObjColor;

	vec3 LightDirection = normalize(LightPos - YPosNorm);
	float Diffrence = max(dot(YPosNorm, LightDirection), 0.0);
	vec3 Diffuse = Diffrence * vec3(1.0f, 1.0f, 1.0f);

	vec3 ViewDirection = normalize(LightPos - YPosNorm);
	vec3 ReflectionDirection = reflect(-LightDirection, YPosNorm);
	float PartialSpec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), 64);

	vec3 Specular = (0.5 * PartialSpec * vec3(1.0f, 1.0f, 1.0f));

	vec4 FinalColor = vec4( ((Ambient + Diffuse + Specular) * ObjColor), 1.0f);

    FragColor = FinalColor;
}
