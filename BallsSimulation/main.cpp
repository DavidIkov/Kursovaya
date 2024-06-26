#include<iostream>
#include<string>

#include"UsefullStuff/Time.h"


#include"HierarchyClasses/Classes/HierarchyRoot.h"
#include"HierarchyClasses/Classes/Frame.h"
#include"HierarchyClasses/Classes/DimensionalTranslator3Dto2D.h"
#include"HierarchyClasses/Classes/Object3D.h"

#include"Graphics/WindowsManager.h"

#include"Graphics/ShaderProgramsManager.h"

#include"UsefullStuff/RandomNumber.h"

#include"Maths/Vector2.h"






unsigned int circlesAmount = 100;

float circleSize = 30;

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

unsigned int getPhysicsGridInd(float x, float y) {
    int xcord = (int)((x + (float)width / 2) / ((float)width / (float)gridSegmentsX));
    int ycord = (int)(((float)height / 2 - y) / ((float)height / (float)gridSegmentsY));
    if (xcord < 0 or ycord < 0) __debugbreak();//bad coordinates
    return ycord * gridSegmentsX + xcord;
}


void SolveCollision(unsigned int c1i, unsigned int c2i) {
    Circle& c1 = Circles[c1i];
    Circle& c2 = Circles[c2i];

    Vector2 p1(c1.frame->gPosition().gPX(), c1.frame->gPosition().gPY());
    Vector2 p2(c2.frame->gPosition().gPX(), c2.frame->gPosition().gPY());

    //we dont want to divide by zero and get NAN
    if ((p1 - p2).SqLength() < 0.01) return;

    Vector2 diff1(c1.dx, c1.dy);
    Vector2 diff2(c2.dx, c2.dy);
    Vector2 mid = (p1 + p2) / 2;
    p1 = mid + (p1 - mid).Unit() * circleSize / 2;
    p2 = mid + (p2 - mid).Unit() * circleSize / 2;
    c1.frame->sPosition({ 0,0,p1.gX(),p1.gY() });
    c2.frame->sPosition({ 0,0,p2.gX(),p2.gY() });

    diff1 = ((diff1.Unit() + (p1 - mid).Unit()) / 2).Unit() * circleSpeed;
    diff2 = ((diff2.Unit() + (p2 - mid).Unit()) / 2).Unit() * circleSpeed;

    c1.dx = diff1.gX();
    c1.dy = diff1.gY();
    c2.dx = diff2.gX();
    c2.dy = diff2.gY();
}


