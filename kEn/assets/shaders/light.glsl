#include "material"

struct attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct directional_light {
	vec3 color;

	vec3 dir;
}; 

struct point_light {
	vec3 color;

	attenuation atten;

	vec3 pos;
}; 

struct spot_light {
	vec3 color;

	attenuation atten;

	vec3 pos;
	vec3 dir;
	float cutoff;
	float outerCutoff;
}; 

const int MAX_LIGHTS = 15;

uniform bool u_UseBlinn = true;

uniform int u_DirectionalCount;
uniform directional_light u_DirectionalLights[MAX_LIGHTS];

uniform int u_PointCount;
uniform point_light u_PointLights[MAX_LIGHTS];

uniform int u_SpotCount;
uniform spot_light u_SpotLights[MAX_LIGHTS];

float calc_attenuation(attenuation atten, float dist) {
	return 1.0 / (atten.constant + dist * (atten.linear + dist * atten.quadratic));
}

vec2 calc_light(vec3 light_dir, vec3 normal, vec3 view_dir) {
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = max(dot(view_dir, reflect_dir), 0.0);
	
	return vec2(diff, spec);
}

vec2 calc_light_blinn(vec3 light_dir, vec3 normal, vec3 view_dir) {
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	float spec = max(dot(normal, halfway_dir), 0.0);
	
	return vec2(diff, spec);
}

vec3 calc_point_light(point_light light, material mat, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(light.pos - frag_pos), normal, view_dir) : calc_light(normalize(light.pos - frag_pos), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular) * calc_attenuation(light.atten, length(light.pos - frag_pos));
}

vec3 calc_dir_light(directional_light light, material mat, vec3 normal, vec3 view_dir) {
	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(-light.dir), normal, view_dir) : calc_light(normalize(-light.dir), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular);
}

vec3 calc_spot_light(spot_light light, material mat, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec3 light_dir = normalize(light.pos - frag_pos);

	float theta = dot(light_dir, normalize(-light.dir));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity;
	if(epsilon == 0)
		intensity = theta > light.cutoff ? 1 : 0;
	else
		intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);    

	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(light.pos - frag_pos), normal, view_dir) : calc_light(normalize(light.pos - frag_pos), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular) * calc_attenuation(light.atten, length(light.pos - frag_pos)) * intensity;
}