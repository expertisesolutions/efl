#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if defined HAVE_DLADDR && ! defined _WIN32
# include <dlfcn.h>
#endif

#include <Eina.h>

#ifdef _WIN32
# include <evil_private.h> /* evil_time_get dladdr */
#endif

#if defined(__APPLE__) && defined(__MACH__)
# include <mach/mach_time.h>
#endif

#include "Eo.h"
#include "eo_ptr_indirection.h"
#include "eo_private.h"
#include "eo_add_fallback.h"

#include "eo_internal.h"

#include "efl_object_override.eo.c"
#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif

#ifdef HAVE_VALGRIND
# include <valgrind.h>
# include <memcheck.h>
#endif
static Eina_Bool _eo_trash_bypass = EINA_FALSE;

#define EO_CLASS_IDS_FIRST 1
#define EFL_OBJECT_OP_IDS_FIRST 1

/* Used inside the class_get functions of classes, see #EFL_DEFINE_CLASS */
EO_API Eina_Lock _efl_class_creation_lock;
EO_API unsigned int _efl_object_init_generation = 1;
int _eo_log_dom = -1;
Eina_Thread _efl_object_main_thread;
static unsigned int efl_del_api_generation = 0;
static Efl_Object_Op _efl_del_api_op_id = 0;
static Eina_Hash *class_overrides;

typedef enum _Eo_Ref_Op {
   EO_REF_OP_NONE,
   EO_REF_OP_NEW,
   EO_REF_OP_FREE,
   EO_REF_OP_REF,
   EO_REF_OP_UNREF,
   EO_REF_OP_REUSE,
} Eo_Ref_Op;

static inline void _eo_log_obj_init(void);
static inline void _eo_log_obj_shutdown(void);
static inline void _eo_log_obj_ref_op(const _Eo_Object *obj, Eo_Ref_Op ref_op);
#ifdef EO_DEBUG
#define EO_LOG_OBJS_BACKTRACE_MAX     1022
#define EO_LOG_OBJS_BACKTRACE_DEFAULT 62
static int _eo_log_objs_dom = -1;
static int _eo_log_objs_backtrace = EO_LOG_OBJS_BACKTRACE_DEFAULT; // 0 = enable tracking but no bt, N>0 = enable tracking and bt of size N
static Eo_Ref_Op _eo_log_objs_level = EO_REF_OP_NONE;
static Eina_Inarray _eo_log_objs_debug;
static Eina_Inarray _eo_log_objs_no_debug;
static double _eo_log_time_start;
static const char *_eo_ref_op_str[] = {
   "ERROR",
   "New  ",
   "Free ",
   "Ref  ",
   "Unref",
   "Reuse",
};
#ifdef HAVE_BACKTRACE
static Eina_Array _eo_log_objs;
static Eina_Spinlock _eo_log_objs_lock;
#endif
#else
static inline void _eo_log_obj_init(void) { }
static inline void _eo_log_obj_shutdown(void) { }
static inline void _eo_log_obj_ref_op(const _Eo_Object *obj EINA_UNUSED, Eo_Ref_Op ref_op EINA_UNUSED) { }
void _eo_log_obj_report(const Eo_Id id EINA_UNUSED, int log_level EINA_UNUSED, const char *func_name EINA_UNUSED, const char *file EINA_UNUSED, int line EINA_UNUSED) { }
#endif

static _Efl_Class **_eo_classes = NULL;
static Eo_Id _eo_classes_last_id = 0;
static Eo_Id _eo_classes_alloc = 0;
static int _efl_object_init_count = 0;
static Eina_Hash *_ops_storage = NULL;
static Eina_Spinlock _ops_storage_lock;

static const Efl_Object_Optional efl_object_optional_cow_default = {};
Eina_Cow *efl_object_optional_cow = NULL;

static size_t _eo_sz = 0;
static size_t _eo_class_sz = 0;

static void _eo_condtor_reset(_Eo_Object *obj);
static inline void *_efl_data_scope_get(const _Eo_Object *obj, const _Efl_Class *klass);
static inline void *_efl_data_xref_internal(const char *file, int line, _Eo_Object *obj, const _Efl_Class *klass, const _Eo_Object *ref_obj);
static inline void _efl_data_xunref_internal(_Eo_Object *obj, void *data, const _Eo_Object *ref_obj);

static inline Efl_Object_Op _efl_object_api_op_id_get_internal(const void *api_func);

/* Start of Dich */


/* We are substracting the mask here instead of "AND"ing because it's a hot path,
 * it should be a valid class at this point, and this lets the compiler do 1
 * substraction at compile time. */
#define _UNMASK_ID(id) ((id) - MASK_CLASS_TAG)
#define ID_CLASS_GET(id) ({ \
      (_Efl_Class *) (((_UNMASK_ID(id) <= _eo_classes_last_id) && (_UNMASK_ID(id) > 0)) ? \
      (_eo_classes[_UNMASK_ID(id) - 1]) : NULL); \
      })

#define EFL_OBJECT_OP_CLASS_PART(op) op >> 16
#define EFL_OBJECT_OP_FUNC_PART(op) op & 0xffff
#define EFL_OBJECT_OP_CREATE_OP_ID(class_id, func_id) ((unsigned short)class_id)<<16|((unsigned short)func_id&0xffff)

static const _Efl_Class *
_eo_op_class_get(Efl_Object_Op op)
{
   short class_id = EFL_OBJECT_OP_CLASS_PART(op);
   return _eo_classes[class_id];
}
#if defined(DEBUG_VTABLE_ALLOCATION)
static int _allocated_memory = 0;

static inline void*
_vtable_alloc(unsigned long n, size_t elem)
{
   _allocated_memory += n*elem;
   return calloc(n, elem);
}
#else
static inline void*
_vtable_alloc(unsigned long n, size_t elem)
{
   return calloc(n, elem);
}
#endif


/**
 * This inits the vtable with a given size
 */
static void
_vtable_init_size(Eo_Vtable *vtable, unsigned int size)
{
   //we assume here that _eo_classes_last_id was called before
   vtable->size = size;
   vtable->chain = _vtable_alloc(vtable->size, sizeof(Eo_Vtable_Node));
}

/**
 * This inits the vtable wit hthe current size of allocated tables
 */
static void
_vtable_init(Eo_Vtable *vtable)
{
   //we assume here that _eo_classes_last_id was called before
   _vtable_init_size(vtable, _eo_classes_last_id);
}

/**
 * This removes all nodes from the klass that are copied from mro
 */
static void
_vtable_mro_free(const _Efl_Class *klass)
{
   const _Efl_Class **mro_itr = klass->mro;
   const Eo_Vtable *vtable = &klass->vtable;
   for (  ; *mro_itr ; mro_itr++)
     {
        const Eo_Vtable *mro_vtable = &(*mro_itr)->vtable;
        if ((*mro_itr) == klass)
          continue;
        for (unsigned int i = 0; i < mro_vtable->size; ++i)
          {
             if (i == klass->class_id)
               continue;
             if (vtable->chain[i].funcs && mro_vtable->chain[i].funcs == vtable->chain[i].funcs)
               {
                  vtable->chain[i].funcs = NULL;
               }
          }
     }
}

static void
_vtable_free(Eo_Vtable *vtable, const Eo_Vtable *root)
{
   if (root)
     {
        EINA_SAFETY_ON_FALSE_RETURN(vtable->size == root->size);
     }

   for (int i = 0; i < vtable->size; ++i)
     {
        if (root && root->chain[i].funcs == vtable->chain[i].funcs)
          vtable->chain[i].count = 0;

        if (vtable->chain[i].count)
          {
             free(vtable->chain[i].funcs);
          }
     }
   free(vtable->chain);
}

/**
 * This takes over all set chains of the src to dest.
 * This should only be called on Eo_Vtables, which are initialized with this value.
 * Previous setted values are going to be overwritten.
 */
static void
_vtable_take_over(Eo_Vtable *dest, const Eo_Vtable *src)
{
   for (int i = 0; i < src->size; ++i)
     {
        if (src->chain[i].funcs)
          {
             dest->chain[i] = src->chain[i];
          }
     }
}

/**
 * Fills the node of the passed class id with a empty none NULL pointer.
 * This is used to indicate that a specific node has a normal 0 size, but is set.
 */
static void
_vtable_insert_empty_funcs(Eo_Vtable *vtable, unsigned short class_id)
{
   vtable->chain[class_id].funcs = (void*)0x1010101;
   vtable->chain[class_id].count = 0;
}

/**
 * duplicate the source node, and write the duplicated values to the destination
 * No logical changes are applied to src.
 */
static void
_vtable_copy_node(Eo_Vtable_Node *dest, const Eo_Vtable_Node *src)
{
   dest->count = src->count;
   dest->funcs = _vtable_alloc(sizeof(op_type_funcs), src->count);
   memcpy(dest->funcs, src->funcs, sizeof(op_type_funcs) * src->count);
}

/**
 * Initialize a node with a empty funcs array of the passed length
 */
static void
_vtable_prepare_empty_node(Eo_Vtable *dest, unsigned int length, unsigned int class_id)
{
   dest->chain[class_id].count = length;
   dest->chain[class_id].funcs = _vtable_alloc(sizeof(op_type_funcs), dest->chain[class_id].count);
}

/**
 * Copy all setted APIs from src to dest.
 * Already set function slots are going to be replaced.
 */
static void
_vtable_merge_defined_api(Eo_Vtable *dest, const Eo_Vtable *src, Eina_Bool *hitmap)
{
   for (unsigned int i = 0; i < src->size; ++i)
     {
        //if there is a source node evalulate if we need to copy it
        if (src->chain[i].funcs)
          {
             if (!dest->chain[i].funcs)
               {
                  dest->chain[i] = src->chain[i];
                  EINA_SAFETY_ON_FALSE_RETURN(hitmap[i] == EINA_FALSE);
               }
             else
               {
                  if (!hitmap[i])
                    {
                       const Eo_Vtable_Node node = dest->chain[i];
                       if (!node.count)
                         _vtable_insert_empty_funcs(dest, i);
                       else
                         _vtable_copy_node(&dest->chain[i], &node); //we copy what we have, and overwrite in the later for loop
                       hitmap[i] = EINA_TRUE;
                    }
                  for (int j = 0; j < src->chain[i].count; ++j)
                    {
                       if (src->chain[i].funcs[j].func)
                         dest->chain[i].funcs[j] = src->chain[i].funcs[j];
                    }
              }
          }
     }
}

/**
 * Ensure that all set nodes from src are also set on dest.
 * No real values are copied, the newly taken or allocated slots will be empty.
 */
static void
_vtable_merge_empty(Eo_Vtable *dest, const Eo_Vtable *src, Eina_Bool *hitmap)
{
   for (unsigned int i = 0; i < src->size; ++i)
     {
        if (src->chain[i].funcs && !dest->chain[i].funcs)
          {
             if (!src->chain[i].count)
               {
                  dest->chain[i].funcs = src->chain[i].funcs;
                  dest->chain[i].count = src->chain[i].count;
               }
             else
               {
                  _vtable_prepare_empty_node(dest, src->chain[i].count, i);
                  hitmap[i] = EINA_TRUE;
               }
          }
     }
}

static inline const op_type_funcs *
_vtable_func_get(const Eo_Vtable *vtable, Efl_Object_Op op)
{
   unsigned short class_id = EFL_OBJECT_OP_CLASS_PART(op);
   unsigned short func_id = EFL_OBJECT_OP_FUNC_PART(op);

   if (EINA_UNLIKELY(vtable->size <= class_id))
     return NULL;
   if (EINA_UNLIKELY(vtable->chain[class_id].count <= func_id))
     return NULL;

   return &vtable->chain[class_id].funcs[func_id];
}

static inline Eina_Bool
_vtable_func_set(Eo_Vtable *vtable, const _Efl_Class *klass,
                 const _Efl_Class *hierarchy_klass, Efl_Object_Op op,
                 Eo_Op_Func_Type func, Eina_Bool allow_same_override)
{
   op_type_funcs *fsrc;
   unsigned short class_id = EFL_OBJECT_OP_CLASS_PART(op);
   unsigned short func_id = EFL_OBJECT_OP_FUNC_PART(op);
   Eo_Vtable_Node *hirachy_node = NULL;
   Eo_Vtable_Node *node = NULL;

   EINA_SAFETY_ON_FALSE_RETURN_VAL(vtable->size >= class_id, EINA_FALSE);

   if (klass->parent && klass->parent->vtable.size > class_id)
     hirachy_node = &klass->parent->vtable.chain[class_id];
   if (hierarchy_klass)
     hirachy_node = &hierarchy_klass->vtable.chain[class_id];
   node = &vtable->chain[class_id];

   EINA_SAFETY_ON_NULL_RETURN_VAL(node->funcs, EINA_FALSE);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(node->count >= func_id, EINA_FALSE);
   fsrc = &node->funcs[func_id];

   if (hierarchy_klass && !func)
     {
        if (!func)
          {
             op_type_funcs funcs = hirachy_node->funcs[func_id];
             klass = funcs.src;
             func = funcs.func;
          }
     }
   else
     {
        if (!allow_same_override && (fsrc->src == klass))
          {
             const _Efl_Class *op_kls = _eo_op_class_get(op);
             ERR("Class '%s': Overriding already set func %p for op %d (%s) with %p.",
                 klass->desc->name, fsrc->func, op, op_kls->desc->name, func);
             return EINA_FALSE;
          }
     }

   fsrc->src = klass;
   fsrc->func = func;

   return EINA_TRUE;
}

/* END OF DICH */

#define _EO_ID_GET(Id) ((Eo_Id) (Id))


static inline Eina_Bool
_eo_is_a_obj(const Eo *eo_id)
{
   Eo_Id oid = (Eo_Id) _EO_ID_GET(eo_id);
   return !!(oid & MASK_OBJ_TAG);
}

static inline Eina_Bool
_eo_is_a_class(const Eo *eo_id)
{
   Eo_Id oid = (Eo_Id) _EO_ID_GET(eo_id);
   return !!(oid & MASK_CLASS_TAG);
}

static inline _Efl_Class *
_eo_class_pointer_get(const Efl_Class *klass_id)
{
   return ID_CLASS_GET((Eo_Id)klass_id);
}

static const char *
_eo_op_desc_name_get(const Efl_Op_Description *desc)
{
   static const char *fct_name = "unknown";

   if (!desc)
     {
        return fct_name;
     }

#ifndef _WIN32
# ifdef HAVE_DLADDR
   static Dl_info info;
   if (dladdr(desc->api_func, &info) != 0)
      fct_name = info.dli_sname;
# endif
#else
   fct_name = desc->api_func; /* Same on windows */
#endif
   return fct_name;
}

static inline const op_type_funcs *
_eo_kls_itr_next(const _Efl_Class *orig_kls, const _Efl_Class *cur_klass,
                 Efl_Object_Op op, Eina_Bool super)
{
   const _Efl_Class **kls_itr = NULL;

   /* Find the kls itr. */
   kls_itr = orig_kls->mro;
   while (*kls_itr && (*kls_itr != cur_klass))
      kls_itr++;

   if (*kls_itr)
     {
        if (super) kls_itr++;
        while (*kls_itr)
          {
             const op_type_funcs *fsrc = _vtable_func_get(&(*kls_itr)->vtable, op);
             if (!fsrc || !fsrc->func)
               {
                  kls_itr++;
                  continue;
               }
             return fsrc;
          }
     }

   return NULL;
}

static inline void
_apply_auto_unref(_Eo_Object *obj, const Eo *eo_obj)
{
   if (EINA_UNLIKELY(obj && obj->auto_unref))
     {
        if (obj->finalized && !(--obj->auto_unref))
          efl_unref(eo_obj);
     }
}

/************************************ EO ************************************/

static EFL_FUNC_TLS _Efl_Class *_super_klass = NULL;

