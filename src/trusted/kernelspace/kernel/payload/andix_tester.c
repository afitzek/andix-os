#include <mm/mm.h>
#include <task/task.h>
#include <loader.h>

extern uint32_t _testerpayload;
extern uint32_t _testerpayload_end;

task_t *load_tester() {
	uint32_t *start = &_testerpayload, *end = &_testerpayload_end;
	const char *name = "Andix tester";
	task_t *task = create_user_task(start, end - start);
	if(task == NULL) {
		return (task);
	}
	task_set_name(task, name);
	add_task(task);
	init_userspace_task(task);
	return (task);
}
