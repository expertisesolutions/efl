#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "Efl.h"
#include "Ecore.h"
#include <Eo.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
#  define __declspec(dllexport)
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */


#include "efl_mono_model_internal.eo.h"
#include "efl_mono_model_internal_child.eo.h"

#include "assert.h"

typedef struct _Efl_Mono_Model_Internal_Data Efl_Mono_Model_Internal_Data;

typedef struct _Properties_Info Properties_Info;
struct _Properties_Info
{
  const char* name;
  const Eina_Value_Type* type;
};

typedef struct _Efl_Mono_Model_Internal_Data
{
  Eina_Array *properties_info;
  Eina_Array *properties_names;
  Eina_Array *items;
} _Efl_Mono_Model_Internal_Data;


#define MY_CLASS EFL_MONO_MODEL_INTERNAL_CLASS

typedef struct _Efl_Mono_Model_Internal_Child_Data
{
  Efl_Mono_Model_Internal_Data* model_pd;
  size_t index;
  Eina_Array *values;
  Eo* child;
  //Eina_Array *items;
} Efl_Mono_Model_Internal_Child_Data;

static int _find_property_index (const char* name, Eina_Array* properties_name)
{
   int i, size = eina_array_count_get(properties_name);
   for (i = 0; i != size; ++i)
   {
     if (!strcmp(properties_name->data[i], name))
       {
         return i;
       }
   }
   return -1;
}

static Eo *
_efl_mono_model_internal_efl_object_constructor(Eo *obj, Efl_Mono_Model_Internal_Data *pd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));

   pd->properties_info = eina_array_new(5);
   pd->properties_names = eina_array_new(5);
   pd->items = eina_array_new(5);

   if (!obj) return NULL;

   return obj;
}

static void
_efl_mono_model_internal_efl_object_destructor(Eo *obj, Efl_Mono_Model_Internal_Data *pd EINA_UNUSED)
{
   efl_destructor(efl_super(obj, MY_CLASS));
}

static void
_efl_mono_model_internal_add_property(Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Data *pd, const char *name, const Eina_Value_Type *type)
{
  Properties_Info* info = malloc(sizeof(Properties_Info));
  info->name = eina_stringshare_add(name);
  info->type = type;
  eina_array_push (pd->properties_info, info);
  eina_array_push (pd->properties_names, eina_stringshare_add(info->name));
}

static const char* _efl_mono_model_properties_names[] = { };

static Eina_Iterator *
_efl_mono_model_internal_efl_model_properties_get(const Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Data *pd EINA_UNUSED)
{
  return EINA_C_ARRAY_ITERATOR_NEW(_efl_mono_model_properties_names);
}

static Efl_Object*
_efl_mono_model_internal_efl_model_child_add(Eo *obj, Efl_Mono_Model_Internal_Data *pd)
{
  Efl_Mono_Model_Internal_Child* child = efl_add (EFL_MONO_MODEL_INTERNAL_CHILD_CLASS, obj);
  assert (child != NULL);
  Efl_Mono_Model_Internal_Child_Data* pcd = efl_data_xref (child, EFL_MONO_MODEL_INTERNAL_CHILD_CLASS, obj);
  pcd->model_pd = pd;
  pcd->index = eina_array_count_get(pd->items);
  pcd->child = child;
  pcd->values = eina_array_new(5);
  eina_array_push (pd->items, pcd);

  return child;
}

static unsigned int
_efl_mono_model_internal_efl_model_children_count_get(const Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Data *pd)
{
  return eina_array_count_get(pd->items);
}

static Eina_Future *
_efl_mono_model_internal_efl_model_children_slice_get(Eo *obj, Efl_Mono_Model_Internal_Data *pd, unsigned int start, unsigned int count EINA_UNUSED)
{
  unsigned int i;
  Eina_Value array = EINA_VALUE_EMPTY;
  Efl_Mono_Model_Internal_Child_Data* pcd;

  eina_value_array_setup(&array, EINA_VALUE_TYPE_OBJECT, count % 8);

  for (i = start; i != start + count; ++i)
  {
    pcd = eina_array_data_get(pd->items, i);
    eina_value_array_append (&array, pcd->child);
  }

  return efl_loop_future_resolved(obj, array);
}

static Eina_Future *
_efl_mono_model_internal_efl_model_property_set(Eo *obj, Efl_Mono_Model_Internal_Data *pd EINA_UNUSED, const char *property EINA_UNUSED, Eina_Value *value EINA_UNUSED)
{
  return efl_loop_future_rejected(obj, ENOSYS);
}

static Eina_Value *
_efl_mono_model_internal_efl_model_property_get(const Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Data *pd EINA_UNUSED, const char *property EINA_UNUSED)
{
  return eina_value_error_new(ENOSYS);
}

/// Model_Internal_Child implementations

static Eina_Future *
_efl_mono_model_internal_child_efl_model_property_set(Eo *obj, Efl_Mono_Model_Internal_Child_Data *pd, const char *property, Eina_Value *value)
{
  int i = _find_property_index (property, pd->model_pd->properties_names);
  int j;
  Eina_Value* old_value;
  Eina_Value* new_value;
  Eina_Value tmp_value;

  if (i >= 0)
  {
    for (j = i - eina_array_count_get (pd->values); j >= 0; --j)
    {
      eina_array_push (pd->values, (void*)1);
      pd->values->data[pd->values->count-1] = NULL;
    }

    old_value = eina_array_data_get (pd->values, i);
    if (old_value)
      eina_value_free (old_value);
    new_value = malloc (sizeof(Eina_Value));
    eina_value_copy (value, new_value);
    eina_value_copy (value, &tmp_value);
    eina_array_data_set (pd->values, i, new_value);


    return efl_loop_future_resolved(obj, tmp_value);
  }
  else
  {
    // not found property
    return efl_loop_future_rejected(obj, EAGAIN);
  }
}

static Eina_Value *
_efl_mono_model_internal_child_efl_model_property_get(const Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Child_Data *pd EINA_UNUSED, const char *property EINA_UNUSED)
{
  unsigned int i = _find_property_index (property, pd->model_pd->properties_names);
  if(eina_array_count_get (pd->values) <= i
     || eina_array_data_get (pd->values, i) == NULL)
    return eina_value_error_new(EAGAIN);
  else
    {
      Eina_Value* src = eina_array_data_get(pd->values, i);
      Eina_Value* clone = malloc (sizeof(Eina_Value));
      eina_value_copy (src, clone);
      return clone;
    }
}

static Eo *
_efl_mono_model_internal_child_efl_object_constructor(Eo *obj, Efl_Mono_Model_Internal_Child_Data *pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, EFL_MONO_MODEL_INTERNAL_CHILD_CLASS));

   return obj;
}

static void
_efl_mono_model_internal_child_efl_object_destructor(Eo *obj, Efl_Mono_Model_Internal_Child_Data *pd EINA_UNUSED)
{
   efl_destructor(efl_super(obj, EFL_MONO_MODEL_INTERNAL_CHILD_CLASS));
}

static Efl_Object*
_efl_mono_model_internal_child_efl_model_child_add(Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Child_Data *pd EINA_UNUSED)
{
  abort();
  return NULL;
}

static Eina_Iterator *
_efl_mono_model_internal_child_efl_model_properties_get(const Eo *obj EINA_UNUSED, Efl_Mono_Model_Internal_Child_Data *pd)
{
  return eina_array_iterator_new (pd->model_pd->properties_names);
}

#include "efl_mono_model_internal.eo.c"
#include "efl_mono_model_internal_child.eo.c"
