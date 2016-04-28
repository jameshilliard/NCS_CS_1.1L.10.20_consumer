/******************************************************************
 *  $Id: soap-ctx.c,v 1.10 2006/07/09 16:24:19 snowdrop Exp $
 *
 * CSOAP Project:  A SOAP client/server library in C
 * Copyright (C) 2003-2004  Ferhat Ayaz
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 * 
 * Email: ferhatayaz@jprogrammer.net
 ******************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include <nanohttp/nanohttp-logging.h>

#include "soap-ctx.h"

SoapCtx *
soap_ctx_new(SoapEnv * env)     /* should only be used internally */
{
  SoapCtx *ctx=NULL;
 
  if (!(ctx = (SoapCtx *) malloc(sizeof(SoapCtx))))
  {
    log_error2("malloc failed (%s)", strerror(errno));
    return NULL;
  }

  ctx->env = env;
  ctx->attachments = NULL;
  ctx->action = NULL;

  return ctx;
}


void
soap_ctx_add_files(SoapCtx * ctx, attachments_t * attachments)
{
  part_t *part;
  char href[MAX_HREF_SIZE]={0};

  if (attachments == NULL)
    return;

  part = attachments->parts;
  while (part)
  {
#ifdef AEI_COVERITY_FIX
    /*CID 31971: Resource Leak(RESOURCE_LEAK)*/
    herror_t err;
    err = soap_ctx_add_file(ctx, part->filename, part->content_type, href);
    herror_release(err);
#else
    soap_ctx_add_file(ctx, part->filename, part->content_type, href);
#endif
    part = part->next;
  }
}


herror_t
soap_ctx_add_file(SoapCtx * ctx, const char *filename,
                  const char *content_type, char *dest_href)
{
  char cid[250]={0};
  char id[250]={0};
  part_t *part;
  static int counter = 1;
  FILE *test = fopen(filename, "r");
  if (!test)
    return herror_new("soap_ctx_add_file", FILE_ERROR_OPEN,
                      "Can not open file '%s'", filename);

  fclose(test);

  /* generate an id */
#ifdef AEI_CISCO_HNAP
  snprintf(id, sizeof(id),"005512345894583%d", counter++);
  sprintf(dest_href, "cid:%s", id);
  snprintf(cid,sizeof(cid) ,"<%s>", id);
#else
  sprintf(id, "005512345894583%d", counter++);
  sprintf(dest_href, "cid:%s", id);
  sprintf(cid, "<%s>", id);
#endif

  /* add part to context */
  part = part_new(cid, filename, content_type, NULL, NULL);
  if (!ctx->attachments)
    ctx->attachments = attachments_new();
  attachments_add_part(ctx->attachments, part);

  return H_OK;
}

part_t *
soap_ctx_get_file(SoapCtx * ctx, xmlNodePtr node)
{
  xmlChar *prop;
  char href[MAX_HREF_SIZE]={0};
  char buffer[MAX_HREF_SIZE]={0};
  part_t *part;

  if (!ctx->attachments)
    return NULL;

  prop = xmlGetProp(node, "href");

  if (!prop)
    return NULL;

#ifdef AEI_COVERITY_FIX
  /*CID 32114: Resource leak(RESOURCE_LEAK))*/
  if(MAX_HREF_SIZE < strlen(prop) + 1)
  {
    xmlFree(prop);
    return NULL;
  }
#endif

  strcpy(href, (const char *) prop);

#ifdef AEI_COVERITY_FIX
  /*CID 32114: Resource leak(RESOURCE_LEAK)*/
  xmlFree(prop);
#endif

  if (!strncmp(href, "cid:", 4))
  {
    for (part = ctx->attachments->parts; part; part = part->next)
    {
#ifdef AEI_CISCO_HNAP
      snprintf(buffer, sizeof(buffer), "<%s>", href + 4);
#else
      sprintf(buffer, "<%s>", href + 4);
#endif
      if (!strcmp(part->id, buffer))
        return part;

    }
  }
  else
  {
    for (part = ctx->attachments->parts; part; part = part->next)
    {
      if (!strcmp(part->location, href))
        return part;

    }
  }

  return NULL;
}

void
soap_ctx_free(SoapCtx * ctx)
{
  if (!ctx)
    return;

  if (ctx->attachments)
    attachments_free(ctx->attachments);

  if (ctx->env)
    soap_env_free(ctx->env);

  if (ctx->action)
    free(ctx->action);

  free(ctx);

  return;
}


herror_t
soap_ctx_new_with_method(const char *urn, const char *method, SoapCtx ** out)
{
  SoapEnv *env;
  herror_t err;
  err = soap_env_new_with_method(urn, method, &env);
  if (err != H_OK)
    return err;
  *out = soap_ctx_new(env);

  return H_OK;
}
