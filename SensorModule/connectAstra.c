#include "connectAstra.h"
#include <stdio.h>

int iNumOfPoint = 0;
AstraData_t* pstAstraData = NULL;
int astra_width = 0;
int astra_height = 0;

AstraData_t connectAstra(AstraContext_t* context){
    int width, height;
    static int iTestCnt = 0;
    
    const int16_t* depthData = GetDepthDataAstraOpenGL(context, &astra_width, &astra_height);
    const uint8_t* colorData = GetColorDataAstraOpenGL(context, &astra_width, &astra_height);

    if(depthData && colorData){
    printf("Thread Test : %d\n", iTestCnt++);
        pstAstraData = (AstraData_t*)calloc(astra_width * astra_height, sizeof(AstraData_t));
        iNumOfPoint = astra_width * astra_height;
        
        for(int y = 0; y < astra_height; ++y){
            for(int x = 0; x < astra_width; ++x){
                int index = y * astra_width + x;
                int depthValue = depthData[index];

                if(depthValue > 0){
                    // Calculate 3D Coordinate (Using Simple Camera Model)
                    float z = depthValue / 1000.0f; // from mm to m
                    float x_pos = (x - astra_width / 2) * z / 570.3f; // 570.3f is focal distance of Astra camera
                    float y_pos = (y - astra_height / 2) * z / 570.3f;

                    // Set color using color data (RGB order)
                    int colorIndex = index * 3; // RGB consist of 3 values.
                    float r = colorData[colorIndex] / 255.0f;
                    float g = colorData[colorIndex + 1] / 255.0f;
                    float b = colorData[colorIndex + 2] / 255.0f;

                    pstAstraData[index].fX = x_pos;
                    pstAstraData[index].fY = -y_pos;
                    pstAstraData[index].fZ = -z;
                    pstAstraData[index].fR = r;
                    pstAstraData[index].fG = g;
                    pstAstraData[index].fB = b;

                    // printf("%lf %lf %lf %lf %lf %lf\n", x_pos, -y_pos, -z, r, g, b);
                }
            }
        }
    }
}