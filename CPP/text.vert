#version 430

layout(std140, binding=2) uniform barka{
	vec4 full_pos; uint I;
};

layout(std140,binding=3) uniform text_machine{
	uvec4 ses[10];
};

layout(std140, binding=1) uniform square{
	vec4 pootis[7];
};

out vec4 COL;

void main(void){
	uint A=gl_VertexID%6, B=gl_VertexID/6, C=B/4, D=C/4;
	vec4 point=pootis[A];
	COL=point; COL.x=(COL.x+1.0)/2.0; COL.y=-(COL.y+1.0)/2.0; COL.z=(ses[D][C]>>8*B)&255;
	point.xy=point.xy*full_pos.x+full_pos.yz;
	point.x+=full_pos.x*B*2;
	point.z=-0.5;
	gl_Position=point;
}

