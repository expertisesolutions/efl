#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eeze.h>
#include "eeze_udev_private.h"

EEZE_API Eina_Stringshare *
eeze_udev_syspath_get_parent_filtered(const char *syspath, const char *subsystem, const char *devtype)
{
   _udev_device *device, *parent;
   Eina_Stringshare *ret = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(syspath, NULL);

   if (!(device = _new_device(syspath)))
     return NULL;
   parent = udev_device_get_parent_with_subsystem_devtype(device, subsystem, devtype);
   if (parent)
     ret = eina_stringshare_add(udev_device_get_syspath(parent));
   udev_device_unref(device);
   return ret;
}

EEZE_API const char *
eeze_udev_syspath_get_parent(const char *syspath)
{
   _udev_device *device, *parent;
   const char *ret;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;
   parent = udev_device_get_parent(device);
   ret = eina_stringshare_add(udev_device_get_syspath(parent));
   udev_device_unref(device);
   return ret;
}

EEZE_API Eina_List *
eeze_udev_syspath_get_parents(const char *syspath)
{
   _udev_device *child, *parent, *device;
   const char *path;
   Eina_List *devlist = NULL;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;

   if (!(parent = udev_device_get_parent(device)))
     return NULL;

   for (; parent; child = parent, parent = udev_device_get_parent(child))
     {
        path = udev_device_get_syspath(parent);
        devlist = eina_list_append(devlist, eina_stringshare_add(path));
     }

   udev_device_unref(device);
   return devlist;
}

EEZE_API const char *
eeze_udev_syspath_get_devpath(const char *syspath)
{
   _udev_device *device;
   const char *name = NULL;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;

   if (!(name = udev_device_get_devnode(device)))
     return NULL;

   name = eina_stringshare_add(name);
   udev_device_unref(device);
   return name;
}

EEZE_API const char *
eeze_udev_syspath_get_devname(const char *syspath)
{
   _udev_device *device;
   const char *name = NULL;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;

   if (!(name = udev_device_get_sysname(device)))
     return NULL;

   name = eina_stringshare_add(name);
   udev_device_unref(device);
   return name;
}

EEZE_API const char *
eeze_udev_syspath_get_subsystem(const char *syspath)
{
   _udev_device *device;
   const char *subsystem;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;
   subsystem = eina_stringshare_add(udev_device_get_property_value(device, "SUBSYSTEM"));
   udev_device_unref(device);
   return subsystem;
}

EEZE_API Eina_Bool
eeze_udev_syspath_check_property(const char *syspath, const char *property, const char *value)
{
   _udev_device *device;
   const char *test;
   Eina_Bool ret = EINA_FALSE;

   if (!syspath || !property || !value)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;
   if ((test = udev_device_get_property_value(device, property)))
     ret = !strcmp(test, value);

   udev_device_unref(device);
   return ret;
}

EEZE_API const char *
eeze_udev_syspath_get_property(const char *syspath,
                               const char *property)
{
   _udev_device *device;
   const char *test;
   Eina_Stringshare *value = NULL;

   if (!syspath || !property)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;
   if ((test = udev_device_get_property_value(device, property)))
     value = eina_stringshare_add(test);

   udev_device_unref(device);
   return value;
}

EEZE_API Eina_Bool
eeze_udev_syspath_check_sysattr(const char *syspath, const char *sysattr, const char *value)
{
   _udev_device *device;
   const char *test;
   Eina_Bool ret = EINA_FALSE;

   if (!syspath || !sysattr || !value)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

   if ((test = udev_device_get_sysattr_value(device, sysattr)))
     ret = !strcmp(test, value);

   udev_device_unref(device);
   return ret;
}

EEZE_API const char *
eeze_udev_syspath_get_sysattr(const char *syspath,
                              const char *sysattr)
{
   _udev_device *device;
   const char *value = NULL, *test;

   if (!syspath || !sysattr)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;

   if ((test = udev_device_get_sysattr_value(device, sysattr)))
     value = eina_stringshare_add(test);

   udev_device_unref(device);
   return value;
}

