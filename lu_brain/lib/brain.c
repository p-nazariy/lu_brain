/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

	#include "brain.h"

	// Basics

	#include "basic/_module.lu"
	#include "lib/_module.lu"
 
	// App

	#include "brain_config.lu" 
 	#include "n/_module.lu"
	#include "rec/_module.lu"
	#include "story/_module.lu"
	#include "w/_module.lu" 
	#include "s/_module.lu"

///////////////////////////////////////////////////////////////////////////////
// Brain API

	Lu_Brain lu_brain_create_from_predefined(lu_size size_in_bytes, lu_size predefined_config_id)
	{
		Lu_Brain_Config config = lu_brain_config_predefined_get(predefined_config_id);
		lu_user_assert(config, "Cannot get Lu_Brain_Config");

		config->size_in_bytes = size_in_bytes;

		return lu_brain_create(*config);
	}

	Lu_Brain lu_brain_create(struct lu_brain_config config)
	{
		Lu_Brain_Config p_config = lu_brain_config_validate(&config);
		lu_user_assert(p_config, "Lu_Brain_Config validation failed");

		Mem brain_mem			= (Mem) mem_perm_create(g_mem_temp, p_config->size_in_bytes); 
		lu_user_assert(brain_mem, "Cannot create brain_mem. Not enough memory?");

		Lu_Brain self 		= (Lu_Brain) mem_alloc(brain_mem, sizeof(struct lu_brain));
		lu_user_assert(self, "Cannot allocate Lu_Brain. Not enough memory?");
		
		self->brain_mem  	= brain_mem; 
		self->config 		= *p_config;
		self->recs 			= arr_create(brain_mem, p_config->recs_size, false);

		self->s_mem 		= s_mem_create(self);
		lu_user_assert(self->s_mem, "Cannot create S_Mem. Not enough memory?");

		self->n_mem 		= n_mem_create(self, p_config->names_size);
		lu_user_assert(self->n_mem, "Cannot create N_Mem. Not enough memory?");

		n_mem_tables_create(self->n_mem, brain_mem);

		lu_brain_print_info(self);

		return self;
	}

	void lu_brain_destroy(Lu_Brain self)
	{
		mem_destroy(self->brain_mem, g_mem_temp);
	}

	void lu_brain_print_info(Lu_Brain self)
	{
		lu_user_assert_void(self, "Lu_Brain is NULL");
		lu_user_assert_void(self->brain_mem, "Mem is NULL");
		Mem_Perm brain_mem = (Mem_Perm) self->brain_mem;

		lu_debug("\n\n---------> Brain #%lu Info <---------", self->config.id);
		lu_debug("\nMemory allocated (bytes): %lu", mem_perm_allocated(brain_mem));
		lu_debug("\nMemory used (bytes): %lu", mem_perm_used(brain_mem));

 		n_mem_print_info(self->n_mem);

 		s_mem_print_info(self->s_mem);

		lu_debug("\n-----------------------------------\n");
	}

	struct lu_brain_config lu_brain_config_get(Lu_Brain self)
	{
		if (self == NULL) 
		{
			lu_user_debug("Lu_Brain is NULL! Returning LU_BC_DEFAULT config");
			return *lu_brain_config_predefined_get(LU_BC_DEFAULT);
		}

		return self->config;
	}

	Lu_Brain lu_brain_reconfigure(Lu_Brain self, struct lu_brain_config config)
	{
		lu_user_assert(NULL, "Not implemented yet");
	}