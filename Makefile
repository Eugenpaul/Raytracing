trace:
	g++ structure.cpp -ltbb io.cpp vectormath.cpp main.cpp tracing.cpp -o tracer
run: 
	./tracer -m examples/model.model -i examples/img.pgm
