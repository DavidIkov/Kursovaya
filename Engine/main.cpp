#include<iostream>
#include<string>


#include"IerarchyClasses/Classes/Program.h"
#include"IerarchyClasses/Classes/Frame.h"

#include"GraphicsPrimitives/WindowsManager.h"

#include"GraphicsPrimitives/ShaderProgramManager.h"



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        std::cout << "pressed e??\n";
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << xpos << ", " << ypos << '\n';
}


int main() {
    
    WindowsManager::Initialize();

    auto monitorData = WindowsManager::getMonitorData(WindowsManager::getCurrentMonitor());

    Window wind(monitorData->width/2, monitorData->height/2, "david window");

   
    wind.SetWindowColor(1.f, 0.5f, 0.f);
    wind.SetSwapInterval(1);
    
    ShaderProgram shader;
    ShaderProgramID shaderID = ShaderProgramManager::AddShaderProgram("main shader", &shader);


    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector2,"ScreenPosition"});
    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector2,"LocalPosition" });
    shader.VertexBufferData.push_back({ ShaderProgramTypeEnum::vector4,"Color" });

    shader.TransferData.push_back({ ShaderProgramTypeEnum::vector4 ,"VertexColor" });
    shader.VertexShaderCodeParts.push_back("t_VertexColor=i_Color;\ngl_Position = vec4(i_ScreenPosition.x, i_ScreenPosition.y, 0.f, 1.f);");
    shader.FragmentShaderCodeParts.push_back("o_PixelColor=t_VertexColor;");

    ShaderProgram shader2=shader;
    ShaderProgramID shader2ID = ShaderProgramManager::AddShaderProgram("main shader with circle", &shader2);

    shader2.TransferData.push_back({ ShaderProgramTypeEnum::vector2,"LocalPosition" });
    shader2.VertexShaderCodeParts.push_back("t_LocalPosition=vec2(i_LocalPosition.x, i_LocalPosition.y);");
    shader2.FragmentShaderCodeParts.push_back("if (t_LocalPosition.x*t_LocalPosition.x+t_LocalPosition.y*t_LocalPosition.y>1) o_PixelColor=vec4(0.f,0.f,0.f,0.f);");

    Program windProgram;
    Frame* frame = new Frame;

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
    frame2->Size.sSX(1.f);

    Frame* frame3 = new Frame;
    frame3->sShaderID(shaderID);
    frame3->ApplyVertexTemplate(VertexTemplateEnum::Square);
    frame3->SetVertexesParameterByIndex(2, { 1.f,0.9f,0.f,0.5f });
    frame3->sParent(frame2);
    frame3->sPriority(2.f);
    frame3->Position.sSX(1.f);
    frame3->Position.sSY(1.f);
    frame3->Size.sPX(100.f);
    frame3->Size.sPY(100.f);


    
    while (!wind.WindowWaitingToBeClosed()) {

        wind.StartUpdatingWindow();

        frame->Rotation += 0.0174f;

        int width, height;
        wind.GetWindowSize(&width, &height);

        float mouseX, mouseY;
        wind.GetCursorPosition(&mouseX, &mouseY);

        frame->Position.sPX(mouseX + frame->gLRSize().gX() / 2);
        frame->Position.sPY(mouseY + frame->gLRSize().gY() / 2);


        windProgram.Render(width,height);
        

        wind.EndUpdatingWindow();
    }

    windProgram.Delete();
    WindowsManager::Uninitialize();
    return 0;

}