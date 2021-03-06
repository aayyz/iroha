/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_MODEL_HASH_PROVIDER_IMPL_HPP
#define IROHA_MODEL_HASH_PROVIDER_IMPL_HPP

#include <algorithm>
#include <common/types.hpp>
#include <crypto/base64.hpp>
#include <crypto/crypto.hpp>
#include <crypto/hash.hpp>
#include <model/model_hash_provider.hpp>


namespace iroha {
  namespace model {
    class HashProviderImpl : public HashProvider<ed25519::pubkey_t::size()> {
     public:
      iroha::hash256_t get_hash(const Proposal &proposal) override;

      iroha::hash256_t get_hash(const Block &block) override;

      iroha::hash256_t get_hash(const Transaction &tx) override;

      iroha::hash256_t  get_hash(const Query& query) override;
    };
  }
}

#endif  // IROHA_MODEL_HASH_PROVIDER_IMPL_HPP
