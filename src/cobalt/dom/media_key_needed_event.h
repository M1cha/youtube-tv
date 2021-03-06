// Copyright 2015 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COBALT_DOM_MEDIA_KEY_NEEDED_EVENT_H_
#define COBALT_DOM_MEDIA_KEY_NEEDED_EVENT_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "cobalt/dom/event.h"
#include "cobalt/script/typed_arrays.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace dom {

// The MediaKeyNeededEvent indicates that the user agent needs a key or license
// to begin or continue playback.
//   https://dvcs.w3.org/hg/html-media/raw-file/eme-v0.1b/encrypted-media/encrypted-media.html#dom-mediakeyneededevent
class MediaKeyNeededEvent : public Event {
 public:
  MediaKeyNeededEvent(const std::string& key_system,
                      const std::string& session_id,
                      const script::Handle<script::Uint8Array>& init_data);

  const std::string& key_system() const { return key_system_; }
  const std::string& session_id() const { return session_id_; }
  script::Handle<script::Uint8Array> init_data() const {
    return script::Handle<script::Uint8Array>(init_data_reference_);
  }

  DEFINE_WRAPPABLE_TYPE(MediaKeyNeededEvent);

 private:
  std::string key_system_;
  std::string session_id_;
  script::ScriptValue<script::Uint8Array>::Reference init_data_reference_;
};

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_MEDIA_KEY_NEEDED_EVENT_H_
