#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include "Eo.h"
#include "eo_suite.h"
#include "eo_test_class_simple.h"

START_TEST(eo_simple)
{
   eo_init();
   Eo *obj = eo_add(EO_BASE_CLASS, NULL);
   fail_if(obj);

   obj = eo_add(SIMPLE_CLASS, NULL);
   fail_if(!obj);
   eo_do(obj, eo_constructor());
   eo_do(obj, eo_destructor());
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

static int _eo_signals_cb_curent = 0;
static int _eo_signals_cb_flag = 0;

static Eina_Bool
_eo_signals_a_changed_cb(void *_data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   int data = (intptr_t) _data;
   _eo_signals_cb_curent++;
   ck_assert_int_eq(data, _eo_signals_cb_curent);
   _eo_signals_cb_flag |= 0x1;
   return EO_CALLBACK_CONTINUE;
}

static Eina_Bool
_eo_signals_a_changed_cb2(void *_data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   _eo_signals_cb_flag |= 0x2;
   return EO_CALLBACK_STOP;
}

static Eina_Bool
_eo_signals_a_changed_never(void *_data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   /* This one should never be called. */
   fail_if(1);
   return EO_CALLBACK_CONTINUE;
}

static Eina_Bool
_eo_signals_eo_del_cb(void *_data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   _eo_signals_cb_flag |= 0x4;
   return EO_CALLBACK_CONTINUE;
}

Eina_Bool
_eo_signals_cb_added_deled(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   const Eo_Callback_Array_Item *callback_array = event_info;

   fail_if((callback_array != data) &&
         (callback_array->func != _eo_signals_cb_added_deled));

   return EO_CALLBACK_CONTINUE;
}

START_TEST(eo_signals)
{
   eo_init();
   static const Eo_Callback_Array_Item callbacks[] = {
          { EV_A_CHANGED, _eo_signals_a_changed_cb },
          { EV_A_CHANGED, _eo_signals_a_changed_cb2 },
          { EV_A_CHANGED, _eo_signals_a_changed_never },
          { EO_EV_DEL, _eo_signals_eo_del_cb },
          { NULL, NULL }
   };
   Eo *obj = eo_add(SIMPLE_CLASS, NULL);

   eo_do(obj, eo_event_callback_add(EO_EV_CALLBACK_ADD, _eo_signals_cb_added_deled, callbacks));
   eo_do(obj, eo_event_callback_add(EO_EV_CALLBACK_DEL, _eo_signals_cb_added_deled, callbacks));
   eo_do(obj, eo_event_callback_array_priority_add(callbacks, -100, (void *) 1));
   eo_do(obj, eo_event_callback_array_add(callbacks, (void *) 3));
   eo_do(obj, eo_event_callback_array_priority_add(callbacks, -50, (void *) 2));
   fail_if(!eo_do(obj, simple_a_set(1)));
   ck_assert_int_eq(_eo_signals_cb_flag, 0x3);

   eo_do(obj, eo_event_callback_array_del(callbacks, (void *) 1));
   eo_do(obj, eo_event_callback_array_del(callbacks, (void *) 2));
   eo_do(obj, eo_event_callback_array_del(callbacks, (void *) 3));
   /* Try to delete something that doesn't exist. */
   eo_do(obj, eo_event_callback_array_del(callbacks, (void *) 4));
   _eo_signals_cb_flag = 0;
   fail_if(!eo_do(obj, simple_a_set(1)));
   ck_assert_int_eq(_eo_signals_cb_flag, 0x0);

   eo_unref(obj);

   eo_shutdown();
}
END_TEST

START_TEST(eo_data_fetch)
{
   eo_init();

   /* Usually should be const, not const only for the test... */
   static Eo_Class_Description class_desc = {
        EO_VERSION,
        "Simple2",
        EO_CLASS_TYPE_REGULAR,
        EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
        NULL,
        10,
        NULL,
        NULL
   };

   const Eo_Class *klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   fail_if(!klass);

   Eo *obj = eo_add(klass, NULL);
   fail_if(!obj);
#ifdef EO_DEBUG
   fail_if(eo_data_scope_get(obj, SIMPLE_CLASS));
#endif
   eo_unref(obj);

   class_desc.data_size = 0;
   klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   fail_if(!klass);

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(eo_data_scope_get(obj, klass));
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

START_TEST(eo_isa_tests)
{
   eo_init();

   const Eo_Class *klass, *iface, *mixin;

     {
        /* Usually should be const, not const only for the test... */
        static Eo_Class_Description class_desc = {
             EO_VERSION,
             "Iface",
             EO_CLASS_TYPE_INTERFACE,
             EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
             NULL,
             0,
             NULL,
             NULL
        };

        iface = eo_class_new(&class_desc, NULL, NULL);
        fail_if(!iface);
     }

     {
        /* Usually should be const, not const only for the test... */
        static Eo_Class_Description class_desc = {
             EO_VERSION,
             "Mixin",
             EO_CLASS_TYPE_MIXIN,
             EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
             NULL,
             0,
             NULL,
             NULL
        };

        mixin = eo_class_new(&class_desc, NULL, NULL);
        fail_if(!mixin);
     }

     {
        /* Usually should be const, not const only for the test... */
        static Eo_Class_Description class_desc = {
             EO_VERSION,
             "Simple2",
             EO_CLASS_TYPE_REGULAR,
             EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
             NULL,
             10,
             NULL,
             NULL
        };

        klass = eo_class_new(&class_desc, EO_BASE_CLASS, iface, mixin, NULL);
        fail_if(!klass);
     }

   Eo *obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(eo_isa(obj, SIMPLE_CLASS));
   fail_if(!eo_isa(obj, iface));
   fail_if(!eo_isa(obj, mixin));
   fail_if(!eo_isa(obj, klass));
   fail_if(!eo_isa(obj, EO_BASE_CLASS));
   eo_unref(obj);

   obj = eo_add(SIMPLE_CLASS, NULL);
   fail_if(!obj);
   fail_if(eo_isa(obj, klass));
   fail_if(eo_isa(obj, iface));
   fail_if(eo_isa(obj, mixin));
   fail_if(!eo_isa(obj, SIMPLE_CLASS));
   fail_if(!eo_isa(obj, EO_BASE_CLASS));
   eo_unref(obj);

   eo_shutdown();
}
END_TEST


START_TEST(eo_composite_tests)
{
   eo_init();

   Eo *obj = eo_add(SIMPLE_CLASS, NULL);
   fail_if(!obj);
   Eo *obj2 = eo_add(SIMPLE_CLASS, NULL);
   fail_if(!obj2);

   eo_composite_attach(obj2, obj);
   eo_do(obj2, eo_parent_set(NULL));
   fail_if(eo_composite_is(obj2));

   eo_unref(obj2);
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

static Eina_Bool _man_should_con = EINA_TRUE;
static Eina_Bool _man_should_des = EINA_TRUE;
static const Eo_Class *cur_klass = NULL;

static void
_man_con(Eo *obj, void *data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   if (_man_should_con)
      eo_manual_free_set(obj, EINA_TRUE);
   eo_do_super(obj, cur_klass, eo_constructor());
}

static void
_man_des(Eo *obj, void *data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, cur_klass, eo_destructor());
   if (_man_should_des)
      eo_manual_free_set(obj, EINA_FALSE);
}


static void
_man_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _man_con),
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _man_des),
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _man_des),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_desc);
}

