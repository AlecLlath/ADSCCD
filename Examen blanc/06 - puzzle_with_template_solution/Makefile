all: puzzle_maker puzzle_with_template

puzzle_maker: puzzle_maker.c
	gcc -g -o puzzle_maker -D_GNU_SOURCE puzzle_maker.c -lcurl -ljansson

puzzle_with_template:
	pyinstaller -F --distpath . --clean puzzle_with_template.py
	rm -r build puzzle_with_template.spec

clean:
	rm puzzle_maker puzzle_with_template 
