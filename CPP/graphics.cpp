#include "graphics.hpp"
#include "guha.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

#define NVAO 1
#define GL_UBO GL_UNIFORM_BUFFER
#define GL_SSB GL_SHADER_STORAGE_BUFFER

GLFWwindow* window;
GLuint RP, TP, vao[NVAO], letter_textures;
GLuint ubo[4], texs[4], poss[4];
void *ptr_ubo=0, *ptr_square=0, *ptr_buffdog=0, *ptr_textor=0, 
	*ptr_texs[4]={0,0,0,0},*ptr_poss[4]={0,0,0,0};
GLbitfield PRW=GL_MAP_PERSISTENT_BIT|GL_MAP_READ_BIT|GL_MAP_WRITE_BIT;
short pos=0;

void output(std::vector<int> t){
	std::vector<float> T{(float)t[0],(float)t[1],(float)t[2],(float)t[3]}; float n=0.0;
	for(unsigned int i=0; i<4; i++) n+=T[i];
	for(unsigned int i=0; i<4; i++) T[i]/=n;
	T.push_back(t[4]);T.push_back(t[5]);
	for(auto i:T) std::cout<<i<<" ";std::cout<<"\n";
	memcpy((char*)ptr_ubo,&T[0], 24);
}

int mean(std::vector<int> t, unsigned int p){ unsigned int n=0; 
	for(unsigned int i=0;i<t.size();i++) if(i==p) continue; else n+=t[i]; return n/(t.size()-1);
}

std::vector<int> diagram={0,0,0,0}, reserve=diagram, etalon=diagram;
int position=0; bool stats=0; std::vector<std::string> kiwi{"11", "B", "00", "A"};
void get_key(GLFWwindow* w, int key, int sc, int act, int mods){
	if(act==GLFW_RELEASE){
		switch(key){
			case GLFW_KEY_UP: pos++; reserve=diagram; diagram=get_node(pos); break;
			case GLFW_KEY_DOWN: pos--; reserve=diagram; diagram=get_node(pos); break;
			case GLFW_KEY_LEFT: diagram=get_node(pos); position++; break;
			case GLFW_KEY_RIGHT: diagram=get_node(pos); position--; break;
			case GLFW_KEY_SPACE: diagram=get_node(pos); stats=!stats; break;
			default: return; break;
		}if(position>3) position=0; else if(position<0) position=3;
		if(diagram.size()<2){ pos-=diagram[0]; etalon=diagram=reserve; return; }
		reserve=diagram;
		switch(position){
			case 1: diagram[0]=reserve[1]; diagram[1]=reserve[3]; diagram[2]=reserve[0]; diagram[3]=reserve[2]; break;
			case 2: diagram[0]=reserve[3]; diagram[1]=reserve[2]; diagram[2]=reserve[1]; diagram[3]=reserve[0]; break;
			case 3: diagram[0]=reserve[2]; diagram[1]=reserve[0]; diagram[2]=reserve[3]; diagram[3]=reserve[1]; break;
			default: break;
		}kiwi=get_names(pos);
		if(stats) diagram[0]=mean(reserve,0);
		diagram.push_back(stats);
		output(diagram);
	}
}

void output_text(int size, int x, int y, std::string lol, GLuint pos, void*pp, GLuint arr, void*ptr){
	//std::cout<<"gluint: "<<arr<<"; ptr: "<<ptr<<"\n";
	float F[3]={(float)size/512.0f, (float)x/512.0f, (float)y/512.0f}; F[1]-=F[0]*lol.size();
	glBindBufferBase(GL_UBO,2,pos);
	glBindBufferBase(GL_UBO,1,ubo[1]);
	glBindBufferBase(GL_UBO,3,arr);
	memcpy((char*)pp,F,12);
	memcpy((char*)ptr,lol.c_str(),lol.size());
	glUseProgram(TP);
	glDrawArrays(GL_TRIANGLES,0,6*lol.size());
}

