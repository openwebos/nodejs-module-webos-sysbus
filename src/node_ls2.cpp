/* @@@LICENSE
*
*      Copyright (c) 2010-2013 LG Electronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* LICENSE@@@ */

#include <ev.h>
#include <glib.h>
#include <iostream>
#include <node.h>
#include <stdlib.h>
#include <v8.h>

#include "node_ls2_call.h"
#include "node_ls2_handle.h"
#include "node_ls2_message.h"

GMainLoop* gMainLoop = 0;

using namespace v8;
using namespace node;
using namespace std;

struct econtext {
    GPollFD* pfd;
    ev_io* iow;
    int nfd, afd;
    gint maxpri;

    ev_prepare pw;
    ev_check cw;
    ev_timer tw;

    GMainContext* gc;
};

static bool prepared = false;

static void timer_cb(EV_P_ ev_timer* w, int revents)
{
    /* nop */
}

static void io_cb(EV_P_ ev_io* w, int revents)
{
    /* nop */
}

static void prepare_cb(EV_P_ ev_prepare* w, int revents)
{
    struct econtext* ctx = (struct econtext*)(((char*)w) - offsetof(struct econtext, pw));
    gint timeout;
    int i;

    g_main_context_prepare(ctx->gc, &ctx->maxpri);

    while (ctx->afd < (ctx->nfd = g_main_context_query(
                                      ctx->gc,
                                      ctx->maxpri,
                                      &timeout,
                                      ctx->pfd,
                                      ctx->afd))
          ) {
        free(ctx->pfd);
        free(ctx->iow);

        ctx->afd = 1;

        while (ctx->afd < ctx->nfd) {
            ctx->afd <<= 1;
        }

        ctx->pfd = (GPollFD*)malloc(ctx->afd * sizeof(GPollFD));
        ctx->iow = (ev_io*)malloc(ctx->afd * sizeof(ev_io));
    }

    for (i = 0; i < ctx->nfd; ++i) {
        GPollFD* pfd = ctx->pfd + i;
        ev_io* iow = ctx->iow + i;

        pfd->revents = 0;

        ev_io_init(
            iow,
            io_cb,
            pfd->fd,
            (pfd->events & G_IO_IN ? EV_READ : 0)
            | (pfd->events & G_IO_OUT ? EV_WRITE : 0)
            );
        iow->data = (void*)pfd;
        ev_set_priority(iow, EV_MINPRI);
        ev_io_start(EV_A_ iow);
    }

    if (timeout >= 0) {
        ev_timer_set(&ctx->tw, timeout * 1e-3, 0.);
        ev_timer_start(EV_A_ &ctx->tw);
    }

    prepared = true;
}

static void check_cb(EV_P_ ev_check* w, int revents)
{
    struct econtext* ctx = (struct econtext*)(((char*)w) - offsetof(struct econtext, cw));
    int i;

    // For some reason libev calls check() before prepare() on the very
    // first iteration
    if (!prepared) {
        return;
    }

    for (i = 0; i < ctx->nfd; ++i) {
        ev_io* iow = ctx->iow + i;

        if (ev_is_pending(iow)) {
            GPollFD* pfd = ctx->pfd + i;
            int revents = ev_clear_pending(EV_A_ iow);

            pfd->revents |= pfd->events &
                ((revents & EV_READ ? G_IO_IN : 0)
                | (revents & EV_WRITE ? G_IO_OUT : 0));
        }

        ev_io_stop(EV_A_ iow);
    }

    if (ev_is_active(&ctx->tw)) {
        ev_timer_stop(EV_A_ &ctx->tw);
    }

    g_main_context_check(ctx->gc, ctx->maxpri, ctx->pfd, ctx->nfd);
    
    g_main_context_dispatch(ctx->gc);
}

static struct econtext default_context;

init(Handle<Object> target)
{
    HandleScope scope;

    gMainLoop = g_main_loop_new(NULL, true);

    GMainContext *gc = g_main_context_default();
    struct econtext *ctx = &default_context;

    ctx->gc     = g_main_context_ref (gc);
    ctx->nfd = 0;
    ctx->afd = 0;
    ctx->iow = 0;
    ctx->pfd = 0;

    ev_prepare_init (&ctx->pw, prepare_cb);
    ev_set_priority (&ctx->pw, EV_MINPRI);
    ev_prepare_start (EV_DEFAULT_ &ctx->pw);
    ev_unref(EV_DEFAULT_UC);

    ev_check_init (&ctx->cw, check_cb);
    ev_set_priority (&ctx->cw, EV_MAXPRI);
    ev_check_start (EV_DEFAULT_ &ctx->cw);

    // If we unref this there are cases where libev thinks there aren't
    // any remaining watchers and quits. Commenting this out means that
    // the following doesn't quit immediately:
    //
    // var ls2 = require('palmbus');
    //
    //ev_unref(EV_DEFAULT_UC);

    ev_init (&ctx->tw, timer_cb);
    ev_set_priority (&ctx->tw, EV_MINPRI);

    LS2Handle::Initialize(target);
    LS2Message::Initialize(target);
    LS2Call::Initialize(target);
}

GMainLoop* GetMainLoop()
{
    return gMainLoop;
}
NODE_MODULE(palmbus, init)
