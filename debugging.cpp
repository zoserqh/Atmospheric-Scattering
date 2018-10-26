//g++ -std=c++0x debugging.cpp  glad.c -o debugging -lglfw -ldl -lGL -lglut -lGLU -lGLEW -lm

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//GLM 0.9.9
#include "tools/glm/glm.hpp" // vec2, vec3, mat4, radians
// Include all GLM extensions
#include "tools/glm/ext.hpp" // perspective, translate, rotate
#include"classes/camera.h"

#include<iostream>
using namespace std;

#define PI 3.14159265

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));

// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos(400.0f, 0.0f, 0.0f);
glm::vec3 m_vLight(1000.0f, 0.0f, 0.0f);
glm::vec3 m_vLightDirection = m_vLight / glm::length(m_vLight);

//CTexture::InitStaticMembers(238653, 256);
int m_nSamples = 3;		// Number of sample rays to use in integral equation
float m_Kr = 0.0025f;		// Rayleigh scattering constant
float m_Kr4PI = m_Kr*4.0f*PI;
float m_Km = 0.0010f;		// Mie scattering constant
float m_Km4PI = m_Km*4.0f*PI;
float m_ESun = 20.0f;		// Sun brightness constant
float m_g = -0.990f;		// The Mie phase asymmetry factor
float m_fExposure = 2.0f;

float m_fInnerRadius = 10.0f;
float m_fOuterRadius = 10.25f;
float m_fScale = 1 / (m_fOuterRadius - m_fInnerRadius);
float m_fWavelength[3];
float m_fWavelength4[3];

float m_fRayleighScaleDepth = 0.25f;
float m_fMieScaleDepth = 0.1f;

const int nSamples = 2;
const float fSamples = 2.0;

