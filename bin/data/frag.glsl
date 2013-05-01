#version 120
#extension GL_EXT_gpu_shader4 : require
#extension GL_ARB_texture_rectangle : enable

flat varying vec3  normal;        // note 'flat varying'
varying vec4 vert;
uniform sampler2DRect tex0;
uniform float PI = 3.14159265358979; 
//varying vec3 texColor;
uniform float timeValX;
//uniform float lightIntensity;

// varying vec3 ecNormal;
// varying vec3 lightDir;

void main(){

	vec4 color;
	vec3 normal = normalize(normal); 
	vec2 pos = gl_TexCoord[0].st;


	//vec3 src = texture2DRect(tex0, pos).rgb;
	//float offset = sin(timeValX)*.5 + .5;
	//float g = sin(pos.x*PI*2.0*20.0)*.5+.5;

	// vec3 direction = normalize(lightDir);
 //  	vec3 normal = normalize(ecNormal);

 //  	lightIntensity = .75;
 //  	float intensity = max(0.0, dot(direction, normal))*lightIntensity;

 //  	vec4 tintColor = vec4(intensity, intensity, intensity, 1.0);
 	//gl_FragColor =  vec4(g,g,g, 1.0)
	// use texture coordinates
	//gl_FragColor =  vec4(texture2DRect(tex0, pos*2048.0).rgb, 1);

	//show UVs
	//gl_FragColor = vec4(pos, 1.0, 1.0); 
	//gl_FragColor = vec4(g,g,g,1.0);
	//show Normals
	gl_FragColor  = vec4(normal, 1);
	//gl_FragColor(offset, 1.0, 1.0, 1.0);
	//gl_FragColor(src,1.0);
}