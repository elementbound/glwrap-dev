build: 
	g++ -c -std=c++11 -I../ ../glwrap/window.cpp -o bin/window.o
	g++ -c -std=c++11 -I../ ../glwrap/resizable_window.cpp -o bin/resizable_window.o
	g++ -c -std=c++11 -I../ ../glwrap/shader.cpp -o bin/shader.o
	g++ -c -std=c++11 -I../ ../glwrap/util.cpp -o bin/util.o
	g++ -c -std=c++11 -I../ ../glwrap/texture2d.cpp -o bin/texture2d.o
	g++ -c -std=c++11 -I../ ../glwrap/texture_util.cpp -o bin/texture_util.o
	g++ -c -std=c++11 -I../ ../glwrap/mesh/basic_mesh.cpp -o bin/basic_mesh.o
	g++ -c -std=c++11 -I../ ../glwrap/mesh/separated_mesh.cpp -o bin/separated_mesh.o
	g++ -c -std=c++11 -I../ meshutil.cpp -o bin/meshutil.o
	g++ -c -std=c++11 -I../ objloader.cpp -o bin/objloader.o
	g++ bin/*.o -lglfw3 -lgdi32 -lopengl32 -lglew32 -lpng -lz -o objloader
	copy /Y ..\glew32.dll .\glew32.dll