//
float fScaleDepth=m_fRayleighScaleDepth;
float fInnerRadius=m_fInnerRadius;
float fOuterRadius=m_fOuterRadius;
float fCameraHeight2=glm::length(camera.Position)*glm::length(camera.Position);
float fOuterRadius2=m_fOuterRadius*m_fOuterRadius;
glm::vec3 v3CameraPos=camera.Position;
glm::vec3 v3LightPos=m_vLightDirection;
float fScale=m_fScale;
float fScaleOverScaleDepth=(1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth;
float fKr4PI=m_Kr4PI;
float fKm4PI=m_Km4PI;
float fKrESun=m_Kr*m_ESun;
float fKmESun=m_Km*m_ESun;

float scale(float fCos);

float earthVerticesaPos[] = {
    // positions                    // normals                  // texture coords
    0.0, 0.0, 1.0,
    0.173648, 0.000000, 0.984808,
    0.171010, 0.030154, 0.984808, 
    0.0, 0.0, 1.0, 
    0.171010, 0.030154, 0.984808, 
    0.163176, 0.059391, 0.984808, 
    0.0, 0.0, 1.0, 
    0.163176, 0.059391, 0.984808, 
    0.150384, 0.086824, 0.984808, 
    0.0, 0.0, 1.0,
    0.150384, 0.086824, 0.984808, 
    0.133022, 0.111619, 0.984808, 
    };


main(){
    
    glm::vec3 aPos[4];
    aPos[0]= glm::vec3(earthVerticesaPos[0],earthVerticesaPos[1],earthVerticesaPos[2]);
    aPos[1]=glm::vec3(earthVerticesaPos[3],earthVerticesaPos[4],earthVerticesaPos[5]);
    aPos[2]=glm::vec3(earthVerticesaPos[6],earthVerticesaPos[7],earthVerticesaPos[8]);
    aPos[3]=glm::vec3(earthVerticesaPos[9],earthVerticesaPos[10],earthVerticesaPos[11]);
    
    ////////////// 
    m_fWavelength[0] = 0.650f;		// 650 nm for red
    m_fWavelength[1] = 0.570f;		// 570 nm for green
    m_fWavelength[2] = 0.475f;		// 475 nm for blue
    
    m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
    m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
    m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);
    glm::vec3 inv_m_fWavelength4(1/m_fWavelength4[0],1/m_fWavelength4[1],1/m_fWavelength4[2]);

    glm::vec3 v3InvWavelength=inv_m_fWavelength4;

    cout<<powf(m_fWavelength[2], 4.0f)<<endl;

    for(int i=0;i<4;i++){ //4
        glm::vec3 FragPos;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        glm::vec3 FrontColor_rgb;
        glm::vec3 FrontSecondaryColor_rgb;

        // Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
        glm::vec3 v3Pos = 10.25f*aPos[i]; //10*
        glm::vec3 v3Ray = v3Pos - v3CameraPos;
        float fFar = glm::length(v3Ray);
        cout<<"i: "<<i<<"   fFar0: "<<fFar<<endl;
        v3Ray /= fFar;
        //cout<<"i: "<<i<<"   v3Ray: "<<v3Ray.x<<", "<<v3Ray.y<<", "<<v3Ray.z<<endl;

        // Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
        float B = 2.0 * dot(v3CameraPos, v3Ray);
        float C = fCameraHeight2 - fOuterRadius2;
        float fDet = max(0.0, B*B - 4.0 * C);
        float fNear = 0.5 * (-B - sqrt(fDet));
        cout<<"i: "<<i<<"   fNear: "<<fNear<<endl;

        // Calculate the ray's starting position, then calculate its scattering offset
        /*
        glm::vec3 v3Start = v3CameraPos + v3Ray * fNear;
        fFar -= fNear;
        float fDepth = exp((fInnerRadius - fOuterRadius) / fScaleDepth);
        float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
        float fLightAngle = dot(v3LightPos, v3Pos) / length(v3Pos);
        float fCameraScale = scale(fCameraAngle);
        float fLightScale = scale(fLightAngle);
        float fCameraOffset = fDepth*fCameraScale;
        float fTemp = (fLightScale + fCameraScale);
        //cout<<"i: "<<i<<"   fDepth: "<<fDepth<<" fCameraAngle: "<<fCameraAngle<<" fLightAngle: "<<fLightAngle<<" fCameraScale: "<<fCameraScale<<" fLightScale: "<<fLightScale<<" fCameraOffset: "<<fCameraOffset<<" fTemp: "<<fTemp<<endl;
        */
        glm::vec3 v3Start = v3CameraPos + v3Ray * fNear;
        fFar -= fNear;
        cout<<"fFar: "<<fFar<<endl;
        float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
        float fStartDepth = exp(-1.0 / fScaleDepth);
        float fStartOffset = fStartDepth*scale(fStartAngle);
        //cout<<"fStartAngle: "<<fStartAngle<<"   fStartDepth: "<<fStartDepth<<"  fStartOffset: "<<fStartOffset<<endl;
        
        // Initialize the scattering loop variables
        //float fSampleLength = fFar / fSamples;
        //float fScaledLength = fSampleLength * fScale;
        //glm::vec3 v3SampleRay = v3Ray * fSampleLength;
        //glm::vec3 v3SamplePoint = v3Start + 0.5f * v3SampleRay; //glm::vec3(0.5) 
        //cout<<"i: "<<i<<"   fSampleLength: "<<fSampleLength<<"  fScaledLength: "<<fScaledLength;
        //cout<<"  v3SampleRay: "<<v3SampleRay.x<<","<<v3SampleRay.y<<","<<v3SampleRay.z<<"   v3SamplePoint: "<<v3SamplePoint.x<<","<<v3SamplePoint.y<<","<<v3SamplePoint.z<<endl;
        
        float fSampleLength = fFar / fSamples;
        float fScaledLength = fSampleLength * fScale;
        glm::vec3 v3SampleRay = v3Ray * fSampleLength;
        glm::vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;
        cout<<"i: "<<i<<"   fSampleLength: "<<fSampleLength<<"  fScaledLength: "<<fScaledLength;
        cout<<"  v3SampleRay: "<<v3SampleRay.x<<","<<v3SampleRay.y<<","<<v3SampleRay.z<<"   v3SamplePoint: "<<v3SamplePoint.x<<","<<v3SamplePoint.y<<","<<v3SamplePoint.z<<endl;

        // Now loop through the sample rays
        glm::vec3 v3FrontColor = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 v3Attenuate;
       /*
        for(int j=0; j<nSamles; j++){ //nSamles
            float fHeight = length(v3SamplePoint);
            float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight)); //1.0
            float fScatter = fDepth*fTemp - fCameraOffset;
            v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
            v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
            v3SamplePoint += v3SampleRay;
            
            cout<<"j: "<<j<<"   fHeight: "<<fHeight<<"  fDepth: "<<fDepth<<"    fScatter: "<<fScatter<<"    fScaleOverScaleDepth: "<<fScaleOverScaleDepth<<endl;
            cout<<"v3Attenuate: "<<v3Attenuate.x<<","<<v3Attenuate.y<<","<<v3Attenuate.z<<endl;
            cout<<"v3FrontColor: "<<v3FrontColor.x<<","<<v3FrontColor.y<<","<<v3FrontColor.z<<endl;
            cout<<"v3SamplePoint: "<<v3SamplePoint.x<<","<<v3SamplePoint.y<<","<<v3SamplePoint.z<<endl;
            cout<<endl;
            
        }
        */
        FrontColor_rgb = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);

        FrontSecondaryColor_rgb = v3Attenuate;

        //cout<<"i: "<<i<<"   FrontColor_rgb: "<<FrontColor_rgb.x<<", "<<FrontColor_rgb.y<<", "<<FrontColor_rgb.z<<endl;
        //cout<<"FrontSecondaryColor_rgb: "<<FrontSecondaryColor_rgb.x<<", "<<FrontSecondaryColor_rgb.y<<", "<<FrontSecondaryColor_rgb.z<<endl;
        //cout<<endl;
    }
    

    //glm::vec3 aaa=glm::vec3(1.0,2.0,3.0) * 2.0f; // glm::vec3(2.0)
    //cout<<"aaa.xyz: "<<aaa.x<<aaa.y<<aaa.z<<endl;
    cout<<"xD"<<endl;
    return 0;
}

float scale(float fCos){
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}