void update(){
	while(!glfwWindowShouldClose(window)){
		glBindBufferBase(GL_UBO,0,ubo[0]);
		glBindBufferBase(GL_UBO,1,ubo[1]);
		glClearColor(0.0,1.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(RP);
		glDrawArrays(GL_TRIANGLES,0,6);
		output_text(50,50,0,kiwi[position%4],poss[0],ptr_poss[0],texs[0],ptr_texs[0]); //11
		output_text(50,300,-100,kiwi[(position+1)%4],poss[1],ptr_poss[1],texs[1],ptr_texs[1]); //B
		output_text(50,50,350,kiwi[(position+2)%4],poss[2],ptr_poss[2],texs[2],ptr_texs[2]);//00
		output_text(50,-200,100,kiwi[(position+3)%4],poss[3],ptr_poss[3],texs[3],ptr_texs[3]);//A
		glfwSwapBuffers(window); glfwPollEvents();
	}
}

class init{
	std::string RFF(std::string textfile){ //std::cout<<textfile<<"\n";
		std::string con="",line;
		std::ifstream file(textfile);
		if(!file) return "";
		while(!file.eof()){
			std::getline(file, line); con.append(line).append("\n");
		}file.close(); 
		//std::cout<<con;
		return con;
	}
	
	void CSP(GLuint &rp, std::string prog){
		std::string _FS=RFF("./"+prog+".frag"), _VS=RFF("./"+prog+".vert");
		const char *FS=_FS.c_str(), *VS=_VS.c_str();
		//std::cout<<FS;
		GLuint vs=glCreateShader(GL_VERTEX_SHADER), fs=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vs,1,&VS,0); glCompileShader(vs);
		glShaderSource(fs,1,&FS,0); glCompileShader(fs);
		
		int ss; bool killer=true; //trouble shooter
		glGetShaderiv(vs, GL_COMPILE_STATUS, &ss);
		std::cout<<((killer&=(ss==GL_TRUE))? 
			"vertex shader "+prog+" ready.":
			"vertex shader "+prog+" did not compile.")<<"\n";
		
		glGetShaderiv(fs, GL_COMPILE_STATUS, &ss);
		std::cout<<((killer&=(ss==GL_TRUE))? 
			"fragment shader "+prog+" ready.":
			"fragment shader "+prog+" did not compile.")<<"\n";
		
		rp=glCreateProgram(); glAttachShader(rp, vs); glAttachShader(rp,fs); glLinkProgram(rp);
		
		glGetProgramiv(rp, GL_LINK_STATUS, &ss);
		std::cout<<((killer&=(ss==GL_TRUE))? 
			"linking of "+prog+" complete.":
			"linking of "+prog+" failed.")<<"\n";
		if(!killer) {
			std::cout<<"\nFATAL ERROR: graphics failed, aborting.\n";
			int size=1000, ll; char* out=new char[1000];
			glGetShaderInfoLog(vs, size, &ll, out);
			std::cout<<out<<"\n\n";
			glGetShaderInfoLog(fs, size, &ll, out);
			std::cout<<out<<"\n\n";
			glGetProgramInfoLog(rp, size, &ll, out);
			std::cout<<out<<"\n";//*/
			delete [] out;
			exit(3);
		}std::cout<<"PROGRAM: "<<rp<<"\n";
	}
public:
	init(){
		if(!glfwInit()) exit(1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window=glfwCreateWindow(512,512, "Venn Diagram", 0,0);
		glfwMakeContextCurrent(window);
		if(glewInit()!=GLEW_OK) exit(2);
		glfwSwapInterval(1);
		
		CSP(RP, "venn");
		CSP(TP, "text");
		glGenVertexArrays(NVAO,vao); glBindVertexArray(vao[0]);
		glGenBuffers(4, ubo);
		glBindBuffer(GL_UBO, ubo[0]); glBindBufferBase(GL_UBO,0,ubo[0]);
		glBufferStorage(GL_UBO,24,0,PRW); ptr_ubo=glMapBufferRange(GL_UBO,0,24,PRW);
		glBindBuffer(GL_UBO, ubo[1]); glBindBufferBase(GL_UBO,1,ubo[1]);
		glBufferStorage(GL_UBO,112,0,PRW); ptr_square=glMapBufferRange(GL_UBO,0,112,PRW);
		float the[28]={
			-1,-1,0,1,
			-1,1,0,1,
			1,1,0,1,
			-1,-1,0,1,
			1,-1,0,1,
			1,1,0,1,
			-0.25,0,0.25,0
		};memcpy((char*)ptr_square,the,112);
		glBindBuffer(GL_UBO, ubo[2]); glBindBufferBase(GL_UBO,2,ubo[2]);
		glBufferStorage(GL_UBO,16,0,PRW); ptr_buffdog=glMapBufferRange(GL_UBO,0,16,PRW);
		glBindBuffer(GL_UBO, ubo[3]); glBindBufferBase(GL_UBO,3,ubo[3]);
		glBufferStorage(GL_UBO,160,0,PRW); ptr_textor=glMapBufferRange(GL_UBO,0,160,PRW);
		
		glGenBuffers(4, texs); glGenBuffers(4, poss);
		for(unsigned int i=0; i<4; i++){ glBindBuffer(GL_UBO, texs[i]);
		glBufferStorage(GL_UBO,160,0,PRW); ptr_texs[i]=glMapBufferRange(GL_UBO,0,160,PRW); }
		for(unsigned int i=0; i<4; i++){ glBindBuffer(GL_UBO, poss[i]);
		glBufferStorage(GL_UBO,160,0,PRW); ptr_poss[i]=glMapBufferRange(GL_UBO,0,160,PRW); }
		
		glEnable(GL_DEPTH_TEST); glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glfwSetKeyCallback(window, get_key);
		
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1,&letter_textures);
		glBindTexture(GL_TEXTURE_2D_ARRAY,letter_textures);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //texture array
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //settings
		glTexStorage3D(GL_TEXTURE_2D_ARRAY,1, GL_RGBA8, 128,128,128);
		int wid, hgt;
		for(char _=33; _<127; _++){ std::string p="./letters/"+std::string(1,_)+".png";
			if(!std::ifstream(p)) continue;
			unsigned char* data=SOIL_load_image(p.c_str(),&wid,&hgt,0,SOIL_LOAD_RGBA);
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,_,wid,hgt,1,GL_RGBA,GL_UNSIGNED_BYTE,data);
			SOIL_free_image_data(data);
		}
	}
};

init _;
//COMPLETE
