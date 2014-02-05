extern int main(int argc,char **argv);
extern void exit(int);

void _start() {
	char *argv[] = {};
	for(;;) {
		exit(main(0, argv));
	}
}
