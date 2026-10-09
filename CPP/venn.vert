#version 430

layout(std140, binding=0) uniform woof{
	vec4 colors; vec4 params;
};
layout(std140, binding=1) uniform square{
	vec4 pootis[7];
};
out vec2 point;
void main(void){
	gl_Position=pootis[gl_VertexID];
	point=gl_Position.xy;
	//color=gl_Position;
}