START_TEST(eo_man_free)
{
   eo_init();

   /* Usually should be const, not const only for the test... */
   static Eo_Class_Description class_desc = {
        EO_VERSION,
        "Simple2",
        EO_CLASS_TYPE_REGULAR,
        EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
        NULL,
        10,
        _man_class_constructor,
        NULL
   };

   const Eo_Class *klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   fail_if(!klass);
   cur_klass = klass;

   Eo *obj = eo_add(klass, NULL);
   fail_if(!obj);
   eo_unref(obj);

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(eo_manual_free(obj));
   eo_unref(obj);

   _man_should_des = EINA_FALSE;
   klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   cur_klass = klass;
   fail_if(!klass);

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(eo_manual_free(obj));
   fail_if(eo_destructed_is(obj));
   eo_unref(obj);
   fail_if(!eo_destructed_is(obj));
   fail_if(!eo_manual_free(obj));

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   eo_unref(obj);
   fail_if(!eo_destructed_is(obj));
   fail_if(!eo_manual_free(obj));

   _man_should_con = EINA_FALSE;
   klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   cur_klass = klass;
   fail_if(!klass);

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(eo_manual_free(obj));
   eo_unref(obj);

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   eo_manual_free_set(obj, EINA_TRUE);
   eo_unref(obj);
   eo_ref(obj);
   eo_unref(obj);
   eo_unref(obj);
   fail_if(!eo_manual_free(obj));

   obj = eo_add(klass, NULL);
   fail_if(!obj);
   eo_manual_free_set(obj, EINA_TRUE);
   eo_unref(obj);
   eo_ref(obj);
   eo_unref(obj);
   eo_unref(obj);
   eo_unref(obj);
   eo_unref(obj);
   fail_if(!eo_manual_free(obj));

   eo_shutdown();
}
END_TEST

