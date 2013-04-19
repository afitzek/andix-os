/*
 * swi_secure.c
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <swi/swi.h>
#include <tee/tee_constants.h>
#include <tee/tee_context.h>
#include <tee/tee_session.h>

void swi_sysd_install(uint8_t* mem, uint32_t length) {
	task_info("KERNEL INSTALLING 0x%X", mem);
}

void swi_set_uuid(TASK_UUID* uuid) {
	if (!is_valid_user_addr(uuid)) {
		// TODO: KILL TASK!
		return;
	}

	task_t* current = get_current_task();
	cpy_uuid(&(current->uuid), uuid);
}

void swi_get_secure_request(TA_RPC* rpc) {
	tee_session* session = NULL;
	tee_context* context = NULL;
	uint8_t procceed = 0;
	TZ_TEE_SPACE* tee = mon_get_tee_space();

	if (!is_valid_user_addr(rpc)) {
		// TODO: KILL TASK!
		return;
	}

	task_t* task = get_current_task();

	if (!is_valid_kernel_addr(task)) {
		// TODO: kernel panic!
		swi_error("CALLED swi_get_secure_request with current task is NULL!");
	}

	// TODO: process answer
	if (tee != NULL ) {
		switch (tee->op) {
		case TZ_TEE_OP_OPEN_SESSION:
			if (rpc->operation == TA_OPEN_SESSION) {
				session = tee_session_find(tee->params.openSession.session);
				if (session == NULL ) {
					tee->ret = TEE_ERROR_GENERIC;
					tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
					tee_error("Failed to find session!");
					break;
				}
				tee->ret = TEE_SUCCESS;
				tee->params.openSession.returnOrigin = TEE_ORIGIN_TRUSTED_APP;
				tee_info("Created session %d", session->_id);
			} else {
				tee->ret = TEE_ERROR_GENERIC;
				tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
				tee_error("Invalid rpc request for tee request!");
			}
			break;
		}
	}

	do {
		// block current task ...
		get_current_task()->state = BLOCKED;
		// wait for TEE request ...
		__smc_1(SSC_TASK_SCHEDULE, 0);

		tee = mon_get_tee_space();
		if (tee != NULL ) {
			switch (tee->op) {
			case TZ_TEE_OP_OPEN_SESSION:
				context = tee_context_find(tee->params.openSession.context);
				if (context == NULL ) {
					tee->ret = TEE_ERROR_BAD_PARAMETERS;
					tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
					tee_error("Failed to find context!");
					continue;
				}

				session = tee_session_create(context);
				if (session == NULL ) {
					tee->ret = TEE_ERROR_OUT_OF_MEMORY;
					tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
					tee_error("Failed to create session!");
					continue;
				}

				tee->params.openSession.session = session->_id;
				rpc->operation = TA_OPEN_SESSION;
				rpc->paramTypes = tee->params.openSession.operation.paramTypes;
				memcpy(rpc->tee_param, tee->params.openSession.operation.params,
						sizeof(TEEC_Paramter));

				rpc->sessionContext = (void*) &session->tee_session_handle;
				break;
			}
		}
	} while (procceed == 0);
}
