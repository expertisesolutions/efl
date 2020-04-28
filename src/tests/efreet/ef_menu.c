#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <evil_unistd.h>

#include <Efreet.h>

#include "ef_test.h"

#if 0
static void
ef_menu_desktop_exec(Efreet_Menu *menu)
{
   Eina_List *l;

   if (menu->entries)
     {
        Efreet_Desktop *desktop;

        EINA_LIST_FOREACH(menu->entries, l, desktop)
          efreet_desktop_exec(desktop, NULL);
     }
   if (menu->sub_menus)
     {
        Efreet_Menu *sub_menu;

        EINA_LIST_FOREACH(menu->sub_menus, l, sub_menu)
          ef_menu_desktop_exec(sub_menu);
     }
}
#endif

int
ef_cb_menu_get(void)
{
   Efreet_Menu *menu;

   menu = efreet_menu_get();
   //    menu = efreet_menu_parse(ef_test_path_get("test.menu"));
   if (!menu)
     {
        printf("efreet_menu_get() returned NULL\n");
        return 0;
     }
   printf("\n");
   efreet_menu_dump(menu, "");
   efreet_menu_free(menu);

   return 1;
}

int
ef_cb_menu_with_slashes(void)
{
   Efreet_Menu *menu;

   menu = efreet_menu_parse(ef_test_path_get("test_menu_slash_bad.menu"));
   if (menu)
     {
        printf("efreet_menu_get() didn't return NULL\n");
        efreet_menu_free(menu);
        return 0;
     }

   return 1;
}

int
ef_cb_menu_save(void)
{
   Efreet_Menu *menu;
   int ret;

   //    menu = efreet_menu_get();
   menu = efreet_menu_parse(ef_test_path_get("test.menu"));
   if (!menu)
     {
        printf("efreet_menu_get() returned NULL\n");
        return 0;
     }
   unlink("/tmp/test.menu");
   ret = efreet_menu_save(menu, "/tmp/test.menu");
   efreet_menu_free(menu);
   return ret;
}

int
ef_cb_menu_edit(void)
{
   Efreet_Menu *menu, *entry;
   Efreet_Desktop *desktop;

   //    menu = efreet_menu_get();
   menu = efreet_menu_parse(ef_test_path_get("test.menu"));
   if (!menu)
     {
        printf("efreet_menu_get() returned NULL\n");
        return 0;
     }
#if 0
   printf("\n");
   efreet_menu_dump(menu, "");
   printf("\n");
#endif

   desktop = efreet_desktop_get(ef_test_path_get("test.desktop"));
   if (!desktop)
     {
        efreet_menu_free(menu);
        printf("No desktop found.\n");
        return 0;
     }

   efreet_menu_desktop_insert(menu, desktop, 0);
#if 0
   printf("\n");
   efreet_menu_dump(menu, "");
   printf("\n");
#endif
   entry = eina_list_data_get(menu->entries);
   if (desktop != entry->desktop)
     {
        efreet_menu_free(menu);
        return 0;
     }

   efreet_menu_desktop_insert(menu, desktop, 2);
#if 0
   printf("\n");
   efreet_menu_dump(menu, "");
   printf("\n");
#endif
   entry = eina_list_nth(menu->entries, 2);
   if (desktop != entry->desktop)
     {
        efreet_menu_free(menu);
        return 0;
     }

   efreet_menu_desktop_insert(menu, desktop, -1);
#if 0
   printf("\n");
   efreet_menu_dump(menu, "");
   printf("\n");
#endif
   entry = eina_list_data_get(eina_list_last(menu->entries));
   if (desktop != entry->desktop)
     {
        efreet_menu_free(menu);
        return 0;
     }

   efreet_menu_free(menu);
   return 1;
}
