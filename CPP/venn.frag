#version 430

in vec2 point;

layout(std140, binding=0) uniform woof{
	vec4 colors; 
	vec4 params;
};
layout(std140, binding=1) uniform square{
	vec4 pootis[7];
};

out vec4 color;

void main(void){
	bool a=length(point-pootis[6].zw)<0.5,b=length(point-pootis[6].xy)<0.5;
	//uint c;
	if(a&&b) color=vec4(colors[3],colors[3]*params.x,colors[3]*params.x,1.0);
	else if(b) color=vec4(colors[1],colors[1]*params.x,colors[1]*params.x,1.0);
	else if(a) color=vec4(colors[2],colors[2]*params.x,colors[2]*params.x,1.0);
	else color=vec4(colors[0],colors[0]*params.x,colors[0]*params.x,1.0);
	//color=vec4(colors[c],colors[c],colors[c],1.0);
}
