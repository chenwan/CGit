#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec3 fs_eye_vector;
in vec3 fs_color;
in vec3 light_color;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 diffuseColor = vec4(fs_color, 1.0);
	vec4 ambientColor = vec4(fs_color, 1.0);
	vec4 specularColor = vec4(light_color, 1.0);
    
    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(normalize(fs_normal), normalize(fs_light_vector)), 0.0, 1.0);

	float specularTerm = pow(clamp(dot(normalize(2 * normalize(fs_normal) * dot(normalize(fs_light_vector), fs_normal) - normalize(fs_light_vector)), normalize(fs_eye_vector)), 0.0, 1.0), 10);
    
    out_Color = diffuseColor * diffuseTerm * 0.6 + ambientColor * 0.2 + specularColor * specularTerm * 0.4;
}