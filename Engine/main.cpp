#include<iostream>
#include<string>


#include"IerarchyClasses/Classes/Program.h"
#include"IerarchyClasses/Classes/Frame.h"

#include"GraphicsPrimitives/WindowsManager.h"

#include"GraphicsPrimitives/ShaderProgramManager.h"

#include"UsefullStuff/RandomNumber.h"




static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << xpos << ", " << ypos << '\n';
}




unsigned int circlesAmount = 500;

float circleSize = 20;

float circleSpeed = 10;



struct Circle {
    float dx, dy;
    Frame* frame;
    unsigned int physicsGridInd;
};
std::vector<Circle> Circles;



unsigned int width, height;

bool PhysicsGridJustGotUpdated = true;
unsigned int gridSegmentsX, gridSegmentsY;
std::vector<std::vector<unsigned int>> PhysicsGrid;

void UpdatePhysicsGrid() {
    gridSegmentsX = (unsigned int)ceilf((float)width / circleSize);
    gridSegmentsY = (unsigned int)ceilf((float)height / circleSize);

    PhysicsGrid.assign(gridSegmentsX * gridSegmentsY, {});
    PhysicsGridJustGotUpdated = true;
}

unsigned int getPhysicsGridInd(float x,float y) {
    int xcord = (int)((x + (float)width / 2) / ((float)width / (float)gridSegmentsX));
    int ycord = (int)(((float)height / 2 - y) / ((float)height / (float)gridSegmentsY));
    if (xcord < 0 or ycord < 0) __debugbreak();//bad coordinates
    return ycord * gridSegmentsX + xcord;
}


void SolveCollision(unsigned int c1i, unsigned int c2i) {
    Circle& c1 = Circles[c1i];
    Circle& c2 = Circles[c2i];

    Vector2 p1(c1.frame->Position.gPX(), c1.frame->Position.gPY());
    Vector2 p2(c2.frame->Position.gPX(), c2.frame->Position.gPY());
    
    //we dont want to divide by zero and get NAN
    if ((p1 - p2).SqLength() < 0.01) return;

    Vector2 diff1(c1.dx, c1.dy);
    Vector2 diff2(c2.dx, c2.dy);
    Vector2 mid = (p1 + p2) / 2;
    p1 = mid + (p1 - mid).Unit() * circleSize / 2;
    p2 = mid + (p2 - mid).Unit() * circleSize / 2;
    c1.frame->Position.sPX(p1.gX());
    c1.frame->Position.sPY(p1.gY());
    c2.frame->Position.sPX(p2.gX());
    c2.frame->Position.sPY(p2.gY());

    diff1 = ((diff1.Unit()+(p1 - mid).Unit())/2).Unit() * circleSpeed;
    diff2 = ((diff2.Unit() + (p2 - mid).Unit()) / 2).Unit() * circleSpeed;

    c1.dx = diff1.gX();
    c1.dy = diff1.gY();
    c2.dx = diff2.gX();
    c2.dy = diff2.gY();
}