static Eo *
_efl_super_cast(const Eo *eo_id, const Efl_Class *cur_klass, Eina_Bool super)
{
   EO_CLASS_POINTER_GOTO(cur_klass, super_klass, err);

#ifdef EO_DEBUG
   if (EINA_UNLIKELY(!_eo_is_a_obj(eo_id) && !_eo_is_a_class(eo_id))) goto err_obj;
#endif

   if (EINA_UNLIKELY(!_eo_is_a_obj(eo_id)))
     goto do_klass;

#ifndef EO_DEBUG
   if (!super && EINA_UNLIKELY(!efl_isa(eo_id, cur_klass)))
#else
   if (EINA_UNLIKELY(!efl_isa(eo_id, cur_klass)))
#endif
     goto err_obj_hierarchy;

   EO_OBJ_POINTER_RETURN_VAL(eo_id, obj, NULL);
   obj->cur_klass = super_klass;
   obj->super = super;
   EO_OBJ_DONE(eo_id);

   return (Eo *) eo_id;

do_klass:
   // efl_super(Class) is extremely rarely used, so TLS write is fine
   EINA_SAFETY_ON_FALSE_RETURN_VAL(super, NULL);
   _super_klass = super_klass;
   return (Eo *) eo_id;

err:
   _EO_POINTER_ERR(cur_klass, "Class (%p) is an invalid ref.", cur_klass);
   return NULL;
#ifdef EO_DEBUG
err_obj:
   _EO_POINTER_ERR(eo_id, "Object (%p) is an invalid ref, class=%p (%s).", eo_id, cur_klass, efl_class_name_get(cur_klass));
   return NULL;
#endif
err_obj_hierarchy:
   _EO_POINTER_ERR(eo_id, "Object (%p) class=%p (%s) is not an instance of class=%p (%s).", eo_id, efl_class_get(eo_id), efl_class_name_get(eo_id), cur_klass, efl_class_name_get(cur_klass));
   return NULL;
}

EO_API Eo *
efl_super(const Eo *eo_id, const Efl_Class *cur_klass)
{
   return _efl_super_cast(eo_id, cur_klass, EINA_TRUE);
}

EO_API Eo *
efl_cast(const Eo *eo_id, const Efl_Class *cur_klass)
{
   return _efl_super_cast(eo_id, cur_klass, EINA_FALSE);
}

EO_API Eina_Bool
_efl_object_call_resolve(Eo *eo_id, const char *func_name, Efl_Object_Op_Call_Data *call, Efl_Object_Op op, const char *file, int line)
{
   const _Efl_Class *klass, *main_klass;
   const _Efl_Class *cur_klass = NULL;
   _Eo_Object *obj = NULL;
   const Eo_Vtable *vtable = NULL;
   const op_type_funcs *func;
   Eina_Bool super = EINA_TRUE;

   if (EINA_UNLIKELY(!eo_id)) goto on_null;


   EO_OBJ_POINTER_RETURN_VAL_PROXY(eo_id, _obj, EINA_FALSE);

   obj = _obj;
   klass = _obj->klass;
   vtable = EO_VTABLE2(obj);
   if (EINA_UNLIKELY(_obj->cur_klass != NULL))
     {
        // YES this is a goto with a label to return. this is a
        // micro-optimization to move infrequent code out of the
        // hot path of the function
        goto obj_super;
     }
obj_super_back:
   _efl_ref(_obj);

   main_klass =  klass;

   /* If we have a current class, we need to itr to the next. */
   if (cur_klass)
     {
        // YES this is a goto with a label to return. this is a
        // micro-optimization to move infrequent code out of the
        // hot path of the function
        goto ok_cur_klass;
     }
   else
     {
        func = _vtable_func_get(vtable, op);
        EINA_PREFETCH_NOCACHE(func);
        // this is not very likely to happen - but may if its an invalid
        // call or a composite object, but either way, it's not very likely
        // so make it a goto to save on instruction cache
        if (!func) goto end;
     }
ok_cur_klass_back:

   if (EINA_LIKELY(func->func && func->src))
     {
        call->eo_id = eo_id;
        call->obj = obj;
        call->func = func->func;
        call->data = _efl_data_scope_get(obj, func->src);

        return EINA_TRUE;
     }

   // very unlikely in general to use a goto to move code out of l1 cache
   // ie instruction cache
   if (func->src != NULL) goto err_func_src;

end:
   /* Try composite objects */
     {
        Eina_List *itr;
        Eo *emb_obj_id;
        EINA_LIST_FOREACH(obj->opt->composite_objects, itr, emb_obj_id)
          {
             EO_OBJ_POINTER_PROXY(emb_obj_id, emb_obj);
             if (EINA_UNLIKELY(!emb_obj)) continue;

             func = _vtable_func_get(&emb_obj->klass->vtable, op);
             if (func == NULL) goto composite_continue;

             if (EINA_LIKELY(func->func && func->src))
               {
                  call->eo_id = _eo_obj_id_get(emb_obj);
                  call->obj = _efl_ref(emb_obj);
                  call->func = func->func;
                  call->data = _efl_data_scope_get(emb_obj, func->src);
                  /* We reffed it above, but no longer need/use it. */
                  _efl_unref(obj);
                  EO_OBJ_DONE(emb_obj_id);
                  return EINA_TRUE;
               }
composite_continue:
             EO_OBJ_DONE(emb_obj_id);
          }
     }

   // all of this is error handling at the end so... rare-ish
   // If it's a do_super call.
   if (cur_klass)
     {
        ERR("in %s:%d: func '%s' (%d) could not be resolved on %s for class '%s' for super of '%s'.",
            file, line, func_name, op, efl_debug_name_get(eo_id), main_klass->desc->name,
            cur_klass->desc->name);
        goto err;
     }
   else
     {
        /* we should not be able to take this branch */
        ERR("in %s:%d: func '%s' (%d) could not be resolved on %s for class '%s'.",
            file, line, func_name, op, efl_debug_name_get(eo_id), main_klass->desc->name);
        goto err;
     }

err_func_src:
   ERR("in %s:%d: you called a pure virtual func '%s' (%d) of class '%s'.",
       file, line, func_name, op, klass->desc->name);
err:
   _apply_auto_unref(obj, eo_id);
   _efl_unref(obj);
   _eo_obj_pointer_done((Eo_Id)eo_id);

   return EINA_FALSE;

   // yes - special "move out of hot path" code blobs with goto's for
   // speed reasons to have intr prefetches work better and miss less
ok_cur_klass:
   func = _eo_kls_itr_next(klass, cur_klass, op, super);
   if (!func) goto end;
   klass = func->src;
   goto ok_cur_klass_back;

obj_super:
   {
      cur_klass = obj->cur_klass;
      super = obj->super;
      obj->cur_klass = NULL;

      if (_obj_is_override(obj) && cur_klass && super &&
          (_eo_class_id_get(cur_klass) == EFL_OBJECT_OVERRIDE_CLASS))
        {
           /* Doing a efl_super(obj, EFL_OBJECT_OVERRIDE_CLASS) should
            * result in calling as if it's a normal class. */
           vtable = &klass->vtable;
           cur_klass = NULL;
        }

   }
   goto obj_super_back;

on_null:
   if (EINA_UNLIKELY(efl_del_api_generation != _efl_object_init_generation))
     {
        _efl_del_api_op_id = _efl_object_api_op_id_get_internal(EFL_FUNC_COMMON_OP_FUNC(efl_del));
        efl_del_api_generation = _efl_object_init_generation;
     }
   if (op != _efl_del_api_op_id)
     WRN("NULL passed to function %s().", func_name);
   return EINA_FALSE;
}

EO_API void
_efl_object_call_end(Efl_Object_Op_Call_Data *call)
{
   if (EINA_LIKELY(!!call->obj))
     {
        _apply_auto_unref(call->obj, call->eo_id);
        _efl_unref(call->obj);
        _eo_obj_pointer_done((Eo_Id)call->eo_id);
     }
}

static inline Eina_Bool
_eo_api_func_equal(const void *api_func1, const void *api_func2)
{
#ifndef _WIN32
   return (api_func1 == api_func2);
#else
   /* On Windows, DLL API's will be exported using the dllexport flag.
    * When used by another library or executable, they will be declared
    * using the dllimport flag. What happens really is that two symbols are
    * created, at two different addresses. So it's impossible to match
    * them. We fallback to plain string comparison based on the
    * function name itself. Slow, but this should rarely happen.
    */
   return (api_func2 && api_func1 && !strcmp(api_func2, api_func1));
#endif
}

static inline Efl_Object_Op
_efl_object_api_op_id_get_internal(const void *api_func)
{
   eina_spinlock_take(&_ops_storage_lock);
#ifndef _WIN32
   Efl_Object_Op op = (uintptr_t) eina_hash_find(_ops_storage, &api_func);
#else
   Efl_Object_Op op = (uintptr_t) eina_hash_find(_ops_storage, api_func);
#endif
   eina_spinlock_release(&_ops_storage_lock);

   return op;
}

/* LEGACY, should be removed before next release */
EO_API Efl_Object_Op
_efl_object_api_op_id_get(const void *api_func)
{
   Efl_Object_Op op = _efl_object_api_op_id_get_internal(api_func);

   if (op == EFL_NOOP)
     {
        ERR("Unable to resolve op for api func %p", api_func);
     }

   return op;
}

EO_API Efl_Object_Op
_efl_object_op_api_id_get(const void *api_func, const Eo *eo_obj, const char *api_func_name, const char *file, int line)
{
   Efl_Object_Op op;

#ifndef EO_DEBUG
   if (!eo_obj) return EFL_NOOP;
#endif
   op = _efl_object_api_op_id_get_internal(api_func);
   if (op == EFL_NOOP)
     {
        EO_OBJ_POINTER(eo_obj, obj);
        eina_log_print(_eo_log_dom, EINA_LOG_LEVEL_ERR,
                       file, api_func_name, line,
                       "Unable to resolve op for api func %p for obj=%p (%s)",
                       api_func, eo_obj, efl_class_name_get(eo_obj));
        _apply_auto_unref(obj, eo_obj);
        return EFL_NOOP;
     }

   return op;
}

/* klass is the klass we are working on. hierarchy_klass is the class whe should
 * use when validating. */
static Eina_Bool
_eo_class_funcs_set(Eo_Vtable *vtable, const Efl_Object_Ops *ops, const _Efl_Class *hierarchy_klass, const _Efl_Class *klass, Eina_Bool override_only, unsigned int class_id, Eina_Bool *hitmap)
{
   unsigned int i, j;
   unsigned int number_of_new_functions = 0;
   const Efl_Op_Description *op_desc;
   const Efl_Op_Description *op_descs;
   const _Efl_Class *override_class;
   const void **api_funcs;
   Eina_Bool check_equal;

   op_descs = ops->descs;
   override_class = override_only ? hierarchy_klass : NULL;

   DBG("Set functions for class '%s':%p", klass->desc->name, klass);

   if (!override_only)
     _vtable_insert_empty_funcs(vtable, class_id);
   if (!op_descs || !ops->count)
     return EINA_TRUE;

#ifdef EO_DEBUG
   check_equal = EINA_TRUE;
#else
   check_equal = !override_only;
#endif
   api_funcs = alloca(ops->count * sizeof(api_funcs[0]));

   /* sanity checks */
   for (i = 0, op_desc = op_descs; i < ops->count; i++, op_desc++)
     {
        if (op_desc->api_func == NULL)
          {
             ERR("Class '%s': NULL API not allowed (NULL->%p '%s').",
                 klass->desc->name, op_desc->func, _eo_op_desc_name_get(op_desc));
             return EINA_FALSE;
          }

        if (check_equal)
          {
             for (j = 0; j < i; j++)
               {
                  if (_eo_api_func_equal(op_desc->api_func, api_funcs[j]))
                    {
                       ERR("Class '%s': API previously defined (%p->%p '%s').",
                           klass->desc->name, op_desc->api_func, op_desc->func, _eo_op_desc_name_get(op_desc));
                       return EINA_FALSE;
                    }
               }

             api_funcs[i] = op_desc->api_func;
          }
        if (_efl_object_api_op_id_get_internal(op_desc->api_func) == EFL_NOOP)
          {
             number_of_new_functions ++;
          }
     }

   if (!override_only)
     {
        if (number_of_new_functions)
          {
             //Before setting any real functions, allocate the node that will contain all the functions
             _vtable_prepare_empty_node(vtable, number_of_new_functions, class_id);
          }
        hitmap[class_id] = EINA_TRUE;
     }

   for (i = 0, j = 0, op_desc = op_descs; i < ops->count; i++, op_desc++)
     {
        Efl_Object_Op op2 = EFL_NOOP;
        short op2_class_id;

        /* Get the opid for the function. */
        op2 = _efl_object_api_op_id_get_internal(op_desc->api_func);

        if (op2 == EFL_NOOP)
          {
             //functions that do not have a op yet, are considered to be belonging to this class
             if (override_only)
               {
                  ERR("Class '%s': Tried overriding a previously undefined function.", klass->desc->name);
                  return EINA_FALSE;
               }

             op2 = EFL_OBJECT_OP_CREATE_OP_ID(class_id, j);
             eina_spinlock_take(&_ops_storage_lock);
#ifndef _WIN32
             eina_hash_add(_ops_storage, &op_desc->api_func, (void *) (uintptr_t) op2);
#else
             eina_hash_add(_ops_storage, op_desc->api_func, (void *) (uintptr_t) op2);
#endif
             eina_spinlock_release(&_ops_storage_lock);
             j ++;
          }

#ifdef EO_DEBUG
        DBG("%p->%p '%s'", op_desc->api_func, op_desc->func, _eo_op_desc_name_get(op_desc));
#endif
        op2_class_id = EFL_OBJECT_OP_CLASS_PART(op2);
         //in case we are having a function overwrite for a specific type, copy the relevant vtable
        if (!hitmap[op2_class_id])
          {
             const Eo_Vtable_Node node = vtable->chain[op2_class_id];
             _vtable_copy_node(&vtable->chain[op2_class_id], &node);
             hitmap[op2_class_id] = EINA_TRUE;
          }
        if (!_vtable_func_set(vtable, klass, override_class, op2, op_desc->func, EINA_TRUE))
          return EINA_FALSE;
     }
   return EINA_TRUE;
}

