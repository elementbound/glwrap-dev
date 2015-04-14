CXX_FLAGS = -std=c++11 -I../
COMMON_DEPS = bin/window.o bin/resizable_window.o bin/shader.o bin/util.o bin/basic_mesh.o bin/separated_mesh.o bin/texture.o bin/texture_util.o
LINKER_SETTINGS = -lglfw3 -lgdi32 -lopengl32 -lglew32 -lpng -lz

build: 
	g++ ${CXX_FLAGS} -c ../glwrap/window.cpp -o bin/window.o
	g++ ${CXX_FLAGS} -c ../glwrap/resizable_window.cpp -o bin/resizable_window.o
	g++ ${CXX_FLAGS} -c ../glwrap/shader.cpp -o bin/shader.o
	g++ ${CXX_FLAGS} -c ../glwrap/util.cpp -o bin/util.o
	g++ ${CXX_FLAGS} -c ../glwrap/mesh/basic_mesh.cpp -o bin/basic_mesh.o
	g++ ${CXX_FLAGS} -c ../glwrap/mesh/separated_mesh.cpp -o bin/separated_mesh.o
	g++ ${CXX_FLAGS} -c ../glwrap/meshutil.cpp -o bin/meshutil.o
	g++ ${CXX_FLAGS} -c texture.cpp -o bin/texture.o
	g++ ${CXX_FLAGS} -c texture_util.cpp -o bin/texture_util.o
	g++ ${CXX_FLAGS} -c texturedev.cpp -o bin/texturedev.o
	g++ ${CXX_FLAGS} -c cubemap.cpp -o bin/cubemap.o
	g++ bin/texturedev.o ${COMMON_DEPS} ${LINKER_SETTINGS} -o texturedev
	g++ bin/cubemap.o ${COMMON_DEPS} bin/meshutil.o ${LINKER_SETTINGS} -o cubemap
	copy /Y ..\glew32.dll .\glew32.dll
	
clean: 
	del bin\*.o