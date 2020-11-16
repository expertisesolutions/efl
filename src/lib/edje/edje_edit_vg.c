#include "edje_private.h"

static const char *
_edje_vector_name_find(Edje_Edit *eed, int vector_id)
{
   if (!eed->base->file) return NULL;
   if (!eed->base->file->image_dir) return NULL;

   if ((unsigned int)vector_id >= eed->base->file->image_dir->vectors_count)
     return NULL;
   return eed->base->file->image_dir->vectors[vector_id].entry;
}

static int
_edje_vector_id_find(Edje_Edit *eed, const char *vector_name)
{
   unsigned int i;

   if (!eed->base->file) return -1;
   if (!eed->base->file->image_dir) return -1;

   //printf("SEARCH IMAGE %s\n", vector_name);

   for (i = 0; i < eed->base->file->image_dir->vectors_count; ++i)
     if (eed->base->file->image_dir->vectors[i].entry
         && !strcmp(vector_name, eed->base->file->image_dir->vectors[i].entry))
       return i;

   return -1;
}

/****************/
/*  VECTOR API  */
/****************/

EAPI Eina_List *
edje_edit_vectors_list_get(Evas_Object *obj)
{
   Eina_List *vectors = NULL;
   unsigned int i;

   GET_ED_OR_RETURN(NULL);

   if (!ed->file) return NULL;
   if (!ed->file->image_dir) return NULL;

   for (i = 0; i < ed->file->image_dir->vectors_count; ++i)
     vectors = eina_list_append(vectors,
                               eina_stringshare_add(ed->file->image_dir->vectors[i].entry));

   return vectors;
}

EAPI int
edje_edit_vector_id_get(Evas_Object *obj, const char *vector_name)
{
   GET_EED_OR_RETURN(-1);

   return _edje_vector_id_find(eed, vector_name);
}

EAPI Eina_List *
edje_edit_vector_usage_list_get(Evas_Object *obj, const char *name, Eina_Bool first_only)
{
   Eina_List *result = NULL;
   Eina_Iterator *it;
   Edje_Part_Collection_Directory_Entry *pce;
   Edje_Part_Image_Use *item;
   Edje_Part *part;
   Edje_Part_Description_Vector *part_desc_vector;
   unsigned int i, j;
   int vector_id;

   GET_ED_OR_RETURN(NULL);

   vector_id = edje_edit_vector_id_get(obj, name);
   if (vector_id < 0)
     return NULL;

   it = eina_hash_iterator_data_new(ed->file->collection);

#define ITEM_ADD()                                                             \
  item = (Edje_Part_Image_Use *)calloc(1, sizeof(Edje_Part_Image_Use));        \
  item->group = eina_stringshare_add(pce->entry);                              \
  item->part = eina_stringshare_add(part->name);                               \
  item->state.name = eina_stringshare_add(part_desc_vector->common.state.name);\
  item->state.value = part_desc_vector->common.state.value;                    \
  result = eina_list_append(result, item);

#define FIND_IN_PART_DESCRIPTION()                           \
  if (part_desc_vector->vg.id == vector_id)                  \
    {                                                        \
       ITEM_ADD();                                           \
       if (first_only)                                       \
         goto end;                                           \
       else                                                  \
         continue;                                           \
    }

   EINA_ITERATOR_FOREACH(it, pce)
     {
        if (!pce->ref) continue;
        for (i = 0; i < pce->ref->parts_count; i++)
          {
             if (!pce->ref->parts) continue;
             part = pce->ref->parts[i];
             if (part->type == EDJE_PART_TYPE_VECTOR)
               {
                  part_desc_vector = (Edje_Part_Description_Vector *)part->default_desc;
                  FIND_IN_PART_DESCRIPTION();
                  for (j = 0; j < part->other.desc_count; j++)
                    {
                       part_desc_vector = (Edje_Part_Description_Vector *)part->other.desc[j];
                       FIND_IN_PART_DESCRIPTION();
                    }
               }
          }
     }

   #undef ITEM_ADD
   #undef FIND_IN_PART_DESCRIPTION
end:
   eina_iterator_free(it);

   return result;
}