EO_API Eina_Bool
efl_class_functions_set(const Efl_Class *klass_id, const Efl_Object_Ops *object_ops, const Efl_Object_Property_Reflection_Ops *reflection_table)
{
   EO_CLASS_POINTER_GOTO(klass_id, klass, err_klass);
   Efl_Object_Ops empty_ops = { 0 };
   Eina_Bool *hitmap;

   // not likely so use goto to alleviate l1 instruction cache of rare code
   if (klass->functions_set) goto err_funcs;
   klass->functions_set = EINA_TRUE;

   if (!object_ops) object_ops = &empty_ops;

   klass->reflection = reflection_table;

   klass->ops_count = object_ops->count;

   klass->class_id = _UNMASK_ID(klass->header.id) - 1;

   _vtable_init(&klass->vtable);
   if (!klass->vtable.chain) goto err_vtable;

   hitmap = alloca(klass->vtable.size);
   memset(hitmap, 0, klass->vtable.size);
   /* Merge in all required vtable entries */
     {
        const _Efl_Class **mro_itr = klass->mro;
        /* take over everything from the parent */
        if (klass->parent)
          {
             _vtable_take_over(&klass->vtable, &klass->parent->vtable);
          }
        /*
         * - jump to the mro entry containing the parent
         * - everything further from the parent to the next elements is already
         *   represented in the vtable of the parent.
         */
        for (  ; *mro_itr ; mro_itr++)
          {
             if (*mro_itr == klass->parent)
               break;
          }
        /**
         * merge in all the APIs that are extended in the current klass for this first time.
         * That means, they are not extended anywhere from the parent further up.
         */
        for ( mro_itr-- ; mro_itr > klass->mro ; mro_itr--)
          {
             _vtable_merge_defined_api(&klass->vtable, &(*mro_itr)->vtable, hitmap);
          }
        /*
         * add slots for the interfaces and mixins we are inheriting from
         */
        for (int i = 0; klass->extensions[i]; i++)
          {
             const _Efl_Class *ext = klass->extensions[i];
             /*for all extensions of the class, ensure that *at least* empty vtables are available, so the efl_isa calls do succeed*/
             _vtable_merge_empty(&klass->vtable, &ext->vtable, hitmap);
          }
     }
     {
        unsigned int i;

        for (i = 0; i < object_ops->count; i++)
          {
             Efl_Object_Op op = _efl_object_api_op_id_get_internal(object_ops->descs[i].api_func);
             if (op == EFL_NOOP) continue; //EFL_NOOP means that this function is not yet defined, this will be handled later
             short class_id = EFL_OBJECT_OP_CLASS_PART(op);
             if (klass->vtable.chain[class_id].count == 0)
               {
                  const _Efl_Class *required_klass = _eo_classes[class_id];
                  /* in case this type is not already inherited, error on everything that is not a mixin */
                  if (klass->desc->type == EFL_CLASS_TYPE_MIXIN)
                    {
                       /* this is when a mixin implemets a regular api, we just prepare a empty node, the rest will be implemented later */
                       _vtable_prepare_empty_node(&klass->vtable, required_klass->vtable.chain[class_id].count, class_id);
                       hitmap[class_id] = EINA_TRUE;
                    }
                  else
                    {
                       ERR("There is an API implemented, whoms type is not part of this class. %s vs. %s", klass->desc->name, required_klass->desc->name);
                       _vtable_prepare_empty_node(&klass->vtable, required_klass->vtable.chain[class_id].count, class_id);
                       hitmap[class_id] = EINA_TRUE;
                    }

               }
          }
     }
   return _eo_class_funcs_set(&klass->vtable, object_ops, klass, klass, EINA_FALSE, klass->class_id, hitmap);
err_funcs:
   ERR("Class %s already had its functions set..", klass->desc->name);
   return EINA_FALSE;
err_klass:
   _EO_POINTER_ERR(klass_id, "Class (%p) is an invalid ref.", klass_id);
   return EINA_FALSE;
err_vtable:
   ERR("failed to allocate vtable for class '%s'", klass->desc->name);
   return EINA_FALSE;
}

static Eo *
_efl_add_internal_start_do(const char *file, int line, const Efl_Class *klass_id, Eo *parent_id, Eina_Bool ref, Eina_Bool is_fallback, Efl_Substitute_Ctor_Cb substitute_ctor, void *sub_ctor_data)
{
   const char *func_name = __func__;
   _Eo_Object *obj;
   Eo_Stack_Frame *fptr = NULL;

   if (is_fallback) fptr = _efl_add_fallback_stack_push(NULL);

   if (class_overrides)
     {
        const Efl_Class *override = eina_hash_find(class_overrides, &klass_id);
        if (override) klass_id = override;
     }

   EO_CLASS_POINTER_GOTO_PROXY(klass_id, klass, err_klass);

   // Check that in the case of efl_add we do pass a parent.
   if (!ref && !parent_id)
     ERR("Creation of '%s' object at line %i in '%s' is done without parent. This should use efl_add_ref.",
         klass->desc->name, line, file);

   if (parent_id)
     {
        EO_OBJ_POINTER_GOTO_PROXY(parent_id, parent, err_parent);
     }

   // not likely so use goto to alleviate l1 instruction cache of rare code
   if (EINA_UNLIKELY(klass->desc->type != EFL_CLASS_TYPE_REGULAR))
     goto err_noreg;

   eina_spinlock_take(&klass->objects.trash_lock);
   obj = eina_trash_pop(&klass->objects.trash);
   if (obj)
     {
        memset(obj, 0, klass->obj_size);
        klass->objects.trash_count--;
     }
   else
     {
        obj = calloc(1, klass->obj_size);
     }
   eina_spinlock_release(&klass->objects.trash_lock);

   obj->opt = eina_cow_alloc(efl_object_optional_cow);
   _efl_ref(obj);
   obj->klass = klass;

   obj->header.id = _eo_id_allocate(obj, parent_id);
   Eo *eo_id = _eo_obj_id_get(obj);

   _eo_log_obj_ref_op(obj, EO_REF_OP_NEW);

   _eo_condtor_reset(obj);

   efl_ref(eo_id);

   /* Reference for the parent if is_ref is done in _efl_add_end */
   if (parent_id) efl_parent_set(eo_id, parent_id);

   /* eo_id can change here. Freeing is done on the resolved object. */
   if (!substitute_ctor) eo_id = efl_constructor(eo_id);
   else eo_id = substitute_ctor(sub_ctor_data, eo_id);
   // not likely so use goto to alleviate l1 instruction cache of rare code
   if (!eo_id) goto err_noid;
   // not likely so use goto to alleviate l1 instruction cache of rare code
   else if (eo_id != _eo_obj_id_get(obj)) goto ok_nomatch;
ok_nomatch_back:
   if (is_fallback) fptr->obj = eo_id;
   if (parent_id) EO_OBJ_DONE(parent_id);
   return eo_id;

ok_nomatch:
     {
        EO_OBJ_POINTER_GOTO_PROXY(eo_id, new_obj, err_newid);
        _efl_ref(new_obj);
        efl_ref(eo_id);
        /* We might have two refs on the old object at this point. */
        efl_parent_set((Eo *) obj->header.id, NULL);
        efl_unref(_eo_obj_id_get(obj));
        _efl_unref(obj);
        EO_OBJ_DONE(eo_id);
     }
   goto ok_nomatch_back;

err_noid:
   ERR("in %s:%d: Object of class '%s' - Error while constructing object",
       file, line, klass->desc->name);
   /* We might have two refs at this point. */
   efl_parent_set((Eo *) obj->header.id, NULL);
   efl_unref(_eo_obj_id_get(obj));
   _efl_unref(obj);
err_newid:
   if (parent_id) EO_OBJ_DONE(parent_id);
   return NULL;
err_noreg:
   ERR("in %s:%d: Class '%s' is not instantiate-able. Aborting.", file, line, klass->desc->name);
   if (parent_id) EO_OBJ_DONE(parent_id);
   return NULL;

err_klass:
   _EO_POINTER_ERR(klass_id, "in %s:%d: Class (%p) is an invalid ref.", file, line, klass_id);
err_parent:
   return NULL;
}

EO_API Eo *
_efl_add_internal_start(const char *file, int line, const Efl_Class *klass_id, Eo *parent_id, Eina_Bool ref, Eina_Bool is_fallback)
{
   return _efl_add_internal_start_do(file, line, klass_id, parent_id, ref, is_fallback, NULL, NULL);
}

EO_API Eo * _efl_add_internal_start_bindings(const char *file, int line, const Efl_Class *klass_id, Eo *parent_id, Eina_Bool ref, Eina_Bool is_fallback, Efl_Substitute_Ctor_Cb substitute_ctor, void *sub_ctor_data)
{
   return _efl_add_internal_start_do(file, line, klass_id, parent_id, ref, is_fallback, substitute_ctor, sub_ctor_data);
}

static Eo *
_efl_add_internal_end(Eo *eo_id, Eo *finalized_id)
{
   EO_OBJ_POINTER_RETURN_VAL(eo_id, obj, NULL);

   // rare so move error handling to end to save l1 instruction cache
   if (!obj->condtor_done) goto err_condtor;
   if (!finalized_id)
     {
        // XXX: Given EFL usage of objects, construction is a perfectly valid thing
        // to do. we shouldn't complain about it as handling a NULL obj creation is
        // the job of the caller. a perfect example here is ecore_con and ecore_ipc
        // where you create a con or ipc obj then set up type/destination/port and
        // the finalize of the constructor does the actual connect and thus this
        // fails or succeeds based on if service is there.
        //
        // until there is a better solution - don't complain here.
        //
        //             ERR("Object of class '%s' - Finalizing the object failed.",
        //                   klass->desc->name);
        goto cleanup;
     }

   obj->finalized = EINA_TRUE;
   _efl_unref(obj);
   EO_OBJ_DONE(eo_id);
   return (Eo *)eo_id;

err_condtor:
     {
        const _Efl_Class *klass = obj->klass;
        ERR("Object of class '%s' - Not all of the object constructors have been executed.",
              klass->desc->name);
     }
cleanup:
   efl_parent_set((Eo *) obj->header.id, NULL);
   efl_unref((Eo *) obj->header.id);
   _efl_unref(obj);
   EO_OBJ_DONE(eo_id);
   return NULL;
}

EO_API Eo *
_efl_add_end(Eo *eo_id, Eina_Bool is_ref, Eina_Bool is_fallback)
{
   if (!eo_id) return NULL;
   Eo *ret = efl_finalize(eo_id);
   ret = _efl_add_internal_end(eo_id, ret);

   if (ret && !is_ref)
     {
        efl_unref(ret);
     }

   if (is_fallback)
     {
        _efl_add_fallback_stack_pop();
     }

   return ret;
}

EO_API void
efl_reuse(const Eo *eo_id)
{
   Eo *obj = (Eo *) eo_id;
   EO_OBJ_POINTER_RETURN(obj, _obj);

   efl_object_override(obj, NULL);
   _efl_object_reuse(_obj);

#ifdef EO_DEBUG
   _eo_log_obj_ref_op(_obj, EO_REF_OP_REUSE);
#endif

   EO_OBJ_DONE(eo_id);
}

void
_eo_free(_Eo_Object *obj, Eina_Bool manual_free EINA_UNUSED)
{
   _Efl_Class *klass = (_Efl_Class*) obj->klass;

   _eo_log_obj_ref_op(obj, EO_REF_OP_FREE);

#ifdef EO_DEBUG
   if (manual_free)
     {
        Eo *obj_id = _eo_obj_id_get(obj);
        if (obj->datarefcount)
          {
             ERR("Object %p data still referenced %d time(s).", obj_id, obj->datarefcount);
          }
        while (obj->xrefs)
          {
             Eina_Inlist *nitr = obj->xrefs->next;
             Eo_Xref_Node *xref = EINA_INLIST_CONTAINER_GET(obj->data_xrefs, Eo_Xref_Node);
             ERR("Object %p is still referenced by object %p. Origin: %s:%d",
                 obj_id, xref->ref_obj, xref->file, xref->line);
             eina_freeq_ptr_main_add(xref, free, sizeof(*xref));
             obj->xrefs = nitr;
          }
        while (obj->data_xrefs)
          {
             Eina_Inlist *nitr = obj->data_xrefs->next;
             Eo_Xref_Node *xref = EINA_INLIST_CONTAINER_GET(obj->data_xrefs, Eo_Xref_Node);
             if (obj_id == xref->ref_obj)
               {
                  WRN("Object %p still has a reference to its own data (subclass: %s). Origin: %s:%d",
                      obj_id, xref->data_klass, xref->file, xref->line);
               }
             else
               {
                  ERR("Data of object %p (subclass: %s) is still referenced by object %p. Origin: %s:%d",
                      obj_id, xref->data_klass, xref->ref_obj, xref->file, xref->line);
               }

             eina_freeq_ptr_main_add(xref, free, sizeof(*xref));
             obj->data_xrefs = nitr;
          }
     }
#endif
   if (obj->opt && _obj_is_override(obj))
     {
        _vtable_free(obj->opt->vtable, &obj->klass->vtable);
        EO_OPTIONAL_COW_SET(obj, vtable, NULL);
     }

   _eo_id_release((Eo_Id) _eo_obj_id_get(obj));
   eina_cow_free(efl_object_optional_cow, (Eina_Cow_Data *) &obj->opt);

   eina_spinlock_take(&klass->objects.trash_lock);
   if ((klass->objects.trash_count <= 8) && (EINA_LIKELY(!_eo_trash_bypass)))
     {
        eina_trash_push(&klass->objects.trash, obj);
        klass->objects.trash_count++;
     }
   else
     {
        eina_freeq_ptr_main_add(obj, free, klass->obj_size);
     }
   eina_spinlock_release(&klass->objects.trash_lock);
}
/*****************************************************************************/

EO_API const Efl_Class *
efl_class_get(const Eo *eo_id)
{
   const Efl_Class *klass;

   if (_eo_is_a_class(eo_id))
     {
        EO_CLASS_POINTER_GOTO(eo_id, _klass, err_klass);
        return EFL_CLASS_CLASS;
     }

   EO_OBJ_POINTER_GOTO(eo_id, obj, err_obj);
   klass = _eo_class_id_get(obj->klass);
   EO_OBJ_DONE(eo_id);
   return klass;

err_klass:
   _EO_POINTER_ERR(eo_id, "Class (%p) is an invalid ref.", eo_id);
err_obj:
   return NULL;
}

EO_API const char *
efl_class_name_get(const Efl_Class *eo_id)
{
   const _Efl_Class *klass;

   if (_eo_is_a_class(eo_id))
     {
        EO_CLASS_POINTER_GOTO(eo_id, _klass, err_klass);
        klass = _klass;
     }
   else
     {
        EO_OBJ_POINTER_GOTO(eo_id, obj, err_obj);
        klass = obj->klass;
        EO_OBJ_DONE(eo_id);
     }
   return klass->desc->name;

err_klass:
   _EO_POINTER_ERR(eo_id, "Class (%p) is an invalid ref.", eo_id);
err_obj:
   return NULL;
}

EO_API size_t
efl_class_memory_size_get(const Efl_Class *eo_id)
{
   const _Efl_Class *klass;

   if (_eo_is_a_class(eo_id))
     {
        EO_CLASS_POINTER_GOTO(eo_id, _klass, err_klass);
        klass = _klass;
     }
   else
     {
        EO_OBJ_POINTER_GOTO(eo_id, obj, err_obj);
        klass = obj->klass;
        EO_OBJ_DONE(eo_id);
     }
   return klass->obj_size;

err_klass:
   _EO_POINTER_ERR(eo_id, "Class (%p) is an invalid ref.", eo_id);
err_obj:
   return 0;
}

static Eina_Bool
_eo_class_mro_has(const _Efl_Class *klass, const _Efl_Class *find)
{
   const _Efl_Class **itr;
   for (itr = klass->mro ; *itr ; itr++)
     {
        if (*itr == find)
          {
             return EINA_TRUE;
          }
     }
   return EINA_FALSE;
}

static Eina_List *
_eo_class_list_remove_duplicates(Eina_List* list)
{
   Eina_List *itr1, *itr2, *itr2n;

   itr1 = eina_list_last(list);
   while (itr1)
     {
        itr2 = eina_list_prev(itr1);

        while (itr2)
          {
             itr2n = eina_list_prev(itr2);

             if (eina_list_data_get(itr1) == eina_list_data_get(itr2))
               {
                  list = eina_list_remove_list(list, itr2);
               }

             itr2 = itr2n;
          }

        itr1 = eina_list_prev(itr1);
     }

   return list;
}

static Eina_List *
_eo_class_mro_add(Eina_List *mro, const _Efl_Class *klass)
{
   if (!klass)
     return mro;

   mro = eina_list_append(mro, klass);

   /* Recursively add MIXINS extensions. */
     {
        const _Efl_Class **extn_itr;

        for (extn_itr = klass->extensions ; *extn_itr ; extn_itr++)
          {
             const _Efl_Class *extn = *extn_itr;
             if (extn->desc->type == EFL_CLASS_TYPE_MIXIN)
               mro = _eo_class_mro_add(mro, extn);
          }
     }

   mro = _eo_class_mro_add(mro, klass->parent);

   return mro;
}

