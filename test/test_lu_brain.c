/**
	Copyright © 2020 Oleh Ihorovych Novosad 

	test_lu_brain tests public interfaces (from library user point of view) and includes 
	general integration tests. It uses nouns with Lu_ prefix only and methods with lu_ prefixes 
	only.
*/

#include "unity.h"
#include "brain/brain.h"

Lu_Brain 			brain;
Lu_Rec 				rec_0;
Lu_Rec 				rec_1;

Lu_Brain_Opts 		brain_opts;
Lu_Rec_Opts 		rec_opts_1;
Lu_Rec_Opts 		rec_opts_2;
Lu_Wave 			wave;

lu_value			data_0[] 		= { 
										0, 0, 0,
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_01[] 		= { 
										1, 0, 0, 
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_2[] 		= { 
										2, 0, 0,
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_3[] 		= { 
										3, 0, 0, 
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_4[] 		= { 
										4, 0, 0, 
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_5[] 		= { 
										5, 0, 0, 
										1, 1, 1,
										1, 1, 1
									};
lu_value			data_6[] 		= { 
										6, 0, 0, 
										1, 1, 1,
										1, 1, 1
									};


// setUp is executed for each test, even if test does nothing
void setUp(void)
{ 
	brain_opts 			= lu_brain_opts_create(1, 200 * 1024);
	rec_opts_1 			= lu_rec_opts_create(
							brain_opts, 
		/*w*/				3, 
		/*h*/				3, 
		/*depth*/			1,
		/*block_size*/		10, 
		/*v_min*/ 			0.0, 
		/*v_max*/			10.0, 
		/*v_neu_size*/		10
	);
			
	rec_opts_2 			= lu_rec_opts_create(
							brain_opts, 
		/*w*/				3, 
		/*h*/				3, 
		/*depth*/			1,
		/*block_size*/		10, 
		/*v_min*/ 			0.0, 
		/*v_max*/			10.0, 
		/*v_neu_size*/		10
	);
	
	brain 				= lu_brain_create(brain_opts);

	TEST_ASSERT(brain);
	TEST_ASSERT(brain->recs);
	TEST_ASSERT(brain->recs->count);

	rec_0 				= lu_brain_rec_get(brain, 0);

	TEST_ASSERT(rec_0);

	rec_1 				= lu_brain_rec_get(brain, 1);

	TEST_ASSERT(rec_1);

	TEST_ASSERT(brain->recs->count);

	wave = lu_wave_create(brain);
}

void tearDown(void)
{	
	lu_wave_destroy(wave);

	lu_brain_opts_destroy(brain_opts);
	lu_brain_destroy(brain);
}

void test_lu_brain_basics(void) 
{ 
	lu_debug("\n n_cell: %lu", sizeof(struct n_cell));
	lu_debug("\n n_lin: %lu", sizeof(struct n_lin));
	lu_size ws = sizeof(struct lu_neu);
	lu_debug("\n w_sig: %lu, 1Kb=%'lu, 1Mb=%'lu, 1Gb=%'lu", ws, 1024 / ws, 1024 * 1024 / ws, 1024 * 1024 * 1024 / ws); 

	/////////////////////////////////////////////////////////
	// Save 

	TEST_ASSERT(brain);
	TEST_ASSERT(brain->recs);
	TEST_ASSERT(brain->recs->count);

	Lu_Wave wave_mem;

	Lu_Story story = lu_story_create(brain, 0); 

		lu_story_push(story, rec_0, data_0);

		lu_block_begin(story);
		lu_story_push(story, rec_0, data_01);
		lu_story_push(story, rec_1, data_2);
		lu_block_end(story);

		lu_story_push(story, rec_0, data_3);
 4z
		// Because we called save (or find or restore) - it automatically 
		// reset number of available blocks inside story. If available lu_block count 
		// was 13 before save (we filled 3 blocks above), after save avalable 
		// lu_block count will be again 16 (this number is configurable)

		lu_block_begin(story);
		lu_story_push(story, rec_0, data_4);
		lu_story_push(story, rec_1, data_5);
		lu_block_end(story);

	lu_wave_save_async(wave, story);
	wave_mem = lu_wave_join(wave);

	// Destroy all temporary info associated with story. 
	// Does not destroy created related neurons.

	lu_story_destroy(story);  

	/////////////////////////////////////////////////////////
	// Find

	story = lu_story_create(brain, 0);

			lu_story_push(story, rec_0, data_0);

			lu_block_begin(story);
			lu_story_push(story, rec_0, data_01);
			lu_story_push(story, rec_1, data_2);
			lu_block_end(story);

			lu_story_push(story, rec_0, data_3);

	wave_mem = lu_wave_find(wave, story);

			lu_block_begin(story);
			lu_story_push(story, rec_0, data_4);
			lu_story_push(story, rec_1, data_5);
			lu_block_end(story);

	lu_wave_find_async(wave, story);
	wave_mem = lu_wave_join(wave);

	lu_story_destroy(story); 

	/////////////////////////////////////////////////////////
	// Lu_Name

	// Lu_Neuron save_neuron = lu_save_resp_neuron(save_response);

	// Lu_Name apple = lu_name_create(brain);

	// lu_name_give(apple, save_neuron);

	/////////////////////////////////////////////////////////
	// Restore

	// Lu_Restore_Resp lu_restore_resp = lu_story_restore(brain, save_neuron, restore_opts);

	/////////////////////////////////////////////////////
	// Restore stories by lu_name

}