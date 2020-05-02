/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

#ifndef _LU_N_MEM_H
#define _LU_N_MEM_H

///////////////////////////////////////////////////////////////////////////////
// Defs

	#define ENT_NULL 0

///////////////////////////////////////////////////////////////////////////////
// Nouns

	typedef struct neu_ent* 	Neu_Ent;
	typedef struct neu_b* 		Neu_B;
	typedef struct neu_name* 	Neu_Name;

	typedef struct lin_ent* 	Lin_Ent;
	typedef struct lin_b* 		Lin_B;


	typedef struct val_layer* 	Val_Layer;

	typedef struct n_col* 		N_Col;
	typedef struct n_rec*		N_Rec;
	typedef struct n_mem* 		N_Mem;

///////////////////////////////////////////////////////////////////////////////
// Neu

	enum neu_ent_id {
		NEU_B_ID = 0,
		NEU_NAME_ID,

		NEU_ENT_ID_SIZE
	};

	// "generic" zviazky pracuvaty ne budut, bo ne nesut prostorovoy 
	// informaciyi. My mayemo znaty prostorove i chasove "znachennia"
	// zviazkiv
	struct neu_ent {
		lu_size 		coms[NEU_ENT_ID_SIZE];

		// tut mozhna bulo prosto postvoruvaty zminni dlia kozhnoyi componenty
		// lu_size 	neu_b;
		// lu_size 	neu_name;
	};

	// basic 

	struct neu_b {
		lu_value 		count;

		lu_size 		l_p_lin_ent;
		lu_size 		l_c_lin_ent;
	};

	struct neu_name {
		lu_char 		name[255];
	};

///////////////////////////////////////////////////////////////////////////////
// Lin

	enum lin_ent_id {
		LIN_B_ID = 0,

		LIN_ENT_ID_SIZE
	};

	struct lin_ent {
		lu_size 		coms[LIN_ENT_ID_SIZE];
	};

	// basic
	struct lin_b {
		lu_size 		p_neu_ent;			// p i c potribni bo po tomu samomu zviazku my mozhemo jty vverh abo vnyz
		lu_size 		c_neu_ent;

		lu_size 		p_lin_ent_prev;
		lu_size 		p_lin_ent_next;

		lu_size 		c_lin_ent_prev; 	// prev i next potribni u vypadku yakshou my vydaliayemo zviazok?
		lu_size 		c_lin_ent_next; 	// yaksho my vydaliayemo zviazok, to my mozhemo podyvytys prev ale my neznayemo next
		 		   							// tomu naspravdi i prev i next potribni
	};


	// // ┣
	// // ┣
	// // ┣
	// // █
	// // ┣
	// // ┣
	// // ┣
	// struct lin2 {

	// 	Neu 	np; // dlia vsih output lin p bude odyn i toy samyy ale ce zrobleno dlia optymizaciyi
	// 	Neu 	nc;	// shvydkosti ruhy nazad po linku (shob ne potribno bulo shukaty nayvyshchyy lin)

	// 	Lin 	lp; // p link, bude NULL dlia pershoho lin, i my budemo dobavliaty lin speredu
	// 				// tobto samyy blyzhchyy do neu lin bude takozh samyy ostaniy
	// 	Lin 	lc; // c link 

	// 	// ce ne bude pracuvaty, bo nam treba robyty dva takyh linka (input i output), sho znachno bilshe
	// 	// pamiati, abo link tilky v odnomu napriamku
	// };