static Eina_List *
_eo_class_mro_init(const Efl_Class_Description *desc, const _Efl_Class *parent, Eina_List *extensions)
{
   Eina_List *mro = NULL;
   Eina_List *extn_itr = NULL;
   Eina_List *extn_pos = NULL;
   const _Efl_Class *extn = NULL;

   /* Add MIXINS extensions. */
   EINA_LIST_FOREACH(extensions, extn_itr, extn)
     {
        if (extn->desc->type != EFL_CLASS_TYPE_MIXIN)
          continue;

        mro = _eo_class_mro_add(mro, extn);
        extn_pos = eina_list_append(extn_pos, eina_list_last(mro));
     }

   /* Check if we can create a consistent mro */
     {
        Eina_List *itr = extn_pos;
        EINA_LIST_FOREACH(extensions, extn_itr, extn)
          {
             if (extn->desc->type != EFL_CLASS_TYPE_MIXIN)
                continue;

             /* Get the first one after the extension. */
             Eina_List *extn_list = eina_list_next(eina_list_data_get(itr));

             /* If we found the extension again. */
             if (eina_list_data_find_list(extn_list, extn))
               {
                  eina_list_free(mro);
                  eina_list_free(extn_pos);
                  ERR("Cannot create a consistent method resolution order for class '%s' because of '%s'.", desc->name, extn->desc->name);
                  return NULL;
               }

             itr = eina_list_next(itr);
          }
     }

   eina_list_free(extn_pos);

   mro = _eo_class_mro_add(mro, parent);
   mro = _eo_class_list_remove_duplicates(mro);
   /* Will be replaced with the actual class pointer */
   mro = eina_list_prepend(mro, NULL);

   return mro;
}

static Eina_Bool
_eo_class_initializer(_Efl_Class *klass)
{
   if (klass->desc->class_initializer)
     return klass->desc->class_initializer(_eo_class_id_get(klass));

   return EINA_TRUE;
}

static void
_eo_class_constructor(_Efl_Class *klass)
{
   klass->constructed = EINA_TRUE;

   klass->construction_thread = eina_thread_self();

   if (klass->desc->class_constructor)
     klass->desc->class_constructor(_eo_class_id_get(klass));
}

static void
eo_class_free(_Efl_Class *klass)
{
   void *data;
   Eina_Thread self = eina_thread_self();

   if ((self != _efl_object_main_thread) &&
       (self != klass->construction_thread))
     CRI("Calling class deconstructor from thread that did not call constructor and is not main thread!\n"
         "This will probably crash!");

   if (klass->constructed)
     {
        if (klass->desc->class_destructor)
           klass->desc->class_destructor(_eo_class_id_get(klass));
        _vtable_mro_free(klass);
        _vtable_free(&klass->vtable, NULL);
     }

   EINA_TRASH_CLEAN(&klass->objects.trash, data)
      eina_freeq_ptr_main_add(data, free, klass->obj_size);

   EINA_TRASH_CLEAN(&klass->iterators.trash, data)
      eina_freeq_ptr_main_add(data, free, 0);

   eina_spinlock_free(&klass->objects.trash_lock);
   eina_spinlock_free(&klass->iterators.trash_lock);

   eina_freeq_ptr_main_add(klass, free, 0);
}

static inline void
_eo_classes_release(void)
{
#ifdef HAVE_MMAP
# ifdef HAVE_VALGRIND
   if (RUNNING_ON_VALGRIND) free(_eo_classes);
   else
# endif
     {
        size_t size;

        size = _eo_classes_alloc * sizeof(_Efl_Class *);
        if (_eo_classes) munmap(_eo_classes, size);
     }
#else
   free(_eo_classes);
#endif
   _eo_classes = NULL;
   _eo_classes_last_id = 0;
   _eo_classes_alloc = 0;
}

static inline void
_eo_classes_expand(void)
{
   unsigned char *ptr;
   size_t newsize, psize;

   _eo_classes_last_id++;
   if (_eo_classes_last_id <= _eo_classes_alloc) return;
   psize = _eo_classes_alloc * sizeof(_Efl_Class *);
#ifdef HAVE_MMAP
# ifdef HAVE_VALGRIND
   if (RUNNING_ON_VALGRIND)
     {
        _eo_classes_alloc += 128;
        newsize = _eo_classes_alloc * sizeof(_Efl_Class *);
        ptr = realloc(_eo_classes, newsize);
        if (!ptr)
          {
             ERR("realloc of eo class table region faile!!");
             abort();
          }
     }
   else
# endif
     {
        _eo_classes_alloc += (MEM_PAGE_SIZE / sizeof(_Efl_Class *));
        newsize = _eo_classes_alloc * sizeof(_Efl_Class *);
        ptr = mmap(NULL, newsize, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON, -1, 0);
        if (ptr == MAP_FAILED)
          {
             ERR("mmap of eo class table region failed!");
             abort();
          }
        if (psize > 0) memcpy(ptr, _eo_classes, psize);
        if (_eo_classes) munmap(_eo_classes, psize);
     }
#else
   _eo_classes_alloc += 128;
   newsize = _eo_classes_alloc * sizeof(_Efl_Class *);
   ptr = realloc(_eo_classes, newsize);
   if (!ptr)
     {
        ERR("realloc of eo class table region faile!!");
        abort();
     }
#endif
   memset(ptr + psize, 0, newsize - psize);
   _eo_classes = (_Efl_Class **)ptr;
}

EO_API const Efl_Class *
efl_class_new(const Efl_Class_Description *desc, const Efl_Class *parent_id, ...)
{
   _Efl_Class *klass;
   va_list p_list;
   size_t extn_sz, mro_sz, mixins_sz;
   Eina_List *extn_list, *mro, *mixins;
   _Efl_Class *parent = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(desc, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(desc->name, NULL);

   if (parent_id)
     {
        parent = _eo_class_pointer_get(parent_id);
        if (!parent)
          return NULL;
     }

   /* Check restrictions on Interface types. */
   if (desc->type == EFL_CLASS_TYPE_INTERFACE)
     {
        EINA_SAFETY_ON_FALSE_RETURN_VAL(!desc->data_size, NULL);
     }

   /* Check parent */
   if (parent)
     {
        /* Verify the inheritance is allowed. */
        switch (desc->type)
          {
           case EFL_CLASS_TYPE_REGULAR:
           case EFL_CLASS_TYPE_REGULAR_NO_INSTANT:
              if ((parent->desc->type != EFL_CLASS_TYPE_REGULAR) &&
                    (parent->desc->type != EFL_CLASS_TYPE_REGULAR_NO_INSTANT))
                {
                   ERR("Regular classes ('%s') aren't allowed to inherit from non-regular classes ('%s').",
                       desc->name, parent->desc->name);
                   return NULL;
                }
              break;
           case EFL_CLASS_TYPE_INTERFACE:
           case EFL_CLASS_TYPE_MIXIN:
              if ((parent->desc->type != EFL_CLASS_TYPE_INTERFACE) &&
                    (parent->desc->type != EFL_CLASS_TYPE_MIXIN))
                {
                   ERR("Non-regular classes ('%s') aren't allowed to inherit from regular classes ('%s').",
                       desc->name, parent->desc->name);
                   return NULL;
                }
              break;
           default:
             ERR("type cannot be INVALID");
             return NULL;
          }
     }

   /* Build class extensions list */
     {
        DBG("Started building extensions list for class '%s'", desc->name);
        extn_list = NULL;
        const _Efl_Class *extn = NULL;
        const Eo_Id *extn_id = NULL;

        va_start(p_list, parent_id);

        extn_id = va_arg(p_list, Eo_Id *);
        while (extn_id)
          {
             extn = _eo_class_pointer_get((Efl_Class *)extn_id);
             if (EINA_LIKELY(extn != NULL))
               {
                  switch (extn->desc->type)
                    {
                     case EFL_CLASS_TYPE_REGULAR_NO_INSTANT:
                     case EFL_CLASS_TYPE_REGULAR:
                     case EFL_CLASS_TYPE_INTERFACE:
                     case EFL_CLASS_TYPE_MIXIN:
                       extn_list = eina_list_append(extn_list, extn);
                       break;
                     default:
                       ERR("type cannot be INVALID");
                       va_end(p_list);
                       return NULL;
                    }
               }
             extn_id = va_arg(p_list, Eo_Id *);
          }

        va_end(p_list);

        extn_list = _eo_class_list_remove_duplicates(extn_list);

        extn_sz = sizeof(_Efl_Class *) * (eina_list_count(extn_list) + 1);

        DBG("Finished building extensions list for class '%s'", desc->name);
     }

   /* Prepare mro list */
     {
        DBG("Started building MRO list for class '%s'", desc->name);

        mro = _eo_class_mro_init(desc, parent, extn_list);
        if (!mro)
          {
             eina_list_free(extn_list);
             return NULL;
          }

        mro_sz = sizeof(_Efl_Class *) * (eina_list_count(mro) + 1);

        DBG("Finished building MRO list for class '%s'", desc->name);
     }

   /* Prepare mixins list */
     {
        Eina_List *itr;
        const _Efl_Class *kls_itr;

        DBG("Started building Mixins list for class '%s'", desc->name);

        mixins = NULL;
        EINA_LIST_FOREACH(mro, itr, kls_itr)
          {
             if ((kls_itr) && (kls_itr->desc->type == EFL_CLASS_TYPE_MIXIN) &&
                   (kls_itr->desc->data_size > 0))
               mixins = eina_list_append(mixins, kls_itr);
          }

        mixins_sz = sizeof(Eo_Extension_Data_Offset) * (eina_list_count(mixins) + 1);
        if ((desc->type == EFL_CLASS_TYPE_MIXIN) && (desc->data_size > 0))
          mixins_sz += sizeof(Eo_Extension_Data_Offset);

        DBG("Finished building Mixins list for class '%s'", desc->name);
     }

   klass = calloc(1, _eo_class_sz + extn_sz + mro_sz + mixins_sz);
   eina_spinlock_new(&klass->objects.trash_lock);
   eina_spinlock_new(&klass->iterators.trash_lock);
   klass->parent = parent;
   klass->desc = desc;
   klass->extensions = (const _Efl_Class **) ((char *) klass + _eo_class_sz);
   klass->mro = (const _Efl_Class **) ((char *) klass->extensions + extn_sz);
   klass->extn_data_off = (Eo_Extension_Data_Offset *) ((char *) klass->mro + mro_sz);

   if (klass->parent)
     {
        /* FIXME: Make sure this alignment is enough. */
        klass->data_offset = klass->parent->data_offset +
           EO_ALIGN_SIZE(klass->parent->desc->data_size);
     }
   else
     {
        /* Data starts after the object size. */
        klass->data_offset = _eo_sz;
     }

   mro = eina_list_remove(mro, NULL);
   mro = eina_list_prepend(mro, klass);
   if ((desc->type == EFL_CLASS_TYPE_MIXIN) && (desc->data_size > 0))
     mixins = eina_list_prepend(mixins, klass);

   /* Copy the extensions and free the list */
     {
        const _Efl_Class *extn = NULL;
        const _Efl_Class **extn_itr = klass->extensions;
        EINA_LIST_FREE(extn_list, extn)
          {
             *(extn_itr++) = extn;

             DBG("Added '%s' extension", extn->desc->name);
          }
        *(extn_itr) = NULL;
     }

   /* Copy the mro and free the list. */
     {
        const _Efl_Class *kls_itr = NULL;
        const _Efl_Class **mro_itr = klass->mro;
        EINA_LIST_FREE(mro, kls_itr)
          {
             *(mro_itr++) = kls_itr;

             DBG("Added '%s' to MRO", kls_itr->desc->name);
          }
        *(mro_itr) = NULL;
     }

   size_t extn_data_off = klass->data_offset;
   if (klass->desc->type != EFL_CLASS_TYPE_MIXIN)
      extn_data_off += EO_ALIGN_SIZE(klass->desc->data_size);

   /* Feed the mixins data offsets and free the mixins list. */
     {
        const _Efl_Class *kls_itr = NULL;
        Eo_Extension_Data_Offset *extn_data_itr = klass->extn_data_off;
        EINA_LIST_FREE(mixins, kls_itr)
          {
             extn_data_itr->klass = kls_itr;
             extn_data_itr->offset = extn_data_off;

             extn_data_off += EO_ALIGN_SIZE(extn_data_itr->klass->desc->data_size);
             extn_data_itr++;

             DBG("Added '%s' to Data Offset info", kls_itr->desc->name);
          }
        extn_data_itr->klass = 0;
        extn_data_itr->offset = 0;
     }

   klass->obj_size = extn_data_off;

     {
        Eo_Id new_id;

        eina_lock_take(&_efl_class_creation_lock);
        new_id = (_eo_classes_last_id + 1) | MASK_CLASS_TAG;
        _eo_classes_expand();
        _eo_classes[_UNMASK_ID(new_id) - 1] = klass;
        eina_lock_release(&_efl_class_creation_lock);

        klass->header.id = new_id;
     }

   if (!_eo_class_initializer(klass))
     {
        return NULL;
     }

   /* If functions haven't been set, invoke it with an empty ops structure. */
   if (!klass->functions_set)
     {
        efl_class_functions_set(_eo_class_id_get(klass), NULL, NULL);
     }

   _eo_class_constructor(klass);

   DBG("Finished building class '%s'", klass->desc->name);

   return _eo_class_id_get(klass);
}

EO_API Eina_Bool
efl_object_override(Eo *eo_id, const Efl_Object_Ops *ops)
{
   EO_OBJ_POINTER_RETURN_VAL(eo_id, obj, EINA_FALSE);
   EO_CLASS_POINTER_GOTO(EFL_OBJECT_OVERRIDE_CLASS, klass, err);

   if (ops)
     {
        Eo_Vtable *vtable = obj->opt->vtable;
        //copy all the vtable nodes that we are going to change later on
        Eina_Bool *hitmap;

        if (!vtable)
          {
             vtable = calloc(1, sizeof(*vtable));
             _vtable_init_size(vtable, obj->klass->vtable.size);
             _vtable_take_over(vtable, &obj->klass->vtable);
          }

        hitmap = alloca(vtable->size * sizeof(Eina_Bool));
        memset(hitmap, 0, vtable->size);

        if (!_eo_class_funcs_set(vtable, ops, obj->klass, klass, EINA_TRUE, obj->klass->class_id, hitmap))
          {
             ERR("Failed to override functions for %s@%p. All previous "
                 "overrides have been reset.", obj->klass->desc->name, eo_id);
             if (obj->opt->vtable == vtable)
               {
                  EO_OPTIONAL_COW_SET(obj, vtable, NULL);
               }
             else
               {
                  _vtable_free(vtable, &obj->klass->vtable);
                  free(vtable);
               }

             goto err;
          }
        EO_OPTIONAL_COW_SET(obj, vtable, vtable);
     }
   else
     {
        if (obj->opt->vtable)
          {
             _vtable_free(obj->opt->vtable, &obj->klass->vtable);
             EO_OPTIONAL_COW_SET(obj, vtable, NULL);
          }
     }

   EO_OBJ_DONE(eo_id);
   return EINA_TRUE;

err:
   EO_OBJ_DONE(eo_id);
   return EINA_FALSE;
}

EO_API Eina_Bool
efl_isa(const Eo *eo_id, const Efl_Class *klass_id)
{
   Efl_Id_Domain domain;
   Eo_Id_Data *data;
   Eo_Id_Table_Data *tdata;
   Eina_Bool isa = EINA_FALSE;

   if (EINA_UNLIKELY(!eo_id)) return EINA_FALSE;

   // Everything can add a override to an existing class, which pretty much means, everything is a efl override
   // This is required in order to support our debug-profile for the users of efl_override
   if (EINA_UNLIKELY(klass_id == EFL_OBJECT_OVERRIDE_CLASS)) return EINA_TRUE;

   // Case where we are looking if eo_id is a class that contain klass_id
   if (EINA_UNLIKELY(_eo_is_a_class(eo_id)))
     {

        EO_CLASS_POINTER_GOTO(klass_id, klass, err_class);
        EO_CLASS_POINTER_GOTO(eo_id, lookinto, err_class0);

        if (EINA_UNLIKELY(lookinto->vtable.size <= klass->class_id))
          return EINA_FALSE;

        return !!lookinto->vtable.chain[klass->class_id].funcs;
     }

   domain = ((Eo_Id)eo_id >> SHIFT_DOMAIN) & MASK_DOMAIN;
   data = _eo_table_data_get();
   tdata = _eo_table_data_table_get(data, domain);
   if (EINA_UNLIKELY(!tdata)) goto err;

   if (EINA_LIKELY(domain != EFL_ID_DOMAIN_SHARED))
     {
        if ((tdata->cache.isa_id == eo_id) &&
            (tdata->cache.klass == klass_id))
          {
             isa = tdata->cache.isa;
             return isa;
          }

        EO_OBJ_POINTER_GOTO(eo_id, obj, err_obj);
        EO_CLASS_POINTER_GOTO(klass_id, klass, err_class);

        const Eo_Vtable vtable = obj->klass->vtable;
        if (EINA_UNLIKELY(vtable.size <= klass->class_id))
          return EINA_FALSE;

        isa = !!vtable.chain[klass->class_id].funcs;

        // Caching the result as we do a lot of serial efl_isa due to evas_object_image using it.
        tdata->cache.isa_id = eo_id;
        tdata->cache.klass = klass_id;
        tdata->cache.isa = isa;
     }
   else
     {
        eina_lock_take(&(_eo_table_data_shared_data->obj_lock));

        if ((tdata->cache.isa_id == eo_id) &&
            (tdata->cache.klass == klass_id))
          {
             isa = tdata->cache.isa;
             // since this is the cache we hope this gets a lot of hits and
             // thus lets assume the hit is the mot important thing thus
             // put the lock release and return here inline in the l1
             // instruction cache hopefully already fetched
             eina_lock_release(&(_eo_table_data_shared_data->obj_lock));
             return isa;
          }

        EO_OBJ_POINTER_GOTO(eo_id, obj, err_shared_obj);
        EO_CLASS_POINTER_GOTO(klass_id, klass, err_shared_class);
        if (EINA_UNLIKELY(obj->klass->vtable.size <= klass->class_id))
          goto err_vtable;

        isa = !!obj->klass->vtable.chain[klass->class_id].funcs;

        // Caching the result as we do a lot of serial efl_isa due to evas_object_image using it.
        tdata->cache.isa_id = eo_id;
        tdata->cache.klass = klass_id;
        tdata->cache.isa = isa;
err_vtable:
        EO_OBJ_DONE(eo_id);
        eina_lock_release(&(_eo_table_data_shared_data->obj_lock));
     }
   return isa;

err_shared_class: EINA_COLD
   _EO_POINTER_ERR(klass_id, "Class (%p) is an invalid ref.", klass_id);
   EO_OBJ_DONE(eo_id);
err_shared_obj: EINA_COLD
   eina_lock_release(&(_eo_table_data_shared_data->obj_lock));
   return EINA_FALSE;

err_class0:
   _EO_POINTER_ERR(eo_id, "Class (%p) is an invalid ref.", eo_id);
   return EINA_FALSE;

err_class: EINA_COLD
   _EO_POINTER_ERR(klass_id, "Class (%p) is an invalid ref.", klass_id);
err_obj:
   return EINA_FALSE;

err: EINA_COLD
   ERR("Object %p is not a valid object in this context: object domain: %d, "
       "current domain: %d, local domain: %d, available domains: [%s %s %s %s]."
       " Are you trying to access this object from another thread?",
       eo_id, (int)domain,
       (int)data->domain_stack[data->stack_top], (int)data->local_domain,
       (data->tables[0]) ? "0" : " ", (data->tables[1]) ? "1" : " ",
       (data->tables[2]) ? "2" : " ", (data->tables[3]) ? "3" : " ");
   return EINA_FALSE;
}

EO_API Eo *
efl_xref_internal(const char *file, int line, Eo *obj_id, const Eo *ref_obj_id)
{
   efl_ref(obj_id);

#ifdef EO_DEBUG
   const char *func_name = __func__;
   EO_OBJ_POINTER_RETURN_VAL_PROXY(obj_id, obj, obj_id);

   Eo_Xref_Node *xref = calloc(1, sizeof(*xref));
   xref->ref_obj = ref_obj_id;
   xref->file = file;
   xref->line = line;

   obj->xrefs = eina_inlist_prepend(obj->xrefs, EINA_INLIST_GET(xref));
   EO_OBJ_DONE(obj_id);
#else
   (void) ref_obj_id;
   (void) file;
   (void) line;
#endif

   return obj_id;
}

EO_API void
efl_xunref(Eo *obj_id, const Eo *ref_obj_id)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
#ifdef EO_DEBUG
   Eo_Xref_Node *xref = NULL;
   EINA_INLIST_FOREACH(obj->xrefs, xref)
     {
        if (xref->ref_obj == ref_obj_id)
          break;
     }

   if (xref)
     {
        obj->xrefs = eina_inlist_remove(obj->xrefs, EINA_INLIST_GET(xref));
        eina_freeq_ptr_main_add(xref, free, sizeof(*xref));
     }
   else
     {
        ERR("ref_obj (%p) does not reference obj (%p). Aborting unref.", ref_obj_id, obj_id);
        EO_OBJ_DONE(obj_id);
        return;
     }
   EO_OBJ_DONE(obj_id);
#else
   (void) ref_obj_id;
#endif
   efl_unref(obj_id);
}

