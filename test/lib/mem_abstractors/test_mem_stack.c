#include "unity.h"
#include "lib/_module.h"
  
void setUp(void)
{

}

void tearDown(void)
{

}

void test_mem_stack_1(void)
{
	Lu_Mem mem = (Lu_Mem) lu_mem_stack_create(lu_g_mem, 512);

    TEST_ASSERT(mem);

    lu_p_byte p;
	
	lu_user_assert_off();

	for (lu_size i = 0; i < 10; i++)
	{
		p = lu_mem_alloc(mem, 512);

		TEST_ASSERT(p == NULL);
	}

	lu_user_assert_on();

	lu_mem_destroy(mem, lu_g_mem);
}
