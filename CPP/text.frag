#version 430

layout(binding=0) uniform sampler2DArray tex;

layout(std140, binding=0) uniform woof{
	vec4 colors; 
	vec4 params;
};

in vec4 COL;
out vec4 color;
void main(void){
	color=vec4(texture(tex,COL.xyz));
	color.yx*=(1-params.y);
}

