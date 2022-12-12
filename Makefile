default: main

%: %.cpp
	g++ -I. $< -o babelki stb_image.cpp Cuboid.cpp BubbleSphere.cpp PlayerSphere.cpp Camera.cpp -lGLEW  -lGL -lglfw 
clean:
	rm a.out *.o *~ babelki
