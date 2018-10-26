//GLM 0.9.9
#include "tools/glm/glm.hpp" // vec2, vec3, mat4, radians
// Include all GLM extensions
#include "tools/glm/ext.hpp" // perspective, translate, rotate

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
using namespace std;

#define PI_M 3.14159265
/*
phi 0-180  
theta 0-360
(0.0,0.0,0.5)
x=sin(phi*(PI_M/180))*cos(theta*(PI_M/180))
y=sin(phi*(PI_M/180))*sin(theta*(PI_M/180))
z=cos(phi*(PI_M/180))

x=sin(i*difphi*(PI_M/180))*cos(j*diftheta*(PI_M/180))
y=sin(i*difphi*(PI_M/180))*sin(j*diftheta*(PI_M/180))
z=cos(i*difphi*(PI_M/180))
tx=theta/360
ty=1-(phi/180)
float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
*/
int main(){
    ofstream myfile;
    myfile.open ("modelAtmosphere.vao");
    myfile << "Writing this to a file.\n";
    /*
    float difphi=10;
    float diftheta=10;
    int nLatitud=(180/difphi)+1;
    int nLong=(360/diftheta)+1;
    */
    int resolLat=18;
    int resolLong=36;
    float difphi=180./resolLat;
    float diftheta=360./resolLong;
    //polo norte
    //myfile<<fixed << setprecision(1)<<0.0<<", "<<0.0<<", "<<1.0<<", "<<0.0<<", "<<0.0<<", "<<1.0<<", "<<0.0<<", "<<1.0<<","<<endl;
    for(int i=0;i<resolLong;i++){
        myfile<<fixed << setprecision(1)<<0.0<<", "<<0.0<<", "<<1.0<<", "<<0.0<<", "<<0.0<<", "<<-1.0<<", "<<0.0<<", "<<1.0<<","<<endl;
        myfile<<fixed << setprecision(6)<<sin(difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<sin(difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<cos(difphi*(PI_M/180))<<", ";//position
        myfile<<fixed << setprecision(6)<<(-1)*sin(difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<(-1)*sin(difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<(-1)*cos(difphi*(PI_M/180))<<", ";//normal
        myfile<<fixed << setprecision(6)<<(i*(diftheta/360))<<", "<<1-(difphi/180)<<","<<endl;//texCoord
        myfile<<fixed << setprecision(6)<<sin(difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<sin(difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<cos(difphi*(PI_M/180))<<", ";//position
        myfile<<fixed << setprecision(6)<<(-1)*sin(difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*sin(difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*cos(difphi*(PI_M/180))<<", ";//normal
        myfile<<fixed << setprecision(6)<<((i+1)*(diftheta/360))<<", "<<1-(difphi/180)<<","<<endl;//texCoord
    }

    //cuerpo    
    for(int j=1;j<resolLat-1;j++){
        for(int i=0;i<resolLong;i++){
            //j,i
            myfile<<fixed << setprecision(6)<<sin(j*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<sin(j*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<cos(j*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin(j*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<(-1)*sin(j*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<(-1)*cos(j*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-(i*(diftheta/360))<<", "<<1-((j*difphi)/180)<<","<<endl;
            //j+1,i
            myfile<<fixed << setprecision(6)<<sin((j+1)*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<sin((j+1)*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<cos((j+1)*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin((j+1)*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<(-1)*sin((j+1)*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<(-1)*cos((j+1)*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-(i*(diftheta/360))<<", "<<1-(((j+1)*difphi)/180)<<","<<endl;
            //j+1,i+1
            myfile<<fixed << setprecision(6)<<sin((j+1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<sin((j+1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<cos((j+1)*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin((j+1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*sin((j+1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*cos((j+1)*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-((i+1)*(diftheta/360))<<", "<<1-(((j+1)*difphi)/180)<<","<<endl;

            //j+1,i+1
            myfile<<fixed << setprecision(6)<<sin((j+1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<sin((j+1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<cos((j+1)*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin((j+1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*sin((j+1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*cos((j+1)*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-((i+1)*(diftheta/360))<<", "<<1-(((j+1)*difphi)/180)<<","<<endl;
            //j,i+1
            myfile<<fixed << setprecision(6)<<sin(j*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<sin(j*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<cos(j*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin(j*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*sin(j*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*cos(j*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-((i+1)*(diftheta/360))<<", "<<1-((j*difphi)/180)<<","<<endl;
            //j,i
            myfile<<fixed << setprecision(6)<<sin(j*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<sin(j*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<cos(j*difphi*(PI_M/180))<<", ";//position
            myfile<<fixed << setprecision(6)<<(-1)*sin(j*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<(-1)*sin(j*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<(-1)*cos(j*difphi*(PI_M/180))<<", ";
            myfile<<fixed << setprecision(6)<<1-(i*(diftheta/360))<<", "<<1-((j*difphi)/180)<<","<<endl;
        }
    }

    //polo sur
    for(int i=0;i<resolLong;i++){
        myfile<<fixed << setprecision(1)<<0.0<<", "<<0.0<<", "<<-1.0<<", "<<0.0<<", "<<0.0<<", "<<1.0<<", "<<0.0<<", "<<0.0<<","<<endl;
        myfile<<fixed << setprecision(6)<<sin((resolLat-1)*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<sin((resolLat-1)*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<cos((resolLat-1)*difphi*(PI_M/180))<<", ";//position
        myfile<<fixed << setprecision(6)<<(-1)*sin((resolLat-1)*difphi*(PI_M/180))*cos(i*diftheta*(PI_M/180))<<", "<<(-1)*sin((resolLat-1)*difphi*(PI_M/180))*sin(i*diftheta*(PI_M/180))<<", "<<(-1)*cos((resolLat-1)*difphi*(PI_M/180))<<", ";//normal
        myfile<<fixed << setprecision(6)<<1-(i*(diftheta/360))<<", "<<((resolLat-1)*difphi/180)<<","<<endl;//texCoord
        myfile<<fixed << setprecision(6)<<sin((resolLat-1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<sin((resolLat-1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<cos((resolLat-1)*difphi*(PI_M/180))<<", ";//position
        myfile<<fixed << setprecision(6)<<(-1)*sin((resolLat-1)*difphi*(PI_M/180))*cos((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*sin((resolLat-1)*difphi*(PI_M/180))*sin((i+1)*diftheta*(PI_M/180))<<", "<<(-1)*cos((resolLat-1)*difphi*(PI_M/180))<<", ";//normal
        myfile<<fixed << setprecision(6)<<1-((i+1)*(diftheta/360))<<", "<<1-((resolLat-1)*difphi/180)<<","<<endl;//texCoord
    }
    cout<<"xD"<<endl;
    cout<<PI_M<<endl;
    cout<<difphi<<endl;
    myfile.close();
    return 0;
}