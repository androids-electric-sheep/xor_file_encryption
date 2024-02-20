all:
	clang-format -i main.c
	cppcheck main.c
	clang main.c -o xor_encrypt
clean:
	rm -vf xor_encrypt
