/*
 * libburrow -- Burrow Client Library
 *
 * Copyright (C) 2011 Tony Wooster (twooster@gmail.com)
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in this directory for full text.
 */

/**
 * @file
 * @brief Attribute function declarations
 */

#include "common.h"

burrow_attributes_st *burrow_attributes_create(burrow_attributes_st *dest, burrow_st *burrow)
{
  if (!burrow || dest) {
    if (!dest) {
      dest = malloc(sizeof(burrow_attributes_st));
      if (!dest)
        return NULL;
    }

    dest->burrow = burrow; /* for the free_fn, if burrow is defined */
    dest->next = dest; /* convention for non-managed */
    dest->prev = dest;
    
  } else if (!dest) {
    dest = burrow_malloc(burrow, sizeof(burrow_attributes_st));
    if (!dest)
      return NULL;

    dest->burrow = burrow;
    dest->prev = NULL;
    dest->next = burrow->attributes_list;
    burrow->attributes_list = dest;

    if (dest->next != NULL)
      dest->next->prev = dest;
  }
  
  dest->set = BURROW_ATTRIBUTES_NONE;
  dest->ttl = 0;
  dest->hide = 0;
  
  return dest;
}

size_t burrow_attributes_size(void)
{
  return sizeof(burrow_attributes_st);
}

burrow_attributes_st *burrow_attributes_clone(burrow_attributes_st *dest, const burrow_attributes_st *src)
{
  dest = burrow_attributes_create(dest, src->burrow);
  if (!dest)
    return NULL;
  dest->ttl = src->ttl;
  dest->hide = src->hide;
  dest->set = src->set;
  return dest;
}

void burrow_attributes_free(burrow_attributes_st *attributes)
{
  if (attributes->burrow != NULL) { 
    if (attributes->next != attributes) { /* a managed attribute object */

      if (attributes->next != NULL) /* not at end */
        attributes->next->prev = attributes->prev;
      if (attributes->prev != NULL) /* not at front */
        attributes->prev->next = attributes->next;
      else /* at front -- update burrow */
        attributes->burrow->attributes_list = attributes->next;

    }
    burrow_free(attributes->burrow, attributes);
  } else {
    free(attributes);
  }
}

void burrow_attributes_unset(burrow_attributes_st *attributes, burrow_attributes_set_t set)
{
  attributes->set = set;
}

bool burrow_attributes_check(burrow_attributes_st *attributes, burrow_attributes_set_t set)
{
  return (attributes->set & set); /* if ANY are set, not if ALL are set */
}


void burrow_attributes_set_ttl(burrow_attributes_st *attributes, uint32_t ttl)
{
  attributes->ttl = ttl;
  attributes->set |= BURROW_ATTRIBUTES_TTL;
}

void burrow_attributes_set_hide(burrow_attributes_st *attributes, uint32_t hide)
{
  attributes->hide = hide;
  attributes->set |= BURROW_ATTRIBUTES_HIDE;
}

uint32_t burrow_attributes_get_ttl(const burrow_attributes_st *attributes)
{
  return attributes->ttl;
}

uint32_t burrow_attributes_get_hide(const burrow_attributes_st *attributes)
{
  return attributes->hide;
}
