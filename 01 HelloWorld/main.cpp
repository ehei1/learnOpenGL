#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main()
{
	glfwInit();

	auto window = glfwCreateWindow( 300, 300, "Hello, World", {}, {} );
	glfwMakeContextCurrent( window );

	glewInit();

	while ( !glfwWindowShouldClose( window ) ) {
		glClear( GL_COLOR_BUFFER_BIT );
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();
	return{};
}