#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"                                        //Extension for positioning
#include "glad/glad.h"
#include "ssLogger/ssLog.hpp"

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

void RenderGraphics(GLuint programID, GLuint vertexbuffer)
{
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
}

using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;

    ssGUI::Text fpsText;
    fpsText.SetNewCharacterColor(glm::u8vec4(0, 255, 0, 255));                          //Set the fps text color to green
    fpsText.SetAlignment(AlignmentHorizontal::RIGHT, AlignmentVertical::TOP);           //Align content to top-right within itself
    fpsText.SetParent(&mainWindow); 

    auto* posExt = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();  
    posExt->SetAlignment(AlignmentHorizontal::RIGHT, AlignmentVertical::TOP);           //Align the text GUI Object to top-right of the main window
    fpsText.AddExtension(posExt);                                                       //Attach the extension to the text GUI Object

    //NOTE: It is very important to do any GL calls *AFTER* the first MainWindow creation
    //      because it loads GLAD when a MainWindow is created the first time.
    GLuint vertexArrayID = 0; 
    GLuint programID = 0; 
    GLuint vertexbuffer = 0;
    InitiateRendering(vertexArrayID, programID, vertexbuffer);

    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    
    uint64_t lastSecond = guiManager.GetBackendInputInterface()->GetElapsedTime();      //Record the last time we displayed FPS
    int frameCount = 0;
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(mainWindow.IsClosed())                                                   //The main window can be closed when this listener is called.
                return;                                                                 //Similar to GUI Objects being marked for deletion.

            if(guiManager.GetBackendInputInterface()->GetElapsedTime() - lastSecond > 1000)
            {
                fpsText.SetText("Fps: "+std::to_string(frameCount));                    //Set the content of the text GUI Object every second
                lastSecond = guiManager.GetBackendInputInterface()->GetElapsedTime();
                frameCount = 0;
            }
            frameCount++;
            
            mainWindow.SaveState();                   //Saves the state ssGUI was using
            mainWindow.SetRenderContext();            //Set the mainWindow to be the current context for OpenGL calls
            
            RenderGraphics(programID, vertexbuffer);  //Render our graphics with the function we created earlier
            
            mainWindow.RestoreState();                //Restores back the state ssGUI was using
        }
    );
    guiManager.SetForceRendering(true);             //Forces ssGUIManager to render every frame
    guiManager.SetTargetFramerate(-1);              //By default, ssGUIManager limits the (update) framerate to 60.
    guiManager.StartRunning();                      //      We set it to a negative number to disable it.
    
    // Cleanup GL stuff
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(programID);
    return 0;
}