int main() {
    WindowsManager::Initialize();

    auto monitorData = WindowsManager::gMonitorData(WindowsManager::gCurrentMonitor());

    Window wind(monitorData->width / 2, monitorData->height / 2, "david window");

    auto& keyboard = wind.Keyboard;


    wind.SetWindowColor(1.f, 0.5f, 0.f);
    wind.SetSwapInterval(1);

    wind.SetCursorMode(CursorModes::Free);

    ShaderProgram shader;
    ShaderProgramID shaderID = ShaderProgramsManager::AddShaderProgram("Circle", &shader);


    shader.VertexBufferData.push_back({ ShaderProgramDataTypes::vector2,"ScreenPosition" });
    shader.VertexBufferData.push_back({ ShaderProgramDataTypes::vector2,"LocalPosition" });
    shader.VertexBufferData.push_back({ ShaderProgramDataTypes::vector4,"Color" });

    shader.TransferData.push_back({ ShaderProgramDataTypes::vector4 ,"VertexColor" });
    shader.VertexShaderCodeParts.push_back("t_VertexColor=i_Color;\ngl_Position = vec4(i_ScreenPosition.x, i_ScreenPosition.y, 0.f, 1.f);");
    shader.FragmentShaderCodeParts.push_back("o_PixelColor=t_VertexColor;");

    shader.TransferData.push_back({ ShaderProgramDataTypes::vector2,"LocalPosition" });
    shader.VertexShaderCodeParts.push_back("t_LocalPosition=i_LocalPosition;");
    shader.FragmentShaderCodeParts.push_back("if (t_LocalPosition.x*t_LocalPosition.x+t_LocalPosition.y*t_LocalPosition.y>1) o_PixelColor=vec4(0.f,0.f,0.f,0.f);");

    shader.CompileShader();

    ShaderProgram shader3d;
    ShaderProgramID shader3dID = ShaderProgramsManager::AddShaderProgram("3d shader", &shader3d);

    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector4,"PositionOnWindow" });
    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector3,"PositionInSpace" });
    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector3,"LocalPosition" });
    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector4,"Color" });
    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector3,"SpaceNormal" });
    shader3d.VertexBufferData.push_back({ ShaderProgramDataTypes::vector3,"LocalNormal" });

    shader3d.TransferData.push_back({ ShaderProgramDataTypes::vector4 ,"VertexColor" });
    shader3d.TransferData.push_back({ ShaderProgramDataTypes::vector3,"LocalPosition" });
    shader3d.TransferData.push_back({ ShaderProgramDataTypes::vector3,"PositionInSpace" });
    shader3d.TransferData.push_back({ ShaderProgramDataTypes::vector3,"SpaceNormal" });

    shader3d.VertexShaderCodeParts.push_back("t_VertexColor=i_Color;\ngl_Position = i_PositionOnWindow;");
    shader3d.VertexShaderCodeParts.push_back("t_PositionInSpace=i_PositionInSpace; t_SpaceNormal=i_SpaceNormal;");
    shader3d.FragmentShaderCodeParts.push_back("float brightness = -1;");
    shader3d.FragmentShaderCodeParts.push_back("for (int i=0;i<u_LightSourcesAmount;i++) brightness=max(brightness,dot(t_SpaceNormal,normalize(ua_LightSources[i]-t_PositionInSpace)));");
    shader3d.FragmentShaderCodeParts.push_back("o_PixelColor=vec4(t_VertexColor.xyz*(brightness+1)/2,t_VertexColor.w);");
    
    shader3d.VertexShaderCodeParts.push_back("t_LocalPosition=i_LocalPosition;");
    
    shader3d.FragmentShaderUniformsData.push_back({ ShaderProgramDataTypes::vector3, "LightSources", 32 });
    shader3d.FragmentShaderUniformsData.push_back({ ShaderProgramDataTypes::integerType, "LightSourcesAmount" });


    shader3d.CompileShader();
    
    shader3d.gUniform().SetUniform1i("u_LightSourcesAmount", 1);
    shader3d.gUniform().SetUniform3fv("ua_LightSources", { 30,0,0 });
   

    HierarchyRoot* hierarchy = wind.Hierarchy;
    //hierarchy->sCameraSize({ 2,2,0,0 });
    //hierarchy->sCameraPosition({ 0,0,0,0 });


    




    hierarchy->UpdateRenderingDataOfChild_OnChildCoordinatesUpdate = false;
    hierarchy->RecalculateCoordinatesOfChildInSpace_OnChildCoordinatesUpdate = false;




    wind.GetWindowSize(&width, &height);

    DimensionalTranslator3Dto2D* translatorTo3D = new DimensionalTranslator3Dto2D(width,height);
    translatorTo3D->sSize({ 1,1,0,0 });
    translatorTo3D->sParent(hierarchy);
    translatorTo3D->RecalculateCoordinatesInSpace(false);
    {

        float topAngle = 90.f / 180.f * 3.14f;
        translatorTo3D->sCameraViewAngle({ 2 * atanf(width / height * 2 * tanf(topAngle / 2)), topAngle });
    }

    {
        Object3D* cube = new Object3D;
        cube->ApplyVertexTemplate(Vertex3DTemplates::Cube, { 1 });
        cube->sPositionFromParent({ 30,0,4 });
        cube->sSize({ 1,1,1 });
        cube->sShaderID(shader3dID);
        cube->sParent(translatorTo3D);
        for (unsigned int i = 0; i < 8; i++) {
            cube->SetVertexParameterByIndex(i, 1, { RandNum(0,1),RandNum(0,1),RandNum(0,1),1.f });
        }
    }
    {
        Object3D* sphere = new Object3D;
        sphere->ApplyVertexTemplate(Vertex3DTemplates::Sphere, { 1,10,10 });
        sphere->sPositionFromParent({ -5,0,4 });
        sphere->sSize({ 1,1,1 });
        sphere->sShaderID(shader3dID);
        sphere->sParent(translatorTo3D);
    }

    Object3D* cube = new Object3D;
    cube->ApplyVertexTemplate(Vertex3DTemplates::Cube, { 1 });
    cube->sPositionFromParent({ 0,0,10 });
    cube->sSize({ 2,2,4 });
    cube->sShaderID(shader3dID);
    cube->sParent(translatorTo3D);
    for (unsigned int i = 0; i < 8; i++) {
        cube->SetVertexParameterByIndex(i, 1, { RandNum(0,1),RandNum(0,1),RandNum(0,1),1.f });
    }




    UpdatePhysicsGrid();

    TimePoint timeS = Time::GetTimePoint();

    Circles.assign(circlesAmount, {});
    hierarchy->ExpandChildrenAmount(circlesAmount);
    for (unsigned int r = 0; r < circlesAmount; r++) {
        Frame* frame = new Frame;
        frame->sShaderID(shaderID);
        frame->ApplyVertexTemplate(Vertex2DTemplates::Square);
        if (r % 40 == 0)
            frame->SetVertexesParameterByIndex(1, { RandNum(0.f,1.f),RandNum(0.f,1.f),RandNum(0.f,1.f),1.f });
        else
            frame->SetVertexesParameterByIndex(1, { 0,0,0 ,1.f });
        frame->sParent(hierarchy);
        frame->sPosition({ 0,0, RandNum(-1.f, 1.f) * width / 2, RandNum(-1.f, 1.f) * height / 2 });
        if (r == 1) {
            frame->sPosition({ 0,0,0,0 });
        }
        frame->sSize({ 0,0,circleSize,circleSize });

        frame->RecalculateCoordinatesInSpace(false);
        frame->UpdateVertexesDataInRenderingData(false);


        Circles[r] = {
            circleSpeed,circleSpeed,
            frame,0,
        };
    }

    //std::cout << Time::GetDuration(timeS, Time::GetTimePoint());

    unsigned int framesCounter = 0;

    float lengths[4](0.f, 0.f, 0.f, 0.f);

    float camRot = 0.f;

    while (!wind.WindowWaitingToBeClosed()) {

        wind.StartUpdatingWindow();

        Vector2 MousePosition;
        Vector2 MouseDelta;
        wind.GetCursorMotionData(&MousePosition, &MouseDelta);


        unsigned int prevWidth = width;
        unsigned int prevHeight = height;

        wind.GetWindowSize(&width, &height);

        if (keyboard.gPressableKeyState(PressableKeys::W)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(0, 0, 0.05f));
        }
        if (keyboard.gPressableKeyState(PressableKeys::A)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(-0.05f,0,0));
        }
        if (keyboard.gPressableKeyState(PressableKeys::S)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(0, 0, -0.05f));
        }
        if (keyboard.gPressableKeyState(PressableKeys::D)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(0.05f, 0, 0));
        }
        if (keyboard.gPressableKeyState(PressableKeys::Q)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(0, -0.05f, 0));
        }
        if (keyboard.gPressableKeyState(PressableKeys::E)) {
            translatorTo3D->sCameraPosition(translatorTo3D->gCameraPosition() + translatorTo3D->gCameraRotation() * Vector3(0, 0.05f, 0));
        }

        if (keyboard.gPressableKeyState(PressableKeys::O)) {
            wind.SetCursorMode(CursorModes::LockedAndInvisible);
        }
        if (keyboard.gPressableKeyState(PressableKeys::L)) {
            wind.SetCursorMode(CursorModes::Free);
        }
        
        {//camera rotation
            Vector2 rotationAngle = Vector2(MouseDelta.gY() / width * 10, -MouseDelta.gX() / width * 10) / 180.f * 3.14f;
            Matrix resultRotation = translatorTo3D->gCameraRotation() * Matrix::FromAnglesXYZ(rotationAngle.gX(), 0, 0);
            Vector3 xv(1, 0, 0);
            Vector3 zv(0, 0, 1);
            Vector3::RotateVectors(xv, zv, rotationAngle.gY(), xv, zv);
            resultRotation = Matrix(3, 3, { xv.gX(),xv.gY(),xv.gZ(),0,1,0,zv.gX(),zv.gY(),zv.gZ() }) * resultRotation;
            translatorTo3D->sCameraRotation(resultRotation);
        }
   


        
        float startTime = Time::GetTime();
        


        if (prevWidth != width or prevHeight != height) {

            //hierarchy->sResolution(width, height);

            UpdatePhysicsGrid();

            /*translatorTo3D->RecalculateActualCords(false);
            translatorTo3D->UpdateVertexesCordsInFilteredOrder(false);
            float topAngle = 90.f/180.f*3.14f;
            translatorTo3D->sCameraAngle({ 2 * atanf(width / height * 2 * tanf(topAngle / 2)), topAngle });*/
        }



        for (unsigned int i = 0; i < circlesAmount; i++) {
            auto& frame = *Circles[i].frame;

            const SPCS& pos = frame.gPosition();

            frame.sPosition({ 0,0,pos.gPX() + Circles[i].dx ,pos.gPY() + Circles[i].dy});

            float px = pos.gPX();
            float py = pos.gPY();
            if (pos.gPX() + circleSize / 2 >(float)width / 2) {
                px = (float)width / 2 - circleSize / 2;
                Circles[i].dx = -Circles[i].dx;
            }
            else if (pos.gPX() - circleSize / 2 < -(float)width / 2) {
                px = -(float)width / 2 + circleSize / 2;
                Circles[i].dx = -Circles[i].dx;
            }
            if (pos.gPY() + circleSize / 2 > (float)height / 2) {
                py = (float)height / 2 - circleSize / 2;
                Circles[i].dy = -Circles[i].dy;
            }
            else if (pos.gPY() - circleSize / 2 < -(float)height / 2) {
                py = -(float)height / 2 + circleSize / 2;
                Circles[i].dy = -Circles[i].dy;
            }

            frame.sPosition({ 0,0,px,py });

            

            if (not PhysicsGridJustGotUpdated)
                for (unsigned int di = 0; di < PhysicsGrid[Circles[i].physicsGridInd].size(); di++) {
                    if (PhysicsGrid[Circles[i].physicsGridInd][di] == i) {
                        PhysicsGrid[Circles[i].physicsGridInd].erase(PhysicsGrid[Circles[i].physicsGridInd].begin() + di);
                        break;
                    }
                }

            Circles[i].physicsGridInd = getPhysicsGridInd(px, py);
            PhysicsGrid[Circles[i].physicsGridInd].push_back(i);

        }

        {
            float ctime = Time::GetTime();
            float timeAdd = ctime - startTime;
            lengths[0] += timeAdd;
            startTime = ctime;
        }

        PhysicsGridJustGotUpdated = false;
        
        {
            unsigned int mainGridInd = 0;

#define collisionCheckCycle(oInd) {unsigned int cInd=oInd;\
for (unsigned int i1 = 0; i1 < PhysicsGrid[mainGridInd].size(); i1++) {\
    for (unsigned int i2 = 0; i2 < PhysicsGrid[oInd].size(); i2++) {\
        const Vector2& p1 = Circles[PhysicsGrid[mainGridInd][i1]].frame->gPosition().gPV();\
        const Vector2& p2 = Circles[PhysicsGrid[oInd][i2]].frame->gPosition().gPV();\
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

        {
            float ctime = Time::GetTime();
            float timeAdd = ctime - startTime;
            lengths[1] += timeAdd;
            startTime = ctime;
        }


        camRot += 1.f;
        cube->sRotationFromParent(Matrix::FromAnglesXYZ(camRot / 180.f * 3.14f, camRot / 180.f * 3.14f, 0.f));
        //hierarchy->sCameraRotation(3.14f / 180.f * camRot);

        for (unsigned int i = 0; i < circlesAmount; i++) {
            auto& frame = *Circles[i].frame;
            frame.RecalculateCoordinatesInSpace(false);
            frame.UpdateVertexesDataInRenderingData(false);
        }

        {
            float ctime = Time::GetTime();
            float timeAdd = ctime - startTime;
            lengths[2] += timeAdd;
            startTime = ctime;
        }

        translatorTo3D->RenderInVirtualSpace();
        hierarchy->RenderInVirtualSpace();


        hierarchy->Render();

        {
            float ctime = Time::GetTime();
            float timeAdd = ctime - startTime;
            lengths[3] += timeAdd;
            startTime = ctime;
        }
        

        wind.EndUpdatingWindow();

        framesCounter++;
        float lensum = lengths[0] + lengths[1] + lengths[2] + lengths[3];
        if (lensum > 1) {
            std::cout << framesCounter;


            
            //move by delta and bound to screen, apply physics, update positions, render

            //on 0: 85.9595%, 10.3213%, 0.206302%, 3.5129%
            //on 50: 17.4993%, 3.28446%, 49.1153%, 30.1009%
            //on 100: 13.7268%, 3.15427%, 53.2648%, 29.854%
            //on 1000: 11.4458%, 5.46018%, 54.0866%, 29.0074%
            //on 3000: 11.3242%, 8.13164%, 52.7283%, 27.8159%


            for (unsigned int i = 0; i < 4; i++) { 
                std::cout << ", " << lengths[i] / lensum * 100.f << "%";
                lengths[i] = 0.f; 
            }
            std::cout << '\n';
            framesCounter = 0;
        }


    }

    wind.Destroy();
    WindowsManager::Uninitialize();
    return 0;

}