int main() {
    
    WindowsManager::Initialize();

    auto monitorData = WindowsManager::getMonitorData(WindowsManager::getCurrentMonitor());

    Window wind(monitorData->width / 2, monitorData->height / 2, "david window");

   
    wind.SetWindowColor(1.f, 0.5f, 0.f);
    wind.SetSwapInterval(1);
    
    ShaderProgram shader;
    ShaderProgramID shaderID = ShaderProgramManager::AddShaderProgram("Circle", &shader);


    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector2,"ScreenPosition"});
    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector2,"LocalPosition" });
    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector4,"Color" });

    shader.TransferData.push_back({ ShaderProgramTypeEnum::vector4 ,"VertexColor" });
    shader.VertexShaderCodeParts.push_back("t_VertexColor=i_Color;\ngl_Position = vec4(i_ScreenPosition.x, i_ScreenPosition.y, 0.f, 1.f);");
    shader.FragmentShaderCodeParts.push_back("o_PixelColor=t_VertexColor;");

    //ShaderProgram shader2=shader;
    //ShaderProgramID shader2ID = ShaderProgramManager::AddShaderProgram("main shader with circle", &shader2);

    shader.TransferData.push_back({ ShaderProgramTypeEnum::vector2,"LocalPosition" });
    shader.VertexShaderCodeParts.push_back("t_LocalPosition=vec2(i_LocalPosition.x, i_LocalPosition.y);");
    shader.FragmentShaderCodeParts.push_back("if (t_LocalPosition.x*t_LocalPosition.x+t_LocalPosition.y*t_LocalPosition.y>1) o_PixelColor=vec4(0.f,0.f,0.f,0.f);");

    Program windProgram;


   /* Frame* frame = new Frame;

    frame->sShaderID(shader2ID);

    frame->AddVertex({ -0.3f,1.f, 0.f,0.f,0.f,0.f });
    frame->AddVertex({ 0.3f,1.f, 0.8f,0.1f,0.2f,1.f });
    frame->AddVertex({ 1.f,-1.f, 0.5f,0.1f,0.f,0.f });
    frame->AddVertex({ -1.f,-1.f, 1.f,0.9f,0.9f,1.f });
    frame->UpdateIndexBuffer();
    frame->sParent(&windProgram);
    frame->sPriority(3.f);
    frame->Size.sPX(100.f);
    frame->Size.sPY(100.f);
    frame->RotationOffset.sSX(-1.f);
    frame->RotationOffset.sSY(-1.f);

    
    Frame* frame2 = new Frame;
    frame2->sShaderID(shaderID);
    frame2->ApplyVertexTemplate(VertexTemplateEnum::Square);
    frame2->SetVertexesParameterByIndex(2, { 0.5f,0.1f,0.7f,0.5f });
    frame2->sParent(frame);

    frame2->sPriority(2.f);
    frame2->Position.sSX(1.f);
    frame2->Position.sSY(1.f);
    frame2->Size.sSX(1.f);*/


    wind.GetWindowSize(&width, &height);

    UpdatePhysicsGrid();


    for (unsigned int r = 0; r < circlesAmount; r++) {
        Frame* frame = new Frame;
        frame->sShaderID(shaderID);
        frame->ApplyVertexTemplate(VertexTemplateEnum::Square);
        if (r%40==0)
            frame->SetVertexesParameterByIndex(2, { RandNum(0.f,1.f),RandNum(0.f,1.f),RandNum(0.f,1.f),1.f });
        else
            frame->SetVertexesParameterByIndex(2, { 0,0,0 ,1.f});
        frame->sParent(&windProgram);
        frame->Position.sPX(RandNum(-1.f, 1.f)* width / 2);
        frame->Position.sPY(RandNum(-1.f, 1.f)* height / 2);
        frame->Size.sPX(circleSize);
        frame->Size.sPY(circleSize);


        Circles.push_back({ 
            circleSpeed,circleSpeed,//RandNum(-10.f,10.f),RandNum(-10.f,10.f),
            frame,0,
            });
    }

    while (!wind.WindowWaitingToBeClosed()) {
        wind.StartUpdatingWindow();

        unsigned int prevWidth = width;
        unsigned int prevHeight = height;

        wind.GetWindowSize(&width, &height);

        if (prevWidth != width or prevHeight != height) {
            UpdatePhysicsGrid();
        }



        for (unsigned int i = 0; i < circlesAmount; i++) {

            auto& frame = *Circles[i].frame;
            frame.Position.sPX(frame.Position.gPX() + Circles[i].dx);
            frame.Position.sPY(frame.Position.gPY() + Circles[i].dy);


            if (frame.Position.gPX() + circleSize / 2 > (float)width / 2) {
                frame.Position.sPX((float)width / 2 - circleSize / 2);
                Circles[i].dx = -Circles[i].dx;
            } else if (frame.Position.gPX() - circleSize / 2 < -(float)width / 2) {
                frame.Position.sPX(-(float)width / 2 + circleSize / 2);
                Circles[i].dx = -Circles[i].dx;
            }
            if (frame.Position.gPY() + circleSize / 2 > (float)height / 2) {
                frame.Position.sPY((float)height / 2 - circleSize / 2);
                Circles[i].dy = -Circles[i].dy;
            }
            else if (frame.Position.gPY() - circleSize / 2 < -(float)height / 2) {
                frame.Position.sPY(-(float)height / 2 + circleSize / 2);
                Circles[i].dy = -Circles[i].dy;
            }

            
            if (not PhysicsGridJustGotUpdated)
                for (unsigned int di = 0; di < PhysicsGrid[Circles[i].physicsGridInd].size(); di++) {
                    if (PhysicsGrid[Circles[i].physicsGridInd][di] == i) {
                        PhysicsGrid[Circles[i].physicsGridInd].erase(PhysicsGrid[Circles[i].physicsGridInd].begin() + di);
                        break;
                    }
                }

            Circles[i].physicsGridInd = getPhysicsGridInd(frame.Position.gPX(), frame.Position.gPY());
            PhysicsGrid[Circles[i].physicsGridInd].push_back(i);





            /*for (unsigned int i2 = 0; i2 < Circles.size(); i2++) {
                if (i2 == i) continue;
                float x1 = Circles[i].frame->Position.gPX();
                float y1 = Circles[i].frame->Position.gPY();
                float x2 = Circles[i2].frame->Position.gPX();
                float y2 = Circles[i2].frame->Position.gPY();
                if (sqrtf((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))<circleSize)
                    SolveCollision(&Circles[i], &Circles[i2]);
            }*/

        }

        PhysicsGridJustGotUpdated = false;

        {
            unsigned int mainGridInd = 0;

#define collisionCheckCycle(oInd) {unsigned int cInd=oInd;\
for (unsigned int i1 = 0; i1 < PhysicsGrid[mainGridInd].size(); i1++) {\
    for (unsigned int i2 = 0; i2 < PhysicsGrid[oInd].size(); i2++) {\
        const Vector2& p1 = Circles[PhysicsGrid[mainGridInd][i1]].frame->Position.gPV();\
        const Vector2& p2 = Circles[PhysicsGrid[oInd][i2]].frame->Position.gPV();\
        if ((p1 - p2).SqLength() < circleSize * circleSize)\
            SolveCollision(PhysicsGrid[mainGridInd][i1], PhysicsGrid[oInd][i2]);\
    }\
}\
}

            for (unsigned int y = 0; y < gridSegmentsY; y++) {
                for (unsigned int x = 0; x < gridSegmentsX; x++) {

                    if (PhysicsGrid[mainGridInd].size() == 0) {
                        mainGridInd++;
                        continue;
                    }


                    if (y != 0) {
                        ////left top
                        if (x != 0 and PhysicsGrid[mainGridInd - gridSegmentsX - 1].size() != 0)
                            collisionCheckCycle(mainGridInd - gridSegmentsX - 1);
                        ////top
                        if (PhysicsGrid[mainGridInd - gridSegmentsX].size() != 0)
                            collisionCheckCycle(mainGridInd - gridSegmentsX);
                        ////right top
                        if (x != gridSegmentsX - 1 and PhysicsGrid[mainGridInd - gridSegmentsX + 1].size() != 0)
                            collisionCheckCycle(mainGridInd - gridSegmentsX + 1);
                    }
                    if (y != gridSegmentsY - 1) {
                        ////left bottom
                        if (x != 0 and PhysicsGrid[mainGridInd + gridSegmentsX - 1].size() != 0)
                            collisionCheckCycle(mainGridInd + gridSegmentsX - 1);
                        ////bottom
                        if (PhysicsGrid[mainGridInd + gridSegmentsX].size() != 0)
                            collisionCheckCycle(mainGridInd + gridSegmentsX);
                        ////right bottom
                        if (x != gridSegmentsX - 1 and PhysicsGrid[mainGridInd + gridSegmentsX + 1].size() != 0)
                            collisionCheckCycle(mainGridInd + gridSegmentsX + 1);
                    }

                    //middle
                    if (PhysicsGrid[mainGridInd].size() >= 2)
                        for (unsigned int i1 = 0; i1 < PhysicsGrid[mainGridInd].size(); i1++) {
                            for (unsigned int i2 = 0; i2 < PhysicsGrid[mainGridInd].size(); i2++) {
                                if (i1 == i2) continue;
                                SolveCollision(PhysicsGrid[mainGridInd][i1], PhysicsGrid[mainGridInd][i2]);
                            }
                        }

                    ////right
                    if (x != gridSegmentsX - 1 and PhysicsGrid[mainGridInd + 1].size() != 0)
                        collisionCheckCycle(mainGridInd + 1);

                    ////left
                    if (x != 0 and PhysicsGrid[mainGridInd - 1].size() != 0)
                        collisionCheckCycle(mainGridInd - 1);





                    mainGridInd++;
                }
            }

#undef collisionCheckCycle
        }



        windProgram.Render(width,height);
        

        wind.EndUpdatingWindow();
    }

    windProgram.Delete();
    WindowsManager::Uninitialize();
    return 0;

}