EO_API Eo *
efl_ref(const Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, (Eo *)obj_id);

   ++(obj->user_refcount);
   if (EINA_UNLIKELY(obj->user_refcount == 1))
     _efl_ref(obj);
   else if (EINA_UNLIKELY(obj->ownership_track && obj->user_refcount == 2))
     efl_event_callback_call((Eo *) obj_id, EFL_EVENT_OWNERSHIP_SHARED, NULL);

#ifdef EO_DEBUG
   _eo_log_obj_ref_op(obj, EO_REF_OP_REF);
#endif
   EO_OBJ_DONE(obj_id);
   return (Eo *)obj_id;
}

EO_API void
efl_unref(const Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);

   if (EINA_UNLIKELY((!obj->unref_compensate && obj->user_refcount == 1 && obj->parent) ||
                     (obj->unref_compensate && obj->user_refcount == 2 && obj->parent)))
     {
        if (!obj->allow_parent_unref)
          CRI("Calling efl_unref instead of efl_del or efl_parent_set(NULL). Temporary fallback in place triggered.");
        EO_OBJ_DONE(obj_id);
        efl_del(obj_id);
        return ;
     }

   _efl_ref(obj);

   if (EINA_UNLIKELY((obj->noref_event) && (!obj->unref_compensate) &&
                     ((obj->user_refcount == 1 && !obj->parent) ||
                      (obj->user_refcount == 2 && obj->parent))))
     {
        // We need to report efl_ref_count correctly during EFL_EVENT_NOREF, so fake it
        // by adjusting efl_ref_count while inside efl_unref (This should avoid
        // infinite loop)
        obj->unref_compensate = EINA_TRUE;

        // The noref event should happen before any object in the
        // tree get affected by the change in refcount.
        efl_event_callback_call((Eo *) obj_id, EFL_EVENT_NOREF, NULL);

        obj->unref_compensate = EINA_FALSE;
     }

   --(obj->user_refcount);

#ifdef EO_DEBUG
   _eo_log_obj_ref_op(obj, EO_REF_OP_UNREF);
#endif
   if (EINA_UNLIKELY((obj->user_refcount <= 0)))
     {
        if (obj->user_refcount < 0)
          {
             ERR("Obj:%s@%p. User refcount (%d) < 0. Too many unrefs.",
                 obj->klass->desc->name, obj_id, obj->user_refcount);
             _eo_log_obj_report((Eo_Id)obj_id, EINA_LOG_LEVEL_ERR, __func__, __FILE__, __LINE__);
             EO_OBJ_DONE(obj_id);
             _efl_unref(obj);
             return;
          }
        _efl_unref(obj);
     }
   else if (EINA_UNLIKELY(obj->ownership_track && obj->user_refcount == 1))
     {
        efl_event_callback_call((Eo *) obj_id, EFL_EVENT_OWNERSHIP_UNIQUE, NULL);
     }

   _apply_auto_unref(obj, obj_id);

   _efl_unref(obj);
   EO_OBJ_DONE(obj_id);
}

EO_API int
efl_ref_count(const Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, 0);
   int ref;
   ref = obj->user_refcount - (obj->unref_compensate ? 1 : 0);
   EO_OBJ_DONE(obj_id);
   return ref;
}

EO_API int
___efl_ref2_count(const Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, 0);
   int ref;
   ref = obj->refcount;
   EO_OBJ_DONE(obj_id);
   return ref;
}

EO_API void
___efl_ref2_reset(const Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
   obj->refcount = 0;
   EO_OBJ_DONE(obj_id);
}


EO_API void
efl_del_intercept_set(Eo *obj_id, Efl_Del_Intercept del_intercept_func)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
   EO_OPTIONAL_COW_SET(obj, del_intercept, del_intercept_func);
   EO_OBJ_DONE(obj_id);
}

EO_API Efl_Del_Intercept
efl_del_intercept_get(const Eo *obj_id)
{
   Efl_Del_Intercept func;

   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, NULL);
   func = obj->opt->del_intercept;
   EO_OBJ_DONE(obj_id);
   return func;
}

void
_eo_condtor_done(Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
   if (obj->condtor_done)
     {
        ERR("Object %p is already constructed at this point.", obj);
        EO_OBJ_DONE(obj_id);
        return;
     }
   obj->condtor_done = EINA_TRUE;
   EO_OBJ_DONE(obj_id);
}

static inline void *
_efl_data_scope_safe_get(const _Eo_Object *obj, const _Efl_Class *klass)
{
   if (EINA_LIKELY(klass->desc->data_size > 0))
     {
        return _efl_data_scope_get(obj, klass);
     }

   return NULL;
}

static inline void *
_efl_data_scope_get(const _Eo_Object *obj, const _Efl_Class *klass)
{
   if (EINA_LIKELY(klass->desc->type != EFL_CLASS_TYPE_MIXIN))
     return ((char *) obj) + klass->data_offset;

   if (EINA_UNLIKELY(klass->desc->data_size == 0))
     {
        return NULL;
     }
   else
     {
        Eo_Extension_Data_Offset *doff_itr = obj->klass->extn_data_off;

        if (!doff_itr)
          return NULL;

        while (doff_itr->klass)
          {
             if (doff_itr->klass == klass)
               return ((char *) obj) + doff_itr->offset;
             doff_itr++;
          }
     }

   return NULL;
}

static inline void *
_efl_data_xref_internal(const char *file, int line, _Eo_Object *obj, const _Efl_Class *klass, const _Eo_Object *ref_obj)
{
   void *data = NULL;
   if (klass != NULL)
     {
        data = _efl_data_scope_safe_get(obj, klass);
        if (data == NULL) return NULL;
     }
#ifdef EO_DEBUG
   (obj->datarefcount)++;
   Eo_Xref_Node *xref = calloc(1, sizeof(*xref));
   xref->ref_obj = _eo_obj_id_get(ref_obj);
   xref->data_klass = klass ? klass->desc->name : NULL;
   xref->file = file;
   xref->line = line;

   obj->data_xrefs = eina_inlist_prepend(obj->data_xrefs, EINA_INLIST_GET(xref));
#else
   (void) ref_obj;
   (void) file;
   (void) line;
#endif
   return data;
}

static inline void
_efl_data_xunref_internal(_Eo_Object *obj EINA_UNUSED, void *data EINA_UNUSED, const _Eo_Object *ref_obj EINA_UNUSED)
{
#ifdef EO_DEBUG
   const _Efl_Class *klass = obj->klass;
   Eo_Xref_Node *xref = NULL;
   Eina_Bool in_range = (((char *)data >= (((char *) obj) + _eo_sz)) &&
                         ((char *)data < (((char *) obj) + klass->obj_size)));
   if (!in_range)
     {
        ERR("Data %p is not in the data range of the object %p (%s).",
            data, _eo_obj_id_get(obj), obj->klass->desc->name);
     }
   if (obj->datarefcount == 0)
     {
        ERR("Data for object %p (%s) is already not referenced.",
            _eo_obj_id_get(obj), obj->klass->desc->name);
     }
   else
     {
        (obj->datarefcount)--;
     }
   EINA_INLIST_FOREACH(obj->data_xrefs, xref)
     {
        if (xref->ref_obj == _eo_obj_id_get(ref_obj))
          break;
     }
   if (xref)
     {
        obj->data_xrefs = eina_inlist_remove(obj->data_xrefs, EINA_INLIST_GET(xref));
        eina_freeq_ptr_main_add(xref, free, sizeof(*xref));
     }
   else
     {
        ERR("ref_obj %p (%s) does not reference data %p of obj %p (%s).",
            _eo_obj_id_get(ref_obj), ref_obj->klass->desc->name, data,
            _eo_obj_id_get(obj), obj->klass->desc->name);
     }
#endif
}

EO_API void *
efl_data_scope_get(const Eo *obj_id, const Efl_Class *klass_id)
{
   void *ret = NULL;
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, NULL);
   EO_CLASS_POINTER_GOTO(klass_id, klass, err_klass);

#ifndef EO_DEBUG
   ret = _efl_data_scope_safe_get(obj, klass);
#else
   if (_eo_class_mro_has(obj->klass, klass))
     {
        ret = _efl_data_scope_safe_get(obj, klass);
        if (!ret && (klass->desc->data_size == 0))
          ERR("Tried getting data of class '%s', but it has none.", klass->desc->name);
     }
   else
     {
        ERR("Tried getting data of class '%s' from object of class '%s', but the former is not a direct inheritance of the latter.",
            klass->desc->name, obj->klass->desc->name);
     }
#endif

err_klass:
   EO_OBJ_DONE(obj_id);
   return ret;
}

EO_API void *
efl_data_scope_safe_get(const Eo *obj_id, const Efl_Class *klass_id)
{
   void *ret = NULL;

   if (!obj_id) return NULL;
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, NULL);
   EO_CLASS_POINTER_GOTO(klass_id, klass, err_klass);
   if (obj->destructed) goto err_klass;

   if (_eo_class_mro_has(obj->klass, klass))
     {
        ret = _efl_data_scope_safe_get(obj, klass);

#ifdef EO_DEBUG
        if (!ret && (klass->desc->data_size == 0))
          ERR("Tried getting data of class '%s', but it has none.", klass->desc->name);
#endif
     }

err_klass:
   EO_OBJ_DONE(obj_id);
   return ret;
}

EO_API void *
efl_data_xref_internal(const char *file, int line, const Eo *obj_id, const Efl_Class *klass_id, const Eo *ref_obj_id)
{
   void *ret = NULL;
   _Efl_Class *klass = NULL;
   const char *func_name = __func__;
   EO_OBJ_POINTER_RETURN_VAL_PROXY(obj_id, obj, NULL);
   EO_OBJ_POINTER_PROXY(ref_obj_id, ref_obj);
   if (ref_obj)
     {
        if (klass_id)
          {
             EO_CLASS_POINTER_GOTO_PROXY(klass_id, klass2, err_klass);
             klass = klass2;
#ifdef EO_DEBUG
             // rare to use goto to keep instruction cache cleaner
             if (!_eo_class_mro_has(obj->klass, klass)) goto err_mro;
#endif
          }

        ret = _efl_data_xref_internal(file, line, obj, klass, ref_obj);
#ifdef EO_DEBUG
        // rare to use goto to keep instruction cache cleaner
        if (klass && !ret && (klass->desc->data_size == 0)) goto err_ret;
#endif
err_klass:
        EO_OBJ_DONE(ref_obj_id);
     }
   EO_OBJ_DONE(obj_id);
   return ret;
#ifdef EO_DEBUG
err_ret:
   ERR("Tried getting data of class '%s', but it has none.", klass->desc->name);
   goto err;
err_mro:
   ERR("Tried getting data of class '%s' from object of class '%s', but the former is not a direct inheritance of the latter.", klass->desc->name, obj->klass->desc->name);
err:
   EO_OBJ_DONE(obj_id);
   EO_OBJ_DONE(ref_obj_id);
   return NULL;
#endif
}

