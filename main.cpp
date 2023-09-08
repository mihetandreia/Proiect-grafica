#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include<vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "mesh.h"
#include "obiect.h"
#include "scena.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"

// functii ce creaza shaderele
GLuint createProgram(char* VSFile, char* FSFile);
std::string readFile(const char *filePath);

// functie ce creaza scena
// aceasta functie trebuie rescrisa de voi
Scena createScena(GLuint programShadere);


// functii ce initializeaza fereastra si
// proceseaza intrarile de la mouse si tastatura
GLFWwindow* initializari();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// configurari
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // initializam fereastra
    GLFWwindow* fereastra = initializari();

    // cream shaderele si folosim programul creat
    // (avand o scena simpla folosim un singur program)
    GLuint program = createProgram("obiect1.vert","obiect1.frag");
    glUseProgram(program);

    //cream scena
    Scena S = createScena(program);


    while(!glfwWindowShouldClose(fereastra))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // procesam intrarile ferestrei: din taste si/sau mouse
        processInput(fereastra);

        // trimitem matricea projection catre shadere
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program,"projection"), 1, GL_FALSE,&projection[0][0]);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        S.DrawScene();

        glfwSwapBuffers(fereastra);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

Scena createScena(GLuint program){

    Scena aux = Scena();
    // load models
    // -----------


    // TO DO: aici incarcati modelele voastre
    // fie luati datele dintr-un fisier, fie puneti datele aici


    // aici aveti un exemplu extrem de sumar cum puteti incarca si folosi doua obiecte.

    locatie l = {0.0, -8.0, 0.0};
    rotatie r = {0.0, 1.0, 1.0, 1.0};
    scalare s = {1.0f, 1.0f, 1.0f};
    scalare s6 = {2.0f, 2.0f, 2.0f};
    scalare s1 = {0.1f, 0.1f, 0.1f};
    scalare s2 = {0.003f, 0.003f, 0.003f};
    scalare s3 = {0.0008f, 0.0008f, 0.0008f};
    scalare s4 = {0.3f, 0.3f, 0.3f};
    scalare s8 = {0.4f, 0.4f, 0.4f};
    scalare s5 = {0.09f, 0.09f, 0.09f};
    scalare s7 = {0.05f, 0.05f, 0.05f};
    scalare s10 = {0.03f, 0.03f, 0.03f};
    scalare s11 = {0.04f, 0.04f, 0.04f};

    // exemplu de a crea o bucata de "podea" de gresie
    aux.addObiect(Obiect(0, "resurse/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj", l, r, s, program));
    //aux.addObiect(Obiect(0, "resurse/grass/grass.obj", l, r, s, program));
    //aux.addObiect(Obiect(0, "resurse/grass/OBJ/GreenLawn.obj", l, r, s1, program));
    aux.addRotatieObiect(0,{270, 1, 0, 0});
       // aux.addRotatieObiect(0,{90, 1, 0, 0});
    // refolosim modelul folosit in alt obiect pentru a reduce consumul de memorie pe GPU
    // precum si traficul de date spre GPU
   // aux.addObiect(Obiect(1, aux.getObiect(0), {1.0, 0.0, 0.0}, r, s, program));
    // putem crea o suprafata de 20 pe 20 cu ele
    //for(int i = 0; i < 5; i++){
   //     for(int j = 0; j < 5; j++){
    //            if (i+j*5 > 1){ //ca sa nu mai adaugam cele 2 obiecte create anterior
     //               aux.addObiect(Obiect(i+j*5, aux.getObiect(0), {i*1.0, 0.0, j*1.0}, r, s, program));
      //         }
      //  }
  // }

    // putem pune pereti de caramida la marginea lor


    //aux.addObiect(Obiect(500, "resurse/sky/brick.obj", {-0.5,0.5,0}, {90, 1, 0, 0}, s, program));
    // adaugam o rotatie acolo pentru a fi corect amplasat
    //aux.addRotatieObiect(500,{90, 1, 0, 0});

    // putem pune peretii de jur imprejurul suprafetei de 20 pe 20
    //for(int i = 0; i < 50; i++){
    //    aux.addObiect(Obiect(500+i+1, aux.getObiect(500), {i,0.5,-0.5}, {90, 0, 1, 0}, s, program));
    //}
    // observati ca putem trece prin pereti, si ca textura se repeta deranjant
    // solutii:
    // puteti impune pozitiei camerei restrictii - sa nu trecem prin pereti
    // folositi mai multe obiecte tip 'perete' luate aleator, si nu repetati acelasi obiect.

    //aux.addObiect(Obiect(13, "resurse/Trees.obj", {30,0,10}, r, s, program));
    //aux.addObiect(Obiect(17, "resurse/cottage/house.obj", {5,0,35}, r, s6, program));
    //aux.addRotatieObiect(17,{90, 0, 1, 0});
///copaci
    aux.addObiect(Obiect(19, "resurse/tree1/Tree.obj", {15,0,10}, r, s, program));

   int i=19;
    aux.addObiect(Obiect(19, "resurse/tree1/Tree.obj", {20,0,10}, r, s, program));
    aux.addObiect(Obiect(i*2, aux.getObiect(19), {45,0,20}, r, s, program));
    aux.addObiect(Obiect(i*3,aux.getObiect(19), {35,0,10}, r, s, program));
    aux.addObiect(Obiect(i*4,aux.getObiect(19), {45,0,10}, r, s, program));
    aux.addObiect(Obiect(i*5, aux.getObiect(19), {15,0,10}, r, s, program));
    aux.addObiect(Obiect(i*6, aux.getObiect(19), {45,0,25}, r, s, program));
    aux.addObiect(Obiect(i*7, aux.getObiect(19), {45,0,30}, r, s, program));
    aux.addObiect(Obiect(i*8, aux.getObiect(19),  {45,0,35}, r, s, program));
    aux.addObiect(Obiect(i*9, aux.getObiect(19), {45,0,40}, r, s, program));
    aux.addObiect(Obiect(i*10,aux.getObiect(19), {45,0,45}, r, s, program));
    aux.addObiect(Obiect(i*11,aux.getObiect(19), {45,0,50}, r, s, program));
    aux.addObiect(Obiect(i*12,aux.getObiect(19), {45,0,55}, r, s, program));
    aux.addObiect(Obiect(i*13, aux.getObiect(19), {45,0,60}, r, s, program));
    aux.addObiect(Obiect(i*14, aux.getObiect(19), {35,0,55}, r, s, program));
    aux.addObiect(Obiect(i*15,aux.getObiect(19), {35,0,50}, r, s, program));
    aux.addObiect(Obiect(i*16, aux.getObiect(19), {35,0,38}, r, s, program));
    aux.addObiect(Obiect(i*17, aux.getObiect(19), {35,0,22}, r, s, program));

    aux.addObiect(Obiect(i*4,aux.getObiect(19), {-1,0,10}, r, s, program));
    aux.addObiect(Obiect(i*5, aux.getObiect(19), {-10,0,10}, r, s, program));
    aux.addObiect(Obiect(i*6, aux.getObiect(19), {-10,0,25}, r, s, program));
    aux.addObiect(Obiect(i*7, aux.getObiect(19), {-10,0,30}, r, s, program));
    aux.addObiect(Obiect(i*8, aux.getObiect(19),  {-10,0,35}, r, s, program));
    aux.addObiect(Obiect(i*9, aux.getObiect(19), {-10,0,40}, r, s, program));
    aux.addObiect(Obiect(i*10,aux.getObiect(19), {-15,0,45}, r, s, program));
    aux.addObiect(Obiect(i*11,aux.getObiect(19), {-15,0,50}, r, s, program));
    aux.addObiect(Obiect(i*12,aux.getObiect(19), {-15,0,55}, r, s, program));
    aux.addObiect(Obiect(i*13, aux.getObiect(19), {-15,0,60}, r, s, program));
    aux.addObiect(Obiect(i*14, aux.getObiect(19), {-15,0,55}, r, s, program));
    aux.addObiect(Obiect(i*15,aux.getObiect(19), {-15,0,50}, r, s, program));
    aux.addObiect(Obiect(i*16, aux.getObiect(19), {-15,0,38}, r, s, program));
    aux.addObiect(Obiect(i*17, aux.getObiect(19), {-15,0,22}, r, s, program));


    //aux.addObiect(Obiect(27, "resurse/tree2/Hazelnut.obj", {30,0,10}, r, s, program));
    ///stiva lemne
    aux.addObiect(Obiect(46, "resurse/wood/wood1.obj", {10,0,23}, r, s, program));
    aux.addRotatieObiect(46,{45, 0, 1, 0});
    ///foc si cort
    aux.addObiect(Obiect(23, "resurse/fire1/campfire.obj", {25,0,25}, r, s4, program));
    aux.addRotatieObiect(23,{180, 0, 1, 0});
    ///pisica
    aux.addObiect(Obiect(27, "resurse/cat/12221_Cat_v1_l3.obj", {5,0,20}, r, s7, program));
    aux.addRotatieObiect(27,{-90, 1, 0, 0});
    aux.addRotatieObiect(27,{180, 0, 1, 0});
    ///broasca
    aux.addObiect(Obiect(43, "resurse/broasca/12268_banjofrog_v1_L3.obj", {20,0,20}, r, s5, program));
    aux.addRotatieObiect(43,{270, 1, 0, 0});
    ///caine 1 si 2
    aux.addObiect(Obiect(24, "resurse/dog/13463_Australian_Cattle_Dog_v3.obj", {3,0,40}, r, s1, program));
    aux.addRotatieObiect(24,{-90, 1, 0, 0});
    aux.addRotatieObiect(24,{-90, 0, 1, 0});

    aux.addObiect(Obiect(47, "resurse/dog/13466_Canaan_Dog_v1_L3.obj", {15,0,40}, r, s1, program));
    aux.addRotatieObiect(47,{-90, 1, 0, 0});
    aux.addRotatieObiect(47,{-90, 0, 1, 0});
    ///cal
    aux.addObiect(Obiect(29, "resurse/horse/10026_Horse_v01-it2.obj", {40,0,15}, r, s2, program));
    aux.addRotatieObiect(29,{-90, 1, 0, 0});
    aux.addRotatieObiect(29,{-90, 0, 1, 0});
    ///leagan
    aux.addObiect(Obiect(31, "resurse/swing/Obj.obj", {10,0,5}, r, s3, program));
    aux.addRotatieObiect(31,{360, 1, 0, 0});
    ///minge
    aux.addObiect(Obiect(45, "resurse/minge/10483_baseball_v1_L3.obj", {0,0,28}, r, s1, program));
    aux.addRotatieObiect(45,{270, 1, 0, 0});
    ///cos
    aux.addObiect(Obiect(40, "resurse/minge/10480_Archery_target_v1_max2011_iteration-2.obj", {22,0, 50}, r, s7, program));
    aux.addRotatieObiect(40,{-90, 1, 0, 0});
    aux.addRotatieObiect(40,{180, 0, 1, 0});
    ///bicicleta
    aux.addObiect(Obiect(37, "resurse/bic/11717_bicycle_v2_L1.obj", {0,0,30}, r, s7, program));
    aux.addRotatieObiect(37,{270, 1, 0, 0});
    ///caprioara
    aux.addObiect(Obiect(42, "resurse/capra/12961_White-Tailed_Deer_v1_l2.obj", {10,0,17}, r, s7, program));
    aux.addRotatieObiect(42,{270, 1, 0, 0});
    ///pasare
    aux.addObiect(Obiect(41, "resurse/pasare/12213_Bird_v1_l3.obj", {10,4.5,23}, r, s7, program));
    aux.addRotatieObiect(41,{-90, 1, 0, 0});
    aux.addRotatieObiect(41,{-90, 0, 1, 0});
    ///ursulet
    aux.addObiect(Obiect(41, "resurse/urs/11706_stuffed_animal_L2.obj", {27,0,25}, r, s7, program));
    aux.addRotatieObiect(41,{-90, 1, 0, 0});
    aux.addRotatieObiect(41,{180, 0, 1, 0});
    ///stropitoare
    aux.addObiect(Obiect(36, "resurse/sakura/Can.obj", {-5,0,16}, r, s7, program));
    aux.addRotatieObiect(36,{180, 0, 1, 0});
    ///lalele
    aux.addObiect(Obiect(38, "resurse/flower/12978_tulip_flower_l3.obj", {-6,0,15}, r, s7, program));
    aux.addRotatieObiect(38,{-90, 1, 0, 0});
    aux.addRotatieObiect(38,{180, 0, 1, 0});
    aux.addObiect(Obiect(39, "resurse/flower/12978_tulip_flower_l3.obj", {-4,0,15}, r, s7, program));
    aux.addRotatieObiect(39,{-90, 1, 0, 0});
    aux.addRotatieObiect(39,{180, 0, 1, 0});
    aux.addObiect(Obiect(40, "resurse/flower/12978_tulip_flower_l3.obj", {-5,0,15}, r, s7, program));
    aux.addRotatieObiect(40,{-90, 1, 0, 0});
    aux.addRotatieObiect(40,{180, 0, 1, 0});
    ///barza
    aux.addObiect(Obiect(42, "resurse/barza/12244_Bird_v1_L2.obj", {-2,0,12}, r, s10, program));
    aux.addRotatieObiect(42,{-90, 1, 0, 0});
    aux.addRotatieObiect(42,{180, 0, 1, 0});




    return aux;
}


