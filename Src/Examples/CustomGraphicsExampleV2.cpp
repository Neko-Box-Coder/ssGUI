#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "glad/glad.h"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

//Modified from https://github.com/opengl-tutorials/ogl/blob/master/common/shader.cpp
int CreateShaders(std::string& vertexShaderCode, std::string& fragmentShaderCode)
{
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint result = GL_FALSE;
    int infoLogLength = 0;

    // Compile Vertex Shader
    ssLOG_LINE("Compiling vertex shader");
    const char* vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> VertexShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
        ssLOG_LINE(&VertexShaderErrorMessage[0]);
    }
    
    // Compile Fragment Shader
    ssLOG_LINE("Compiling fragment shader");
    const char* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> FragmentShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        ssLOG_LINE(&FragmentShaderErrorMessage[0]);
    }
    
    // Link the program
    ssLOG_LINE("Linking program");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        ssLOG_LINE(&ProgramErrorMessage[0]);
    }
    
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

void InitiateRendering( GLuint& vertexArrayID, GLuint& programID, GLuint& vertexbuffer)
{
    std::string vertexShader =  
        "#version 330 core\n\
        \
        // Input vertex data, different for all executions of this shader.\n\
        layout(location = 0) in vec3 vertexPosition_modelspace;\
        \
        void main()\
        {\
            gl_Position.xyz = vertexPosition_modelspace;\
            gl_Position.w = 1.0;\
        }";
    
    std::string fragmentShader =    
        "#version 330 core\n\
        \
        // Ouput data\n\
        out vec4 color;\
        \
        void main()\
        {\
            color = vec4(1,0,0,1);\
        }";

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = CreateShaders(vertexShader, fragmentShader);

    //This is in clip space, so they are normalzied (0 to 1) instead of from 0 to width/height
    static const GLfloat g_vertex_buffer_data[] = 
    { 
        -1.0f, -1.0f, 0.5f,
         1.0f, -1.0f, 0.5f,
         0.0f,  1.0f, 0.5f,
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);    
}

void RenderGraphics(GLuint programID, GLuint vertexbuffer, ssGUI::MainWindow& mainWindow)
{
    //If we want to draw graphics on top of ssGUI, we need to call this 
    //  so that GUI is drawn to backbuffer before we draw anything to the back buffer
    //mainWindow.GetBackendDrawingInterface()->DrawToBackBuffer();

    mainWindow.SaveDrawingState();                   //Saves the state ssGUI was using
    mainWindow.SetRenderContext();            //Set the mainWindow to be the current context for OpenGL calls

    glEnable(GL_DEPTH_TEST);
    
    // Use our shader
    glUseProgram(programID);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Enable first attribute buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);
    glUseProgram(0);
    
    glDisable(GL_DEPTH_TEST);
    mainWindow.RestoreDrawingState();                //Restores back the state ssGUI was using
}

void CalculateAndShowFPS(ssGUI::ssGUIManager& guiManager, ssGUI::Text& fpsText)
{
    static uint64_t lastRecordedTime = guiManager.GetElapsedTimeInMillisecond();   //Record the last time we displayed FPS
    static int frameCount = 0;
    if(guiManager.GetElapsedTimeInMillisecond() - lastRecordedTime > 1000)
    {
        fpsText.SetText("Fps: "+std::to_string(frameCount));                       //Set the content of the text GUI Object every second
        lastRecordedTime = guiManager.GetElapsedTimeInMillisecond();
        frameCount = 0;
    }
    frameCount++;
}

using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;

    auto* fpsText = mainWindow.AddChild<ssGUI::Text>();
    fpsText->SetNewTextColor(glm::u8vec4(0, 255, 0, 255));                          //Set the fps text color to green
    fpsText->SetTextAlignment(AlignmentHorizontal::RIGHT, AlignmentVertical::TOP);      //Align content to top-right within itself
    fpsText->SetAnchorType(ssGUI::Enums::AnchorType::TOP_RIGHT);                    //Anchor the text widget to the top-right of the window

    //NOTE: It is very important to do any GL calls *AFTER* the first MainWindow creation
    //      because it loads GLAD when a MainWindow is created the first time.
    GLuint vertexArrayID = 0; 
    GLuint programID = 0; 
    GLuint vertexbuffer = 0;
    InitiateRendering(vertexArrayID, programID, vertexbuffer);

    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            CalculateAndShowFPS(guiManager, *fpsText);
            RenderGraphics(programID, vertexbuffer, mainWindow);    //Render our graphics with the function we created earlier
        }
    );
    guiManager.SetRedrawEveryFrame(true);               //Forces ssGUIManager to render every frame
    guiManager.SetTargetFramerate(-1);                  //By default, ssGUIManager limits the (update) framerate to 60.
    guiManager.StartRunning();                          //      We set it to a negative number to disable it.
    
    // Cleanup GL stuff
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(programID);
    return 0;
}