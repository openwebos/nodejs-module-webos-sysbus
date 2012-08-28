// @@@LICENSE
//
//      Copyright (c) 2010-2012 Hewlett-Packard Development Company, L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// LICENSE@@@

var pb = require('webos-sysbus');
var sys = require('sys');
var _ = require('underscore')._;

function responseArrived(message) {
	sys.log("responseArrived[" + message.responseToken() + "]:" + message.payload());
}

sys.log("creating ls2 handle object");

var h = new pb.Handle("com.sample.service", false);

var p = {msg: "Rob"};
var s = JSON.stringify(p);
var call = h.call("palm://com.palm.node_js_service/test", s);
call.addListener('response', responseArrived);
