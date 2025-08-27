#version 460 core
out vec4 FragColor;

in VData {
	vec3 VPos;
} FVertData;

float NormalScale = 1.0;
vec3 XPosNorm = {NormalScale, 0.0, 0.0};
vec3 XNegNorm = {-NormalScale, 0.0, 0.0};
vec3 YPosNorm = {0.0, NormalScale, 0.0};
vec3 YNegNorm = {0.0, -NormalScale, 0.0};
vec3 ZPosNorm = {0.0, 0.0, NormalScale};
vec3 ZNegNorm = {0.0, 0.0, -NormalScale};

vec3 PointNorm = FVertData.VPos;

uniform vec3 LightPos;
uniform vec3 CameraPos;

void main() {
	if ((gl_PrimitiveID) == 1){
		PointNorm = normalize(PointNorm + XPosNorm);
	}
	if ((gl_PrimitiveID) == 2){
		PointNorm = normalize(PointNorm + XNegNorm);
	}
	if ((gl_PrimitiveID) == 3){
		PointNorm = normalize(PointNorm + YPosNorm);
	}
	if ((gl_PrimitiveID) == 4){
		PointNorm = normalize(PointNorm + YNegNorm);
	}
	if ((gl_PrimitiveID) == 5){
		PointNorm = normalize(PointNorm + ZPosNorm);
	}
	if ((gl_PrimitiveID) == 6){
		PointNorm = normalize(PointNorm + ZNegNorm);
	}

	vec3 ObjColor = {.09f, .15f, .90f};

	// Section: Ambient
	vec3 Ambient = (0.1 * vec3(1.0f, 1.0f, 1.0f)) * ObjColor;

	// Section: Diffuse
	vec3 LightDirection = normalize(LightPos - PointNorm);
	float Diffrence = max(dot(PointNorm, LightDirection), 0.0);
	vec3 Diffuse = Diffrence * vec3(1.0f, 1.0f, 1.0f);

	// Section: Specular
	vec3 ViewDirection = normalize(CameraPos - LightPos);
	vec3 ReflectionDirection = reflect(-LightDirection, PointNorm);
	float PartialSpec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), 32);

	vec3 Specular = (0.5 * PartialSpec * vec3(1.0f, 1.0f, 1.0f));

	vec4 FinalColor = vec4( ((Ambient + Diffuse + Specular) * ObjColor), 1.0f);

    FragColor = FinalColor;
}