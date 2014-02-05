#include <stdio.h>
#include <lock.h>


int main() {
	static __thread int tlsbss, tlsdata = 2;
	static glbbss, glbdata = 4;
	printf("I just came to say hello!\n");
	printf("tls is at %p %p,\n", &tlsbss, &tlsdata);
	printf("glb at %p %p\n", &glbbss, &glbdata);
	tlsbss = 43;
	printf("tls data is %d,\n", tlsdata);
	printf("tls bss is %d\n", tlsbss);
	glbbss = 42;
	printf("glb data is %d\n", glbdata);
	printf("glb bss is %d\n", glbbss);

//	mutex_t mutex;
//	mutex_lock(&mutex);
	for (;;);
	return 0;
}