EO_API void
efl_data_xunref_internal(const Eo *obj_id, void *data, const Eo *ref_obj_id)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
   EO_OBJ_POINTER(ref_obj_id, ref_obj);
   if (ref_obj)
     {
        _efl_data_xunref_internal(obj, data, ref_obj);
        EO_OBJ_DONE(ref_obj_id);
     }
   EO_OBJ_DONE(obj_id);
}

static void
_eo_table_del_cb(void *in)
{
   Eo_Id_Data *data = in;
   _eo_free_ids_tables(data);
}

/* FIXME: Support other domains and tables, at the moment only the main
 * domain and table.
 * This is used by the gdb debug helper script */
Eo_Id_Data *_eo_gdb_main_domain = NULL;

EO_API Eina_Bool
efl_object_init(void)
{
   const char *log_dom = "eo";
   if (_efl_object_init_count++ > 0)
     return EINA_TRUE;

   eina_init();

#if HAVE_VALGRIND
   _eo_trash_bypass = RUNNING_ON_VALGRIND;
#endif

   _efl_object_main_thread = eina_thread_self();

   _eo_sz = EO_ALIGN_SIZE(sizeof(_Eo_Object));
   _eo_class_sz = EO_ALIGN_SIZE(sizeof(_Efl_Class));

   _eo_classes = NULL;
   _eo_classes_last_id = EO_CLASS_IDS_FIRST - 1;
   _eo_log_dom = eina_log_domain_register(log_dom, EINA_COLOR_LIGHTBLUE);
   if (_eo_log_dom < 0)
     {
        EINA_LOG_ERR("Could not register log domain: %s.", log_dom);
        return EINA_FALSE;
     }

   if (!eina_lock_recursive_new(&_efl_class_creation_lock))
     {
        ERR("Could not init lock.");
        return EINA_FALSE;
     }

   if (!eina_spinlock_new(&_ops_storage_lock))
     {
        ERR("Could not init lock.");
        return EINA_FALSE;
     }

   _eo_log_obj_init();

   eina_magic_string_static_set(EO_EINA_MAGIC, EO_EINA_MAGIC_STR);
   eina_magic_string_static_set(EO_FREED_EINA_MAGIC,
                                EO_FREED_EINA_MAGIC_STR);
   eina_magic_string_static_set(EO_CLASS_EINA_MAGIC,
                                EO_CLASS_EINA_MAGIC_STR);
#ifndef _WIN32
   _ops_storage = eina_hash_pointer_new(NULL);
#else
   _ops_storage = eina_hash_string_superfast_new(NULL);
#endif

   _eo_table_data_shared = _eo_table_data_new(EFL_ID_DOMAIN_SHARED);
   if (!_eo_table_data_shared)
     {
        ERR("Could not allocate shared table data");
        return EINA_FALSE;
     }
   _eo_table_data_shared_data = _eo_table_data_shared->tables[EFL_ID_DOMAIN_SHARED];

   // specially force eoid data to be creanted so we can switch it to domain 0
   Eo_Id_Data *data = _eo_table_data_new(EFL_ID_DOMAIN_MAIN);
   _eo_gdb_main_domain = data;
   if (!data)
     {
        ERR("Could not allocate main table data");
        return EINA_FALSE;
     }
   if (!eina_tls_cb_new(&_eo_table_data, _eo_table_del_cb))
     {
        ERR("Could not allocate TLS for eo domain data");
        _eo_table_del_cb(data);
        return EINA_FALSE;
     }
   eina_tls_set(_eo_table_data, data);
   _efl_object_main_thread = eina_thread_self();

   efl_object_optional_cow =
         eina_cow_add("Efl Object Optional Data", sizeof(Efl_Object_Optional),
                      64, &efl_object_optional_cow_default, EINA_TRUE);

   _efl_add_fallback_init();

   eina_log_timing(_eo_log_dom,
                   EINA_LOG_STATE_STOP,
                   EINA_LOG_STATE_INIT);

   /* bootstrap EFL_CLASS_CLASS */
   (void) EFL_CLASS_CLASS;
   /* bootstrap EFL_OBJECT_CLASS */
   (void) EFL_OBJECT_CLASS;

   return EINA_TRUE;
}

EO_API Eina_Bool
efl_object_shutdown(void)
{
   size_t i;
   _Efl_Class **cls_itr = _eo_classes + _eo_classes_last_id - 1;

   if (--_efl_object_init_count > 0)
     return EINA_TRUE;

#ifdef EO_DEBUG
   {
      Efl_Object *obj;
      Eina_Iterator *objects;
      objects = eo_objects_iterator_new();
      printf("Objects leaked by EO:\n");
      printf("class@pointer - user-refcount internal-refcount\n");
      EINA_ITERATOR_FOREACH(objects, obj)
        {
           printf("%s@%p - %d %d \n", efl_class_name_get(obj), obj, efl_ref_count(obj), ___efl_ref2_count(obj));
        }
      eina_iterator_free(objects);
   }
#endif


   eina_log_timing(_eo_log_dom,
                   EINA_LOG_STATE_START,
                   EINA_LOG_STATE_SHUTDOWN);

   _efl_add_fallback_shutdown();

   for (i = 0 ; i < _eo_classes_last_id ; i++, cls_itr--)
     {
        if (*cls_itr)
          eo_class_free(*cls_itr);
     }

   eina_lock_take(&_efl_class_creation_lock);
   _eo_classes_release();
   eina_lock_release(&_efl_class_creation_lock);

   eina_hash_free(_ops_storage);
   _ops_storage = NULL;

   eina_spinlock_free(&_ops_storage_lock);
   eina_lock_free(&_efl_class_creation_lock);

   _eo_free_ids_tables(_eo_table_data_get());
   eina_tls_free(_eo_table_data);
   if (_eo_table_data_shared)
     {
        _eo_free_ids_tables(_eo_table_data_shared);
        _eo_table_data_shared = NULL;
        _eo_table_data_shared_data = NULL;
     }

   eina_cow_del(efl_object_optional_cow);
   efl_object_optional_cow = NULL;

   _eo_log_obj_shutdown();

   eina_log_domain_unregister(_eo_log_dom);
   _eo_log_dom = -1;

   ++_efl_object_init_generation;

   eina_shutdown();
   return EINA_FALSE;
}


EO_API Efl_Id_Domain
efl_domain_get(void)
{
   Eo_Id_Data *data = _eo_table_data_get();
   return data->local_domain;
}

EO_API Efl_Id_Domain
efl_domain_current_get(void)
{
   Eo_Id_Data *data = _eo_table_data_get();
   return data->domain_stack[data->stack_top];
}

EO_API Eina_Bool
efl_domain_switch(Efl_Id_Domain domain)
{
   Eo_Id_Data *data = _eo_table_data_get();
   Eo_Id_Data *new_data;
   if ((domain < EFL_ID_DOMAIN_MAIN) || (domain > EFL_ID_DOMAIN_THREAD) ||
       (domain == EFL_ID_DOMAIN_SHARED))
     {
        ERR("Invalid domain %i being switched to", domain);
        return EINA_FALSE;
     }
   if ((data) && (data->local_domain == domain))
     return EINA_TRUE;

   new_data = _eo_table_data_new(domain);
   if (!new_data)
     {
        ERR("Could not allocate domain %i table data", domain);
        return EINA_FALSE;
     }
   if (data) _eo_free_ids_tables(data);
   new_data->local_domain = domain;
   new_data->domain_stack[new_data->stack_top] = domain;
   eina_tls_set(_eo_table_data, new_data);
   return EINA_TRUE;
}

static inline Eina_Bool
_efl_domain_push(Eo_Id_Data *data, Efl_Id_Domain domain)
{
   if (data->stack_top >= (sizeof(data->domain_stack) - 1))
     {
        ERR("Failed to push domain %i on stack. Out of stack space at %i",
            domain, data->stack_top);
        return EINA_FALSE;
     }
   data->stack_top++;
   data->domain_stack[data->stack_top] = domain;
   return EINA_TRUE;
}

static inline void
_efl_domain_pop(Eo_Id_Data *data)
{
   if (data->stack_top > 0) data->stack_top--;
}

EO_API Eina_Bool
efl_domain_current_push(Efl_Id_Domain domain)
{
   Eo_Id_Data *data = _eo_table_data_get();
   return _efl_domain_push(data, domain);
}

EO_API void
efl_domain_current_pop(void)
{
   Eo_Id_Data *data = _eo_table_data_get();
   _efl_domain_pop(data);
}

EO_API Eina_Bool
efl_domain_current_set(Efl_Id_Domain domain)
{
   Eo_Id_Data *data = _eo_table_data_get();
   if ((domain < EFL_ID_DOMAIN_MAIN) || (domain > EFL_ID_DOMAIN_THREAD))
     {
        ERR("Invalid domain %i being set", domain);
        return EINA_FALSE;
     }
   data->domain_stack[data->stack_top] = domain;
   return EINA_TRUE;
}

EO_API Efl_Domain_Data *
efl_domain_data_get(void)
{
   Eo_Id_Data *data = _eo_table_data_get();
   return (Efl_Domain_Data *)data;
}

EO_API Efl_Id_Domain
efl_domain_data_adopt(Efl_Domain_Data *data_in)
{
   Eo_Id_Data *data = _eo_table_data_get();
   Eo_Id_Data *data_foreign = (Eo_Id_Data *)data_in;

   if (!data_foreign)
     {
        ERR("Trying to adopt NULL domain data [data=%p in=%p]", data, data_in);
        return EFL_ID_DOMAIN_INVALID;
     }
   if (data_foreign->local_domain == data->local_domain)
     {
        ERR("Trying to adopt EO ID domain %i, is the same as the local %i [data=%p in=%p foreign=%p]",
            data_foreign->local_domain, data->local_domain, data, data_in, data_foreign);
        return EFL_ID_DOMAIN_INVALID;
     }
   if (data->tables[data_foreign->local_domain])
     {
        ERR("Trying to adopt an already adopted domain [data=%p in=%p foreign=%p]", data, data_in, data_foreign);
        return EFL_ID_DOMAIN_INVALID;
     }
   data->tables[data_foreign->local_domain] =
     data_foreign->tables[data_foreign->local_domain];
   _efl_domain_push(data, data_foreign->local_domain);
   return data->domain_stack[data->stack_top];
}

EO_API Eina_Bool
efl_domain_data_return(Efl_Id_Domain domain)
{
   Eo_Id_Data *data = _eo_table_data_get();

   if ((domain < EFL_ID_DOMAIN_MAIN) || (domain > EFL_ID_DOMAIN_THREAD))
     {
        ERR("Invalid domain %i being returned to owning thread", domain);
        return EINA_FALSE;
     }
   if (domain == data->local_domain)
     {
        ERR("Cannot return the local domain %i back to its owner [data=%p]", domain, data);
        return EINA_FALSE;
     }
   data->tables[domain] = NULL;
   _efl_domain_pop(data);
   return EINA_TRUE;
}

EO_API Eina_Bool
efl_compatible(const Eo *obj, const Eo *obj_target)
{
   Efl_Id_Domain domain1 = ((Eo_Id)obj >> SHIFT_DOMAIN) & MASK_DOMAIN;
   Efl_Id_Domain domain2 = ((Eo_Id)obj_target >> SHIFT_DOMAIN) & MASK_DOMAIN;
   if (domain1 == domain2) return EINA_TRUE;
   DBG("Object %p and %p are not compatible. Domain %i and %i do not match",
       obj, obj_target, domain1, domain2);
   return EINA_FALSE;
}

EO_API Eina_Bool
efl_class_override_register(const Efl_Class *klass, const Efl_Class *override)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(klass, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(override, EINA_FALSE);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!efl_isa(override, klass), EINA_FALSE);
   if (!class_overrides)
     class_overrides = eina_hash_pointer_new(NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(class_overrides, EINA_FALSE);

   eina_hash_set(class_overrides, &klass, override);
   return EINA_TRUE;
}

EO_API Eina_Bool
efl_class_override_unregister(const Efl_Class *klass, const Efl_Class *override)
{
   const Efl_Class *set;
   EINA_SAFETY_ON_NULL_RETURN_VAL(klass, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(override, EINA_FALSE);
   if (!class_overrides) return EINA_TRUE;

   set = eina_hash_find(class_overrides, &klass);
   if (set != override) return EINA_FALSE;
   return eina_hash_del_by_key(class_overrides, &klass);
}

EO_API Eina_Bool
efl_destructed_is(const Eo *obj_id)
{
   Eina_Bool is;
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, EINA_FALSE);
   is = obj->destructed;
   EO_OBJ_DONE(obj_id);
   return is;
}

EO_API void
efl_manual_free_set(Eo *obj_id, Eina_Bool manual_free)
{
   EO_OBJ_POINTER_RETURN(obj_id, obj);
   obj->manual_free = manual_free;
   EO_OBJ_DONE(obj_id);
}

EO_API Eina_Bool
efl_manual_free(Eo *obj_id)
{
   EO_OBJ_POINTER_RETURN_VAL(obj_id, obj, EINA_FALSE);

   // rare to use goto to keep instruction cache cleaner
   if (obj->manual_free == EINA_FALSE) goto err_manual_free;
   // rare to use goto to keep instruction cache cleaner
   if (!obj->destructed) goto err_not_destructed;
   _eo_free(obj, EINA_TRUE);
   EO_OBJ_DONE(obj_id);
   return EINA_TRUE;

err_manual_free:
   ERR("Tried to manually free the object %p while the option has not been set; see efl_manual_free_set for more information.", obj);
   goto err;
err_not_destructed:
   ERR("Tried deleting the object %p while still referenced(%d).", obj_id, obj->refcount);
   goto err;
err:
   EO_OBJ_DONE(obj_id);
   return EINA_FALSE;
}

EO_API const char *
efl_debug_name_get(const Eo *obj_id)
{
   const char *override = "";
   const char *name, *clsname;
   Eina_Strbuf *sb;

   if (!obj_id) return "(null)";

   if (_eo_is_a_class(obj_id))
     {
        const char *clstype;

        EO_CLASS_POINTER(obj_id, klass);
        if (!klass || !klass->desc)
          return eina_slstr_printf("Invalid_Class_ID(invalid)@%p", obj_id);

        switch (klass->desc->type)
          {
           case EFL_CLASS_TYPE_REGULAR: clstype = "regular"; break;
           case EFL_CLASS_TYPE_REGULAR_NO_INSTANT: clstype = "abstract"; break;
           case EFL_CLASS_TYPE_INTERFACE: clstype = "interface"; break;
           case EFL_CLASS_TYPE_MIXIN: clstype = "mixin"; break;
           default: clstype = "invalid"; break;
          }

        return eina_slstr_printf("%s(%s)@%p", klass->desc->name, clstype, obj_id);
     }

   EO_OBJ_POINTER(obj_id, obj);
   if (!obj) return eina_slstr_printf("Invalid_Object_ID@%p", obj_id);

   sb = eina_strbuf_new();
   name = efl_name_get(obj_id);
   clsname = obj->klass->desc->name;
   if (_obj_is_override(obj)) override = "(override)";

   if (name)
     eina_strbuf_append_printf(sb, "%s%s@%p[%d]:'%s'", clsname, override, obj_id, (int) obj->refcount, name);
   else
     eina_strbuf_append_printf(sb, "%s%s@%p[%d]", clsname, override, obj_id, (int) obj->refcount);

   if (!obj->cur_klass)
     {
        efl_debug_name_override((Eo *) obj_id, sb);
     }
   else
     {
        if (obj->super)
          efl_debug_name_override(efl_super(obj_id, (Efl_Class *) obj->cur_klass->header.id), sb);
        else
          efl_debug_name_override(efl_cast(obj_id, (Efl_Class *) obj->cur_klass->header.id), sb);
        obj->super = EINA_FALSE;
        obj->cur_klass = NULL;
     }

   EO_OBJ_DONE(obj_id);
   return eina_slstr_strbuf_new(sb);
}

