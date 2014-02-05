#include <communication_types.h>

typedef uint32_t mutex_t;
typedef uint32_t sem_t;

#define LOCKED		1
#define UNLOCKED	0

// forward decl. for every usecase :)
void yield(void);

/*
 * Mutex using exclusive monitor instructions based on:
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dht0008a/ch01s03s02.html
 * It is important, that everything that can run between LDREX and STREX clears
 * the monitor with CLREX to force a retry. Otherwise the operation is not save.
 *
 * GCC inline asm-adapted. GCC optimizes this quite well (try -O1).
 */

inline static void mutex_init(mutex_t *m) {
	*m = UNLOCKED;
}

inline static void mutex_lock(mutex_t *m) {
	mutex_t val;
	uint32_t retry;

	while (1) {			// (1) and break below gives slightly better asm output (one cmp instruction less :) because continue has no conditon to check)
		asm volatile (
				"LDREX %0, [%1] \n"
				: "=r" (val)
				: "r" (m)
				: "memory");
		if (val == LOCKED) {
			yield();
			continue;
		}
		asm volatile (
				"STREX %0, %2, [%1] \n"
				: "=&r" (retry)
				: "r" (m), "r" (LOCKED)
				: "memory"
				);
		if (!retry)
			break;
	}

	asm volatile ("DMB \n"		// Data Memory Barrier required for consistent memory view.
			::: "memory");
}

// The thread must hold the mutex, therefore it is save to use a non-exclusive str.
// The str invalidates an exclusive monitor for this address.
inline static void mutex_unlock(mutex_t *m) {
	asm volatile ("DMB \n"
			::: "memory");
	*m = UNLOCKED;
}


inline static void sem_init(sem_t *s, sem_t val) {
	*s = val;
}

inline static void sem_dec(sem_t *s) {
	sem_t val;
	uint32_t retry;

	while (1) {
		asm volatile (
				"LDREX %0, [%1] \n"
				: "=r" (val)
				: "r" (s)
				: "memory");
		if (val == 0) {
			yield();
			continue;
		}
		val--;
		asm volatile (
				"STREX %0, %2, [%1] \n"
				: "=&r" (retry)
				: "r" (s), "r" (val)
				: "memory"
				);
		if (!retry)
			break;
	}

	asm volatile ("DMB \n"
			::: "Memory");
}

inline static void sem_inc(sem_t *s) {
	sem_t val;
	uint32_t retry;

	asm volatile ("DMB \n"
			::: "Memory");
	while (1) {
		asm volatile (
				"LDREX %0, [%1] \n"
				: "=r" (val)
				: "r" (s)
				: "memory");
		val++;
		asm volatile (
				"STREX %0, %2, [%1] \n"
				: "=&r" (retry)
				: "r" (s), "r" (val)
				: "memory"
				);
		if (!retry)
			break;
	}
}