START_TEST(eo_refs)
{
   eo_init();
   Eo *obj = eo_add(SIMPLE_CLASS, NULL);
   Eo *obj2 = eo_add(SIMPLE_CLASS, NULL);
   Eo *obj3 = eo_add(SIMPLE_CLASS, NULL);

   eo_xref(obj, obj2);
   fail_if(eo_ref_get(obj) != 2);
   eo_xref(obj, obj3);
   fail_if(eo_ref_get(obj) != 3);

   eo_xunref(obj, obj2);
   fail_if(eo_ref_get(obj) != 2);
   eo_xunref(obj, obj3);
   fail_if(eo_ref_get(obj) != 1);

#ifdef EO_DEBUG
   eo_xunref(obj, obj3);
   fail_if(eo_ref_get(obj) != 1);

   eo_xref(obj, obj2);
   fail_if(eo_ref_get(obj) != 2);

   eo_xunref(obj, obj3);
   fail_if(eo_ref_get(obj) != 2);

   eo_xunref(obj, obj2);
   fail_if(eo_ref_get(obj) != 1);
#endif

   /* Check we don't seg if there's an extra xref. */
   eo_xref(obj, obj2);
   eo_unref(obj);

   eo_unref(obj);
   eo_unref(obj2);
   eo_unref(obj3);

   /* Check hierarchy */
   obj = eo_add(SIMPLE_CLASS, NULL);
   obj2 = eo_add(SIMPLE_CLASS, obj);

   Eo *wref;
   eo_do(obj2, eo_wref_add(&wref));
   fail_if(!wref);

   eo_unref(obj2);

   fail_if(!wref); /* Parent is still holding a reference. */

   eo_unref(obj);

   fail_if(wref);

   /* Just check it doesn't seg atm. */
   obj = eo_add(SIMPLE_CLASS, NULL);
   eo_ref(obj);
   eo_unref(obj);
   eo_unref(obj);

   obj = eo_add(SIMPLE_CLASS, NULL);
   obj2 = eo_add(SIMPLE_CLASS, obj);
   eo_unref(obj2);
   eo_ref(obj2);
   eo_del(obj2);
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

START_TEST(eo_weak_reference)
{
   eo_init();

   Eo *obj = eo_add(SIMPLE_CLASS, NULL);
   Eo *obj2 = eo_add(SIMPLE_CLASS, NULL);
   Eo *wref, *wref2, *wref3;
   eo_do(obj, eo_wref_add(&wref));
   fail_if(!wref);

   eo_unref(obj);
   fail_if(wref);

   obj = eo_add(SIMPLE_CLASS, NULL);
   eo_do(obj, eo_wref_add(&wref));

   eo_ref(obj);
   fail_if(!wref);

   eo_unref(obj);
   fail_if(!wref);

   eo_unref(obj);
   fail_if(wref);

   obj = eo_add(SIMPLE_CLASS, NULL);

   eo_do(obj, eo_wref_add(&wref));
   eo_do(obj, eo_wref_del(&wref));
   fail_if(wref);

   eo_do(obj, eo_wref_add(&wref));
   eo_do(obj2, eo_wref_del(&wref));
   fail_if(!wref);
   eo_wref_del_safe(&wref);
   fail_if(wref);

   wref = obj;
   eo_do(obj, eo_wref_del(&wref));
   fail_if(wref);

   wref = wref2 = wref3 = NULL;
   eo_do(obj, eo_wref_add(&wref), eo_wref_add(&wref2), eo_wref_add(&wref3));
   fail_if(!wref);
   fail_if(!wref2);
   fail_if(!wref3);
   eo_do(obj, eo_wref_del(&wref), eo_wref_del(&wref2), eo_wref_del(&wref3));
   fail_if(wref);
   fail_if(wref2);
   fail_if(wref3);

   eo_do(obj, eo_wref_add(&wref2), eo_wref_add(&wref3));
   wref = obj;
   eo_do(obj, eo_wref_del(&wref));
   fail_if(wref);
   eo_do(obj, eo_wref_del(&wref2), eo_wref_del(&wref3));

   eo_unref(obj);
   eo_unref(obj2);


   eo_shutdown();
}
END_TEST

static void
_a_set(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   fail_if(EINA_TRUE);
}

static void
_op_errors_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {
        EO_OP_FUNC(SIMPLE_ID(SIMPLE_SUB_ID_LAST), _a_set),
        EO_OP_FUNC(SIMPLE_ID(SIMPLE_SUB_ID_LAST + 1), _a_set),
        EO_OP_FUNC(0x0F010111, _a_set),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_desc);
}