GLFWwindow* initializari()
{
    /*
        functia initializeaza bibliotecile glfw si glad, creaza o fereastra si
            o ataseaza  unui context OpenGL
    */

    //initializam glfw
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cream fereastra
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "scena_01", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    // facem ca aceasta fereastra sa fie contextul curent


    //atasam fereastra contextului opengl
    //glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<" nu s-a initializat biblioteca GLAD!";
    }


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    return window;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


GLuint createProgram(char* VSFile, char* FSFile)
{

    /*
        functia creaza un program shader folosind ca fisiere sursa VSFile si FSFile
        IN: numele fisierelor sursa
        OUT: aliasul programului shader
    */

    GLuint vertexShader, fragmentShader, shaderProgram;
    int success;
    char infoLog[512];


    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    if( 0 == vertexShader )
    {
        std::cout << "Error creating vertex shader." << std::endl;
        exit(1);
    }

    std::string shaderCode = readFile(VSFile);
    const char *codeArray = shaderCode.c_str();
    glShaderSource( vertexShader, 1, &codeArray, NULL );

    glCompileShader(vertexShader);

    // verficam daca s-a reusit compilarea codului

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if( 0 == fragmentShader )
    {
        std::cout << "Error creating fragment shader." << std::endl;
        exit(1);
    }


    shaderCode = readFile(FSFile);
    codeArray = shaderCode.c_str();
    glShaderSource( fragmentShader, 1, &codeArray, NULL );


    glCompileShader(fragmentShader);

    // se verifica compilarea codului

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //cream programul

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // se verifica procesul de link

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath;
        std::cerr << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
