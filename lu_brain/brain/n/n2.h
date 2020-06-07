/**
	Copyright © 2020 Oleh Ihorovych Novosad 
*/

#ifndef _LU_N_H
#define _LU_N_H

///////////////////////////////////////////////////////////////////////////////
// Neu

	struct n_neu
	{
		S_Base 			s_base;
		lu_size 		lid; 		

		// ostanni linky
		N_Lin 			c_l;
		N_Lin 			p_l;
	};

	struct n_lin
	{
		N_Neu 	p;
		N_Neu 	c;
	
		N_Lin   p_p;
		N_Lin 	p_n;

		N_Lin 	c_p;
		N_Lin   c_n;
	};


#endif // _LU_N_H