EO_API int
efl_callbacks_cmp(const Efl_Callback_Array_Item *a, const Efl_Callback_Array_Item *b)
{
   if (a->desc == b->desc) return 0;
   else if (a->desc > b->desc) return 1;
   else return -1;
}


#ifdef EO_DEBUG
/* NOTE: cannot use ecore_time_get()! */
static inline double
_eo_log_time_now(void)
{
#ifdef _WIN32
   return evil_time_get();
#elif defined(__APPLE__) && defined(__MACH__)
   static double clk_conv = -1.0;

   if (EINA_UNLIKELY(clk_conv < 0))
     {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info(&info);
        if (err == 0)
          clk_conv = 1e-9 * (double)info.numer / (double)info.denom;
        else
          clk_conv = 1e-9;
     }

   return clk_conv * mach_absolute_time();
#else
#if defined (HAVE_CLOCK_GETTIME)
   struct timespec t;
   static int clk_id = -1;

   if (EINA_UNLIKELY(clk_id == -2)) goto try_gettimeofday;
   if (EINA_UNLIKELY(clk_id == -1))
     {
     retry_clk_id:
        clk_id = CLOCK_MONOTONIC;
        if (EINA_UNLIKELY(clock_gettime(clk_id, &t)))
          {
             WRN("CLOCK_MONOTONIC failed!");
             clk_id = CLOCK_REALTIME;
             if (EINA_UNLIKELY(clock_gettime(clk_id, &t)))
               {
                  WRN("CLOCK_REALTIME failed!");
                  clk_id = -2;
                  goto try_gettimeofday;
               }
          }
     }
   else
     {
        if (EINA_UNLIKELY(clock_gettime(clk_id, &t)))
          {
             WRN("clk_id=%d previously ok, now failed... retry", clk_id);
             goto retry_clk_id;
          }
     }
   return (double)t.tv_sec + (((double)t.tv_nsec) / 1000000000.0);

 try_gettimeofday:
#endif
   {
      struct timeval timev;

      gettimeofday(&timev, NULL);
      return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
   }
#endif
}

#ifdef HAVE_BACKTRACE
typedef struct _Eo_Log_Obj_Entry {
   Eo_Id id;
   const _Eo_Object *obj;
   const _Efl_Class *klass;
   double timestamp;
   Eo_Ref_Op ref_op;
   unsigned bt_size;
   unsigned bt_hits;
   uintptr_t bt_hash;
   void *bt[];
} Eo_Log_Obj_Entry;

static void
_eo_log_obj_find(const Eo_Id id, const Eo_Log_Obj_Entry **added, int *added_idx, const Eo_Log_Obj_Entry **deleted)
{
   const Eo_Log_Obj_Entry *entry;
   Eina_Array_Iterator it;
   unsigned int idx;

   *added_idx = -1;
   *added = NULL;
   *deleted = NULL;

   eina_spinlock_take(&_eo_log_objs_lock);
   EINA_ARRAY_ITER_NEXT(&_eo_log_objs, idx, entry, it)
     {
        if (EINA_UNLIKELY(id == entry->id))
          {
             if (entry->ref_op == EO_REF_OP_FREE)
               *deleted = entry;
             else if (entry->ref_op == EO_REF_OP_NEW)
               {
                  *added_idx = idx;
                  *added = entry;
                  *deleted = NULL; /* forget previous add, if any */
               }
          }
     }
   eina_spinlock_release(&_eo_log_objs_lock);
}

static void
_eo_log_obj_entry_show(const Eo_Log_Obj_Entry *entry, int log_level, const char *func_name, const char *file, int line, double now)
{
   unsigned i;

   eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                  "%s obj_id=%p obj=%p, class=%p (%s) [%0.4fs, %0.4fs ago] [%d hits]:",
                  _eo_ref_op_str[entry->ref_op],
                  (void *)entry->id,
                  entry->obj,
                  entry->klass,
                  entry->klass->desc->name,
                  entry->timestamp - _eo_log_time_start, now - entry->timestamp,
                  entry->bt_hits);

   // Skip EO_API and _eo_log_obj_ref_op()
   for (i = 2; i < entry->bt_size; i++)
     {
#ifdef HAVE_DLADDR
        Dl_info info;

        if (dladdr(entry->bt[i], &info))
          {
             if (info.dli_sname)
               {
                  eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                                 "   0x%016llx: %s+%llu (in %s 0x%llx)",
                                 (unsigned long long)(uintptr_t)entry->bt[i],
                                 info.dli_sname,
                                 (unsigned long long)(uintptr_t)((char *)entry->bt[i] - (char *)info.dli_saddr),
                                 info.dli_fname ? info.dli_fname : "??",
                                 (unsigned long long)(uintptr_t)info.dli_fbase);
                  continue;
               }
             else if (info.dli_fname)
               {
                  const char *fname;

#ifdef _WIN32
                  fname = strrchr(info.dli_fname, '\\');
#else
                  fname = strrchr(info.dli_fname, '/');
#endif
                  if (!fname) fname = info.dli_fname;
                  else fname++;

                  eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                                 "   0x%016llx: %s+%llu (in %s 0x%llx)",
                                 (unsigned long long)(uintptr_t)entry->bt[i],
                                 fname,
                                 (unsigned long long)(uintptr_t)((char *)entry->bt[i] - (char *)info.dli_fbase),
                                 info.dli_fname,
                                 (unsigned long long)(uintptr_t)info.dli_fbase);
                  continue;
               }
          }
#endif

        eina_log_print(_eo_log_objs_dom, log_level, func_name, file, line,
                       "   0x%016llx", (unsigned long long)(uintptr_t)entry->bt[i]);
     }
}
#endif

#ifdef HAVE_BACKTRACE
static uintptr_t
_eo_log_obj_backtrace_hash(Eo_Log_Obj_Entry *entry)
{
   if (!entry->bt_hash)
     {
        entry->bt_hash = (uintptr_t) 4294967291;
        for (unsigned k = 0; k < entry->bt_size; k++)
          entry->bt_hash ^= ((uintptr_t) (entry->bt[k]));
     }

   return entry->bt_hash;
}

static Eina_Bool
_eo_log_obj_entry_is_new_backtrace(const Eina_List *entries, Eo_Log_Obj_Entry *entry)
{
   Eina_Bool ret = EINA_TRUE;
   Eo_Log_Obj_Entry *other;
   const Eina_List *li;
   uintptr_t hash;

   hash = _eo_log_obj_backtrace_hash(entry);
   EINA_LIST_FOREACH(entries, li, other)
     if (_eo_log_obj_backtrace_hash(other) == hash)
       {
          other->bt_hits++;
          ret = EINA_FALSE;
       }

   return ret;
}

static Eina_List *
_eo_log_obj_find_all(const Eo_Id id, int start_idx)
{
   Eo_Log_Obj_Entry *entry;
   Eina_List *entries = NULL;
   unsigned int idx;

   eina_spinlock_take(&_eo_log_objs_lock);
   for (idx = start_idx + 1; idx < eina_array_count(&_eo_log_objs); idx++)
     {
        entry = eina_array_data_get(&_eo_log_objs, idx);
        if (entry->id != id) continue;
        if (entry->ref_op > _eo_log_objs_level) continue;
        if (entry->ref_op == EO_REF_OP_FREE) break;
        if (_eo_log_obj_entry_is_new_backtrace(entries, entry))
          entries = eina_list_append(entries, entry);
     }
   eina_spinlock_release(&_eo_log_objs_lock);
   return entries;
}
#endif

inline void
_eo_log_obj_report(const Eo_Id id, int log_level, const char *func_name, const char *file, int line)
{
#ifdef HAVE_BACKTRACE
   const Eo_Log_Obj_Entry *added, *deleted;
   Eo_Log_Obj_Entry *current = NULL;
   int added_idx = -1;
   double now;

   if (EINA_LIKELY(!_eo_log_objs_level)) return;

   _eo_log_obj_find(id, &added, &added_idx, &deleted);

   if ((!added) && (!deleted))
     {
        if ((!_eo_log_objs_debug.len) && (!_eo_log_objs_no_debug.len))
          {
             eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                            "obj_id=%p was neither created or deleted.", (void *)id);
          }
        else if ((_eo_log_objs_debug.len) && (_eo_log_objs_no_debug.len))
          {
             eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                            "obj_id=%p was neither created or deleted (EO_LIFECYCLE_DEBUG='%s', EO_LIFECYCLE_NO_DEBUG='%s').",
                            (void *)id, getenv("EO_LIFECYCLE_DEBUG"), getenv("EO_LIFECYCLE_NO_DEBUG"));
          }
        else if (_eo_log_objs_debug.len)
          {
             eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                            "obj_id=%p was neither created or deleted (EO_LIFECYCLE_DEBUG='%s').",
                            (void *)id, getenv("EO_LIFECYCLE_DEBUG"));
          }
        else
          {
             eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                            "obj_id=%p was neither created or deleted (EO_LIFECYCLE_NO_DEBUG='%s').",
                            (void *)id, getenv("EO_LIFECYCLE_NO_DEBUG"));
          }
        return;
     }

   now = _eo_log_time_now();

#ifdef HAVE_BACKTRACE
   if ((_eo_log_objs_backtrace >= 0) && deleted)
     {
        void **bt = NULL;
        int size = 0;

        if (_eo_log_objs_backtrace > 0)
          {
             bt = alloca(sizeof(void *) * (_eo_log_objs_backtrace + 2));
             size = backtrace(bt, (_eo_log_objs_backtrace + 2));
             if (EINA_UNLIKELY(size < 1))
               {
                  bt = NULL;
                  size = 0;
               }
          }

        current = calloc(1, sizeof(Eo_Log_Obj_Entry) + size * sizeof(void *));
        if (EINA_UNLIKELY(!current)) return;

        current->id = id;
        current->timestamp = now;
        current->obj = deleted->obj;
        current->klass = deleted->klass;
        current->ref_op = EO_REF_OP_NONE;
        current->bt_size = size;
        current->bt_hash = 0;
        current->bt_hits = 1;
        if (bt && size)
          memcpy(current->bt, bt, size * sizeof(void *));
     }
#endif

   if (added)
     {
        _eo_log_obj_entry_show(added, log_level, func_name, file, line, now);

        if (_eo_log_objs_level > EO_REF_OP_FREE)
          {
             Eina_List *entries = _eo_log_obj_find_all(id, added_idx);
             const Eo_Log_Obj_Entry *entry;

             EINA_LIST_FREE(entries, entry)
               _eo_log_obj_entry_show(entry, log_level, func_name, file, line, now);
          }
     }

   if (deleted)
     {
        _eo_log_obj_entry_show(deleted, log_level, func_name, file, line, now);
        eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                       "obj_id=%p was already deleted %0.4f seconds ago!",
                       (void *)id, now - deleted->timestamp);
     }

   if (current)
     {
        eina_log_print(_eo_log_objs_dom, log_level, file, func_name, line,
                       "obj_id=%p current use from:", (void *)id);
        _eo_log_obj_entry_show(current, log_level, func_name, file, line, now);
        free(current);
     }

#else
   (void)id;
   (void)log_level;
   (void)func_name;
   (void)file;
   (void)line;
#endif
}

#ifdef HAVE_BACKTRACE
static Eo_Log_Obj_Entry *
_eo_log_obj_entry_ref_op(const _Eo_Object *obj, Eo_Ref_Op refop, unsigned size, void *const *bt)
{
   Eo_Log_Obj_Entry *entry;
   Eina_Bool ret;

   entry = calloc(1, sizeof(Eo_Log_Obj_Entry) + size * sizeof(void *));
   if (EINA_UNLIKELY(!entry)) return NULL;

   entry->id = (Eo_Id)_eo_obj_id_get(obj);
   entry->timestamp = _eo_log_time_now();
   entry->obj = obj;
   entry->klass = obj->klass;
   entry->ref_op = refop;
   entry->bt_size = size;
   entry->bt_hash = 0;
   entry->bt_hits = 1;
   if (size && bt)
     memcpy(entry->bt, bt, size * sizeof(void *));

   eina_spinlock_take(&_eo_log_objs_lock);
   ret = eina_array_push(&_eo_log_objs, entry);
   eina_spinlock_release(&_eo_log_objs_lock);
   if (!ret)
     {
        free(entry);
        return NULL;
     }

   return entry;
}

static inline void
_eo_log_obj_entry_free(Eo_Log_Obj_Entry *entry)
{
   free(entry);
}
#endif

static int
_eo_class_name_slice_cmp(const void *pa, const void *pb)
{
   const Eina_Slice *a = pa;
   const Eina_Slice *b = pb;

   if (a->len < b->len) return -1;
   if (a->len > b->len) return 1;
   return memcmp(a->mem, b->mem, a->len);
}

static Eina_Bool
_eo_log_obj_desired(const _Eo_Object *obj)
{
   Eina_Slice cls_name;

   if (EINA_LIKELY((_eo_log_objs_debug.len == 0) &&
                   (_eo_log_objs_no_debug.len == 0)))
     return EINA_TRUE;

   cls_name.mem = obj->klass->desc->name;
   cls_name.len = strlen(cls_name.mem);

   if (_eo_log_objs_no_debug.len)
     {
        if (eina_inarray_search_sorted(&_eo_log_objs_no_debug, &cls_name, _eo_class_name_slice_cmp) >= 0)
          return EINA_FALSE;
     }

   if (!_eo_log_objs_debug.len)
     return EINA_TRUE;

   if (eina_inarray_search_sorted(&_eo_log_objs_debug, &cls_name, _eo_class_name_slice_cmp) >= 0)
     return EINA_TRUE;

   return EINA_FALSE;
}

static void
_eo_log_obj_ref_op(const _Eo_Object *obj, Eo_Ref_Op ref_op)
{
   if (EINA_LIKELY(_eo_log_objs_level < ref_op)) return;
   if (EINA_LIKELY(!_eo_log_obj_desired(obj))) return;

#ifdef HAVE_BACKTRACE
   if (_eo_log_objs_backtrace >= 0)
     {
        void **bt = NULL;
        int size = 0;

        if (_eo_log_objs_backtrace > 0)
          {
             bt = alloca(sizeof(void *) * (_eo_log_objs_backtrace + 2));
             size = backtrace(bt, (_eo_log_objs_backtrace + 2));
             if (EINA_UNLIKELY(size < 1)) return;
          }

        _eo_log_obj_entry_ref_op(obj, ref_op, size, bt);
     }
#endif

   EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                    "%s obj_id=%p class=%p (%s) [%0.4f]",
                    _eo_ref_op_str[ref_op], _eo_obj_id_get(obj),
                    obj->klass, obj->klass->desc->name,
                    _eo_log_time_now() - _eo_log_time_start);
}

