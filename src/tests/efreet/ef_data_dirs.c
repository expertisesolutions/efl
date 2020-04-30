#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <eina_stdio.h>
#include <string.h>

#ifdef _WIN32
# include <evil_private.h> /* setenv unsetenv */
#endif

#include <Ecore_File.h>
#include <Efreet.h>

int
ef_cb_efreet_data_home(void)
{
   const char *tmp;
   int ret = 1;

   efreet_shutdown();
   setenv("XDG_DATA_HOME", "/var/tmp", 1);
   efreet_init();

   tmp = efreet_data_home_get();
   if (strcmp(tmp, "/var/tmp"))
     {
        printf("efreet_data_home_get() returned incorrect "
               "value (%s) on XDG_DATA_HOME=/var/tmp\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_DATA_HOME");
   setenv("HOME", "/home/tmp", 1);
   efreet_init();

   tmp = efreet_data_home_get();
   if (strcmp(tmp, "/home/tmp/.local/share"))
     {
        printf("efreet_data_home_get() returned incorrect "
               "value (%s) on blank XDG_DATA_HOME\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_DATA_HOME");
   unsetenv("HOME");
#ifdef _WIN32
   unsetenv("USERPROFILE");
#endif
   efreet_init();

   tmp = efreet_data_home_get();
   if (strcmp(tmp, "/tmp/.local/share"))
     {
        printf("efreet_data_home_get() returned incorrect "
               "value (%s) on blank XDG_DATA_HOME and blank HOME\n", tmp);
        ret = 0;
     }

   return ret;
}

int
ef_cb_efreet_config_home(void)
{
   const char *tmp;
   int ret = 1;

   efreet_shutdown();
   setenv("XDG_CONFIG_HOME", "/var/tmp", 1);
   efreet_init();

   tmp = efreet_config_home_get();
   if (strcmp(tmp, "/var/tmp"))
     {
        printf("efreet_config_home_get() returned incorrect "
               "value (%s) on XDG_CONFIG_HOME=/var/tmp\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_CONFIG_HOME");
   setenv("HOME", "/home/tmp", 1);
   efreet_init();

   tmp = efreet_config_home_get();
   if (strcmp(tmp, "/home/tmp/.config"))
     {
        printf("efreet_config_home_get() returned incorrect "
               "value (%s) on blank XDG_CONFIG_HOME\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_CONFIG_HOME");
   unsetenv("HOME");
#ifdef _WIN32
   unsetenv("USERPROFILE");
#endif
   efreet_init();

   tmp = efreet_config_home_get();
   if (strcmp(tmp, "/tmp/.config"))
     {
        printf("efreet_config_home_get() returned incorrect "
               "value (%s) on blank XDG_CONFIG_HOME and blank HOME\n", tmp);
        ret = 0;
     }

   return ret;
}

int
ef_cb_efreet_cache_home(void)
{
   const char *tmp;
   int ret = 1;

   efreet_shutdown();
   setenv("XDG_CACHE_HOME", "/var/tmp", 1);
   efreet_init();

   tmp = efreet_cache_home_get();
   if (strcmp(tmp, "/var/tmp"))
     {
        printf("efreet_cache_home_get() returned incorrect "
               "value (%s) on XDG_CACHE_HOME=/var/tmp\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_CACHE_HOME");
   setenv("HOME", "/home/tmp", 1);
   efreet_init();

   tmp = efreet_cache_home_get();
   if (strcmp(tmp, "/home/tmp/.cache"))
     {
        printf("efreet_cache_home_get() returned incorrect "
               "value (%s) on blank XDG_CACHE_HOME\n", tmp);
        ret = 0;
     }

   /* reset efreet here so we can set a new home dir */
   efreet_shutdown();
   unsetenv("XDG_CACHE_HOME");
   unsetenv("HOME");
#ifdef _WIN32
   unsetenv("USERPROFILE");
#endif
   efreet_init();

   tmp = efreet_cache_home_get();
   if (strcmp(tmp, "/tmp/.cache"))
     {
        printf("efreet_cache_home_get() returned incorrect "
               "value (%s) on blank XDG_CACHE_HOME and blank HOME\n", tmp);
        ret = 0;
     }

   return ret;
}

int
ef_cb_efreet_data_dirs(void)
{
   Eina_List *tmp, *l;
   int ret = 1;
   unsigned int i;
   unsigned int ok;
   char dirs[128], *val;
   char *vals[] = {"/var/tmp/a", "/tmp/b", "/usr/local/share", "/etc", NULL};
   char *def_vals[] = {DATA_DIR, "/usr/share", "/usr/local/share", NULL};

   dirs[0] = '\0';
   for (i = 0; vals[i]; i++)
     {
        if (i > 0) strcat(dirs, ":");
        strcat(dirs, vals[i]);
     }

   efreet_shutdown();
   setenv("XDG_DATA_DIRS", dirs, 1);
   efreet_init();

   ok = 0;
   tmp = efreet_data_dirs_get();
   for (i = 0; vals[i]; i++)
     {
        char *found;

        found = eina_list_search_unsorted(tmp, EINA_COMPARE_CB(strcmp), vals[i]);
        if (!ecore_file_exists(vals[i]) && found)
          {
             printf("efreet_data_dirs_get() includes non-existing dir (%s) when "
                    "%s set\n", vals[i], dirs);
             ret = 0;
             continue;
          }
        if (ecore_file_exists(vals[i]) && !found)
          {
             printf("efreet_data_dirs_get() is missing dir (%s) when "
                    "%s set\n", vals[i], dirs);
             ret = 0;
             continue;
          }
        if (ecore_file_exists(vals[i]) && found)
          ok++;
     }
   if (eina_list_count(tmp) != ok)
     {
        printf("efreet_data_dirs_get() returned more values then it "
               "should have given %s as input\n", dirs);
        ret = 0;
     }

   efreet_shutdown();
   unsetenv("XDG_DATA_DIRS");
   efreet_init();

   i = 0;
   tmp = efreet_data_dirs_get();
   if (eina_list_count(tmp) != 3)
     {
        printf("efreet_data_dirs_get() nodes is differnet from expected default\n");
        ret = 0;
     }

   EINA_LIST_FOREACH(tmp, l, val)
     {
        if (!def_vals[i])
          {
             printf("efreet_data_dirs_get() returned more values then it "
                    "should have given %s as input\n", dirs);
             ret = 0;
             break;
          }

        if (strcmp(val, def_vals[i]))
          {
             printf("efreet_data_dirs_get() returned incorrect value (%s) when "
                    "XDG_DATA_DIRS= is set %s\n", val, def_vals[i]);
             ret = 0;
          }

        i++;
     }
   return ret;
}

int
ef_cb_efreet_config_dirs(void)
{
   Eina_List *tmp, *l;
   int ret = 1;
   unsigned int i;
   unsigned int ok;
   char dirs[128], *val;
   char *vals[] = {"/var/tmp/a", "/tmp/b", "/usr/local/share", "/etc", NULL};
   char *def_vals[] = {"/etc/xdg", NULL};

   dirs[0] = '\0';

   for (i = 0; vals[i]; i++)
     {
        if (i > 0) strcat(dirs, ":");
        strcat(dirs, vals[i]);
     }

   efreet_shutdown();
   setenv("XDG_CONFIG_DIRS", dirs, 1);
   efreet_init();

   ok = 0;
   tmp = efreet_config_dirs_get();
   for (i = 0; vals[i]; i++)
     {
        char *found;

        found = eina_list_search_unsorted(tmp, EINA_COMPARE_CB(strcmp), vals[i]);
        if (!ecore_file_exists(vals[i]) && found)
          {
             printf("efreet_data_dirs_get() includes non-existing dir (%s) when "
                    "%s set\n", vals[i], dirs);
             ret = 0;
             continue;
          }
        if (ecore_file_exists(vals[i]) && !found)
          {
             printf("efreet_data_dirs_get() is missing dir (%s) when "
                    "%s set\n", vals[i], dirs);
             ret = 0;
             continue;
          }
        if (ecore_file_exists(vals[i]) && found)
          ok++;
     }
   if (eina_list_count(tmp) != ok)
     {
        printf("efreet_data_dirs_get() returned more values then it "
               "should have given %s as input\n", dirs);
        ret = 0;
     }

   efreet_shutdown();
   unsetenv("XDG_CONFIG_DIRS");
   efreet_init();

   i = 0;
   tmp = efreet_config_dirs_get();
   EINA_LIST_FOREACH(tmp, l, val)
     {
        if (!def_vals[i])
          {
             printf("efreet_config_dirs_get() returned more values then it "
                    "should have given %s as input\n", dirs);
             ret = 0;
             break;
          }

        if (strcmp(val, def_vals[i]))
          {
             printf("efreet_config_dirs_get() returned incorrect value (%s) when "
                    "XDG_CONFIG_DIRS= is set\n", val);
             ret = 0;
          }

        i++;
     }
   return ret;
}
