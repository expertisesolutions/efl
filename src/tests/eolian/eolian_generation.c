#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <Eina.h>
#include <Eolian.h>

#include "eolian_suite.h"

static Eina_Bool
_files_compare (const char *ref_filename, const char *tmp_filename)
{
   Eina_Bool result = EINA_FALSE;

   FILE *tmp_file = NULL, *ref_file = NULL;
   char *tmp_content = NULL, *ref_content = NULL;

   tmp_file = fopen(tmp_filename, "rb");
   if (!tmp_file)
     {
        printf("Unable to open %s\n", tmp_filename);
        goto end;
     }
   ref_file = fopen(ref_filename, "rb");
   if (!ref_file)
     {
        printf("Unable to open %s\n", ref_filename);
        goto end;
     }

   fseek(tmp_file, 0, SEEK_END);
   long tmp_filesize = ftell(tmp_file);
   if (tmp_filesize < 0) goto end;
   fseek(tmp_file, 0, SEEK_SET);
   tmp_content = malloc(tmp_filesize + 1);
   if (fread(tmp_content, tmp_filesize, 1, tmp_file) != 1)
     goto end;
   tmp_content[tmp_filesize] = '\0';
   while (tmp_content[tmp_filesize - 1] == '\n')
     tmp_content[--tmp_filesize] = '\0';

   fseek(ref_file, 0, SEEK_END);
   long ref_filesize = ftell(ref_file);
   if (ref_filesize < 0) goto end;
   fseek(ref_file, 0, SEEK_SET);
   ref_content = malloc(ref_filesize + 1);
   if (fread(ref_content, ref_filesize, 1, ref_file) != 1)
     goto end;
   ref_content[ref_filesize] = '\0';
   while (ref_content[ref_filesize - 1] == '\n')
     ref_content[--ref_filesize] = '\0';

   if (tmp_filesize != ref_filesize) goto end;

   result = !memcmp(ref_content, tmp_content, ref_filesize);
end:
   if (tmp_file) fclose(tmp_file);
   if (ref_file) fclose(ref_file);
   if (tmp_content) free(tmp_content);
   if (ref_content) free(ref_content);

   return result;
}

static int
_eolian_gen_execute(const char *eo_filename, const char *type, const char *output_filename)
{
   char command[PATH_MAX];
   if (snprintf(command, PATH_MAX,
                EOLIAN_GEN" -S -I \""TESTS_SRC_DIR"/data\" -o %s:%s %s",
                type, output_filename, eo_filename) > PATH_MAX)
     {
        printf("eolian gen command too long for buffer\n");
        abort();
     }
   fprintf(stderr, "%s\n", command);
   return system(command);
}

EFL_START_TEST(eolian_dev_impl_code)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_object_impl.c", tmp_dir);

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/object_impl.eo", "i", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/object_impl_ref.c", output_filepath));
   /* Check that nothing is added */
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/object_impl.eo", "i", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/object_impl_ref.c", output_filepath));

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/object_impl_add.eo", "i", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/object_impl_add_ref.c", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_types_generation)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_typedef.eo.h", tmp_dir);

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/typedef.eo", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/typedef_ref.h", output_filepath));

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/struct.eo", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/struct_ref.h", output_filepath));

   snprintf(output_filepath, PATH_MAX, "%s/eolian_typedef.eo.stub.h", tmp_dir);

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/typedef.eo", "s", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/typedef_ref_stub.h", output_filepath));

   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/struct.eo", "s", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/struct_ref_stub.h", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_default_values_generation)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_class_simple.eo.c", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/class_simple.eo", "c", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/class_simple_ref.c", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_override_generation)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_override.eo.c", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/override.eo", "c", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/override_ref.c", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_functions_descriptions)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_class_simple.eo.h", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/class_simple.eo", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/class_simple_ref_eo.h", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_import)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_import_types.eot.h", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/import_types.eot", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/import_types_ref.h", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_docs)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_docs.eo.h", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/eo_docs.eo", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/docs_ref.h", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(eolian_function_pointers)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   // .eot
   snprintf(output_filepath, PATH_MAX, "%s/eolian_function_pointers.eot.h", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/function_types.eot", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/function_types_ref.h", output_filepath));

   // .eo.h
   snprintf(output_filepath, PATH_MAX, "%s/eolian_function_pointers.eo.h", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/function_as_argument.eo", "h", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/function_as_argument_ref.h", output_filepath));

   // .eo.c
   snprintf(output_filepath, PATH_MAX, "%s/eolian_function_pointers.eo.c", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/function_as_argument.eo", "c", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/function_as_argument_ref.c", output_filepath));

   // .eo.imp.c
   snprintf(output_filepath, PATH_MAX, "%s/eolian_function_pointers.c", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/function_as_argument.eo", "i", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/function_as_argument_impl_ref.c", output_filepath));
   /* Check that nothing is added */
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/function_as_argument.eo", "i", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/function_as_argument_impl_ref.c", output_filepath));
}
EFL_END_TEST

EFL_START_TEST(owning)
{
   const char *tmp_dir = eina_environment_tmp_get();
   char output_filepath[PATH_MAX + 128] = "";

   snprintf(output_filepath, PATH_MAX, "%s/eolian_owning.eo.c", tmp_dir);
   remove(output_filepath);
   fail_if(0 != _eolian_gen_execute(TESTS_SRC_DIR"/data/owning.eo", "c", output_filepath));
   fail_if(!_files_compare(TESTS_SRC_DIR"/data/owning_ref.c", output_filepath));

}
EFL_END_TEST

void eolian_generation_test(TCase *tc)
{
   tcase_add_test(tc, eolian_types_generation);
   // tcase_add_test(tc, eolian_default_values_generation);
   // tcase_add_test(tc, eolian_override_generation);
   // tcase_add_test(tc, eolian_dev_impl_code);
   // tcase_add_test(tc, eolian_functions_descriptions);
   // tcase_add_test(tc, eolian_import);
   // tcase_add_test(tc, eolian_docs);
   // tcase_add_test(tc, eolian_function_pointers);
   // tcase_add_test(tc, owning);
}