START_TEST(eo_op_errors)
{
   eo_init();

   static const Eo_Class_Description class_desc = {
        EO_VERSION,
        "Simple",
        EO_CLASS_TYPE_REGULAR,
        EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
        NULL,
        0,
        _op_errors_class_constructor,
        NULL
   };

   const Eo_Class *klass = eo_class_new(&class_desc, SIMPLE_CLASS, NULL);
   fail_if(!klass);

   Eo *obj = eo_add(klass, NULL);

   /* Out of bounds op for a legal class. */
   fail_if(eo_do(obj, EO_BASE_ID(0x0111)));

   /* Ilegal class. */
   fail_if(eo_do(obj, 0x0F010111));

   fail_if(eo_ref_get(obj) != 1);

   eo_ref(obj);
   fail_if(eo_ref_get(obj) != 2);

   eo_ref(obj);
   fail_if(eo_ref_get(obj) != 3);

   eo_unref(obj);
   fail_if(eo_ref_get(obj) != 2);

   eo_unref(obj);
   fail_if(eo_ref_get(obj) != 1);

   eo_unref(obj);

   obj = eo_add(SIMPLE_CLASS, NULL);
   fail_if(!eo_do(obj, simple_a_print()));
   fail_if(!eo_do(obj, simple_a_print()));
   fail_if(!eo_do(obj, simple_a_set(1)));
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

static void
_fake_free_func(void *data)
{
   if (!data)
      return;

   int *a = data;
   ++*a;
}

START_TEST(eo_generic_data)
{
   eo_init();
   Eo *obj = eo_add(SIMPLE_CLASS, NULL);
   void *data;

   eo_do(obj, eo_base_data_set("test1", (void *) 1, NULL));
   eo_do(obj, eo_base_data_get("test1", &data));
   fail_if(1 != (intptr_t) data);
   eo_do(obj, eo_base_data_del("test1"));
   eo_do(obj, eo_base_data_get("test1", &data));
   fail_if(data);

   eo_do(obj, eo_base_data_set("test1", (void *) 1, NULL));
   eo_do(obj, eo_base_data_set("test2", (void *) 2, NULL));
   eo_do(obj, eo_base_data_get("test1", &data));
   fail_if(1 != (intptr_t) data);
   eo_do(obj, eo_base_data_get("test2", &data));
   fail_if(2 != (intptr_t) data);

   eo_do(obj, eo_base_data_get("test2", &data));
   fail_if(2 != (intptr_t) data);
   eo_do(obj, eo_base_data_del("test2"));
   eo_do(obj, eo_base_data_get("test2", &data));
   fail_if(data);

   eo_do(obj, eo_base_data_get("test1", &data));
   fail_if(1 != (intptr_t) data);
   eo_do(obj, eo_base_data_del("test1"));
   eo_do(obj, eo_base_data_get("test1", &data));
   fail_if(data);

   int a = 0;
   eo_do(obj, eo_base_data_set("test3", &a, _fake_free_func));
   eo_do(obj, eo_base_data_get("test3", &data));
   fail_if(&a != data);
   eo_do(obj, eo_base_data_get("test3", NULL));
   eo_do(obj, eo_base_data_del("test3"));
   fail_if(a != 1);

   a = 0;
   eo_do(obj, eo_base_data_set("test3", &a, _fake_free_func));
   eo_do(obj, eo_base_data_set("test3", NULL, _fake_free_func));
   fail_if(a != 1);
   a = 0;
   data = (void *) 123;
   eo_do(obj, eo_base_data_set(NULL, &a, _fake_free_func));
   eo_do(obj, eo_base_data_get(NULL, &data));
   fail_if(data);
   eo_do(obj, eo_base_data_del(NULL));

   a = 0;
   eo_do(obj, eo_base_data_set("test3", &a, _fake_free_func));
   eo_do(obj, eo_base_data_set("test3", NULL, NULL));
   fail_if(a != 1);
   eo_do(obj, eo_base_data_set("test3", &a, _fake_free_func));

   eo_unref(obj);
   fail_if(a != 2);

   eo_shutdown();
}
END_TEST

START_TEST(eo_magic_checks)
{
   char _buf[sizeof(long)]; /* Just enough to hold eina magic + a bit more. */
   char *buf = _buf;
   eo_init();

   memset(_buf, 1, sizeof(_buf));

   Eo *obj = eo_add(SIMPLE_CLASS, (Eo *) buf);
   fail_if(obj);

   while (1)
     {
        Eo *parent = NULL;
        Eo *wref = NULL;
        Eo *obj2 = NULL;

        obj = eo_add((Eo_Class *) buf, NULL);
        fail_if(obj);

        obj = eo_add(SIMPLE_CLASS, NULL);
        fail_if(!obj);

        fail_if(eo_do((Eo *) buf, EO_NOOP));
        fail_if(eo_do_super((Eo *) buf, SIMPLE_CLASS, EO_NOOP));
        fail_if(eo_do_super(obj, (const Eo_Class *) buf, EO_NOOP));
        fail_if(eo_class_get((Eo *) buf));
        fail_if(eo_class_name_get((Eo_Class*) buf));
        fail_if(eo_class_get(obj) != SIMPLE_CLASS);
        fail_if(eo_class_get(SIMPLE_CLASS) != EO_CLASS_CLASS);
        eo_class_funcs_set((Eo_Class *) buf, NULL);
        eo_do((Eo_Class *) buf, NULL);
        eo_do_super((Eo_Class *) buf, SIMPLE_CLASS, EO_NOOP);
        eo_do_super(SIMPLE_CLASS, (Eo_Class *) buf, EO_NOOP);

        fail_if(eo_class_new(NULL, (Eo_Class *) buf), NULL);

        eo_xref(obj, (Eo *) buf);
        eo_xunref(obj, (Eo *) buf);
        eo_xref((Eo *) buf, obj);
        eo_xunref((Eo *) buf, obj);

        eo_ref((Eo *) buf);
        eo_unref((Eo *) buf);
        eo_del((Eo *) buf);

        eo_isa((Eo *) buf, SIMPLE_CLASS);
        eo_isa(obj, (Eo_Class *) buf);

        fail_if(0 != eo_ref_get((Eo *) buf));

        eo_do((Eo *) buf,
	      eo_wref_add(&wref),
	      eo_parent_get(&parent));
        fail_if(wref);
        fail_if(parent);

        eo_error_set((Eo *) buf);

        fail_if(eo_data_scope_get((Eo *) buf, SIMPLE_CLASS));

        eo_composite_attach((Eo *) buf, obj);
        eo_composite_attach(obj, (Eo *) buf);
        eo_composite_detach((Eo *) buf, obj);
        eo_composite_detach(obj, (Eo *) buf);
        eo_composite_is((Eo *) buf);

        eo_do(obj, eo_event_callback_forwarder_add(NULL, (Eo *) buf));
        eo_do(obj, eo_event_callback_forwarder_del(NULL, (Eo *) buf));

        eo_manual_free_set((Eo *) buf, EINA_TRUE);
        eo_manual_free((Eo *) buf);
        eo_destructed_is((Eo *) buf);

        obj2 = NULL;
        eo_do(obj, eo_parent_set((Eo *) buf));
        eo_do(obj, eo_parent_get(&obj2));
        fail_if(obj2 && (obj2 == (Eo *) buf));

        eo_unref(obj);

        if (!buf)
           break;
        else
           buf = NULL;
     }

   eo_shutdown();
}
END_TEST

/* MULTI */
static Eo_Op MULTI_BASE_ID;
#define MULTI_ID(sub_id) (MULTI_BASE_ID + sub_id)
#define multi_a_print() MULTI_ID(MULTI_SUB_ID_A_PRINT)
#define multi_class_hi_print() MULTI_ID(MULTI_SUB_ID_CLASS_HI_PRINT)

static void
_a_print(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   printf("Hey\n");
}

static void
_class_hi_print(Eo_Class *klass EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   printf("Hi\n");
}

enum {
     MULTI_SUB_ID_A_PRINT,
     MULTI_SUB_ID_CLASS_HI_PRINT,
     MULTI_SUB_ID_LAST
};

static void
_eo_multiple_do_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {
        EO_OP_FUNC(MULTI_ID(MULTI_SUB_ID_A_PRINT), _a_print),
        EO_OP_FUNC_CLASS(MULTI_ID(MULTI_SUB_ID_CLASS_HI_PRINT), _class_hi_print),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_desc);
}

