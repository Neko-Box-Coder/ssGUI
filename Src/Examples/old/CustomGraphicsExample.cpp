#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/Layout.hpp"

// I don't have macOS, so <GL/gl.h> might also work, idk.
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <SFML/OpenGL.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Modified version of this:
//Credit to: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html

//===================================================================================
//OpenGL code
//===================================================================================
/* Global variables */
GLfloat anglePyramid = 0.0f;                // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;                   // Rotational angle for cube [NEW]
int refreshMills = 15;                      // refresh interval in milliseconds [NEW]

GLfloat PyramidRotateSpeed = 0.5 * 3;       // Rotational angle for pyramid [NEW]
GLfloat CubeRotateSpeed = 0.5 * 3;          // Rotational angle for pyramid [NEW]


/* Initialize OpenGL Graphics */
void initGL() 
{
    glEnable(GL_DEPTH_TEST);                                // Enable depth testing for z-culling
    glDepthFunc(GL_LESS);                                   // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                                // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() 
{
    glMatrixMode(GL_MODELVIEW);                                 // To operate on model-view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();                                           // Reset the model-view matrix
    glTranslatef(1.5f, 0.0f, -7.0f);                            // Move right and into the screen
    glRotatef(angleCube, 1.0f, 1.0f, 1.0f);                     // Rotate about (1,1,1)-axis [NEW]

    glBegin(GL_QUADS);                                          // Begin drawing the color cube with 6 quads
        // Top face (y = 1.0f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( 1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f,  1.0f);
        glVertex3f( 1.0f, 1.0f,  1.0f);

        // Bottom face (y = -1.0f)
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);

        // Front face  (z = 1.0f)
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f( 1.0f, -1.0f, 1.0f);

        // Back face (z = -1.0f)
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);

        // Left face (x = -1.0f)
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);

        // Right face (x = 1.0f)
        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(1.0f,  1.0f, -1.0f);
        glVertex3f(1.0f,  1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube

    // Render a pyramid consists of 4 triangles
    glLoadIdentity();                                   // Reset the model-view matrix
    glTranslatef(-1.5f, 0.0f, -6.0f);                   // Move left and into the screen
    glRotatef(anglePyramid, 1.0f, 1.0f, 0.0f);          // Rotate about the (1,1,0)-axis [NEW]

    glBegin(GL_TRIANGLES);                              // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(1.0f,0.0f,0.0f);       // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);       // Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glColor3f(0.0f,1.0f,0.0f);       // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid

    // Update the rotational angle after each refresh [NEW]
    anglePyramid += PyramidRotateSpeed;
    angleCube -= CubeRotateSpeed;
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) 
{   // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    
    // Enable perspective projection with fovy, aspect, zNear and zFar
    //gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glLoadMatrixf(glm::value_ptr(perspective));
}

//===================================================================================
//UI code
//===================================================================================
int main()
{
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 400));

    //Create a window for rotation speed control
    ssGUI::StandardWindow window;
    window.GetWindowTitleObject()->SetText("Rotation Control");
    window.SetParent(&mainWindow);
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
    window.GetExtension<ssGUI::Extensions::Layout>()->SetUpdateContainerMinMaxSize(false);   //By default, layout extension overrides the min max size
    window.SetMinSize(glm::vec2(240, 200));
    float sizes[] = {0.2, 0.2, 0.2, 0.4};
    window.GetExtension<ssGUI::Extensions::Layout>()->AddPreferredSizeMultiplier(sizes, 4);

    //Pyramid Rotation Speed Text
    ssGUI::Text pyramidText;
    pyramidText.SetText("Pyramid Rotation Speed:");
    pyramidText.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    pyramidText.SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
    pyramidText.SetParent(&window);

    //Pyramid Rotation Slider Slider
    ssGUI::Slider pyramidSlider;
    pyramidSlider.SetSize(glm::vec2(150, 10));
    auto ecb = ssGUI::Factory::Create<ssGUI::EventCallbacks::SliderValueFinishedChangingEventCallback>();
    ecb->AddEventListener
    (
        "pyramidControl",
        [&](ssGUI::EventInfo& info)
        {
            PyramidRotateSpeed = static_cast<ssGUI::Slider*>(info.EventCallbackContainer)->GetSliderValue() * 3;
        }
    );
    pyramidSlider.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
    pyramidSlider.AddEventCallback(ecb);
    window.GetExtension<ssGUI::Extensions::Layout>()->AddChildWithWrapper(&pyramidSlider);

    //Cube Rotation Speed Text
    ssGUI::Text* cubeText = pyramidText.Clone(true);
    cubeText->SetText("Cube Rotation Speed:");

    //Cube Slider Wrapper so that slider's size is not overriden by layout
    ssGUI::Slider* cubeSlider = static_cast<ssGUI::Slider*>(pyramidSlider.Clone(true));
    ecb = cubeSlider->GetAnyEventCallback<ssGUI::EventCallbacks::SliderValueFinishedChangingEventCallback>();
    ecb->RemoveEventListener("pyramidControl");
    ecb->AddEventListener
    (
        "cubeControl",
        [&](ssGUI::EventInfo& info)
        {
            CubeRotateSpeed = static_cast<ssGUI::Slider*>(info.EventCallbackContainer)->GetSliderValue() * 3;
        }
    );
    window.GetExtension<ssGUI::Extensions::Layout>()->AddChildWithWrapper(cubeSlider);

    //Create ssGUIManager and start it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject(&mainWindow);
    guiManager.SetForceRendering(true);

    //We want to draw GUI objects on top of OpenGL objects
    //GUI Object updates before being render, so we can just draw OpenGL objects after GUI Objects update
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            //Save OpenGL status
            mainWindow.GetBackendDrawingInterface()->SaveState();

            //Draw OpenGL scene
            initGL();
            reshape(mainWindow.GetSize().x, mainWindow.GetSize().y);
            display();
            
            //Restore OpenGL status
            mainWindow.GetBackendDrawingInterface()->RestoreState();
        }
    );

    guiManager.StartRunning();
    return 0;
}