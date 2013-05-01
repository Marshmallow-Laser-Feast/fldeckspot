#extension GL_EXT_gpu_shader4 : require

flat varying vec3  normal;        // note 'flat varying'
varying vec4 vert;
uniform sampler2DRect tex0;
//varying vec3 texColor;
uniform float timeValX;

// varying vec3 ecNormal;
// varying vec3 lightDir;


void main(){
	normal = gl_NormalMatrix * gl_Normal;
	vert = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	//ecNormal = normalize(normal);
	//lightDir = normalize(lightPosition.xyz - vert);

	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;

	
	//vert = gl_Position;
	//texColor = texture2DRect(tex0, gl_TexCoord[0].st).rgb;
}