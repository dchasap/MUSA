/*
 * Copyright - Barcelona Supercomputing Center.
 * All rights reserved.
 *
 * Redistribution in source and binary forms, with or without
 * modification, is not permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "Policy.h"

#include "core/utils/Log.h"

namespace sim {
namespace logic {
namespace memory {
namespace policy {

bool PolicyConfigRegister::add(const std::string name, const engine::Config &config)
{
    auto result = register_.emplace(name, config);
    if (result.second) {
        sim::utils::Log::debug2() << "Registered Policy " << name;
    }
    return result.second;
}


engine::Config PolicyConfigRegister::get(const std::string name) const
{
    auto retval = register_.find(name);
    if (retval != register_.end()) {
        return (*retval).second;
    } else {
        sim::utils::Log::error() << "Policy " << name << " not registered in PolicyConfigRegister";
        exit(1);
        return engine::Config();
    }
}


}  // namespace policy
}  // namespace memory
}  // namespace logic
}  // namespace sim
