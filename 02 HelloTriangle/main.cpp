#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>


int main()
{
	glfwInit();
	auto window = glfwCreateWindow( 300, 300, "Hello, World", {}, {} );
	glfwMakeContextCurrent( window );

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


	std::ifstream file( R"(./vertex.glsl)" );
	if ( !file.is_open() ) {
		throw std::invalid_argument( "file is not found" );
	}

	// vertex shader
	auto vertexShader = 0u;
	{
		std::ifstream fstream( R"(./vertex.glsl)" );
		
		std::stringstream sstream;
		sstream << fstream.rdbuf();

		auto code = sstream.str();
		auto shaderCode = code.c_str();
		vertexShader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vertexShader, 1, std::addressof( shaderCode ), {} );
		glCompileShader( vertexShader );

		int success{};
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, std::addressof( success ) );

		if ( !success ) {
			char infoLog[ 512 ]{};
			glGetShaderInfoLog( vertexShader, static_cast<GLsizei>( std::size( infoLog ) ), {}, infoLog );
			std::cout << "vertex shader was failed to compile\n" << infoLog << std::endl;

			throw std::invalid_argument( "vertext shader compiling was failed" );
		}
	}
	
	// fragment shader
	auto fragmentShader = 0u;
	{
		std::ifstream fstream( R"(./fragment.glsl)" );
		std::stringstream sstream;
		sstream << fstream.rdbuf();
		
		auto code = sstream.str();
		auto shaderCode = code.c_str();
		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fragmentShader, 1, std::addressof( shaderCode ), {} );
		glCompileShader( fragmentShader );

		int success{};
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, std::addressof( success ) );

		if ( !success ) {
			char infoLog[ 512 ]{};
			glGetShaderInfoLog( vertexShader, static_cast<GLsizei>( std::size( infoLog ) ), {}, infoLog );
			std::cout << "fragment shader was failed to compile\n" << infoLog << std::endl;

			throw std::invalid_argument( "fragment shader compiling was failed" );
		}
	}
	
	// linking shader
	auto shaderProgram = 0u;
	{
		shaderProgram = glCreateProgram();

		glAttachShader( shaderProgram, vertexShader );
		glAttachShader( shaderProgram, fragmentShader );
		glLinkProgram( shaderProgram );

		int success{};
		glGetProgramiv( shaderProgram, GL_LINK_STATUS, std::addressof( success ) );

		if ( !success ) {
			char infoLog[ 512 ]{};
			glGetProgramInfoLog( shaderProgram, static_cast<GLsizei>( std::size( infoLog ) ), {}, infoLog );
			std::cout << "shader linking was failed to compile\n" << infoLog << std::endl;

			throw std::invalid_argument( "shader linking was failed" );
		}

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
	}
	

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VAO{}, VBO{};
	glGenVertexArrays( 1, std::addressof( VAO ) );
	glGenBuffers( 1, std::addressof( VBO ) );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), {} );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );

	while ( !glfwWindowShouldClose( window ) ) {
		glClear( GL_COLOR_BUFFER_BIT );
		
		glUseProgram( shaderProgram );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteProgram( shaderProgram );
	glfwTerminate();
	return{};
}