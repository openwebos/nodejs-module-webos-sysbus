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

#include <glib.h>
#include <iostream>
#include <node.h>
#include <stdlib.h>
#include <v8.h>
#include <list>
#include <map>
#include <algorithm>

#include "node_ls2_call.h"
#include "node_ls2_handle.h"
#include "node_ls2_message.h"

GMainLoop* gMainLoop = 0;

using namespace v8;
using namespace node;
using namespace std;

struct econtext {
    GPollFD* pfd;
    int nfd, afd;
    gint maxpri;

    uv_prepare_t pw;
    uv_check_t cw;
    uv_timer_t tw;
    uv_poll_t* poll;

    GMainContext* gc;
};

static uv_timer_t timeout_handle;
static bool query = false;

static void timeout_cb(uv_timer_t* w, int revents)
{
    /* nop */
}

static void uv_timeout_cb(uv_timer_t *handle, int status)
{
    query = true;
    uv_timer_stop(&timeout_handle);
}

// a map of fds to GPollFDs 
std::multimap<int, GPollFD *>pfdMap;

static void poll_cb(uv_poll_t* handle, int status, int events)
{
    GPollFD *pfd = (GPollFD *) handle->data;
    int fd = pfd->fd;

    // Iterate over *all* GPollFDs matching the watcher's fd
    std::pair <std::multimap<int,GPollFD *>::iterator, std::multimap<int,GPollFD *>::iterator> ret;
    ret = pfdMap.equal_range(fd);
    for (std::multimap<int,GPollFD *>::iterator it=ret.first; it!=ret.second; ++it) {
        pfd = it->second;
        pfd->revents |= pfd->events & ((events & UV_READABLE ? G_IO_IN : 0) | (events & UV_WRITABLE ? G_IO_OUT : 0));
    }

    pfdMap.erase(fd);
    uv_poll_stop(handle); 
}

static void prepare_cb(uv_prepare_t* w, int revents)
{
    struct econtext* ctx = (struct econtext*)(((char*)w) - offsetof(struct econtext, pw));
    gint timeout;
    int i;

    // return if uv_timeout is active
    if (!query)
        return;

    g_main_context_prepare(ctx->gc, &ctx->maxpri);

    // Get all sources from glib main context
    while (ctx->afd < (ctx->nfd = g_main_context_query(
                                      ctx->gc,
                                      ctx->maxpri,
                                      &timeout,
                                      ctx->pfd,
                                      ctx->afd))
          ) {
        free(ctx->pfd);
        free(ctx->poll);

        ctx->afd = 1;

        while (ctx->afd < ctx->nfd) {
            ctx->afd <<= 1;
        }

        ctx->pfd = (GPollFD*)malloc(ctx->afd * sizeof(GPollFD));
        ctx->poll = (uv_poll_t*) malloc(ctx->afd * sizeof(uv_poll_t));
    }

    // Create poll handle and start polling
    for (i = 0; i < ctx->nfd; ++i) {

        GPollFD* pfd = ctx->pfd + i;
        uv_poll_t* pollw = ctx->poll + i;
        int fd = pfd->fd;

        // libuv does not support more than one watcher on same fd
        // Create a map of fds and check for duplicate fd
        std::multimap<int,GPollFD *>::iterator it = pfdMap.find(fd);
        if (it != pfdMap.end()) {
                pfdMap.insert(std::pair<int, GPollFD *>(fd, pfd));
                continue;
        }
        // insert into map
        pfdMap.insert(std::pair<int, GPollFD *>(fd, pfd));
        // Feed gcontext fd data to libuv and start polling
        pfd->revents = 0;
        //TODO: Make this a list of pfd structs, maybe? Not necessary with the multimap...
        pollw->data = pfd;

        uv_poll_init(uv_default_loop(), pollw, pfd->fd);	
        uv_poll_start(pollw, UV_READABLE | UV_WRITABLE, poll_cb);
    }

    if (timeout >= 0) {
        uv_timer_start(&ctx->tw, timeout_cb, timeout * 1e-3, 0);
    }
}

static void check_cb(uv_check_t* w, int revents)
{
    struct econtext* ctx = (struct econtext*)(((char*)w) - offsetof(struct econtext, cw));

    if (uv_is_active((uv_handle_t*) &ctx->tw)) {
        uv_timer_stop(&ctx->tw);
    }

    int ready = g_main_context_check(ctx->gc, ctx->maxpri, ctx->pfd, ctx->nfd);
    if(ready)
        g_main_context_dispatch(ctx->gc);  

    // libuv is too fast for glib, hold on for a while
    query = false;
    if (!uv_is_active((uv_handle_t*) &timeout_handle)) {
        uv_timer_start(&timeout_handle, uv_timeout_cb, 1, 0);   // 1ms
    }
}

static struct econtext default_context;

init(Handle<Object> target)
{
    HandleScope scope;
    gMainLoop = g_main_loop_new(NULL, true);

    GMainContext *gc = g_main_context_default();
    struct econtext *ctx = &default_context;

    ctx->gc = g_main_context_ref (gc);
    ctx->nfd = 0;
    ctx->afd = 0;
    ctx->pfd = 0;

    query = true;

    // Prepare
    uv_prepare_init (uv_default_loop(), &ctx->pw);
    uv_prepare_start (&ctx->pw, prepare_cb);
    uv_unref((uv_handle_t*) &ctx->pw);

    uv_check_init(uv_default_loop(), &ctx->cw);
    uv_check_start (&ctx->cw, check_cb); 
    uv_unref((uv_handle_t*) &ctx->cw);

    // Timer
    uv_timer_init(uv_default_loop(), &ctx->tw);
    uv_timer_init(uv_default_loop(), &timeout_handle);

    LS2Handle::Initialize(target);
    LS2Message::Initialize(target);
    LS2Call::Initialize(target);
}

GMainLoop* GetMainLoop()
{
    return gMainLoop;
}

NODE_MODULE(webos_sysbus, init)