static inline void
_eo_log_obj_init(void)
{
   const char *s;

   _eo_log_objs_dom = eina_log_domain_register("eo_lifecycle", EINA_COLOR_BLUE);
   _eo_log_time_start = _eo_log_time_now();

#ifdef HAVE_BACKTRACE
   eina_array_step_set(&_eo_log_objs, sizeof(Eina_Array), 4096);
   eina_spinlock_new(&_eo_log_objs_lock);
#endif
   eina_inarray_step_set(&_eo_log_objs_debug, sizeof(Eina_Inarray), sizeof(Eina_Slice), 0);
   eina_inarray_step_set(&_eo_log_objs_no_debug, sizeof(Eina_Inarray), sizeof(Eina_Slice), 0);

   s = getenv("EO_LIFECYCLE_BACKTRACE");
   if (s && *s)
     {
        _eo_log_objs_backtrace = atoi(s);
        if (_eo_log_objs_backtrace > EO_LOG_OBJS_BACKTRACE_MAX)
          _eo_log_objs_backtrace = EO_LOG_OBJS_BACKTRACE_MAX;
        else if (_eo_log_objs_backtrace < 0)
          _eo_log_objs_backtrace = 0;
     }
   else
     _eo_log_objs_backtrace = EO_LOG_OBJS_BACKTRACE_DEFAULT;

   s = getenv("EO_LIFECYCLE_DEBUG");
   if ((s) && (s[0] != '\0'))
     {
        char *es;
        int lvl = (int)strtol(s, &es, 10);
        _eo_log_objs_level = EO_REF_OP_FREE;
        if ((es != s) && (*es == ':'))
          {
             if (lvl >= 3)
               {
                  _eo_log_objs_level = EO_REF_OP_REUSE;
                  EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                                   "will log new, free, ref, unref and reuse");
               }
             else if (lvl == 2)
               {
                  _eo_log_objs_level = EO_REF_OP_UNREF;
                  EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                                   "will log new, free, ref and unref");
               }
             s = es + 1;
          }

        if ((strcmp(s, "*") == 0) || (strcmp(s, "1") == 0))
          {
             EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                              "will log all object allocation and free");
          }
        else
          {
             Eina_Slice slice;
             const Eina_Slice *itr;
             do
               {
                  char *p = strchr(s, ',');
                  slice.mem = s;
                  if (p)
                    {
                       slice.len = p - s;
                       s = p + 1;
                    }
                  else
                    {
                       slice.len = strlen(s);
                       s = NULL;
                    }
                  eina_inarray_push(&_eo_log_objs_debug, &slice);
               }
             while (s);
             eina_inarray_sort(&_eo_log_objs_debug, _eo_class_name_slice_cmp);

             EINA_INARRAY_FOREACH(&_eo_log_objs_debug, itr)
               {
                  EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                                "will log class '" EINA_SLICE_STR_FMT "'",
                                   EINA_SLICE_STR_PRINT(*itr));
               }
          }
#ifndef HAVE_BACKTRACE
        WRN("EO_LIFECYCLE_DEBUG='%s' but your system has no backtrace()!", s);
#endif
     }

   if (EINA_LIKELY(!_eo_log_objs_level)) return;

   DBG("logging object allocation and free, use EINA_LOG_LEVELS=eo_lifecycle:4");

   s = getenv("EO_LIFECYCLE_NO_DEBUG");
   if ((s) && (s[0] != '\0'))
     {
        if ((strcmp(s, "*") == 0) || (strcmp(s, "1") == 0))
          {
             EINA_LOG_DOM_ERR(_eo_log_objs_dom,
                              "expected class names to not log allocation and free, got '%s'", s);
          }
        else
          {
             Eina_Slice slice;
             const Eina_Slice *itr;
             do
               {
                  char *p = strchr(s, ',');
                  slice.mem = s;
                  if (p)
                    {
                       slice.len = p - s;
                       s = p + 1;
                    }
                  else
                    {
                       slice.len = strlen(s);
                       s = NULL;
                    }
                  eina_inarray_push(&_eo_log_objs_no_debug, &slice);
               }
             while (s);
             eina_inarray_sort(&_eo_log_objs_no_debug, _eo_class_name_slice_cmp);

             EINA_INARRAY_FOREACH(&_eo_log_objs_no_debug, itr)
               {
                  EINA_LOG_DOM_DBG(_eo_log_objs_dom,
                                   "will NOT log class '" EINA_SLICE_STR_FMT "'",
                                   EINA_SLICE_STR_PRINT(*itr));
               }
          }
     }
}

static inline void
_eo_log_obj_shutdown(void)
{
#ifdef HAVE_BACKTRACE
   Eo_Log_Obj_Entry *entry;
   Eina_Array_Iterator it;
   unsigned int idx;

   eina_spinlock_take(&_eo_log_objs_lock);
   if (eina_log_domain_level_check(_eo_log_objs_dom, EINA_LOG_LEVEL_INFO))
     {
        void * const *itr = _eo_log_objs.data;
        void * const *itr_end = itr + _eo_log_objs.count;
        double now = _eo_log_time_now();
        size_t leaks = 0;

        for (; itr < itr_end; itr++)
          {
             void * const *cur;
             entry = *itr;
             if (entry->ref_op == EO_REF_OP_FREE) continue;
             for (cur = itr + 1; cur < itr_end; cur++)
               {
                  const Eo_Log_Obj_Entry *cur_entry = *cur;
                  if (EINA_UNLIKELY((cur_entry->id == entry->id) && (cur_entry->ref_op == EO_REF_OP_FREE)))
                    break;
               }
             if (EINA_UNLIKELY(cur == itr_end))
               {
                  EINA_LOG_DOM_INFO(_eo_log_objs_dom,
                                    "leaking obj_id=%p obj=%p class=%p (%s) [%0.4fs, %0.4f ago]",
                                    (void *)entry->id,
                                    entry->obj,
                                    entry->klass,
                                    entry->klass->desc->name,
                                    entry->timestamp - _eo_log_time_start, now - entry->timestamp);
                  _eo_log_obj_entry_show(entry, EINA_LOG_LEVEL_DBG, __func__, __FILE__, __LINE__, now);
                  leaks++;
               }
          }
        if (leaks)
          EINA_LOG_DOM_WARN(_eo_log_objs_dom, "Leaked %zd objects! Check details with EINA_LOG_LEVELS=eo_lifecycle:4", leaks);
        else
          EINA_LOG_DOM_INFO(_eo_log_objs_dom, "No leaked objects!");
     }
   EINA_ARRAY_ITER_NEXT(&_eo_log_objs, idx, entry, it)
     _eo_log_obj_entry_free(entry);
   eina_array_flush(&_eo_log_objs);
   eina_spinlock_release(&_eo_log_objs_lock);
   eina_spinlock_free(&_eo_log_objs_lock);
#endif

   eina_inarray_flush(&_eo_log_objs_debug);
   eina_inarray_flush(&_eo_log_objs_no_debug);
}
#endif

typedef struct
{
   Eina_Iterator iterator;
   unsigned int cur_kl_id;
} _Eo_Classes_Iterator;

static Eina_Bool
_eo_classes_iterator_next(Eina_Iterator *it, void **data)
{
   _Eo_Classes_Iterator *eo_it = (_Eo_Classes_Iterator *)it;

   if (eo_it->cur_kl_id == _eo_classes_last_id) return EINA_FALSE;
   *data = _eo_class_id_get(_eo_classes[eo_it->cur_kl_id]);
   eo_it->cur_kl_id++;
   return EINA_TRUE;
}

static void
_eo_classes_iterator_free(Eina_Iterator *it)
{
   EINA_MAGIC_SET(it, EINA_MAGIC_NONE);
   free(it);
}

EO_API Eina_Iterator *
eo_classes_iterator_new(void)
{
   _Eo_Classes_Iterator *it;

   it = calloc(1, sizeof (*it));
   if (!it) return NULL;

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = _eo_classes_iterator_next;
   it->iterator.free = _eo_classes_iterator_free;
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   return (Eina_Iterator *)it;
}

typedef struct
{
   Eina_Iterator iterator;
   Eo_Id_Table_Data *tdata;
   Table_Index mid_table_id;
   Table_Index table_id;
   Table_Index entry_id;
} _Eo_Objects_Iterator;

static Eina_Bool
_eo_objects_iterator_next(Eina_Iterator *it, void **data)
{
   Table_Index mid_table_id, table_id, entry_id;
   Eo_Id_Table_Data *tdata;
   _Eo_Objects_Iterator *eo_it = (_Eo_Objects_Iterator *)it;
   if (!eo_it->tdata) return EINA_FALSE;

   tdata = eo_it->tdata;
   mid_table_id = eo_it->mid_table_id;
   table_id = eo_it->table_id;
   entry_id = eo_it->entry_id;
   while (mid_table_id < MAX_MID_TABLE_ID)
     {
        if (tdata->eo_ids_tables[mid_table_id])
          {
             while (table_id < MAX_TABLE_ID)
               {
                  if (TABLE_FROM_IDS)
                    {
                       while (entry_id < MAX_ENTRY_ID)
                         {
                            _Eo_Id_Entry *entry = &(TABLE_FROM_IDS->entries[entry_id]);
                            if (entry->active)
                              {
                                 Eo *obj = _eo_header_id_get((Eo_Header *) entry->ptr);
                                 *data = obj;
                                 eo_it->mid_table_id = mid_table_id;
                                 eo_it->table_id = table_id;
                                 eo_it->entry_id = entry_id + 1;
                                 return EINA_TRUE;
                              }
                            entry_id++;
                         }
                       entry_id = 0;
                    }
                  table_id++;
               }
             table_id = 0;
          }
        mid_table_id++;
     }
   return EINA_FALSE;
}

static void
_eo_objects_iterator_free(Eina_Iterator *it)
{
   EINA_MAGIC_SET(it, EINA_MAGIC_NONE);
   free(it);
}

EO_API Eina_Iterator *
eo_objects_iterator_new(void)
{
   _Eo_Objects_Iterator *it;
   Eo_Id_Table_Data *tdata = _eo_table_data_table_get(_eo_table_data_get(), EFL_ID_DOMAIN_MAIN);

   if (!tdata) return NULL;

   it = calloc(1, sizeof (*it));
   if (!it) return NULL;

   it->tdata = tdata;
   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = _eo_objects_iterator_next;
   it->iterator.free = _eo_objects_iterator_free;
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   return (Eina_Iterator *)it;
}

static Eina_Bool
_eo_value_setup(const Eina_Value_Type *type EINA_UNUSED, void *mem)
{
   Eo **tmem = mem;
   *tmem = NULL;
   return EINA_TRUE;
}

static Eina_Bool
_eo_value_flush(const Eina_Value_Type *type EINA_UNUSED, void *mem)
{
   Eo **tmem = mem;
   if (*tmem)
     {
        efl_unref(*tmem);
        *tmem = NULL;
     }
   return EINA_TRUE;
}

static Eina_Bool
_eo_value_vset(const Eina_Value_Type *type EINA_UNUSED, void *mem, va_list args)
{
   Eo **dst = mem;
   Eo *src = va_arg(args, Eo *);
   efl_replace(dst, src);
   return EINA_TRUE;
}

static Eina_Bool
_eo_value_pset(const Eina_Value_Type *type EINA_UNUSED,
              void *mem, const void *ptr)
{
   Eo **dst = mem;
   Eo * const *src = ptr;
   efl_replace(dst, *src);
   return EINA_TRUE;
}

static Eina_Bool
_eo_value_pget(const Eina_Value_Type *type EINA_UNUSED,
              const void *mem, void *ptr)
{
   Eo * const *src = mem;
   Eo **dst = ptr;
   *dst = *src;
   return EINA_TRUE;
}

static Eina_Bool
_eo_value_convert_to(const Eina_Value_Type *type EINA_UNUSED, const Eina_Value_Type *convert, const void *type_mem, void *convert_mem)
{
   Eo * const *eo = type_mem;

   if (convert == EINA_VALUE_TYPE_STRINGSHARE ||
       convert == EINA_VALUE_TYPE_STRING)
     {
        const char *other_mem;
        char buf[256];
        snprintf(buf, sizeof(buf), "Object id: %p, class: %s, name: %s",
                 *eo, efl_class_name_get(efl_class_get(*eo)),
                 efl_debug_name_get(*eo));
        other_mem = buf;
        return eina_value_type_pset(convert, convert_mem, &other_mem);
     }
   return EINA_FALSE;
}

static Eina_Bool
_eo_value_copy(const Eina_Value_Type *type EINA_UNUSED, const void *mem, void *ptr)
{
   Eo * const *src = mem;
   Eo **dst = ptr;

   if (!src || !dst) return EINA_FALSE;
   *dst = efl_ref(*src);

   return EINA_TRUE;
}

static const Eina_Value_Type _EINA_VALUE_TYPE_OBJECT = {
  .version = EINA_VALUE_TYPE_VERSION,
  .value_size = sizeof(Eo *),
  .name = "Efl_Object",
  .setup = _eo_value_setup,
  .flush = _eo_value_flush,
  .copy = _eo_value_copy,
  .compare = NULL,
  .convert_to = _eo_value_convert_to,
  .convert_from = NULL,
  .vset = _eo_value_vset,
  .pset = _eo_value_pset,
  .pget = _eo_value_pget
};

EO_API const Eina_Value_Type *EINA_VALUE_TYPE_OBJECT = &_EINA_VALUE_TYPE_OBJECT;

static const Efl_Object_Property_Reflection*
_efl_class_reflection_find(const _Efl_Class *klass, const char *property_name)
{
   const _Efl_Class **klass_iter = klass->extensions;
   const Efl_Object_Property_Reflection_Ops *ref_ops = klass->reflection;
   unsigned int i;

   for (i = 0; ref_ops && i < ref_ops->count; ++i)
     {
        if (eina_streq(property_name, ref_ops->table[i].property_name))
          return &ref_ops->table[i];
     }

   if (klass->parent)
     {
        const Efl_Object_Property_Reflection *ref;

        ref = _efl_class_reflection_find(klass->parent, property_name);
        if (ref) return ref;
     }

   for (; *klass_iter; klass_iter++)
     {
        const Efl_Object_Property_Reflection *ref;

        ref = _efl_class_reflection_find(*klass_iter, property_name);
        if (ref) return ref;
     }

   return NULL;
}

EO_API Eina_Error
efl_property_reflection_set(Eo *obj_id, const char *property_name, Eina_Value value)
{
   Eina_Error r = EINA_ERROR_NOT_IMPLEMENTED;
   Eina_Bool freed = EINA_FALSE;

   EO_OBJ_POINTER_GOTO(obj_id, obj, end);
   const Efl_Object_Property_Reflection *reflection = _efl_class_reflection_find(obj->klass, property_name);

   if (reflection && reflection->set)
     {
        r = reflection->set(obj_id, value);
        freed = EINA_TRUE;
     }

 end:
   if (!freed) eina_value_flush(&value);
   EO_OBJ_DONE(obj_id);
   return r;
}

EO_API Eina_Value
efl_property_reflection_get(const Eo *obj_id, const char *property_name)
{
   Eina_Value r = eina_value_error_init(EINA_ERROR_NOT_IMPLEMENTED);

   EO_OBJ_POINTER_GOTO(obj_id, obj, end);
   const Efl_Object_Property_Reflection *reflection = _efl_class_reflection_find(obj->klass, property_name);

   if (reflection && reflection->get)
     r = reflection->get(obj_id);

 end:
   EO_OBJ_DONE(obj_id);

   return r;
}

EO_API Eina_Bool
efl_property_reflection_exist(Eo *obj_id, const char *property_name)
{
   Eina_Bool r = EINA_FALSE;
   EO_OBJ_POINTER_GOTO(obj_id, obj, end);
   const Efl_Object_Property_Reflection *reflection = _efl_class_reflection_find(obj->klass, property_name);

   if (reflection) r = EINA_TRUE;
 end:
   EO_OBJ_DONE(obj_id);
   return r;
}

EO_API Efl_Class_Type
efl_class_type_get(const Efl_Class *klass_id)
{
   EO_CLASS_POINTER_RETURN_VAL(klass_id, klass, EFL_CLASS_TYPE_INVALID);

   return klass->desc->type;
}


EO_API Eina_Bool
efl_ownable_get(const Eo *obj)
{
   int ref = efl_ref_count(obj);

   if (efl_parent_get(obj))
     ref --;

   if (ref <= 0)
     ERR("There is no free reference to pass this object. Please check that this object is really owned by you.");
   return (ref > 0);
}
