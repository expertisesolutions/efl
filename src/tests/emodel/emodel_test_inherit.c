
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <check.h>

#include <Emodel.h>

START_TEST(emodel_test_test_inherit)
{
}
END_TEST

void
emodel_test_inherit(TCase *tc)
{
   tcase_add_test(tc, emodel_test_test_inherit);
}
