#include <mm/mm.h>
#include <task/thread.h>
#include <loader.h>

extern uint32_t _testerpayload;
extern uint32_t _testerpayload_end;

struct thread_t *load_tester() {
	uint8_t *start = &_testerpayload, *end = &_testerpayload_end;
	const char *name = "Andix tester";
	struct user_process_t *proc = create_user_process(start, end - start, name);
	if(proc == NULL) {
		return NULL;
	}
	return (struct thread_t*) proc->threads.head->data;
}