EAPI Eina_Bool
edje_edit_vector_del(Evas_Object *obj, const char *name)
{
   Edje_Vector_Directory_Entry *de, *de_last;
   unsigned int i, j;
   Eina_List *used;
   Eina_Iterator *it;
   Edje_Part_Collection_Directory_Entry *pce;

   GET_EED_OR_RETURN(EINA_FALSE);
   GET_ED_OR_RETURN(EINA_FALSE);

   if (!name) return EINA_FALSE;
   if (!ed->file) return EINA_FALSE;
   if (!ed->path) return EINA_FALSE;

   used = edje_edit_vector_usage_list_get(obj, name, EINA_TRUE);
   if (used)
     {
        edje_edit_image_usage_list_free(used);
        WRN("Vector \"%s\" is used", name);
        return EINA_FALSE;
     }
   edje_edit_image_usage_list_free(used);

   /* Create Image_Directory if not exist */
   if (!ed->file->image_dir)
     goto invalid_image;

   for (i = 0; i < ed->file->image_dir->vectors_count; ++i)
     {
        de = ed->file->image_dir->vectors + i;

        if ((de->entry) && (!strcmp(name, de->entry)))
          break;
     }
   if (i == ed->file->image_dir->vectors_count)
     goto invalid_image;

   de_last = ed->file->image_dir->vectors + ed->file->image_dir->vectors_count - 1;

   {
      char entry[PATH_MAX];
      char last_entry[PATH_MAX];
      Eet_File *eetf;
      void *data;
      int size = 0;

      /* open the eet file */
      eetf = _edje_edit_eet_open(ed, EET_FILE_MODE_READ_WRITE);
      if (!eetf)
        return EINA_FALSE;

      snprintf(entry, sizeof(entry), "edje/vectors/%i", de->id);

      if (eet_delete(eetf, entry) <= 0)
        {
           ERR("Unable to delete \"%s\" vector entry", entry);
           _edje_edit_eet_close(eetf);
           return EINA_FALSE;
        }
      if (de_last->id != de->id)
        {
           snprintf(last_entry, sizeof(last_entry), "edje/vectors/%i", de_last->id);
           data = eet_read(eetf, last_entry, &size);
           eet_delete(eetf, last_entry);
           eet_write(eetf, entry, data, size, 0);
        }

      _edje_if_string_free(ed, &de->entry);
      --ed->file->image_dir->vectors_count;

      if (de_last->id != de->id)
        {
           Edje_Part *part;
           Edje_Part_Description_Vector *part_desc_vector;

           de->entry = de_last->entry;
           it = eina_hash_iterator_data_new(ed->file->collection);
           EINA_ITERATOR_FOREACH(it, pce)
             {
                if (!pce->ref) continue;
                for (i = 0; i < pce->ref->parts_count; i++)
                  {
                     if (!pce->ref->parts) continue;
                     part = pce->ref->parts[i];
                     if (part->type == EDJE_PART_TYPE_VECTOR)
                       {
                          part_desc_vector = (Edje_Part_Description_Vector *)part->default_desc;
                          if (part_desc_vector->vg.id == de_last->id)
                            part_desc_vector->vg.id = de->id;

                          for (j = 0; j < part->other.desc_count; j++)
                            {
                               part_desc_vector = (Edje_Part_Description_Vector *)part->other.desc[j];
                               if (part_desc_vector->vg.id == de_last->id)
                                 part_desc_vector->vg.id = de->id;
                            }
                       }
                  }
                if (!_edje_edit_collection_save(eetf, pce->ref))
                  {
                     _edje_edit_eet_close(eetf);
                     return EINA_FALSE;
                  }
             }
           eina_iterator_free(it);
        }
      ed->file->image_dir->vectors = realloc(ed->file->image_dir->vectors,
                                             sizeof(Edje_Vector_Directory_Entry) *
                                             ed->file->image_dir->vectors_count);
      /* write the edje_file */
      if (!_edje_edit_edje_file_save(eetf, ed->file))
        {
           _edje_edit_eet_close(eetf);
           return EINA_FALSE;
        }

      _edje_edit_eet_close(eetf);
   }
   _edje_edit_flag_script_dirty(eed, EINA_TRUE);

   return EINA_TRUE;

invalid_image:
   WRN("Unable to find vector entry part \"%s\"", name);
   return EINA_FALSE;
}

/****************/
/*  IMAGES API  */
/****************/

EAPI const char *
edje_edit_state_vector_get(Evas_Object *obj, const char *part, const char *state, double value)
{
   Edje_Part_Description_Vector *vec;
   const char *vector;

   GET_PD_OR_RETURN(NULL);

   if (rp->part->type != EDJE_PART_TYPE_VECTOR)
     return NULL;

   vec = (Edje_Part_Description_Vector *)pd;

   vector = _edje_vector_name_find(eed, vec->vg.id);
   if (!vector) return NULL;

   //printf("GET IMAGE for %s [%s]\n", state, image);
   return eina_stringshare_add(vector);
}

EAPI Eina_Bool
edje_edit_state_vector_set(Evas_Object *obj, const char *part, const char *state, double value, const char *vector)
{
   Edje_Part_Description_Vector *vec;
   int id;

   if ((!obj) || (!part) || (!state) || (!vector))
     return EINA_FALSE;

   eina_error_set(0);
   GET_PD_OR_RETURN(EINA_FALSE);

   if (rp->part->type != EDJE_PART_TYPE_VECTOR)
     return EINA_FALSE;

   vec = (Edje_Part_Description_Vector *)pd;

   id = _edje_vector_id_find(eed, vector);

   if (id > -1) vec->vg.id = id;
   else return EINA_FALSE;

   edje_object_calc_force(obj);
   return EINA_TRUE;
}