///////////////////////////////////////////////////////////////////////////////
// N_Mem
 
	enum n_col_type {
		NCT_COL,			
		NCT_PER, 
		NCT_PER_DIAG
	};

	struct n_col {
		N_Rec 				n_rec; 		// vlasnyk

		enum n_col_type 	type;

		lu_size 			x;
		lu_size 			y;

		List 				exc_neus;  	// poperedni i potochnyy zbudzheni neurony

		lu_size*			neus; 		// odnomirnyy masyv
	};

	static N_Col n_col_create(Mem mem, N_Rec rec, lu_size x, lu_size y, enum n_col_type);

	static lu_value n_col_norm(N_Col self, lu_value request); 
	static struct lu_size_range n_col_indx_range(N_Col self, lu_value val);
	static lu_value n_col_calc_sig(N_Col self, lu_size val_step_i, lu_value val);
	static lu_value n_col_step_norm_dist(N_Col self);

	struct n_rec {
		N_Mem 				n_mem; 			// vlasnyk
		Lu_Rec 				rec;			// poviazanyy z rec

		lu_size 			n_cols_w;
		lu_size 			n_cols_h; 
		N_Col* 				n_cols;
		N_Col*				n_col_colors;
		N_Col* 				n_col_pers;

		// Ci dani spilni dlia vsih n_cols
		// i odnakovi dlia znachen i perepadiv
		lu_value 			orig_min_val;
		lu_value 			orig_max_val;
		lu_value 			max_val;
		lu_value 			val_step;

		lu_value*			val_steps;  	// preobchysleni kroky
		lu_size 			val_ssp_i; 		// (tilky dlia poshuku) signif similarity percent 
		lu_size 			val_neu_size;
	};

	static N_Rec n_rec_create(N_Mem n_mem, Lu_Rec n_rec);

	static inline void n_rec_n_col_set(N_Rec self, lu_size x, lu_size y, N_Col n_col) { self->n_cols[x + y * self->n_cols_w] = n_col; }
	static inline N_Col n_rec_n_col_get(N_Rec self, lu_size x, lu_size y) { return self->n_cols[x + y * self->n_cols_w]; }

	struct n_mem {
		Lu_Brain 			brain;

		Mem_Table 			neu_ents;
		Mem_Table 			lin_ents;

		Mem_Table 			neu_bs;
		Mem_Table 			neu_names;

		Mem_Table 			lin_bs;
		// Mem_Table 		lin_weights;

		Arr 				n_recs;
	};
	
	static N_Mem n_mem_create(Lu_Brain brain);


///////////////////////////////////////////////////////////////////////////////
// N_Mem Neu getters

	static inline Neu_Ent neu_ent_get(N_Mem n_mem, lu_size neu_ent_id) 
	{  	
		return (Neu_Ent) mem_table_get(n_mem->neu_ents, neu_ent_id);
	} 

	static inline Neu_B neu_b_get(N_Mem n_mem, lu_size neu_b_id) 
	{  	
		return (Neu_B) mem_table_get(n_mem->neu_bs, neu_b_id);
	} 

	static inline Neu_Name neu_name_get(N_Mem n_mem, lu_size neu_name_id) 
	{  	
		return (Neu_Name) mem_table_get(n_mem->neu_names, neu_name_id);
	}

	static inline Neu_B neu_ent_b_get(N_Mem n_mem, lu_size neu_ent_id) 
	{  	
		Neu_Ent neu_ent = (Neu_Ent) mem_table_get(n_mem->neu_ents, neu_ent_id);
		lu_size com_id = neu_ent->coms[NEU_B_ID];
		return com_id ? neu_b_get(n_mem, com_id) : NULL;
	}

	static inline Neu_Name neu_ent_name_get(N_Mem n_mem, lu_size neu_ent_id)
	{
		Neu_Ent neu_ent = (Neu_Ent) mem_table_get(n_mem->neu_ents, neu_ent_id);
		lu_size com_id = neu_ent->coms[NEU_NAME_ID];
		return com_id ? neu_name_get(n_mem, com_id) : NULL;
	}

///////////////////////////////////////////////////////////////////////////////
// N_Mem Lin getters

	static inline Lin_Ent lin_ent_get(N_Mem n_mem, lu_size lin_ent_id) 
	{  	
		return (Lin_Ent) mem_table_get(n_mem->lin_ents, lin_ent_id);
	} 

	static inline Lin_B lin_b_get(N_Mem n_mem, lu_size lin_b_id) 
	{  	
		return (Lin_B) mem_table_get(n_mem->lin_bs, lin_b_id);
	} 

	static inline Lin_B lin_ent_b_get(N_Mem n_mem, lu_size lin_ent_id)
	{
		Lin_Ent lin_ent = (Lin_Ent) mem_table_get(n_mem->lin_ents, lin_ent_id);
		lu_size com_id = lin_ent->coms[LIN_B_ID];
		return com_id ? lin_b_get(n_mem, com_id) : NULL;
	}

#endif // _LU_N_MEM_H
