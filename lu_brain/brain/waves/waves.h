/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

///////////////////////////////////////////////////////////////////////////////
// Nouns
	typedef struct sig* 						Sig;

	typedef struct save_wave_create_opts* 		Save_Wave_Create_Opts;
	typedef struct channel_data* 				Channel_Data;
	typedef struct save_wave* 					Save_Wave;

	typedef struct find_wave_create_opts*  		Find_Wave_Create_Opts;
	typedef struct find_wave* 					Find_Wave;

	typedef struct restore_wave_create_opts*	Restore_Wave_Create_Opts;
	typedef struct restore_wave* 				Restore_Wave;

///////////////////////////////////////////////////////////////////////////////
// Sig

	struct sig {
		lu_size 	neu_ent;
		lu_value 	p;
	};

///////////////////////////////////////////////////////////////////////////////
// Wave API

	// typedef here to break cyclic dependency
	typedef struct gate* 						Gate;

	struct wave {
		Gate 	gate;
	};

	static void wave_init(Wave self, Gate gate);

	
	struct wave_resp {
		
	};

///////////////////////////////////////////////////////////////////////////////
// Save 

	struct save_wave_create_opts
	{
		lu_size 			max_story_size_in_blocks;
	};

	static void save_wave_create_opts_init(Save_Wave_Create_Opts);

	struct save_opts {

	};

	struct channel_data {
		lu_size width;
		lu_size height;
		lu_size block_count;
		lu_p_value data;
	};

	static inline lu_size channel_data_size(Channel_Data self)
	{
		return sizeof(lu_value) * self->width * self->height * self->block_count;
	}

	static Channel_Data channel_data_create(Mem mem, Rec rec, lu_size block_count);

	struct save_wave {
		struct wave 		super;
		struct save_opts 	save_opts;
		Story 				story;

		// tymchasove zberezhenia linkiv na prev data 
		// vidsutnist danyh oznachaye povtoruvanist danyh
		lu_p_value* 		fill_data;
	};

	struct save_resp {

	};

	static void save_wave_init(Save_Wave, Story, Save_Opts);

	static void save_wave_process(Save_Wave self);

	static void save_wave_prev_data_reset(Save_Wave self);


//////////////////////////7/////////////////////////////////////////////////////
// Find

	struct find_wave_create_opts
	{

	};

	static void find_wave_create_opts_init(Find_Wave_Create_Opts);

	struct find_opts {

	};

	struct find_wave {
		struct wave 		super;
		struct find_opts 	find_opts;
		Story 				story;
	};

	struct find_resp {

	};

	static void find_wave_init(Find_Wave, Story, Find_Opts);

	static void find_wave_process(Find_Wave self);

///////////////////////////////////////////////////////////////////////////////
// Perc API

lu_p_void perc_cont_create(Rec rec, lu_p_void prev, lu_p_void curr, lu_p_void next);

///////////////////////////////////////////////////////////////////////////////
// Restore API

	struct restore_wave_create_opts
	{

	};

	static void restore_wave_create_opts_init(Restore_Wave_Create_Opts);

	struct restore_opts {

	};

	struct restore_wave {
		struct wave 			super;
		struct restore_opts 	restore_opts;
		Brain 					brain;
	};

	struct restore_resp {

	};

	static void restore_wave_init(Restore_Wave, Brain, Restore_Opts);

	static void restore_wave_process(Restore_Wave self);