static const Eo_Op_Description _eo_multiple_do_op_desc[] = {
     EO_OP_DESCRIPTION(MULTI_SUB_ID_A_PRINT, "Print property A"),
     EO_OP_DESCRIPTION_CLASS(MULTI_SUB_ID_CLASS_HI_PRINT, "Print Hi"),
     EO_OP_DESCRIPTION_SENTINEL
};


START_TEST(eo_multiple_do)
{
   eo_init();

   /* Usually should be const, not const only for the test... */
   static Eo_Class_Description class_desc = {
        EO_VERSION,
        "Inherit",
        EO_CLASS_TYPE_REGULAR,
        EO_CLASS_DESCRIPTION_OPS(&MULTI_BASE_ID, _eo_multiple_do_op_desc, MULTI_SUB_ID_LAST),
        NULL,
        0,
        _eo_multiple_do_class_constructor,
        NULL
   };

   const Eo_Class *klass = eo_class_new(&class_desc, SIMPLE_CLASS, NULL);
   fail_if(!klass);

   Eo *obj = eo_add(klass, NULL);
   fail_if(!obj);

   fail_if(!eo_do(obj, simple_a_print(), multi_a_print(), multi_a_print()));
   fail_if(!eo_do(klass, simple_class_hi_print(), multi_class_hi_print(), multi_class_hi_print()));

   eo_unref(obj);

   eo_shutdown();
}
END_TEST

