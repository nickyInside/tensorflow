/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_CORE_TPU_KERNELS_TPU_PROGRAM_GROUP_INTERFACE_H_
#define TENSORFLOW_CORE_TPU_KERNELS_TPU_PROGRAM_GROUP_INTERFACE_H_

#include <stdint.h>

#include <memory>
#include <vector>

#include "absl/time/time.h"
#include "absl/types/span.h"
#include "tensorflow/compiler/tf2xla/host_compute_metadata.pb.h"
#include "tensorflow/compiler/xla/service/hlo.pb.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/tpu/kernels/tpu_compilation_cache_key.h"
#include "tensorflow/core/tpu/kernels/tpu_executable_info.pb.h"
#include "tensorflow/core/tpu/tpu_ops_c_api.h"

namespace tensorflow {
namespace tpu {

// An interface to holds all the programs and metadatas generated by the
// compiler, including those for the sharding/unsharding programs.
class TpuProgramGroupInterface {
 public:
  virtual ~TpuProgramGroupInterface() = default;

  // Check if whether sharding/unsharding program exists.
  virtual bool has_sharding_program() const = 0;

  // Computes program count.
  virtual size_t program_count() const = 0;

  // Computes total program size.
  virtual int64_t program_size() const = 0;

  // Unloads and destroys safely TPU programs.
  virtual void UnloadAndDestroyPrograms() = 0;

  // Logs program memory summary.
  virtual bool LogProgramMemorySummary() = 0;

  // Logs TPU Compilation statistics.
  virtual Status LogCompilationStats(const TpuCompilationCacheKey& key,
                                     absl::Duration duration) = 0;

  // Hlo metadatas. The pointers can only be used as long as the cache entry is
  // referenced.
  virtual absl::Span<const xla::HloProto* const> hlo_metadatas() const = 0;

  // Boolean array to indicate if the modification of variables are
  // allowed.
  virtual const std::vector<bool>& may_modify_variables_list() const = 0;

  // Gets may modify variables value of the TPU program for the given core
  // `index`.
  virtual bool may_modify_variables(int index) const = 0;

  // Get Executable Info Proto
  virtual const TPUExecutableInfoProto& executable_info(int index) const = 0;

  // Get HostTransferInfo Proto
  virtual const TPUHostTransferInfoProto& host_transfer_info(
      int index) const = 0;

  // Get XLA_TpuProgram Proto
  virtual const XLA_TpuProgram* tpu_program(int index) const = 0;
};

}  // namespace tpu
}  // namespace tensorflow

#endif  // TENSORFLOW_CORE_TPU_KERNELS_TPU_PROGRAM_GROUP_INTERFACE_H_