EEZE_API Eina_Bool
eeze_udev_syspath_set_sysattr(const char *syspath,
                              const char *sysattr,
                              double value)
{
   _udev_device *device;
   Eina_Bool ret = EINA_FALSE;

   if (!syspath || !sysattr)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

#ifndef OLD_LIBUDEV
   char val[16];
   int test;

   sprintf(val, "%f", value);
   test = udev_device_set_sysattr_value(device, sysattr, val);
   if (test == 0)
     ret = EINA_TRUE;
#else
   (void)value;
#endif

  udev_device_unref(device);
  return ret;
}

EEZE_API Eina_List *
eeze_udev_syspath_get_sysattr_list(const char *syspath)
{
   _udev_device *device;
   _udev_list_entry *devs, *cur;
   Eina_List *syslist = NULL;

   if (!syspath)
     return NULL;

   if (!(device = _new_device(syspath)))
     return NULL;

   devs = udev_device_get_sysattr_list_entry(device);
   udev_list_entry_foreach(cur, devs)
     {
        syslist = eina_list_append(syslist,
                   eina_stringshare_add(udev_list_entry_get_name(cur)));
     }

   udev_device_unref(device);
   return syslist;
}

EEZE_API Eina_Bool
eeze_udev_syspath_is_mouse(const char *syspath)
{
   _udev_device *device = NULL;
   Eina_Bool mouse = EINA_FALSE;
   const char *test = NULL;

   if (!syspath)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

   test = udev_device_get_property_value(device, "ID_INPUT_MOUSE");

   if (test && (test[0] == '1'))
     mouse = EINA_TRUE;

   udev_device_unref(device);
   return mouse;
}

EEZE_API Eina_Bool
eeze_udev_syspath_is_kbd(const char *syspath)
{
   _udev_device *device = NULL;
   Eina_Bool kbd = EINA_FALSE;
   const char *test = NULL;

   if (!syspath)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

   test = udev_device_get_property_value(device, "ID_INPUT_KEYBOARD");

   if (test && (test[0] == '1'))
     kbd = EINA_TRUE;

   udev_device_unref(device);
   return kbd;
}

EEZE_API Eina_Bool
eeze_udev_syspath_is_touchpad(const char *syspath)
{
   _udev_device *device = NULL;
   Eina_Bool touchpad = EINA_FALSE;
   const char *test;

   if (!syspath)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

   test = udev_device_get_property_value(device, "ID_INPUT_TOUCHPAD");

   if (test && (test[0] == '1'))
     touchpad = EINA_TRUE;

   udev_device_unref(device);
   return touchpad;
}

EEZE_API Eina_Bool
eeze_udev_syspath_is_joystick(const char *syspath)
{
   _udev_device *device = NULL;
   Eina_Bool joystick = EINA_FALSE;
   const char *test;

   if (!syspath)
     return EINA_FALSE;

   if (!(device = _new_device(syspath)))
     return EINA_FALSE;

   test = udev_device_get_property_value(device, "ID_INPUT_JOYSTICK");

   if (test && (test[0] == '1'))
     joystick = EINA_TRUE;

   udev_device_unref(device);
   return joystick;
}

EEZE_API const char *
eeze_udev_devpath_get_syspath(const char *devpath)
{
   _udev_enumerate *en;
   _udev_list_entry *devs, *cur;
   const char *ret = NULL;

   if (!devpath)
     return NULL;

   en = udev_enumerate_new(udev);

   if (!en)
     return NULL;

   udev_enumerate_add_match_property(en, "DEVNAME", devpath);
   udev_enumerate_scan_devices(en);
   devs = udev_enumerate_get_list_entry(en);
   udev_list_entry_foreach(cur, devs)
     {
        ret = eina_stringshare_add(udev_list_entry_get_name(cur));
        break; /*just in case there's more than one somehow */
     }
   udev_enumerate_unref(en);
   return ret;
}

EEZE_API int 
eeze_udev_syspath_get_sysnum(const char *syspath)
{
   _udev_device *device;
   const char *test;
   int ret = -1;

   if (!syspath)
     return -1;

   if (!(device = _new_device(syspath)))
     return -1;

   if ((test = udev_device_get_sysnum(device)))
     ret = atoi(test);

   udev_device_unref(device);
   return ret;
}