START_TEST(eo_add_do_and_custom)
{
   Simple_Public_Data *pd = NULL;
   Eo *obj = NULL;
   eo_init();

   obj = eo_add_custom(SIMPLE_CLASS, NULL, eo_constructor());
   fail_if(!obj);
   eo_unref(obj);

   obj = eo_add(SIMPLE_CLASS, NULL, simple_a_set(7));
   fail_if(!obj);
   pd = eo_data_scope_get(obj, SIMPLE_CLASS);
   fail_if(pd->a != 7);
   eo_unref(obj);

   obj = eo_add_custom(SIMPLE_CLASS, NULL, eo_constructor(), simple_a_set(7));
   fail_if(!obj);
   pd = eo_data_scope_get(obj, SIMPLE_CLASS);
   fail_if(pd->a != 7);
   eo_unref(obj);

   eo_shutdown();
}
END_TEST

START_TEST(eo_pointers_indirection)
{
#ifdef HAVE_EO_ID
   eo_init();

   static const Eo_Class_Description class_desc = {
        EO_VERSION,
        "Simple",
        EO_CLASS_TYPE_REGULAR,
        EO_CLASS_DESCRIPTION_OPS(NULL, NULL, 0),
        NULL,
        0,
        NULL,
        NULL
   };

   const Eo_Class *klass = eo_class_new(&class_desc, EO_BASE_CLASS, NULL);
   fail_if(!klass);

   /* Check simple id validity */
   Eo *obj = eo_add(klass, NULL);
   fail_if(!obj);
   fail_if(!eo_isa(obj, klass));
   obj = (Eo *)((char *)(obj) + 1);
   fail_if(eo_isa(obj, klass));
   obj = (Eo *)((char *)(obj) - 1);
   fail_if(!eo_isa(obj, klass));
   eo_unref(obj);
   fail_if(eo_isa(obj, klass));

   /* Check id invalidity after deletion */
   Eo *obj1 = eo_add(klass, NULL);
   fail_if(!obj1);
   eo_unref(obj1);
   Eo *obj2 = eo_add(klass, NULL);
   fail_if(!obj2);
   fail_if(!eo_isa(obj2, klass));
   fail_if(eo_isa(obj1, klass));
   eo_unref(obj2);

#define NB_OBJS 10000
   unsigned int obj_id;
   Eo **objs = calloc(NB_OBJS, sizeof(Eo *));
   fail_if(!objs);
   /* Creation of the objects */
   for ( obj_id = 0; obj_id < NB_OBJS; obj_id++)
     {
        objs[obj_id] = eo_add(klass, NULL);
        if(!objs[obj_id])
          fail_if(!objs[obj_id]);
        if(!eo_isa(objs[obj_id], klass))
          fail_if(!eo_isa(objs[obj_id], klass));
     }
   /* Deletion of a few objects */
   for ( obj_id = 0; obj_id < NB_OBJS; obj_id+=2000)
     {
        eo_unref(objs[obj_id]);
        if(eo_isa(objs[obj_id], klass))
          fail_if(eo_isa(objs[obj_id], klass));
     }
   /* Creation of the deleted objects */
   for ( obj_id = 0; obj_id < NB_OBJS; obj_id+=2000)
     {
        objs[obj_id] = eo_add(klass, NULL);
        if(!objs[obj_id])
          fail_if(!objs[obj_id]);
        if(!eo_isa(objs[obj_id], klass))
          fail_if(!eo_isa(objs[obj_id], klass));
     }
   /* Deletion of all the objects */
   for ( obj_id = 0; obj_id < NB_OBJS; obj_id++)
      eo_unref(objs[obj_id]);
   /* Just be sure that we trigger an already freed error */
   eo_unref(objs[0]);
   free(objs);

   eo_shutdown();
#endif
}
END_TEST

void eo_test_general(TCase *tc)
{
   tcase_add_test(tc, eo_generic_data);
   tcase_add_test(tc, eo_op_errors);
   tcase_add_test(tc, eo_simple);
   tcase_add_test(tc, eo_weak_reference);
   tcase_add_test(tc, eo_refs);
   tcase_add_test(tc, eo_magic_checks);
   tcase_add_test(tc, eo_data_fetch);
   tcase_add_test(tc, eo_man_free);
   tcase_add_test(tc, eo_composite_tests);
   tcase_add_test(tc, eo_isa_tests);
   tcase_add_test(tc, eo_multiple_do);
   tcase_add_test(tc, eo_add_do_and_custom);
   tcase_add_test(tc, eo_signals);
   tcase_add_test(tc, eo_pointers_indirection);
}
