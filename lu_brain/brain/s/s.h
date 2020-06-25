/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

#ifndef _LU_S_H
#define _LU_S_H

///////////////////////////////////////////////////////////////////////////////
// S Net
// 

	struct s_v_link {
		S_Neu 					vl;
		S_Neu 					vp;
		S_Neu 					nl;
		S_Neu 					np;
	};

	static inline S_V_Link s_v_link_init(S_V_Link self);

	struct s_neu {
		enum s_net_type    		type;			

		S_Layer 				layer;
		lu_size 				x;    	// bude t index dlia a_neu
		lu_size 				y;
		lu_size 				z;

		S_V_Link				b;
		S_V_Link				d;

		S_Neu  	 				t;		// zviazok po t		

		// she maye buty zviazok z s_neu z inshyh s_rec na tyh rivniah de ce 
		// mozhlyvo

		S_Neu 					neus;
		S_Neu_Conf 				conf;
	}; 

	static S_Neu s_neu_init(S_Neu self, enum s_net_type type, S_Layer, lu_size x, lu_size y, lu_size z);
	static void s_neu_b_layer_connect(S_Neu self, S_Layer);
	static void s_a_neu_b_layer_connect(S_Neu self, S_Layer);
	static inline void s_neu_t_connect(S_Neu v_neu, S_Neu p_neu) 
	{
		v_neu->t = p_neu;
		p_neu->t = v_neu;
	}

///////////////////////////////////////////////////////////////////////////////
// S Dopomizhni
//

	struct s_neu_conf {
		lu_value 				orig_min;
		lu_value 				orig_max;
		lu_value 				max;

		lu_value 				step;
		lu_value*				steps;  		// preobchysleni kroky
		lu_size 				neu_size;
		lu_size 				nsc; 			// (tilky dlia poshuku) nei sim count
	};

	static S_Neu_Conf s_neu_conf_init(S_Neu_Conf, Mem mem, lu_value min, lu_value max, lu_size neu_size, lu_size nsc);
	static inline lu_value s_neu_conf_norm(S_Neu_Conf self, lu_value request);
	static inline lu_size s_neu_conf_ix(S_Neu_Conf self, lu_value val);
	static inline struct lu_size_range s_neu_conf_ix_range(S_Neu_Conf self, lu_value val);
	static inline lu_value s_neu_conf_calc_sig(S_Neu_Conf self, lu_size val_step_i, lu_value val);
	static inline lu_value s_neu_conf_step_norm_dist(S_Neu_Conf self);

	struct s_layer {
		// vlasnyk
		S_Rec 					rec;

		enum s_layer_type 		type;

		lu_size 				l;
		lu_size 				w;
		lu_size 				h;
		lu_size 				d;				// komponenty v pershomu layer

		S_Neu* 					neus;
	};

	static S_Layer s_layer_init(S_Layer self, S_Rec rec, Mem mem, enum s_layer_type type, lu_size l, lu_size w, lu_size h, lu_size d);
	static inline void s_layer_neu_set(S_Layer self, lu_size x, lu_size y, S_Neu val) { self->neus[y * self->w + x] = val; }
	static inline S_Neu s_layer_neu_get(S_Layer self, lu_size x, lu_size y) { return self->neus[y * self->w + x]; }
	static void s_layer_cells_create(S_Layer self, Mem mem, S_Neu_Conf conf);
	
	// velyki masyvy v s_rec shob vykorystaty perevahu cpu keshuvania
	struct s_rec {
		// vlasnyk
		S_Mem 					mem; 			

		// poviazanyy z rec
		Lu_Rec 					rec;			

		lu_size 				id;

		lu_size 				cells_w;
		lu_size 				cells_h; 
		lu_size 				cells_d;		

		lu_size 				neus_size;
		lu_size 				neus_count;
		struct s_neu* 			neus;

		lu_size 				v_links_size;
		lu_size 				v_links_count;
		struct s_v_link* 		v_links;

		lu_size 				vp_layers_size;
		struct s_neu_conf 		v_conf;	
		struct s_layer* 		v_layers;
		struct s_neu_conf 		p_conf;
		struct s_layer* 		p_layers;

		lu_size 				blocks_size;
		lu_size 				a_layers_size;
		struct s_layer* 		av_layers;
		struct s_layer* 		ap_layers;
	};

	static S_Rec s_rec_create(S_Mem mem, Lu_Rec s_rec);
	static S_Neu s_rec_neu_init(S_Rec, Mem mem, enum s_net_type type, S_Layer, lu_size x, lu_size y, lu_size z);
	static S_V_Link s_rec_v_link_init(S_Rec);
	static S_Neu s_rec_cell_init(S_Rec self, Mem mem, S_Neu neu);
	static S_Neu s_rec_full_neu_init(S_Rec self, S_Neu neu);
	static S_Neu s_rec_a_neu_init(S_Rec self, S_Neu neu);

	static inline S_Neu s_rec_v_cell_get(S_Rec self, lu_size l, lu_size x, lu_size y)
	{
		S_Layer layer = &self->v_layers[l];
		return layer->neus[x + y * layer->w];
	}

	struct s_mem {
		Lu_Brain 				brain;

		Arr 					recs;
	};
	
	static S_Mem s_mem_create(Lu_Brain brain);
	static inline S_Rec s_mem_s_rec_get(S_Mem self, lu_size indx) { return arr_get(self->recs, indx); }

#endif // _LU_S_H
