#version 430

layout(binding=0) uniform sampler2DArray tex;

in vec4 COL;
out vec4 color;
void main(void){
	color=vec4(texture(tex,COL.xyz));
}

