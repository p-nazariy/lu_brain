/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

///////////////////////////////////////////////////////////////////////////////
// Save 

	struct w_save_create_opts
	{
		lu_size 				blocks_max_size;
	};

	static void save_wave_create_opts_init(W_Save_Create_Opts);

	struct lu_save_opts {	
		lu_value 				contrast;
	};

	struct w_save {
		struct lu_wave 			super;
		struct lu_save_opts 	save_opts;

		// tymchasove zberezhenia linkiv na prev data 
		// vidsutnist danyh oznachaye povtoruvanist danyh
		lu_p_value* 			fill_data;
	}; 
	
	struct lu_save_resp {

	};

	static void w_save_init_and_reset(W_Save, Lu_Story, Lu_Save_Opts);
	static void w_save_data_send(W_Save self, Lu_Save_Opts opts, S_Rec s_rec, lu_p_value data, lu_size block_i);

